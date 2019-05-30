#include "OpenGLRenderDevice.hpp"

#include "Core/Common.hpp"
#include "DataStructures/Array.hpp"

#include <SDL.h>
#include <GL/glew.h>

static bool addShader(GLuint shaderProgram, const String& text, GLenum type, Array<GLuint>* shaders);
static void addAllAttributes(GLuint program, const String& vertexShaderText, const String& attributeKeyword);
static bool checkShaderError(GLuint shader, int flag, bool isProgram, const String& errorMessage);
static void addShaderUniforms(GLuint shaderProgram, const String& shaderText, Map<String, GLint>& uniformMap, Map<String, GLint>& samplerMap);

bool OpenGLRenderDevice::isInitialized = false;

OpenGLRenderDevice::OpenGLRenderDevice(Window& window) :
	shaderVersion(""),
	version(0),
	boundFBO(0),
	viewportFBO(0),
	boundVAO(0),
	boundShader(0)
{
	context = SDL_GL_CreateContext(window.GetWindowHandle());
	glewExperimental = GL_TRUE;
	GLenum res = glewInit();
	if (res != GLEW_OK)
	{
		DEBUG_LOG(LOG_TYPE_RENDERER, LOG_ERROR, "%s", glewGetErrorString(res));
	}

	struct FBOData fboWindowData;
	fboWindowData.width = window.GetWidth();
	fboWindowData.height = window.GetHeight();
	fboMap[0] = fboWindowData;
}

OpenGLRenderDevice::~OpenGLRenderDevice()
{
	SDL_GL_DeleteContext(context);
}

void OpenGLRenderDevice::globalInit()
{
	if (isInitialized) return;

	int32 major = 3;
	int32 minor = 2;

	if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE) != 0)
	{
		DEBUG_LOG(LOG_TYPE_RENDERER, LOG_WARNING, "Could not set core OpenGL profile");
	}
	if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, major) != 0)
	{
		DEBUG_LOG(LOG_TYPE_RENDERER, LOG_ERROR, "Could not set major OpenGL version to %d: %s", major, SDL_GetError());
	}
	if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minor) != 0)
	{
		DEBUG_LOG(LOG_TYPE_RENDERER, LOG_ERROR, "Could not set minor OpenGL version to %d: %s", minor, SDL_GetError());
	}
	isInitialized = true;
}

uint32 OpenGLRenderDevice::createRenderTarget(uint32 texture, int32 width, int32 height, enum FramebufferAttachment attachment, uint32 attachmentNumber, uint32 miplevel)
{
	uint32 fbo;
	glGenFramebuffers(1, &fbo);
	setFBO(fbo);

	GLenum attachmentTypeGL = attachment + attachmentNumber;
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentTypeGL, GL_TEXTURE_2D, texture, miplevel);

	struct FBOData data;
	data.width = width;
	data.height = height;
	fboMap[fbo] = data;

	return fbo;
}

uint32 OpenGLRenderDevice::releaseRenderTarget(uint32 fbo)
{
	if (fbo == 0) return 0;

	Map<uint32, FBOData>::iterator it = fboMap.find(fbo);
	if (it == fboMap.end()) return 0;

	glDeleteFramebuffers(1, &fbo);
	fboMap.erase(it);
	return 0;
}

uint32 OpenGLRenderDevice::createVertexArray(const float** vertexData, const uint32* vertexElementSizes, uint32 numVertexComponents, uint32 numInstanceComponents, uint32 numVertices, const uint32* indices, uint32 numIndices, BufferUsage usage)
{
	unsigned int numBuffers = numVertexComponents + numInstanceComponents + 1;

	GLuint VAO;
	GLuint* buffers = new GLuint[numBuffers];
	uintptr* bufferSizes = new uintptr[numBuffers];

	glGenVertexArrays(1, &VAO);
	setVAO(VAO);

	glGenBuffers(numBuffers, buffers);
	for (uint32 i = 0, attribute = 0; i < numBuffers - 1; i++)
	{
		enum BufferUsage attribUsage = usage;
		bool inInstanceMode = false;
		if (i >= numVertexComponents)
		{
			attribUsage = USAGE_DYNAMIC_DRAW;
			inInstanceMode = true;
		}

		uint32 elementSize = vertexElementSizes[i];
		const void* bufferData = inInstanceMode ? nullptr : vertexData[i];
		uintptr dataSize = inInstanceMode ? elementSize * sizeof(float) : elementSize * sizeof(float) * numVertices;

		glBindBuffer(GL_ARRAY_BUFFER, buffers[i]);
		glBufferData(GL_ARRAY_BUFFER, dataSize, bufferData, attribUsage);
		bufferSizes[i] = dataSize;

		// OpenGL does not support attribs with more than 4 elements
		uint32 elementSizeDiv = elementSize / 4;
		uint32 elementSizeRem = elementSize % 4;
		for (uint32 j = 0; j < elementSizeDiv; j++)
		{
			glEnableVertexAttribArray(attribute);
			glVertexAttribPointer(attribute, 4, GL_FLOAT, GL_FALSE, elementSize * sizeof(GLfloat), (const GLvoid*)(sizeof(GLfloat) * j * 4));
			if (inInstanceMode)
			{
				glVertexAttribDivisor(attribute, 1);
			}
			attribute++;
		}
		if (elementSizeRem != 0)
		{
			glEnableVertexAttribArray(attribute);
			glVertexAttribPointer(attribute, elementSize, GL_FLOAT, GL_FALSE, elementSize * sizeof(GLfloat), (const GLvoid*)(sizeof(GLfloat) * elementSizeDiv * 4));
			if (inInstanceMode)
			{
				glVertexAttribDivisor(attribute, 1);
			}
			attribute++;
		}
	}

	uintptr indicesSize = numIndices * sizeof(uint32);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[numBuffers - 1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, usage);
	bufferSizes[numBuffers - 1] = indicesSize;

	struct VertexArray vaoData;
	vaoData.buffers = buffers;
	vaoData.bufferSizes = bufferSizes;
	vaoData.numBuffers = numBuffers;
	vaoData.numElements = numIndices;
	vaoData.usage = usage;
	vaoData.instanceComponentsStartIndex = numVertexComponents;
	vaoMap[VAO] = vaoData;

	return VAO;
}

void OpenGLRenderDevice::updateVertexArrayBuffer(uint32 vao, uint32 bufferIndex, const void* data, uintptr dataSize)
{
	if (vao == 0) return;

	Map<uint32, VertexArray>::iterator it = vaoMap.find(vao);
	if (it == vaoMap.end()) return;

	const struct VertexArray* vaoData = &it->second;
	enum BufferUsage usage;

	if (bufferIndex >= vaoData->instanceComponentsStartIndex)
	{
		usage = USAGE_DYNAMIC_DRAW;
	}
	else
	{
		usage = vaoData->usage;
	}

	setVAO(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vaoData->buffers[bufferIndex]);

	if (vaoData->bufferSizes[bufferIndex] >= dataSize)
	{
		glBufferSubData(GL_ARRAY_BUFFER, 0, dataSize, data);
	}
	else
	{
		glBufferData(GL_ARRAY_BUFFER, dataSize, data, usage);
		vaoData->bufferSizes[bufferIndex] = dataSize;
	}
}

uint32 OpenGLRenderDevice::releaseVertexArray(uint32 vao)
{
	if (vao == 0) return 0;

	Map<uint32, VertexArray>::iterator it = vaoMap.find(vao);
	if (it == vaoMap.end()) return 0;

	const struct VertexArray* vaoData = &it->second;

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(vaoData->numBuffers, vaoData->buffers);
	delete[] vaoData->buffers;
	delete[] vaoData->bufferSizes;
	vaoMap.erase(it);
	return 0;
}

uint32 OpenGLRenderDevice::createSampler(SamplerFilter minFilter, SamplerFilter magFilter, SamplerWrapMode wrapU, SamplerWrapMode wrapV, float anisotropy)
{
	uint32 result = 0;
	glGenSamplers(1, &result);
	glSamplerParameteri(result, GL_TEXTURE_WRAP_S, wrapU);
	glSamplerParameteri(result, GL_TEXTURE_WRAP_T, wrapV);
	glSamplerParameteri(result, GL_TEXTURE_MAG_FILTER, magFilter);
	glSamplerParameteri(result, GL_TEXTURE_MIN_FILTER, minFilter);
	if (anisotropy != 0.0f && minFilter != FILTER_NEAREST && minFilter != FILTER_LINEAR)
	{
		glSamplerParameterf(result, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropy);
	}
	return result;
}

uint32 OpenGLRenderDevice::releaseSampler(uint32 sampler)
{
	return uint32();
}

uint32 OpenGLRenderDevice::createTexture2D(int32 width, int32 height, const void* data, PixelFormat dataFormat, PixelFormat interalFormat, bool generateMipmaps, bool compress)
{
	return uint32();
}

uint32 OpenGLRenderDevice::releaseTexture2D(uint32 texture2D)
{
	return uint32();
}

uint32 OpenGLRenderDevice::createUniformBuffer(const void* data, uintptr dataSize, BufferUsage usage)
{
	return uint32();
}

void OpenGLRenderDevice::updateUniformBuffer(uint32 buffer, const void* data, uintptr dataSize)
{
}

uint32 OpenGLRenderDevice::releaseUniformBuffer(uint32 buffer)
{
	return uint32();
}

uint32 OpenGLRenderDevice::createShaderProgram(const String& shaderText)
{
	return uint32();
}

void OpenGLRenderDevice::setShaderUniformBuffer(uint32 shader, const String& uniformBufferName, uint32 buffer)
{
}

void OpenGLRenderDevice::setShaderSampler(uint32 shader, const String& samplerName, uint32 texture, uint32 sampler, uint32 unit)
{
}

uint32 OpenGLRenderDevice::releaseShaderProgram(uint32 shader)
{
	return uint32();
}

/* TODO
 * For Completing the drawing process:
 * - Ensure appropriate render targets are bound
 * - Ensure appropriate sized rendering viewport
 * - Ensure appropriate blend mode for each render target
 * - Ensure appropriate scissor rect, if any
 * - Ensure appropriate polygon and culling modes
 * - Ensure appropriate depth and stencil modes
 * - Ensure appropriate shader programs are bound
 * - Update appropriate uniform buffers
 * - Bind appropriate textures/samplers
 * - Update shader with appropriate samplers and uniform buffers
 * - Bind the element array buffer and vertex attributes
 * - If only 1 instance is being rendered, issue a draw call to render
 * the indexed primitive array
 * - Otherwise, perform an instanced draw call for the number of desired
 * instances
 */

void OpenGLRenderDevice::draw(uint32 fbo, uint32 shader, uint32 vao, PrimitiveType primitiveType, uint32 numInstances, uint32 numElements)
{
	if (numInstances == 0) return;

	setFBO(fbo);
	setViewport(fbo);
	setShader(shader);
	setVAO(vao);

	if (numInstances == 1)
	{
		glDrawElements(primitiveType, (GLsizei)numElements, GL_UNSIGNED_INT, 0);
	}
	else
	{
		glDrawElementsInstanced(primitiveType, (GLsizei)numElements, GL_UNSIGNED_INT, 0, numInstances);
	}
}

void OpenGLRenderDevice::clear(uint32 fbo, bool shouldClearColor, bool shouldClearDepth, bool shouldClearStencil, const Color& color, uint32 stencil)
{
	setFBO(fbo);
	uint32 flags = 0;
	if (shouldClearColor)
	{
		flags |= GL_COLOR_BUFFER_BIT;
		glClearColor(color[0], color[1], color[2], color[3]);
	}
	if (shouldClearDepth)
	{
		flags |= GL_DEPTH_BUFFER_BIT;
	}
	if (shouldClearStencil)
	{
		flags |= GL_STENCIL_BUFFER_BIT;
		glStencilMask(stencil);
	}
	glClear(flags);
}

void OpenGLRenderDevice::setFBO(uint32 fbo)
{
	if (fbo == boundFBO) return;

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	boundFBO = fbo;
}

void OpenGLRenderDevice::setViewport(uint32 fbo)
{
	if (fbo == viewportFBO) return;

	glViewport(0, 0, fboMap[fbo].width, fboMap[fbo].height);
	viewportFBO = fbo;
}

void OpenGLRenderDevice::setVAO(uint32 vao)
{
	if (vao == boundVAO) return;

	glBindVertexArray(vao);
	boundVAO = vao;
}

void OpenGLRenderDevice::setShader(uint32 shader)
{
	if (shader == boundShader) return;

	glUseProgram(shader);
	boundShader = shader;
}

uint32 OpenGLRenderDevice::getVersion()
{
	if (version != 0) return version;

	int32 majorVersion;
	int32 minorVersion;

	glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
	glGetIntegerv(GL_MINOR_VERSION, &minorVersion);

	version = (uint32)(majorVersion * 100 + minorVersion * 10);
	return version;
}

String OpenGLRenderDevice::getShaderVersion()
{
	if (!shaderVersion.empty()) return shaderVersion;

	uint32 cur_version = getVersion();

	if (cur_version >= 330)
	{
		shaderVersion = StringFuncs::ToString(version);
	}
	else if (cur_version >= 320)
	{
		shaderVersion = "150";
	}
	else if (cur_version >= 310)
	{
		shaderVersion = "140";
	}
	else if (cur_version >= 300)
	{
		shaderVersion = "130";
	}
	else if (cur_version >= 210)
	{
		shaderVersion = "120";
	}
	else if (cur_version >= 200)
	{
		shaderVersion = "110";
	}
	else
	{
		int32 majorVersion = cur_version / 100;
		int32 minorVersion = (cur_version / 10) % 10;
		DEBUG_LOG(LOG_TYPE_RENDERER, LOG_ERROR, "Error: OpenGL Version %d.%d does not support shaders.\n", majorVersion, minorVersion);
		return "";
	}

	return shaderVersion;
}

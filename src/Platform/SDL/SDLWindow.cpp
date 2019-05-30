#include "SDLWindow.hpp"

#include "../../Rendering/RenderDevice.hpp"



SDLWindow::SDLWindow(const Application& app, int32 widthIn, int32 heightIn, const String& title) : width(widthIn), height(heightIn)
{
	RenderDevice::globalInit();
	window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
}

SDLWindow::~SDLWindow()
{
	SDL_DestroyWindow(window);
}

WindowHandle SDLWindow::GetWindowHandle()
{
	return window;
}

void SDLWindow::ChangeSize(int32 width, int32 height)
{
	// Change window size w/ this
}

void SDLWindow::Present()
{
	SDL_GL_SwapWindow(window);
}

int32 SDLWindow::GetWidth() const
{
	return width;
}

int32 SDLWindow::GetHeight() const
{
	return height;
}

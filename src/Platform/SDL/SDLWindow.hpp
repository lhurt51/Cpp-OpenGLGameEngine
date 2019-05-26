#pragma once

#include "../../Core/Common.hpp"
#include "../../Core/Application.hpp"
#include "../../DataStructures/String.hpp"

#include <SDL2/include/SDL.h>

typedef SDL_Window* WindowHandle;

class SDLWindow
{

	WindowHandle	window;
	int32			width;
	int32			height;

public:

	SDLWindow(const Application& app, int32 widthIn, int32 heightIn, const String& title);
	virtual ~SDLWindow();

	WindowHandle GetWindowHandle();
	void ChangeSize(int32 width, int32 height);
	void Present();
	int32 GetWidth() const;
	int32 GetHeight() const;

};
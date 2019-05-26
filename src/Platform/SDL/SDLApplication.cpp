#include "SDLApplication.hpp"

#include <SDL2/include/SDL.h>

uint32 SDLApplication::numInstances = 0;

SDLApplication::SDLApplication()
{
	numInstances++;
	bIsAppRunning = true;
}

SDLApplication* SDLApplication::Create()
{
	uint32 initialized = SDL_WasInit(SDL_INIT_EVERYTHING);

	if (initialized != SDL_INIT_EVERYTHING && SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		return nullptr;
	}

	return new SDLApplication();
}

SDLApplication::~SDLApplication()
{
	numInstances--;
	if (numInstances == 0)
	{
		SDL_Quit();
	}
}

void SDLApplication::ProcessMessages(double delta)
{
	SDL_Event e;
	(void)delta;

	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
		case SDL_QUIT:
			bIsAppRunning = false;
			break;
		default:
			break;
		};
	}
}

void SDLApplication::IsRunning()
{
	return bIsAppRunning;
}

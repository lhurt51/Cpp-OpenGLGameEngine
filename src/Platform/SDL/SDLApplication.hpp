#pragma once

#include "../../Core/Common.hpp"

class SDLApplication
{

	bool			bIsAppRunning;
	static uint32	numInstances;

	SDLApplication();

public:

	static SDLApplication* Create();
	virtual ~SDLApplication();

	virtual void ProcessMessages(double delta);
	virtual void IsRunning();

};
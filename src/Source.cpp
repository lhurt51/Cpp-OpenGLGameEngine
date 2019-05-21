#include "Game\Game.hpp"

#include <iostream>
#include <GL/glew.h>
#include <SDL.h>
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#undef main

int main(int argc, char** argv)
{
	Game game;

#ifdef TESTCODE
	std::cout << "Test" << std::endl;
	for (int i = 1; i < argc; ++i)
		std::cout << argv[i] << std::endl;
#endif

	return 0;
}
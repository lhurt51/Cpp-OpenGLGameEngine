#include "Game.hpp"

#include <iostream>

Game::Game(void)
{
	std::cout << "Constructed" << std::endl;
}

Game::~Game(void)
{
	std::cout << "Deconstructed" << std::endl;
}
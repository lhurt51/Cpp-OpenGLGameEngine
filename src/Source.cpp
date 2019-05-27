#include <iostream>
#include "Core/Common.hpp"

#ifdef main
	#undef main
#endif

int main(int argc, char** argv)
{

	std::cout << "Starting" << std::endl;

#ifdef TESTCODE
	std::cout << "Test" << std::endl;
	for (int i = 1; i < argc; ++i)
		std::cout << argv[i] << std::endl;
#endif

	return 0;
}
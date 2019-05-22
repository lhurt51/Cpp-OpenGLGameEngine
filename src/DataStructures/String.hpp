#pragma once

#include <string>
#include <sstream>

#include "Array.hpp"
#include "../Core/Common.hpp"

#define String std::string

struct StringFuncs
{
	template<typename T>
	static FORCEINLINE String ToString(T val)
	{
		std::ostringstream convert;
		convert << val;
		return convert.str();
	}

	static Array<String> Split(const String& s, const char delim);
};
#include "String.hpp"

Array<String> StringFuncs::Split(const String& s, const char delim)
{
	Array<String>		tokens;
	String				token;
	std::istringstream	tokenStream(s);

	while (std::getline(tokenStream, token, delim))
	{
		tokens.push_back(token);
	}

	return tokens;
}
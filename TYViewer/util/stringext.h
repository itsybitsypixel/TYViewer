#pragma once

#include <string>
#include <vector>

class StringExt
{
public:
	static void trimEnd(std::string& s, char trimCharacter = '\0');

	static std::vector<std::string> split(const std::string& s, char splitChar);
	static std::string getTypeFromFilename(const std::string& s);

	static void readUntilNull(const char* buffer, size_t position, std::string& s);
	
};
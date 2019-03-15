#include "stringext.h"

#include <sstream>

void StringExt::trimEnd(std::string& s, char trimCharacter)
{
	s.erase(std::find(s.begin(), s.end(), trimCharacter), s.end());
}

std::vector<std::string> StringExt::split(const std::string& s, char splitChar)
{
	std::stringstream ss(s);
	std::string segment;
	std::vector<std::string> seglist;

	while (std::getline(ss, segment, splitChar))
	{
		seglist.push_back(segment);
	}

	return seglist;
}
std::string StringExt::getTypeFromFilename(const std::string& s)
{
	std::vector<std::string> segments = split(s, '.');
	return segments[segments.size() - 1];
}

void StringExt::readUntil(const char * buffer, size_t position, char delimiter, std::string & s)
{
	do
	{
		s += (char)buffer[position];
		position++;
	} while (buffer[position] != delimiter);
}

void StringExt::readUntilNull(const char* buffer, size_t position, std::string& s)
{
	readUntil(buffer, position, '\0', s);
}

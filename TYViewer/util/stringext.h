#pragma once

#include <string>

inline std::string nts(const char* buffer, size_t offset, size_t max_length = -1)
{
	size_t index = 0;
	std::string res = "";
	while (buffer[offset + index] != '\0')
	{
		if (index >= max_length)
			break;

		res += (char)buffer[offset + index];
		index++;
	}
	return res;
}

inline std::string str(const char* buffer, size_t offset, size_t length)
{
	std::string res = "";
	for (size_t i = offset; i < offset + length; i++)
	{
		res += (char)buffer[i];
	}
	return res;
}
#pragma once

#include <string>

struct Config
{
	std::string applicationPath;
	std::string modelPath;
	std::string texturePath;

	std::string modelName;

	unsigned int width;
	unsigned int height;
};
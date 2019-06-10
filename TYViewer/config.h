#pragma once

#include <string>
#include <unordered_map>

class Config
{
public:
	static std::string model;
	static std::string archive;

	static unsigned int windowResolutionX;
	static unsigned int windowResolutionY;

	static float backgroundR;
	static float backgroundG;
	static float backgroundB;
	

	static bool save(const std::string& path);
	static bool load(const std::string& path);
};
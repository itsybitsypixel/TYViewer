#include "config.h"

#include <fstream>

#include <sstream>
#include <iostream>

std::string Config::model = "";
std::string Config::archive = "";

unsigned int Config::windowResolutionX = 1280;
unsigned int Config::windowResolutionY = 720;

float Config::backgroundR = 0.2f;
float Config::backgroundG = 0.2f;
float Config::backgroundB = 0.2f;


bool Config::save(const std::string& path)
{
	std::ofstream stream;
	stream.open(path, std::ios::out | std::ofstream::trunc);

	if (stream.fail())
		return false;

	stream << "Model" << "=" << model << std::endl;
	stream << "Archive" << "=" << archive << std::endl;

	stream << "WindowResolutionX" << "=" << std::to_string(windowResolutionX) << std::endl;
	stream << "WindowResolutionY" << "=" << std::to_string(windowResolutionY) << std::endl;

	stream << "Background" << "=" <<
		std::to_string(backgroundR) << " " <<
		std::to_string(backgroundG) << " " <<
		std::to_string(backgroundB) << std::endl;

	stream.close();
	return true;
}

bool Config::load(const std::string& path)
{
	std::ifstream stream;
	stream.open(path, std::ios::in);

	if (stream.fail())
		return false;

	std::string line;
	while (std::getline(stream, line))
	{
		if (line.empty())
			continue;

		std::string name;
		std::string value;

		size_t cutoff = line.find_first_of("=");

		name = line.substr(0, cutoff);
		value = line.substr(cutoff + 1);

		if (name == "Model")
		{
			model = value;
		}
		else if (name == "Archive")
		{
			archive = value;
		}

		else if (name == "WindowResolutionX")
		{
			windowResolutionX = std::stoi(value);
		}
		else if (name == "WindowResolutionY")
		{
			windowResolutionY = std::stoi(value);
		}

		else if (name == "Background")
		{
			std::istringstream iss(value);

			std::vector<float> rgb;
			std::string s;
			while (std::getline(iss, s, ' '))
			{
				rgb.push_back(std::stof(s));
			}

			backgroundR = rgb[0];
			backgroundG = rgb[1];
			backgroundB = rgb[2];
		}
	}
}

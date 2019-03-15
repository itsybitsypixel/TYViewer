#include "mad.h"

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

#include <unordered_map>

#include "util/stringext.h"

namespace Loader
{
	void MAD::loadFromFile(const std::string& filepath)
	{
		std::ifstream ifs(filepath, std::ios::binary);
		std::vector<char> data(std::istreambuf_iterator<char>(ifs), {});

		if (ifs.fail())
		{
			std::cout << "[Error] Failed to read file at '" << filepath << "'." << std::endl;
			return;
		}

		loadFromMemory(&data[0], data.size());
	}
	void MAD::loadFromMemory(const char* data, size_t size)
	{
		std::unordered_map<std::string, std::string> properties;

		enum MADReadingState
		{
			NONE,
			SEEKING,
			READING_DEFINITION,
			READING_MATERIAL,
		};

		std::stringstream ss(data);
		MADReadingState state = SEEKING;

		std::string line;
		unsigned int lineIndex = 0;
		while (std::getline(ss, line))
		{
			lineIndex++;

			// 'p' will be used by multiple functions.
			size_t p;

			// Check for comments and remove them.
			p = line.find("//");
			if (p != std::string::npos)
			{
				line = std::string(line.begin(), line.begin() + p);
			}

			// Check for return character and remove.
			p = line.find('\r');
			if (p != std::string::npos)
			{
				line = std::string(line.begin(), line.begin() + p);
			}

			// Check for beginning of material description.
			// Only begins if name is at beginning of line
			// to avoid false results.
			p = line.find("name");
			if (p != std::string::npos && p == 0)
			{
				state = READING_MATERIAL;

				// Get material name.
				std::string name = std::string(line.begin() + 5, line.end() - 1);
				continue;
			}

			if (state == SEEKING)
			{
				// Check for beginning of material description.
				// Only begins if name is at beginning of line
				// to avoid false results.
				p = line.find("name");
				if (p != std::string::npos && p == 0)
				{
					// Change reading state.
					state = READING_MATERIAL;

					std::vector<std::string> subs = StringExt::split(line, ' ');

					std::string name	= subs[1];
					std::string alias	= (subs.size() > 2) ? subs[3] : "";

					continue;
				}
			}
			else if (state == READING_MATERIAL)
			{
				if (line == "")
				{
					state == SEEKING;
					continue;
				}

				std::vector<std::string> subs = StringExt::split(line, ' ');
				if (properties.find(subs[0]) == properties.end())
					properties[subs[0]] = subs[0];

				int abs = 0;
			}
		}

		int ass = 0;
	}
}
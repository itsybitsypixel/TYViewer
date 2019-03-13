#pragma once

#include <string>

namespace Loader
{
	class Asset
	{
	public:
		virtual void loadFromFile(const std::string& filepath) = 0;
		virtual void loadFromMemory(const char* data, size_t size) = 0;
	};
}
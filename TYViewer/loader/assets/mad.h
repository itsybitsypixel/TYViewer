#pragma once

#include <unordered_map>
#include <vector>

#include "loader/asset.h"

namespace Loader
{
	struct MADMaterial
	{
		std::string name;
	};

	class MAD : public Asset
	{
	public:
		virtual void loadFromFile(const std::string& filepath) override;
		virtual void loadFromMemory(const char* data, size_t size) override;

	private:
		std::unordered_map<std::string, unsigned int> m_definitions;
		std::vector<MADMaterial> m_materials;
	};
}
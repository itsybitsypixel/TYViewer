#pragma once

#include <unordered_map>

struct MaterialProperties
{
	unsigned int id = 0;

	bool filter_gc = false;

	std::pair<bool, bool> clampuv;

};

class Material
{
public:

private:
	std::string m_name;
	std::unordered_map<std::string, float> m_properties;
};
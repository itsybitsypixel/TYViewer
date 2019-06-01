#pragma once

#include <utility>
#include <string>

#include <fstream>

#include <unordered_map>

class Parser
{
public:
	static std::pair<std::string, std::string> parseShader(std::ifstream& stream, const std::unordered_map<std::string, int>& properties);

private:
	static bool parseShaderStatement(const std::string& statement, const std::unordered_map<std::string, int>& properties);
};
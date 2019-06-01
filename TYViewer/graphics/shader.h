#pragma once

#include <string>
#include <unordered_map>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct ShaderProgramSource
{
	std::string vertexSource;
	std::string fragmentSource;
};

class Shader
{
public:
	Shader(std::ifstream& stream, const std::unordered_map<std::string, int>& properties);
	~Shader();

	void bind() const;
	void unbind() const;

	void setUniform1i(const std::string& name, int v);
	void setUniform4f(const std::string& name, glm::vec4 v);
	void setUniformMat4(const std::string& name, glm::mat4 mat);

private:
	unsigned int m_id;
	std::unordered_map<std::string, int> m_uniformLocationCache;

	std::unordered_map<std::string, int> properties;

	unsigned int create(const std::string& vertexShader, const std::string& fragmentShader);
	unsigned int compile(unsigned int type, const std::string& source);

	int getUniformLocation(const std::string& name);
};
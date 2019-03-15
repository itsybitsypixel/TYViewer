#include "shader.h"

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

Shader::Shader()
	: m_id(0)
{}
Shader::Shader(const std::string& filepath)
	: m_id(0)
{
	ShaderProgramSource source = parse(filepath);
	m_id = create(source.vertexSource, source.fragmentSource);
}
Shader::~Shader()
{
	glDeleteProgram(m_id);
}

ShaderProgramSource Shader::parse(const std::string& filepath)
{
	// Parses *.shader file.
	// In the future we'll need to implement the features shown in the Ty games's *.shader files.

	std::ifstream stream(filepath);

	enum class ShaderType
	{
		None = -1,
		Vertex = 0,
		Fragment = 1
	};

	std::stringstream ss[2];
	ShaderType type = ShaderType::None;

	std::string line;
	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				type = ShaderType::Vertex;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				type = ShaderType::Fragment;
			}
		}
		else
		{
			ss[(int)type] << line << '\n';
		}
	}

	return { ss[0].str(), ss[1].str() };
}
unsigned int Shader::create(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = compile(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = compile(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}
unsigned Shader::compile(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

void Shader::bind() const
{
	glUseProgram(m_id);
}
void Shader::unbind() const
{
	glUseProgram(0);
}

void Shader::setUniform1i(const std::string& name, int v)
{
	glUniform1i(getUniformLocation(name), v);
}
void Shader::setUniform4f(const std::string& name, glm::vec4 v)
{
	glUniform4f(getUniformLocation(name), v.x, v.y, v.z, v.w);
}
void Shader::setUniformMat4(const std::string& name, glm::mat4 mat)
{
	glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
}

int Shader::getUniformLocation(const std::string& name)
{
	if (m_uniformLocationCache.find(name) != m_uniformLocationCache.end())
		return m_uniformLocationCache[name];

	int location = glGetUniformLocation(m_id, name.c_str());
	if (location == -1)
		std::cout << "Warning: uniform " << name << " doesnt't exist!" << std::endl;
	m_uniformLocationCache[name] = location;
	return location;
}

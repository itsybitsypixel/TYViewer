#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "drawable.h"

class Renderer
{
public:
	Renderer();

	void initialize();

	void clear();
	void draw(const Drawable& drawable, Shader& shader);
	void render(GLFWwindow* window);
	
private:
	glm::vec3 m_clearColor;
};
#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "drawable.h"

class Renderer
{
public:
	Renderer();

	void initialize();

	void drawHollowBox(const glm::vec3& min, const glm::vec3& max, const glm::vec4& colour);
	void drawSphere(const glm::vec3& p, float r, const glm::vec4& colour, int quality = 16);

	void clear(const glm::vec4& colour = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
	void draw(const Drawable& drawable, Shader& shader);
	void render(GLFWwindow* window);
};
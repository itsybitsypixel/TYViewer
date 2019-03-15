#include "renderer.h"

Renderer::Renderer() :
	m_clearColor(0.2f, 0.2f, 0.2f)
{}

void Renderer::initialize()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
}

void Renderer::clear()
{
	glClearColor(m_clearColor.x, m_clearColor.y, m_clearColor.z, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void Renderer::draw(const Drawable& drawable, Shader& shader)
{
	drawable.draw(shader);
}
void Renderer::render(GLFWwindow* window)
{
	glfwSwapBuffers(window);
	glfwPollEvents();
}

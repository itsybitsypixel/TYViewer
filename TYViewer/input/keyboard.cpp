#include "keyboard.h"

void Keyboard::process(GLFWwindow* window, float dt)
{
	m_previous = m_current;
	for (int i = 0; i < GLFW_KEY_LAST; i++)
	{
		m_current[i].first = glfwGetKey(window, i) == GLFW_PRESS;

		if (m_current[i].first)
			m_current[i].second += dt;
		else
			m_current[i].second = 0.0f;
	}
}

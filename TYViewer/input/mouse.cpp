#include "mouse.h"

Mouse::Mouse()
{
	m_currentMousePositionX = 0.0f;
	m_currentMousePositionY = 0.0f;

	m_previousMousePositionX = 0.0f;
	m_previousMousePositionY = 0.0f;

	m_mouseDeltaX = 0.0f;
	m_mouseDeltaY = 0.0f;

	for (int i = 0; i < GLFW_MOUSE_BUTTON_LAST; i++)
	{
		m_current[i].first = false;
		m_current[i].second = 0.0f;
	}
}

void Mouse::process(GLFWwindow* window, float dt)
{
	m_previousMousePositionX = m_currentMousePositionX;
	m_previousMousePositionY = m_currentMousePositionY;
	glfwGetCursorPos(window, &m_currentMousePositionX, &m_currentMousePositionY);
	m_mouseDeltaX = m_currentMousePositionX - m_previousMousePositionX;
	m_mouseDeltaY = m_currentMousePositionY - m_previousMousePositionY;

	for (int i = 0; i < GLFW_MOUSE_BUTTON_LAST; i++)
	{
		m_current[i].first = glfwGetMouseButton(window, i) == GLFW_PRESS;

		if (m_current[i].first)
			m_current[i].second += dt;
		else
			m_current[i].second = 0.0f;
	}
}

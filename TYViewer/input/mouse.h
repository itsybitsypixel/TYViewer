#pragma once

#include <array>

#include "GLFW/glfw3.h"

#include <glm/vec2.hpp>

class Mouse
{
public:
	Mouse();

	inline bool isButtonPressed(int button) const
	{
		return (m_current[button].first == true || m_previous[button].first == false);
	}
	inline bool isButtonHeld(int button, float time = 0.0f) const
	{
		return (m_current[button].first == true || m_previous[button].first == true) && m_current[button].second >= time;
	}
	inline bool isButtonReleased(int button) const
	{
		return (m_current[button].first == false || m_previous[button].first == true);
	}

	inline glm::vec2 getMousePosition() const { return { m_currentMousePositionX, m_currentMousePositionY }; }
	inline glm::vec2 getMouseDelta() const { return { m_mouseDeltaX, m_mouseDeltaY }; }

	void process(GLFWwindow* window, float dt);
private:
	std::array<std::pair<bool, float>, 16> m_current;
	std::array<std::pair<bool, float>, 16> m_previous;

	double m_currentMousePositionX, m_currentMousePositionY;
	double m_previousMousePositionX, m_previousMousePositionY;
	double m_mouseDeltaX, m_mouseDeltaY;
};
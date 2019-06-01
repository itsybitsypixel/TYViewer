#pragma once

#include <array>

#include "GLFW/glfw3.h"

#include <glm/vec2.hpp>

class Mouse
{
public:
	static Mouse* instance;

	static void onMouseScrolled(GLFWwindow* window, double xoffset, double yoffset);


	static void initialize(GLFWwindow* window);

	static inline bool isButtonPressed(int button)
	{
		return (instance->current[button].first == true || instance->previous[button].first == false);
	}
	static inline bool isButtonHeld(int button, float time = 0.0f)
	{
		return (instance->current[button].first == true || instance->previous[button].first == true) && instance->current[button].second >= time;
	}
	static inline bool isButtonReleased(int button)
	{
		return (instance->current[button].first == false || instance->previous[button].first == true);
	}

	static inline glm::vec2 getMousePosition() { return { instance->currentMousePositionX, instance->currentMousePositionY }; }
	static inline glm::vec2 getMouseDelta() { return { instance->mouseDeltaX, instance->mouseDeltaY }; }

	static inline double getMouseScrollDelta() 
	{ 
		double d = instance->verticalMouseScroll;
		instance->verticalMouseScroll = 0.0;
		return d; 
	}

	static void process(GLFWwindow* window, float dt);

protected:
	std::array<std::pair<bool, float>, 16> current;
	std::array<std::pair<bool, float>, 16> previous;

	double currentMousePositionX, currentMousePositionY;
	double previousMousePositionX, previousMousePositionY;
	double mouseDeltaX, mouseDeltaY;
	
	double verticalMouseScroll;
};
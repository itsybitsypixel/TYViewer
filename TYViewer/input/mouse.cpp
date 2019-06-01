#include "mouse.h"

Mouse* Mouse::instance = NULL;

// Goddamnit, I hate callbacks...
// Forces static classes, makes code look stupid, blah blah
// I hate it, I hate it, I hate it!
// But I'm probably not writing this stuff correctly...
// Oh well!
// If anyone reads this, feel free to rewrite my stupid, stupid code.

void Mouse::onMouseScrolled(GLFWwindow* window, double xoffset, double yoffset)
{
	instance->verticalMouseScroll = yoffset;
}

void Mouse::initialize(GLFWwindow* window)
{
	instance = new Mouse();

	glfwSetScrollCallback(window, onMouseScrolled);

	for (int i = 0; i < GLFW_MOUSE_BUTTON_LAST; i++)
	{
		instance->current[i].first = false;
		instance->current[i].second = 0.0f;
	}
}

void Mouse::process(GLFWwindow* window, float dt)
{
	instance->previousMousePositionX = instance->currentMousePositionX;
	instance->previousMousePositionY = instance->currentMousePositionY;
	glfwGetCursorPos(window, &instance->currentMousePositionX, &instance->currentMousePositionY);
	instance->mouseDeltaX = instance->currentMousePositionX - instance->previousMousePositionX;
	instance->mouseDeltaY = instance->currentMousePositionY - instance->previousMousePositionY;

	for (int i = 0; i < GLFW_MOUSE_BUTTON_LAST; i++)
	{
		instance->current[i].first = glfwGetMouseButton(window, i) == GLFW_PRESS;

		if (instance->current[i].first)
			instance->current[i].second += dt;
		else
			instance->current[i].second = 0.0f;
	}
}

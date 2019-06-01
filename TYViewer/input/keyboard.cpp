#include "keyboard.h"

Keyboard* Keyboard::instance = NULL;

void Keyboard::initialize(GLFWwindow* window)
{
	instance = new Keyboard();

	for (int i = 0; i < GLFW_KEY_LAST; i++)
	{
		instance->current[i].first = false;
		instance->current[i].second = 0.0f;
	}
}

void Keyboard::process(GLFWwindow* window, float dt)
{
	instance->previous = instance->current;
	for (int i = 0; i < GLFW_KEY_LAST; i++)
	{
		instance->current[i].first = glfwGetKey(window, i) == GLFW_PRESS;

		if (instance->current[i].first)
			instance->current[i].second += dt;
		else
			instance->current[i].second = 0.0f;
	}
}

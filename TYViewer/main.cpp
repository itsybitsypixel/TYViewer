#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <chrono>

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "graphics/submesh.h"
#include "graphics/shader.h"
#include "graphics/camera.h"

#include "graphics/texture.h"

#include "loader/assets/Model.h"

#include "input/mouse.h"
#include "input/keyboard.h"

#include "application.h"

#define DEFAULT_RESOLUTION_X 1920
#define DEFAULT_RESOLUTION_Y 1080

Application* application;

void onWindowResized(GLFWwindow* window, int width, int height)
{
	application->resize(width, height);
}

int main()
{
	GLFWwindow* window;

	if (!glfwInit())
	{
		std::cout << "[FATAL] Failed to initialize GLFW!" << std::endl;
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(DEFAULT_RESOLUTION_X, DEFAULT_RESOLUTION_Y, "TYViewer", NULL, NULL);
	if (!window)
	{
		std::cout << "[FATAL] Failed to create GLFWwindow!" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	gladLoadGL();

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "[FATAL] Failed to initialize GLAD!" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwSetWindowSizeCallback(window, onWindowResized);


	application = new Application(window);
	application->initialize();
	application->run();

	return 0;
}
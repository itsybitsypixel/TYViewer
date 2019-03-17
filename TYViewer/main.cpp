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

#include "loader/assets/MDL.h"

#include "input/mouse.h"
#include "input/keyboard.h"

#include "application.h"
#include "config.h"


#define DEFAULT_RESOLUTION_WIDTH	1280
#define DEFAULT_RESOLUTION_HEIGHT	720

Application* application;

void onWindowResized(GLFWwindow* window, int width, int height)
{
	application->resize(width, height);
}

int main(int argc, char* argv[])
{
	std::string applicationPath = std::string(argv[0]);
	applicationPath = applicationPath.substr(0, applicationPath.find_last_of("\\/")) + "\\";

	std::string modelName = "";
	std::string modelPath = "res/models/default.mdl";
	
	if (argc > 1)
	{
		modelPath = std::string(argv[1]);
	}
	modelName = modelPath.substr(modelPath.find_last_of("\\/") + 1, modelPath.size() - 1);
	modelPath = modelPath.substr(0, modelPath.find_last_of("\\/")) + "\\";

	Config config = { 
		applicationPath, modelPath, modelPath + "DDS/", 
		modelName,
		DEFAULT_RESOLUTION_WIDTH, DEFAULT_RESOLUTION_HEIGHT };


	std::cout << "[Application path] "	<< config.applicationPath << std::endl;
	std::cout << "[Model path] "		<< config.modelPath << std::endl;
	std::cout << "[Texture path] "		<< config.texturePath << std::endl;
	std::cout << "[Model name] "		<< config.modelName << std::endl;



	GLFWwindow* window;

	if (!glfwInit())
	{
		std::cout << "[FATAL] Failed to initialize GLFW!" << std::endl;
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(config.width, config.height, "TYViewer", NULL, NULL);
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


	application = new Application(window, config);
	application->initialize();
	application->run();

	return 0;
}
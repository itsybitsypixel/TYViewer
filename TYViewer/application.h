#pragma once

#include <string>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "input/keyboard.h"
#include "input/mouse.h"

#include "graphics/camera.h"
#include "graphics/renderer.h"
#include "graphics/shader.h"

#include "graphics/submesh.h"
#include "graphics/texture.h"

#include "loader/assets/model.h"
#include "loader/assets/mad.h"


#define DEFAULT_TEXTURE_FOLDER	"C:/Users/Datorn/Desktop/Ty the Tasmanian Tiger/Content/DDS/"
#define DEFAULT_MODEL_FOLDER	"C:/Users/Datorn/Desktop/Ty the Tasmanian Tiger/Content/"

#define DEFAULT_RESOLUTION_X 1920
#define DEFAULT_RESOLUTION_Y 1080


class Application
{
public:
	Application(GLFWwindow* window);

	void initialize();
	void run();
	void terminate();

	void update(float dt);
	void render(Shader& shader);

	void resize(int width, int height);

private:
	GLFWwindow* m_window;

	int m_width;
	int m_height;

	Keyboard m_keyboard;
	Mouse m_mouse;

	Renderer m_renderer;
	Camera m_camera;


	std::vector<Submesh*> m_submeshes;
	std::unordered_map<std::string, Texture*> m_textures;
};
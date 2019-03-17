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

#include "loader/assets/mdl.h"

#include "model.h"
#include "config.h"

class Application
{
public:
	Application(GLFWwindow* window, const Config& config);

	void initialize();
	void run();
	void terminate();

	void update(float dt);
	void render(Shader& shader);

	void resize(int width, int height);

private:
	GLFWwindow* m_window;
	Config m_config;

	Keyboard m_keyboard;
	Mouse m_mouse;

	Renderer m_renderer;
	Camera m_camera;

	Model* model;
};
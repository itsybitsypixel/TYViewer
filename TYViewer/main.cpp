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

#include "graphics/texture.h"

#include "loader/assets/Model.h"

#include "input/keyboard.h"

int main()
{
	GLFWwindow* window;

	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(640, 480, "TYViewer", NULL, NULL);
	if (!window)
	{
		std::cout << "Failed to create GLFWwindow!" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	gladLoadGL();

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD!" << std::endl;
		return -1;
	}

	Vertex vertices[4] = 
	{
		{ glm::vec3(-0.5, -0.5f, 0.0f), glm::vec3(0, 0, 0), glm::vec2(0.0f, 0.0f), glm::vec3(0, 0, 0) },
		{ glm::vec3( 0.5, -0.5f, 0.0f), glm::vec3(0, 0, 0), glm::vec2(1.0f, 0.0f), glm::vec3(0, 0, 0) },
		{ glm::vec3( 0.5,  0.5f, 0.0f), glm::vec3(0, 0, 0), glm::vec2(1.0f, 1.0f), glm::vec3(0, 0, 0) },
		{ glm::vec3(-0.5,  0.5f, 0.0f), glm::vec3(0, 0, 0), glm::vec2(0.0f, 1.0f), glm::vec3(0, 0, 0) }
	};

	unsigned int indices[6] =
	{
		0, 2, 1,
		2, 3, 0
	};

	Loader::Model model;
	model.loadFromFile("res/models/Act_01_ty.mdl");

	std::vector<Submesh*> submeshes;
	std::unordered_map<std::string, Texture*> textures;

	for (int i = 0; i < model.subobjects.size(); i++)
	{
		for (int j = 0; j < model.subobjects[i].submeshes.size(); j++)
		{
			std::vector<Vertex> vertices;
			for (int k = 0; k < model.subobjects[i].submeshes[j].vertices.size(); k++)
			{
				// Convert from Loader::Vertex to Graphics Vertex
				vertices.push_back(
					Vertex({ 
						{
							model.subobjects[i].submeshes[j].vertices[k].x,
							model.subobjects[i].submeshes[j].vertices[k].y,
							model.subobjects[i].submeshes[j].vertices[k].z,
						},
						{
							model.subobjects[i].submeshes[j].vertices[k].normalX,
							model.subobjects[i].submeshes[j].vertices[k].normalY,
							model.subobjects[i].submeshes[j].vertices[k].normalZ,
						},
						{
							model.subobjects[i].submeshes[j].vertices[k].u,
							model.subobjects[i].submeshes[j].vertices[k].v,
						},
						{
							model.subobjects[i].submeshes[j].vertices[k].skinX,
							model.subobjects[i].submeshes[j].vertices[k].skinY,
							model.subobjects[i].submeshes[j].vertices[k].skinZ
						}
					}));
			}

			Texture* texture = NULL;
			if (textures.find(model.subobjects[i].submeshes[j].ident) != textures.end())
			{
				texture = textures[model.subobjects[i].submeshes[j].ident];
			}
			else
			{
				texture = new Texture("res/textures/" + model.subobjects[i].submeshes[j].ident + ".dds");
				textures[model.subobjects[i].submeshes[j].ident] = texture;
			}

			// Load default uv texture.
			if (texture == NULL)
			{
				if (textures.find("default") != textures.end())
				{
					texture = textures["default"];
				}
				else
				{
					texture = new Texture("res/textures/default.dds");
					textures["default"] = texture;
				}
			}

			submeshes.push_back(new Submesh(vertices, model.subobjects[i].submeshes[j].indices, *texture));
		}
	}


	Shader shader("res/shaders/default.shader");
	shader.bind();
	shader.setUniform1i("u_Texture", 0);

	
	shader.unbind();

	float radius = 180.0f;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);

	Keyboard keyboard;

	float elapsed = 0.0f;
	float previous = 0.0f;

	float dt = 0.0f;

	float rotX = 0.0f;

	while (!glfwWindowShouldClose(window))
	{
		elapsed = glfwGetTime();
		dt = elapsed - previous;
		previous = elapsed;

		keyboard.process(window, dt);

		glClearColor(0.3f, 0.3f, 0.3f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection = glm::mat4(-1.0f);
		projection = glm::perspective(glm::radians(55.0f), (float)1920 / (float)1080, 0.1f, 50500.0f);

		if (keyboard.isKeyHeld(GLFW_KEY_LEFT))
		{
			rotX -= 3.0f * dt;
		}
		if (keyboard.isKeyHeld(GLFW_KEY_RIGHT))
		{
			rotX += 3.0f * dt;
		}
		
		float camX = sin(rotX) * radius;
		float camZ = cos(rotX) * radius;
		glm::mat4 view;
		view = glm::lookAt(glm::vec3(camX, 70.0, camZ), glm::vec3(0.0, 70.0, 0.0), glm::vec3(0.0, 1.0, 0.0));


		shader.bind();

		shader.setUniformMat4("view", view);
		shader.setUniformMat4("projection", projection);

		for (int i = 0; i < submeshes.size(); i++)
		{
			submeshes[i]->draw(shader);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
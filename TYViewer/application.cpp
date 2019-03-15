#include "application.h"

void Application::resize(int width, int height)
{
	m_width = width;
	m_height = height;

	m_camera.setAspectRatio((float)m_width / (float)m_height);

	glViewport(0, 0, m_width, m_height);
}


Application::Application(GLFWwindow* window) :
	m_window(window),
	m_width	(DEFAULT_RESOLUTION_X),
	m_height(DEFAULT_RESOLUTION_Y),
	m_renderer(),
	m_camera(glm::vec3(0.0f, 0.0f, 0.0f), 60.0f, (float)m_width / (float)m_height)
{}

void Application::initialize()
{
	m_renderer.initialize();

	Loader::MAD mad;
	mad.loadFromFile(std::string(DEFAULT_MODEL_FOLDER) + "global.mad");

	Loader::Model model;
	model.loadFromFile(std::string(DEFAULT_MODEL_FOLDER) + "room_z1_02.mdl");

	m_textures["default"] = new Texture("res/textures/default.dds");

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
			if (m_textures.find(model.subobjects[i].submeshes[j].ident) != m_textures.end())
			{
				texture = m_textures[model.subobjects[i].submeshes[j].ident];
			}
			else
			{
				texture = new Texture(DEFAULT_TEXTURE_FOLDER + model.subobjects[i].submeshes[j].ident + ".dds");
				m_textures[model.subobjects[i].submeshes[j].ident] = texture;
			}

			m_submeshes.push_back(new Submesh(vertices, model.subobjects[i].submeshes[j].indices, *texture));
		}
	}
}
void Application::run()
{
	Shader shader("res/shaders/default.shader");
	shader.bind();
	shader.setUniform1i("u_Texture", 0);

	float elapsed = 0.0f;
	float previous = 0.0f;

	float dt = 0.0f;

	while (!glfwWindowShouldClose(m_window))
	{
		elapsed = glfwGetTime();
		dt = elapsed - previous;
		previous = elapsed;

		m_keyboard.process(m_window, dt);
		m_mouse.process(m_window, dt);

		update(dt);
		render(shader);
	}

	terminate();
}
void Application::terminate()
{
	for (int i = 0; i < m_submeshes.size(); i++)
	{
		delete m_submeshes[i];
	}
	for (auto it = m_textures.begin(); it != m_textures.end(); ++it)
	{
		delete it->second;
	}

	m_submeshes.clear();
	m_textures.clear();

	glfwTerminate();
}

void Application::update(float dt)
{
	if (m_mouse.isButtonHeld(GLFW_MOUSE_BUTTON_MIDDLE))
	{
		m_camera.localRotate({ m_mouse.getMouseDelta().x, -m_mouse.getMouseDelta().y });
	}

	float modifier = 1.0f;

	if (m_keyboard.isKeyHeld(GLFW_KEY_LEFT_SHIFT))
	{
		modifier = 5.0f;
	}
	else if (m_keyboard.isKeyHeld(GLFW_KEY_LEFT_CONTROL))
	{
		modifier = 0.05f;
	}

	if (m_keyboard.isKeyHeld(GLFW_KEY_W))
	{
		m_camera.localTranslate(glm::vec3(0.0f, 0.0f, 1.0f) * modifier * dt);
	}
	if (m_keyboard.isKeyHeld(GLFW_KEY_S))
	{
		m_camera.localTranslate(glm::vec3(0.0f, 0.0f, -1.0f) * modifier * dt);
	}
	if (m_keyboard.isKeyHeld(GLFW_KEY_A))
	{
		m_camera.localTranslate(glm::vec3(1.0f, 0.0f, 0.0f) * modifier * dt);
	}
	if (m_keyboard.isKeyHeld(GLFW_KEY_D))
	{
		m_camera.localTranslate(glm::vec3(-1.0f, 0.0f, 0.0f) * modifier * dt);
	}
}
void Application::render(Shader& shader)
{
	m_renderer.clear();

	// Display as a left-handed coordinate system.
	glm::mat4 view;
	view = m_camera.getViewMatrix();
	view = glm::scale(view, glm::vec3(1.0f, 1.0f, -1.0f));

	shader.setUniformMat4("view", view);
	shader.setUniformMat4("projection", m_camera.getViewProjection());

	for (int i = 0; i < m_submeshes.size(); i++)
	{
		m_submeshes[i]->draw(shader);
	}

	m_renderer.render(m_window);
}

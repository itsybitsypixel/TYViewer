#include "application.h"

void Application::resize(int width, int height)
{
	m_config.width = width;
	m_config.height = height;

	m_camera.setAspectRatio((float)m_config.width / (float)m_config.height);

	glViewport(0, 0, m_config.width, m_config.height);
}


Application::Application(GLFWwindow* window, const Config& config) :
	m_window(window),
	m_config(config),
	m_renderer(),
	m_camera(glm::vec3(0.0f, 0.0f, 300.0f), 70.0f, (float)m_config.width / (float)m_config.height, 0.1, 50000.0f)
{}

void Application::initialize()
{
	m_renderer.initialize();

	Loader::MDL mdl;
	mdl.loadFromFile(m_config.modelPath + m_config.modelName);

	model = new Model(m_config.texturePath);
	model->create(mdl);
}
void Application::run()
{
	Shader shader(m_config.applicationPath + "res/shaders/default.shader");
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
	delete model;

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

	m_renderer.draw(*model, shader);

	m_renderer.render(m_window);
}

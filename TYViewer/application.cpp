#include "application.h"

std::string Application::APPLICATION_PATH = "";
std::string Application::ARCHIVE_PATH = "";

void Application::resize(int width, int height)
{
	if (width == 0 || height == 0)
	{
		width = 1;
		height = 1;
	}

	Config::windowResolutionX = width;
	Config::windowResolutionY = height;

	camera.setAspectRatio((float)width / (float)height);

	glViewport(0, 0, width, height);
}

Application::Application(GLFWwindow* window) :
	window(window),
	renderer(),
	camera(glm::vec3(0.0f, 20.0f, -100.0f), glm::vec3(90.0f, 0.0f, 0.0f), 70.0f, (float)Config::windowResolutionX / (float)Config::windowResolutionY, 0.2, 30000.0f),
	mesh(NULL),
	grid(NULL),
	shader(NULL)
{}


void Application::initialize()
{
	content.initialize();
	content.loadRKV(Application::ARCHIVE_PATH);

	Mouse::initialize(window);
	Keyboard::initialize(window);

	renderer.initialize();

	//content.defaultTexture = content.load<Texture>("front_yellow.dds");

	grid = new Grid({ 800, 800 }, 50.0f, glm::vec4(0.4f, 0.4f, 0.4f, 1.0f));

	Font* font = content.load<Font>("font_frontend_pc.wfn");
	//labels.push_back(new Text("This is a longer sentence with spaces!", font, glm::vec3(0,0,0)));

	shader = content.load<Shader>("standard.shader");
	shader->bind();
	shader->setUniform4f("tintColour", glm::vec4(1, 1, 1, 1));
	shader->setUniform1i("diffuseTexture", 0);

	basic = content.load<Shader>("standard.shader");
	basic->bind();
	basic->setUniform4f("tintColour", glm::vec4(1, 1, 1, 1));

	if(Config::model != "")
		models.push_back(content.load<Model>(Config::model));
	else
		models.push_back(content.load<Model>("act_01_ty.mdl"));
}
void Application::run()
{
	float elapsed = 0.0f;
	float previous = 0.0f;

	float dt = 0.0f;

	while (!glfwWindowShouldClose(window))
	{
		elapsed = glfwGetTime();
		dt = elapsed - previous;
		previous = elapsed;

		Keyboard::process(window, dt);
		Mouse::process(window, dt);

		update(dt);
		render(*shader);
		
		glfwPollEvents();
	}

	terminate();
}
void Application::terminate()
{
	Config::save(Application::APPLICATION_PATH + "config.cfg");
	glfwTerminate();
}

void Application::update(float dt)
{
	float mouseInputX = Mouse::getMouseDelta().x;
	float mouseInputY = Mouse::getMouseDelta().y;

	float horizontal	= 
		(Keyboard::isKeyHeld(GLFW_KEY_A)) ?  1.0f : 
		(Keyboard::isKeyHeld(GLFW_KEY_D)) ? -1.0f : 
		0.0f;
	float vertical		= 
		(Keyboard::isKeyHeld(GLFW_KEY_W)) ?  1.0f : 
		(Keyboard::isKeyHeld(GLFW_KEY_S)) ? -1.0f : 
		0.0f;

	if (Mouse::isButtonHeld(GLFW_MOUSE_BUTTON_MIDDLE))
	{
		camera.localRotate(glm::vec3(mouseInputX, -mouseInputY, 0.0f) * 0.1f);
	}

	if (Keyboard::isKeyHeld(GLFW_KEY_LEFT_CONTROL))
		camera.localTranslate(glm::vec3(horizontal, 0.0f, vertical) * 120.0f * dt);
	else if (Keyboard::isKeyHeld(GLFW_KEY_LEFT_SHIFT))
		camera.localTranslate(glm::vec3(horizontal, 0.0f, vertical) * 1520.0f * dt);
	else
		camera.localTranslate(glm::vec3(horizontal, 0.0f, vertical) * 820.0f * dt);

	if (Keyboard::isKeyPressed(GLFW_KEY_1))
	{
		drawGrid = !drawGrid;
	}
	if (Keyboard::isKeyPressed(GLFW_KEY_2))
	{
		drawBounds = !drawBounds;
	}
	if (Keyboard::isKeyPressed(GLFW_KEY_3))
	{
		drawColliders = !drawColliders;
	}
	if (Keyboard::isKeyPressed(GLFW_KEY_4))
	{
		drawBones = !drawBones;
	}

	if (Keyboard::isKeyPressed(GLFW_KEY_F))
	{
		wireframe = !wireframe;
		if (wireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}

	if (Keyboard::isKeyPressed(GLFW_KEY_T))
	{
		Debug::log
		(
			"Camera Position : { " +
			std::to_string(camera.getPosition().x) +
			", " +
			std::to_string(camera.getPosition().y) +
			", " +
			std::to_string(camera.getPosition().z) + " }"
		);
		Debug::log
		(
			"Camera Rotation : { " +
			std::to_string(camera.getRotation().x) +
			", " +
			std::to_string(camera.getRotation().y) +
			", " +
			std::to_string(camera.getRotation().z) + " }"
		);
	}

	if (Keyboard::isKeyHeld(GLFW_KEY_KP_ADD))
	{
		camera.setFieldOfView(camera.getFieldOfView() - 30.0f * dt);
		if (camera.getFieldOfView() < 1.0f)
			camera.setFieldOfView(1.0f);
	}
	else if (Keyboard::isKeyHeld(GLFW_KEY_KP_SUBTRACT))
	{
		camera.setFieldOfView(camera.getFieldOfView() + 30.0f * dt);
		if (camera.getFieldOfView() > 120.0f)
			camera.setFieldOfView(120.0f);
	}
}

void Application::render(Shader& shader)
{
	renderer.clear();

	// Display as a left-handed coordinate system.
	glm::mat4 view;
	view = camera.getViewMatrix();
	view = glm::scale(view, glm::vec3(1.0f, 1.0f, -1.0f));

	glm::mat4 projection;
	projection = camera.getProjectionMatrix();

	glm::mat4 vpmatrix = projection * view;

	shader.bind();
	shader.setUniformMat4("VPMatrix", vpmatrix);

	for (auto& model : models)
	{
		renderer.draw(*model, shader);
	}

	for (auto& label : labels)
	{
		label->draw(shader);
	}

	basic->bind();
	basic->setUniformMat4("VPMatrix", vpmatrix);
	basic->setUniformMat4("modelMatrix", glm::mat4(1.0f));

	if (drawGrid)
	{
		renderer.draw(*grid, *basic);
	}

	for (auto& model : models)
	{
		if (drawBounds)
		{
			renderer.drawHollowBox(model->bounds_crn, model->bounds_size, glm::vec4(1, 1, 1, 1));

			for (auto& bounds : model->bounds)
			{
				renderer.drawHollowBox(bounds.corner, bounds.size, glm::vec4(1, 1, 1, 1));
			}
		}

		if (drawColliders)
		{
			for (auto& collider : model->colliders)
			{
				renderer.drawSphere(collider.position, collider.size / 2.0f, glm::vec4(1, 0, 0, 1));
			}
		}

		if (drawBones)
		{
			for (auto& bone : model->bones)
			{
				renderer.drawSphere(bone.defaultPosition, 2.0f, glm::vec4(1, 1, 1, 1));
			}
		}
	}

	renderer.render(window);
}

#include "renderer.h"

#include "vertex.h"

#define _USE_MATH_DEFINES
#include <math.h>

Renderer::Renderer()
{}

void Renderer::initialize()
{
	glEnable(GL_DEPTH_TEST);

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::drawHollowBox(const glm::vec3& min, const glm::vec3& max, const glm::vec4& colour)
{
	std::vector<Vertex> vertices =
	{
		Vertex(glm::vec4(min.x,			min.y, min.z,			1), colour),
		Vertex(glm::vec4(min.x,			min.y, min.z + max.z,	1), colour),
		Vertex(glm::vec4(min.x + max.x, min.y, min.z + max.z,	1), colour),
		Vertex(glm::vec4(min.x + max.x, min.y, min.z,			1), colour),
											
		Vertex(glm::vec4(min.x,			min.y + max.y, min.z,			1), colour),
		Vertex(glm::vec4(min.x,			min.y + max.y, min.z + max.z,	1), colour),
		Vertex(glm::vec4(min.x + max.x, min.y + max.y, min.z + max.z,	1), colour),
		Vertex(glm::vec4(min.x + max.x, min.y + max.y, min.z,			1), colour)
	};

	std::vector<unsigned int> indices =
	{
		0, 1,
		1, 2,
		2, 3,
		3, 0,

		0, 4,
		1, 5,
		2, 6,
		3, 7,

		4, 5,
		5, 6,
		6, 7,
		7, 4
	};

	unsigned int vao, vbo, ebo;

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 24 * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, position));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, normal));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, colour));

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, texcoord));

	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, skin));

	glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
}

void Renderer::drawSphere(const glm::vec3& p, float r, const glm::vec4& colour, int quality)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	for (unsigned int i = 0; i < quality * 3; i++)
	{
		float f = ((float)(i % quality) / (float)quality) * M_PI * 2;

		//vertices.push_back({ glm::vec3(std::cos(f), std::sin(f), 0.0f) * r, colour });

		if (i < quality)
			vertices.push_back(Vertex(
				glm::vec4(
					p.x + std::cos(f) * r, 
					p.y + std::sin(f) * r, 
					p.z + 0.0f, 
					1.0f), 
				colour));
		else if (i >= quality && i < quality * 2)
			vertices.push_back(Vertex(
				glm::vec4(
					p.x + std::cos(f) * r, 
					p.y + 0.0f, 
					p.z + std::sin(f) * r, 
					1.0f), 
				colour));
		else
			vertices.push_back(Vertex(
				glm::vec4(
					p.x + 0.0f, 
					p.y + std::cos(f) * r, 
					p.z + std::sin(f) * r, 
					1.0f), 
				colour));


		indices.push_back(i);
		if ((i + 1) % quality == 0)
			indices.push_back((i + 1) - quality);
		else
			indices.push_back(i + 1);
	}


	unsigned int vao, vbo, ebo;

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, position));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, normal));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, colour));

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, texcoord));

	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, skin));

	glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);


	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
}

void Renderer::clear(const glm::vec4& colour)
{
	glClearColor(colour.x, colour.y, colour.z, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void Renderer::draw(const Drawable& drawable, Shader& shader)
{
	drawable.draw(shader);
}
void Renderer::render(GLFWwindow* window)
{
	glfwSwapBuffers(window);
}

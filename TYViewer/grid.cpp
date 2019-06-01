#include "grid.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>



Grid::Grid(const glm::vec2& size, float gridNodeSize, const glm::vec4& colour) :
	size(size),
	gridNodeSize(gridNodeSize),
	colour(colour)
{
	setup();
}

Grid::~Grid()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
}

void Grid::setup()
{
	int countX = (int)(size.x / gridNodeSize);
	int countZ = (int)(size.y / gridNodeSize);

	unsigned int lineCount = 0;

	for (unsigned int x = 0; x <= countX; x++)
	{
		glm::vec3 a = -(glm::vec3(size.x, 0, -(size.y)) / 2.0f) + glm::vec3(x, 0, 0) * gridNodeSize;
		glm::vec3 b = -(glm::vec3(size.x, 0, size.y) / 2.0f) + glm::vec3(x, 0, 0) * gridNodeSize;
		glm::vec4 c = (countX % 2 == 0 && x == (countX) / 2) ? glm::vec4(0, 0, 1, 1) : colour;

		vertices.push_back(Vertex(glm::vec4(a.x, a.y, a.z, 1.0f), c));
		vertices.push_back(Vertex(glm::vec4(b.x, b.y, b.z, 1.0f), c));

		indices.push_back(lineCount * 2);
		indices.push_back(lineCount * 2 + 1);

		lineCount++;
	}

	for (unsigned int z = 0; z <= countZ; z++)
	{
		glm::vec3 a = -(glm::vec3(-(size.x), 0, size.y) / 2.0f) + glm::vec3(0, 0, z) * gridNodeSize;
		glm::vec3 b = -(glm::vec3(size.x, 0, size.y) / 2.0f) + glm::vec3(0, 0, z) * gridNodeSize;
		glm::vec4 c = (countZ % 2 == 0 && z == (countZ) / 2) ? glm::vec4(1, 0, 0, 1) : colour;

		vertices.push_back(Vertex(glm::vec4(a.x, a.y, a.z, 1.0f), c));
		vertices.push_back(Vertex(glm::vec4(b.x, b.y, b.z, 1.0f), c));

		indices.push_back(lineCount * 2);
		indices.push_back(lineCount * 2 + 1);

		lineCount++;
	}

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

	glBindVertexArray(0);
}

void Grid::draw(Shader& shader) const
{
	glBindVertexArray(vao);

	shader.bind();
	glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);
}

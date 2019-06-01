#include "mesh.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

Mesh::Mesh() :
	Drawable(),
	Transformable(glm::vec3(0, 0, 0)),
	m_vertices(),
	m_indices(),
	m_texture(NULL),
	vao(0),
	vbo(0),
	ebo(0)
{}

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, Texture* texture) :
	Drawable(),
	Transformable({ 0.0f, 0.0f, 0.0f }),
	m_vertices(vertices), m_indices(indices), m_texture(texture),
	vao(0),
	vbo(0),
	ebo(0)
{
	setup();
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
}

void Mesh::setup()
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);

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

void Mesh::draw(Shader& shader) const
{
	shader.bind();
	m_texture->bind();

	shader.setUniformMat4("modelMatrix", getMatrix());

	glBindVertexArray(vao);

	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr);
}

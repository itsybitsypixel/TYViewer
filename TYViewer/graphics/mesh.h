#pragma once

#include <vector>

#include "vertex.h"
#include "texture.h"
#include "shader.h"

#include "drawable.h"
#include "transformable.h"

class Mesh : public Drawable, public Transformable
{
public:
	Mesh();
	Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, Texture* texture);
	~Mesh();

	virtual void draw(Shader& shader) const override;

private:
	void setup();

	unsigned int vao, vbo, ebo;

	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;

	Texture* m_texture;
};
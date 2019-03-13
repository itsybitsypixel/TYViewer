#pragma once

#include <vector>

#include "vertex.h"
#include "texture.h"
#include "shader.h"

#include "drawable.h"
#include "transformable.h"

class Submesh : public Drawable, public Transformable
{
public:
	Submesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const Texture& texture);

	virtual void draw(Shader& shader) const override;

private:
	void setup();

	unsigned int vao, vbo, ebo;

	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;
	Texture m_texture;
};
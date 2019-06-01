#pragma once

#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

struct Vertex
{
	glm::vec4 position;
	glm::vec4 normal;
	glm::vec4 colour;
	glm::vec2 texcoord;
	glm::vec3 skin;

	Vertex(glm::vec4 position) :
		position(position),
		normal(0, 0, 0, 0),
		colour(1, 1, 1, 1),
		texcoord(0, 0),
		skin(0, 0, 0)
	{}
	Vertex(glm::vec4 position, glm::vec2 texcoord) :
		position(position),
		normal(0, 0, 0, 0),
		colour(1, 1, 1, 1),
		texcoord(texcoord),
		skin(0, 0, 0)
	{}
	Vertex(glm::vec4 position, glm::vec4 colour) :
		position(position),
		normal(0, 0, 0, 0),
		colour(colour),
		texcoord(0, 0),
		skin(0, 0, 0)
	{}
	Vertex(glm::vec4 position, glm::vec4 normal, glm::vec4 colour, glm::vec2 texcoord, glm::vec3 skin) :
		position(position),
		normal(normal),
		colour(colour),
		texcoord(texcoord),
		skin(skin)
	{}
};
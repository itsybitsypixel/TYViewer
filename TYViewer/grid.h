#include <vector>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "graphics/drawable.h"
#include "graphics/vertex.h"

class Grid : public Drawable
{
public:
	Grid(const glm::vec2& size, float gridSize = 1.0f, const glm::vec4& colour = glm::vec4(0.3f, 0.3f, 0.3f, 1.0f));
	~Grid();
	
	virtual void draw(Shader& shader) const override;

private:
	void setup();


	unsigned int vao, vbo, ebo;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	glm::vec2 size;
	float gridNodeSize;
	glm::vec4 colour;
};
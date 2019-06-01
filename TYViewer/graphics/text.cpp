#include "text.h"

#include <vector>

Text::Text(const std::string& text, Font* font, const glm::vec3& position) :
	text(text),
	font(font),
	mesh(NULL),
	Transformable::Transformable(position)
{
	generate();
}

void Text::generate()
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	float xPos = 0;
	float yPos = 0;

	unsigned int vertexCount = 0;

	for (unsigned int i = 0; i < text.size(); i++)
	{
		if (text[i] == ' ')
		{
			xPos += font->getSpaceWidth();
			continue;
		}

		FontRegion region;
		if (font->tryGetFontRegion(text[i], region))
		{
			float width		= region.width * 256;
			float height	= region.height * 256;

			glm::vec4 a = glm::vec4(xPos, yPos, 0, 1);
			glm::vec4 b = glm::vec4(xPos + width, yPos, 0, 1);
			glm::vec4 c = glm::vec4(xPos, yPos - height, 0, 1);
			glm::vec4 d = glm::vec4(xPos + width, yPos - height, 0, 1);

			vertices.push_back(Vertex(a, glm::vec2(region.min[0], 1.0f - region.min[1])));
			vertices.push_back(Vertex(b, glm::vec2(region.max[0], 1.0f - region.min[1])));
			vertices.push_back(Vertex(c, glm::vec2(region.min[0], 1.0f - region.max[1])));
			vertices.push_back(Vertex(d, glm::vec2(region.max[0], 1.0f - region.max[1])));

			indices.push_back(0 + vertexCount);
			indices.push_back(1 + vertexCount);
			indices.push_back(2 + vertexCount);

			indices.push_back(1 + vertexCount);
			indices.push_back(3 + vertexCount);
			indices.push_back(2 + vertexCount);

			vertexCount += 4;

			xPos += region.xAdvance;
		}
	}

	mesh = new Mesh(vertices, indices, font->getTexture());
}

void Text::draw(Shader& shader) const
{
	shader.bind();
	shader.setUniformMat4("modelMatrix", Transformable::getMatrix());

	if (mesh)
	{
		mesh->draw(shader);
	}
}

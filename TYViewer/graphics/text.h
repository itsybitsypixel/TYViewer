#pragma once

#include <string>

#include "drawable.h"
#include "transformable.h"
#include "mesh.h"

#include "util/font.h"

class Text : public Drawable, public Transformable
{
public:
	Text(const std::string& text, Font* font, const glm::vec3& position);

	void generate();

	virtual void draw(Shader& shader) const override;

private:
	std::string text;
	Font* font;

	Mesh* mesh;
};
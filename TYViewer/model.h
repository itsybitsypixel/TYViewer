#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "graphics/drawable.h"
#include "graphics/transformable.h"

#include "graphics/vertex.h"
#include "graphics/submesh.h"
#include "graphics/texture.h"

#include "loader/assets/mdl.h"

class Model : public Drawable, public Transformable
{
public:
	Model(const std::string& texturePath);
	void create(const Loader::MDL& mdl);

	virtual void draw(Shader& shader) const override;

private:
	std::string m_texturePath;

	std::vector<Submesh> m_submeshes;
	std::unordered_map<std::string, Texture*> m_textures;
};
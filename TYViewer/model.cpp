#include "model.h"

Model::Model(const std::string& texturePath) :
	Transformable(glm::vec3(0.0f, 0.0f, 0.0f)),
	m_texturePath(texturePath)
{}

void Model::create(const Loader::MDL& mdl)
{
	m_textures["default"] = new Texture("res/textures/default.dds");

	for (int i = 0; i < mdl.subobjects.size(); i++)
	{
		for (int j = 0; j < mdl.subobjects[i].submeshes.size(); j++)
		{
			std::vector<Vertex> vertices;
			for (int k = 0; k < mdl.subobjects[i].submeshes[j].vertices.size(); k++)
			{
				// Convert from Loader::Vertex to Graphics Vertex
				vertices.push_back(
					Vertex({
						{
							mdl.subobjects[i].submeshes[j].vertices[k].x,
							mdl.subobjects[i].submeshes[j].vertices[k].y,
							mdl.subobjects[i].submeshes[j].vertices[k].z,
						},
						{
							mdl.subobjects[i].submeshes[j].vertices[k].normalX,
							mdl.subobjects[i].submeshes[j].vertices[k].normalY,
							mdl.subobjects[i].submeshes[j].vertices[k].normalZ,
						},
						{
							mdl.subobjects[i].submeshes[j].vertices[k].u,
							mdl.subobjects[i].submeshes[j].vertices[k].v,
						},
						{
							mdl.subobjects[i].submeshes[j].vertices[k].skinX,
							mdl.subobjects[i].submeshes[j].vertices[k].skinY,
							mdl.subobjects[i].submeshes[j].vertices[k].skinZ
						}
						}));
			}

			Texture* texture = NULL;
			if (m_textures.find(mdl.subobjects[i].submeshes[j].ident) != m_textures.end())
			{
				texture = m_textures[mdl.subobjects[i].submeshes[j].ident];
			}
			else
			{
				// A submeshes 'ident' does not always reflect it's filename. 
				// It actually reperesents the texture/material name defined in 'global.mad'.
				// But we'll load them as files directly right now cause it's simpler.
				texture = new Texture(m_texturePath + mdl.subobjects[i].submeshes[j].ident + ".dds");
				m_textures[mdl.subobjects[i].submeshes[j].ident] = texture;
			}

			if (texture == NULL)
				texture = m_textures["default"];

			m_submeshes.push_back(Submesh(vertices, mdl.subobjects[i].submeshes[j].indices, texture));
		}
	}
}

void Model::draw(Shader& shader) const
{
	for (auto& submesh : m_submeshes)
	{
		submesh.draw(shader);
	}
}

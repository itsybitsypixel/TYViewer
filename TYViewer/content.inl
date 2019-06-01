template<>
inline Texture* Content::load(const std::string& name)
{
	if (archive == NULL)
	{
		Debug::log("Failed to load asset because no archive is loaded!");
		return defaultTexture;
	}

	if (textures.find(name) != textures.end())
	{
		return textures[name];
	}
	else
	{
		std::vector<char> data;
		if (archive->getFileData(name, data))
		{
			unsigned int id = SOIL_load_OGL_texture_from_memory(reinterpret_cast<unsigned char*>(&data[0]), data.size(), 0, 0, SOIL_FLAG_INVERT_Y);
			textures[name] = new Texture(id);

			return textures[name];
		}
	}

	return defaultTexture;
}
template<>
inline Shader* Content::load(const std::string& name)
{
	if (archive == NULL)
	{
		Debug::log("Failed to load asset because no archive is loaded!");
		return NULL;
	}

	if (shaders.find(name) != shaders.end())
	{
		return shaders[name];
	}
	else
	{
		File file;
		if (archive->getFile(name, file))
		{
			std::unordered_map<std::string, int> properties;
			properties["TEX"] = 1;
			properties["AREF"] = 0;
			properties["LIT"] = 0;
			properties["SHADOW"] = 0;
			properties["FOG"] = 0;
			properties["TEXMTX"] = 0;
			properties["BLACKTRANS"] = 0;
			properties["SKIN"] = 0;
			properties["SHADOWNORMS"] = 0;
			properties["OMNI"] = 0;

			std::ifstream stream(archive->path);
			stream.seekg(file.offset);

			Shader* shader = new Shader(stream, properties);
			shaders[name] = shader;

			stream.close();


			return shaders[name];
		}
	}

	return NULL;
}

template<>
inline Model* Content::load(const std::string& name)
{
	if (archive == NULL)
	{
		Debug::log("Failed to load asset because no archive is loaded!");
		return NULL;
	}

	if (models.find(name) != models.end())
	{
		return models[name];
	}
	else
	{
		std::vector<char> data;
		if (archive->getFileData(name, data))
		{
			mdl2 mdl;
			mdl.load(data.data(), 0);

			std::vector<Mesh*> meshes;

			std::vector<Collider> colliders;
			std::vector<Bounds> bounds;
			std::vector<Bone> bones;
			
			for (auto& subobj : mdl.subobjects)
			{
				for (auto& mesh : subobj.meshes)
				{
					std::vector<Vertex> vertices;
					std::vector<unsigned int> indices;

					int triangleIndex = 0;
					for (auto& segment : mesh.segments)
					{
						for (auto& vertex : segment.vertices)
						{
							Vertex v =
							{
								glm::vec4(vertex.position[0], vertex.position[1],vertex.position[2], 1.0f),
								glm::vec4(vertex.normal[0], vertex.normal[1],vertex.normal[2], 1.0f),
								glm::vec4(vertex.colour[0], vertex.colour[1],vertex.colour[2], vertex.colour[3]),
								glm::vec2(vertex.texcoord[0], vertex.texcoord[1]),
								glm::vec3(vertex.skin[0], vertex.skin[1], vertex.skin[2])
							};

							vertices.push_back(v);
						}

						for (unsigned int i = 0; i < segment.vertices.size() - 2; i++)
						{
							indices.push_back(0 + triangleIndex);
							indices.push_back(2 + triangleIndex);
							indices.push_back(1 + triangleIndex);

							triangleIndex++;
						}
						triangleIndex += 2;
					}

					meshes.push_back(new Mesh(vertices, indices, load<Texture>(mesh.material + ".dds")));
				}
			}
			
			
			/*
			unsigned int amount_of_subobjects = from_bytes<uint16_t>(&data[0], 6);
			unsigned int amount_of_colliders = from_bytes<uint16_t>(&data[0], 8);
			unsigned int amount_of_bones = from_bytes<uint16_t>(&data[0], 10);

			int offset_subobjects = from_bytes<uint32_t>(&data[0], 12);
			int offset_colliders = from_bytes<uint32_t>(&data[0], 16);
			int offset_bones = from_bytes<uint32_t>(&data[0], 20);

			glm::vec3 bounds_crn = glm::vec3(
				from_bytes<float>(&data[0], 32),
				from_bytes<float>(&data[0], 36),
				from_bytes<float>(&data[0], 40));

			glm::vec3 bounds_size = glm::vec3(
				from_bytes<float>(&data[0], 48),
				from_bytes<float>(&data[0], 52),
				from_bytes<float>(&data[0], 56));

			for (int offset = offset_subobjects; offset < offset_subobjects + amount_of_subobjects * MDL2_SUBMESH_SIZE; offset += MDL2_SUBMESH_SIZE)
			{
				glm::vec3 s_bounds_crn = glm::vec3(
					from_bytes<float>(&data[0], offset),
					from_bytes<float>(&data[0], offset + 4),
					from_bytes<float>(&data[0], offset + 8));

				glm::vec3 s_bounds_max = glm::vec3(
					from_bytes<float>(&data[0], offset + 16),
					from_bytes<float>(&data[0], offset + 20),
					from_bytes<float>(&data[0], offset + 24));

				bounds.push_back({ s_bounds_crn, s_bounds_max });


				int submesh_count	= from_bytes<int16_t>(&data[0], offset + 66);
				int submesh_offset	= from_bytes<int16_t>(&data[0], offset + 68);

				for (unsigned int i = 0; i < submesh_count; i++)
				{
					std::vector<Vertex> vertices;
					std::vector<unsigned int> indices;

					std::string submesh_ident = "";
					submesh_ident = nts(&data[0], from_bytes<int32_t>(&data[0], submesh_offset + (i * 16)));

					std::string segment_material = "";
					segment_material = nts(&data[0], from_bytes<int32_t>(&data[0], submesh_offset + (i * 16) + 0));

					int segment_offset = from_bytes<int32_t>(&data[0], submesh_offset + (i * 16) + 4);
					int segment_count = from_bytes<int32_t>(&data[0], submesh_offset + (i * 16) + 12);


					int triangle_index = 0;

					for (int j = 0; j < segment_count; j++)
					{
						int vertex_count = from_bytes<int32_t>(&data[0], segment_offset + MDL2_SEGMENT_VERTEX_LIST_COUNT_OFFSET);

						for (int k = 0; k < vertex_count; k++)
						{
							Vertex vertex =
							{
								// position
								glm::vec4
								(
									from_bytes<float>(&data[0], segment_offset + MDL2_SEGMENT_VERTEX_LIST_OFFSET + (k * 12) + 0),
									from_bytes<float>(&data[0], segment_offset + MDL2_SEGMENT_VERTEX_LIST_OFFSET + (k * 12) + 4),
									from_bytes<float>(&data[0], segment_offset + MDL2_SEGMENT_VERTEX_LIST_OFFSET + (k * 12) + 8),
									1.0f
								),

								// normal
								glm::vec4
								(
									byte_to_single(&data[0], segment_offset + MDL2_SEGMENT_VERTEX_LIST_OFFSET + vertex_count * 12 + 4 + (k * 4) + 0),
									byte_to_single(&data[0], segment_offset + MDL2_SEGMENT_VERTEX_LIST_OFFSET + vertex_count * 12 + 4 + (k * 4) + 1),
									byte_to_single(&data[0], segment_offset + MDL2_SEGMENT_VERTEX_LIST_OFFSET + vertex_count * 12 + 4 + (k * 4) + 2),
									1.0f
								),

								// colour
								glm::vec4
								(
									1.0f,
									1.0f,
									1.0f,
									1.0f
								),

								// texcoord
								glm::vec2
								(
									from_bytes<int16_t>(&data[0], segment_offset + MDL2_SEGMENT_VERTEX_LIST_OFFSET + vertex_count * 12 + 4 + vertex_count * 4 + 4 + k * 8) / 4096.0f,
									abs((from_bytes<int16_t>(&data[0], segment_offset + MDL2_SEGMENT_VERTEX_LIST_OFFSET + vertex_count * 12 + 4 + vertex_count * 4 + 4 + k * 8 + 2) / 4096.0f) - 1.0f)
								),

								// skin
								glm::vec3
								(
									from_bytes<int16_t>(&data[0], segment_offset + MDL2_SEGMENT_VERTEX_LIST_OFFSET + vertex_count * 12 + 4 + vertex_count * 4 + 4 + k * 8 + 4) / 4096.0f,
									from_bytes<int8_t>(&data[0], segment_offset + MDL2_SEGMENT_VERTEX_LIST_OFFSET + vertex_count * 12 + 4 + vertex_count * 4 + 4 + k * 8 + 6),
									from_bytes<int8_t>(&data[0], segment_offset + MDL2_SEGMENT_VERTEX_LIST_OFFSET + vertex_count * 12 + 4 + vertex_count * 4 + 4 + k * 8 + 7)
								)
							};

							vertices.push_back(vertex);
						}

						for (int k = 0; k < vertex_count - 2; k++)
						{
							indices.push_back(0 + triangle_index);
							indices.push_back(2 + triangle_index);
							indices.push_back(1 + triangle_index);

							triangle_index++;
						}
						triangle_index += 2;

						segment_offset += MDL2_SEGMENT_VERTEX_LIST_OFFSET + vertex_count * 12 + 4 + vertex_count * 4 + 4 + vertex_count * 8;

						while (from_bytes<int16_t>(&data[0], segment_offset) != -1)
						{
							segment_offset += 2;
						}
					}

					meshes.push_back(new Mesh(vertices, indices, load<Texture>(submesh_ident + ".dds")));
				}
			}

			for (int offset = offset_colliders; offset < offset_colliders + amount_of_colliders * 32; offset += 32)
			{
				glm::vec3 position(
					from_bytes<float>(&data[0], offset),
					from_bytes<float>(&data[0], offset + 4),
					from_bytes<float>(&data[0], offset + 8));

				float size = from_bytes<float>(&data[0], offset + 12);

				colliders.push_back({ position, size });
			}

			for (int offset = offset_bones; offset < offset_bones + amount_of_bones * 16; offset += 16)
			{
				bones.push_back
				(
					{
						glm::vec3(
							from_bytes<float>(&data[0], offset),
							from_bytes<float>(&data[0], offset + 4),
							from_bytes<float>(&data[0], offset + 8)
						)
					}
				);
			}
			*/

			models[name] = new Model(meshes);
			models[name]->bounds_crn = glm::vec3(mdl.bounds.x, mdl.bounds.y, mdl.bounds.z);
			models[name]->bounds_size = glm::vec3(mdl.bounds.sx, mdl.bounds.sy, mdl.bounds.sz);

			models[name]->colliders = colliders;
			models[name]->bounds = bounds;
			models[name]->bones = bones;

			return models[name];
		}
	}

	return NULL;
}

template<>
inline Font* Content::load(const std::string& name)
{
	if (archive == NULL)
	{
		Debug::log("Failed to load asset because no archive is loaded!");
		return NULL;
	}

	if (fonts.find(name) != fonts.end())
	{
		return fonts[name];
	}
	else
	{
		std::vector<char> data;
		if (archive->getFileData(name, data))
		{
			WFN fontInfo;
			fontInfo.load(data);

			std::unordered_map<char, FontRegion> regions;

			for (unsigned int i = 0; i < 256; i++)
			{
				if (fontInfo.regions[i].available)
				{
					regions[i].min[0] = fontInfo.regions[i].min[0];
					regions[i].min[1] = fontInfo.regions[i].min[1];

					regions[i].max[0] = fontInfo.regions[i].max[0];
					regions[i].max[1] = fontInfo.regions[i].max[1];

					regions[i].width = regions[i].max[0] - regions[i].min[0];
					regions[i].height = regions[i].max[1] - regions[i].min[1];

					regions[i].xAdvance = fontInfo.regions[i].xAdvance;
				}
			}

			std::string textureName = name.substr(0, name.find_last_of('.')) + ".wtx";

			fonts[name] = new Font(regions, load<Texture>(textureName), fontInfo.spaceWidth);

			return fonts[name];
		}
	}

	return NULL;
}
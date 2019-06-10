#pragma once

#include <string>
#include <fstream>
#include <iostream>

#include <unordered_map>
#include <vector>

#include "SOIL/SOIL2.h"

#include "loader/archive.h"

#include "loader/assets/wfn.h"

#include "loader/assets/mdl2.h"

#include "graphics/texture.h"
#include "graphics/shader.h"
#include "util/font.h"
#include "model.h"

#include "util/bitconverter.h"
#include "util/stringext.h"

#include "debug.h"


#define MDL2_SUBMESH_SIZE 80

#define MDL2_SEGMENT_VERTEX_LIST_COUNT_OFFSET	12
#define MDL2_SEGMENT_VERTEX_LIST_OFFSET 52

class Content
{
public:
	// If specified texture cannot be loaded, return this.
	// This is your job to assign!
	//    Wait, who am I talking to..? myself..?
	//    I even assigned it, why is old me so aggresive..?
	Texture* defaultTexture = NULL;

	void initialize();
	bool loadRKV(const std::string& path);

	template<typename T>
	T* load(const std::string& name)
	{}

	template<>
	Texture* load<Texture>(const std::string& name);

	template<>
	Shader* load<Shader>(const std::string& name);

	template<>
	Model* load<Model>(const std::string& name);

	template<>
	Font* load<Font>(const std::string& name);

private:
	void createDefaultTexture();

	Archive* archive;

	std::unordered_map<std::string, Texture*> textures;
	std::unordered_map<std::string, Shader*> shaders;
	std::unordered_map<std::string, Model*> models;
	std::unordered_map<std::string, Font*> fonts;
};

#include "content.inl"
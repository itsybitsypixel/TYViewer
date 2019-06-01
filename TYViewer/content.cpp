#include "content.h"

void Content::initialize()
{
	createDefaultTexture();
}

bool Content::loadRKV(const std::string& path)
{
	archive = new Archive();
	return archive->load(path);
}

void Content::createDefaultTexture()
{
	// Gosh darn it, you said this map didn't need cs source!!!
	//const unsigned char* data = new unsigned char[16]
	//{ 
	//  // 4x4
	//	// PURPLE	BLACK
	//	// BLACK	PURPLE
	//	255, 0, 255, 255,	0, 0, 0, 0, 
	//	0, 0, 0, 0,			255, 0, 255, 255 
	//};

	// White
	const unsigned char* data = new unsigned char[16]
	{ 
		// 4x4
		// WHITE WHITE
		// WHITE WHITE

		255, 255, 255, 255, 255, 255, 255, 255, 
		255, 255, 255, 255, 255, 255, 255, 255 
	};

	int width = 2;
	int height = 2;

	defaultTexture = new Texture(SOIL_create_OGL_texture(data, &width, &height, 4, 0, 0));

	delete[] data;
}

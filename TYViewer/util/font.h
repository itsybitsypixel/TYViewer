#pragma once

#include <unordered_map>

#include "graphics/texture.h"

struct FontRegion
{
	float min[2];
	float max[2];

	float width;
	float height;

	float xAdvance;
};

class Font
{
public:
	Font(const std::unordered_map<char, FontRegion>& regions, Texture* texture, float spaceWidth);

	bool tryGetFontRegion(char c, FontRegion& region);

	Texture* getTexture() const;
	float getSpaceWidth() const;

private:
	float spaceWidth;

	Texture* texture;
	std::unordered_map<char, FontRegion> regions;
};
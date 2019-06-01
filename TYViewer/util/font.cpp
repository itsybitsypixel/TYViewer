#include "font.h"

Font::Font(const std::unordered_map<char, FontRegion>& regions, Texture* texture, float spaceWidth) :
	spaceWidth(spaceWidth),
	texture(texture),
	regions(regions)
{}

bool Font::tryGetFontRegion(char c, FontRegion& region)
{
	if (regions.find(c) != regions.end())
	{
		region = regions[c];
		return true;
	}

	return false;
}

Texture* Font::getTexture() const
{
	return texture;
}

float Font::getSpaceWidth() const
{
	return spaceWidth;
}

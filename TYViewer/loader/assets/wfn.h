#pragma once

#include <vector>

// --FONT FILE--
struct WFN
{
	struct WFNRegion
	{
		bool available = false;

		float min[2];
		float max[2];

		float xAdvance;
	};

	unsigned int characterCount;
	float spaceWidth;

	WFNRegion regions[256];

	unsigned int firstCharacter;
	unsigned int characterDataOffset;

	void load(const std::vector<char>& data);
};
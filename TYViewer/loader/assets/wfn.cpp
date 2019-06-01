#include "wfn.h"

#include "util/bitconverter.h"

void WFN::load(const std::vector<char>& data)
{
	characterCount		= from_bytes<uint32_t>(&data[0], 0);
	spaceWidth			= from_bytes<float>(&data[0], 12);
	firstCharacter		= (int)from_bytes<float>(&data[0], 36);
	characterDataOffset	= from_bytes<uint32_t>(&data[0], 40);

	for (unsigned int i = firstCharacter; i < firstCharacter + characterCount; i++)
	{
		unsigned int offset = characterDataOffset + i * 32;

		regions[i].available = true;

		regions[i].min[0] = from_bytes<float>(&data[0], offset + 8);
		regions[i].min[1] = from_bytes<float>(&data[0], offset + 12);

		regions[i].max[0] = from_bytes<float>(&data[0], offset + 16);
		regions[i].max[1] = from_bytes<float>(&data[0], offset + 20);

		regions[i].xAdvance = from_bytes<float>(&data[0], offset + 24);
	}
}

#include "mdl.h"

#include <fstream>
#include <iostream>

#include "util/bitconverter.h"
#include "util/stringext.h"

namespace Loader
{
	void MDL::loadFromFile(const std::string& filepath)
	{
		std::ifstream ifs(filepath, std::ios::binary);
		std::vector<char> data(std::istreambuf_iterator<char>(ifs), {});

		if (ifs.fail())
		{
			std::cout << "[Failed to read file] " << filepath << std::endl;
			return;
		}

		loadFromMemory(&data[0], data.size());
	}

	void MDL::loadFromMemory(const char* data, size_t size)
	{
		// Documentation (or the lack of it);
		// This function is not perfect and there is still data that should 
		// be read in but is either not used at this moment or not discovered. 
		// While this code might not be easy to read I've tried to keep it 
		// linear so it can be followed along with the original .mdl file.
		// Therefore I wont be commenting  or documenting this code as it 
		// is likely to change.

		unsigned int amount_of_subobjects	= BitConverter::toUInt16(data, 6);
		unsigned int amount_of_collisions	= BitConverter::toUInt16(data, 8);
		unsigned int amount_of_bones		= BitConverter::toUInt16(data, 10);

		int offset_subobjects	= BitConverter::toInt32(data, 12);
		int offset_collisions	= BitConverter::toInt32(data, 16);
		int offset_bones		= BitConverter::toInt32(data, 20);

		for (int offset = offset_subobjects; 
			offset < offset_subobjects + amount_of_subobjects * MDL2_SUBMESH_SIZE; 
			offset += MDL2_SUBMESH_SIZE)
		{
			Subobject subobject;

			subobject.bounds = 
			{
				// position
				BitConverter::toSingle(&data[0], offset + 0),
				BitConverter::toSingle(&data[0], offset + 4),
				BitConverter::toSingle(&data[0], offset + 8),

				// size
				BitConverter::toSingle(&data[0], offset + 16),
				BitConverter::toSingle(&data[0], offset + 20),
				BitConverter::toSingle(&data[0], offset + 24),

				// origin
				BitConverter::toSingle(&data[0], offset + 32),
				BitConverter::toSingle(&data[0], offset + 36),
				BitConverter::toSingle(&data[0], offset + 40),
			};

			subobject.name	= ""; StringExt::readUntilNull(data, BitConverter::toInt32(data, offset + 48), subobject.name);
			subobject.ident = ""; 
			if (BitConverter::toInt32(&data[0], offset + 52) != -1)
			{
				StringExt::readUntilNull(data, BitConverter::toInt32(data, offset + 52) + 1, subobject.ident);
			}

			int submesh_count	= BitConverter::toInt16(data, offset + 66);
			int submesh_offset	= BitConverter::toInt32(data, offset + 68);

			for (int i = 0; i < submesh_count; i++)
			{
				Submesh submesh;

				submesh.ident = ""; StringExt::readUntilNull(data, BitConverter::toInt32(data, submesh_offset + (i * 16)), submesh.ident);

				int segment_count	= BitConverter::toInt32(data, submesh_offset + (i * 16) + 12);
				int segment_offset	= BitConverter::toInt32(data, submesh_offset + (i * 16) + 4);

				int triangle_index = 0;

				for (int j = 0; j < segment_count; j++)
				{
					int vertex_count = BitConverter::toInt32(data, segment_offset + MDL2_SEGMENT_VERTEX_LIST_COUNT_OFFSET);

					for (int k = 0; k < vertex_count; k++)
					{
						Vertex vertex =
						{
							// position
							BitConverter::toSingle(&data[0], segment_offset + MDL2_SEGMENT_VERTEX_LIST_OFFSET + (k * 12) + 0),
							BitConverter::toSingle(&data[0], segment_offset + MDL2_SEGMENT_VERTEX_LIST_OFFSET + (k * 12) + 4),
							BitConverter::toSingle(&data[0], segment_offset + MDL2_SEGMENT_VERTEX_LIST_OFFSET + (k * 12) + 8),

							// normals
							BitConverter::toSingleFromByte(data[segment_offset + MDL2_SEGMENT_VERTEX_LIST_OFFSET + vertex_count * 12 + 4 + (k * 4) + 0]),
							BitConverter::toSingleFromByte(data[segment_offset + MDL2_SEGMENT_VERTEX_LIST_OFFSET + vertex_count * 12 + 4 + (k * 4) + 1]),
							BitConverter::toSingleFromByte(data[segment_offset + MDL2_SEGMENT_VERTEX_LIST_OFFSET + vertex_count * 12 + 4 + (k * 4) + 2]),

							// uv (god... I forgot how ugly this is...)
							// mapping [0 - 4095] to [0.0 - 1.0]
							BitConverter::toInt16(data, segment_offset + MDL2_SEGMENT_VERTEX_LIST_OFFSET + vertex_count * 12 + 4 + vertex_count * 4 + 4 + k * 8) / 4096.0f,
							abs((BitConverter::toInt16(data, segment_offset + MDL2_SEGMENT_VERTEX_LIST_OFFSET + vertex_count * 12 + 4 + vertex_count * 4 + 4 + k * 8 + 2) / 4096.0f) - 1.0f),

							// skin
							BitConverter::toInt16(&data[0], segment_offset + MDL2_SEGMENT_VERTEX_LIST_OFFSET + vertex_count * 12 + 4 + vertex_count * 4 + 4 + k * 8 + 4) / 4096.0f,
							BitConverter::toInt8(&data[0], segment_offset + MDL2_SEGMENT_VERTEX_LIST_OFFSET + vertex_count * 12 + 4 + vertex_count * 4 + 4 + k * 8 + 6),
							BitConverter::toInt8(&data[0], segment_offset + MDL2_SEGMENT_VERTEX_LIST_OFFSET + vertex_count * 12 + 4 + vertex_count * 4 + 4 + k * 8 + 7),
						};

						submesh.vertices.push_back(vertex);
					}

					for (int k = 0; k < vertex_count - 2; k++)
					{
						submesh.indices.push_back(0 + triangle_index);
						submesh.indices.push_back(2 + triangle_index);
						submesh.indices.push_back(1 + triangle_index);

						triangle_index++;
					}
					triangle_index += 2;

					segment_offset += MDL2_SEGMENT_VERTEX_LIST_OFFSET + vertex_count * 12 + 4 + vertex_count * 4 + 4 + vertex_count * 8;

					while (BitConverter::toInt16(data, segment_offset) != -1)
					{
						segment_offset += 2;
					}
				}

				subobject.submeshes.push_back(submesh);
			}

			subobjects.push_back(subobject);
		}
	}
}
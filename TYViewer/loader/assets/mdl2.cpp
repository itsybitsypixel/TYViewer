#include "mdl2.h"

#include "util/bitconverter.h"
#include "util/stringext.h"

#include <iostream>

bool mdl2::load(const char* buffer, size_t offset)
{
	if (from_bytes<uint32_t>(buffer, 0) != 843859021)
	{
		// Signature check
		return false;
	}

	unsigned int frag_count = from_bytes<uint16_t>(buffer, 4);
	unsigned int subobject_count = from_bytes<uint16_t>(buffer, 6);
	unsigned int collider_count = from_bytes<uint16_t>(buffer, 8);
	unsigned int bone_count = from_bytes<uint16_t>(buffer, 10);

	size_t subobject_offset = from_bytes<uint32_t>(buffer, 12);
	size_t collider_offset = from_bytes<uint32_t>(buffer, 16);
	size_t bone_offset = from_bytes<uint32_t>(buffer, 20);

	bounds =
	{
		from_bytes<float>(buffer, offset + 32),	from_bytes<float>(buffer, offset + 36),	from_bytes<float>(buffer, offset + 40),
		from_bytes<float>(buffer, offset + 48), from_bytes<float>(buffer, offset + 52), from_bytes<float>(buffer, offset + 56),
		from_bytes<float>(buffer, offset + 64), from_bytes<float>(buffer, offset + 68), from_bytes<float>(buffer, offset + 72)
	};

	name = nts(buffer, from_bytes<uint32_t>(buffer, 68));

	subobjects = std::vector<Subobject>(subobject_count);
	for (unsigned int i = 0; i < subobject_count; i++)
	{
		subobjects[i] = parse_subobject(buffer, offset + subobject_offset);

		subobject_offset += 80;
	}

	return true;
}



mdl2::Subobject mdl2::parse_subobject(const char* buffer, size_t offset)
{
	Bounds bounds =
	{
		from_bytes<float>(buffer, offset),		from_bytes<float>(buffer, offset + 4),	from_bytes<float>(buffer, offset + 8),
		from_bytes<float>(buffer, offset + 16), from_bytes<float>(buffer, offset + 20), from_bytes<float>(buffer, offset + 24),
		from_bytes<float>(buffer, offset + 32), from_bytes<float>(buffer, offset + 36), from_bytes<float>(buffer, offset + 40)
	};

	std::string name = nts(buffer, from_bytes<uint32_t>(buffer, offset + 48));
	std::string material = nts(buffer, from_bytes<uint32_t>(buffer, offset + 52));

	unsigned int triangle_count = from_bytes<uint32_t>(buffer, offset + 56);

	unsigned int mesh_count = from_bytes<uint16_t>(buffer, offset + 66);
	size_t mesh_offset = from_bytes<uint32_t>(buffer, offset + 68);

	std::vector<Mesh> meshes(mesh_count);
	for (unsigned int i = 0; i < mesh_count; i++)
	{
		meshes[i] = parse_mesh(buffer, mesh_offset);

		mesh_offset += 16;
	}

	return { bounds, name, material, triangle_count, meshes };
}

mdl2::Mesh mdl2::parse_mesh(const char* buffer, size_t offset)
{
	std::string material = nts(buffer, from_bytes<uint32_t>(buffer, offset));
	uint32_t segment_offset = from_bytes<uint32_t>(buffer, offset + 4);

	unsigned int segment_count = from_bytes<uint32_t>(buffer, offset + 12);

	std::vector<Segment> segments(segment_count);
	for (unsigned int i = 0; i < segment_count; i++)
	{
		size_t size = 0;
		segments[i] = parse_segment(buffer, segment_offset, size);

		segment_offset += size;
	}

	return { material, segments };
}

mdl2::Segment mdl2::parse_segment(const char* buffer, size_t offset, size_t& size)
{
	unsigned int amount_of_vertices = from_bytes<uint32_t>(buffer, offset + 12);
	std::vector<Vertex> vertices(amount_of_vertices);

	size = 52 + (amount_of_vertices * 12) +
		4 + (amount_of_vertices * 4) +
		4 + (amount_of_vertices * 8) +
		4 + (amount_of_vertices * 4);


	// POSITIONS
	for (unsigned int i = 0; i < amount_of_vertices; i++)
	{
		size_t p = offset + 52 + (i * 12);

		float x = from_bytes<float>(buffer, p);
		float y = from_bytes<float>(buffer, p + 4);
		float z = from_bytes<float>(buffer, p + 8);

		vertices[i].position[0] = x;
		vertices[i].position[1] = y;
		vertices[i].position[2] = z;
	}

	// NORMALS
	for (unsigned int i = 0; i < amount_of_vertices; i++)
	{
		size_t p = offset + 52 + (amount_of_vertices * 12) + 4 + (i * 4);

		float x = byte_to_single(buffer, p);
		float y = byte_to_single(buffer, p + 1);
		float z = byte_to_single(buffer, p + 2);

		vertices[i].normal[0] = x;
		vertices[i].normal[1] = y;
		vertices[i].normal[2] = z;
	}

	// TEXCOORDS
	for (unsigned int i = 0; i < amount_of_vertices; i++)
	{
		size_t p = offset + 52 + (amount_of_vertices * 12) + 4 + (amount_of_vertices * 4) + 4 + (i * 8);

		float x = from_bytes<int16_t>(buffer, p) / 4096.0f;
		float y = std::abs((from_bytes<int16_t>(buffer, p + 2) / 4096.0f) - 1.0f);

		vertices[i].texcoord[0] = x;
		vertices[i].texcoord[1] = y;
	}

	// SKIN
	for (unsigned int i = 0; i < amount_of_vertices; i++)
	{
		size_t p = offset + 52 + (amount_of_vertices * 12) + 4 + (amount_of_vertices * 4) + 4 + (i * 8) + 4;

		float x = from_bytes<int16_t>(buffer, p) / 4096.0f;
		float y = (float)from_bytes<int8_t>(buffer, p + 2);
		float z = (float)from_bytes<int8_t>(buffer, p + 3);

		vertices[i].skin[0] = x;
		vertices[i].skin[1] = y;
		vertices[i].skin[2] = z;
	}

	// COLOUR
	for (unsigned int i = 0; i < amount_of_vertices; i++)
	{
		size_t p = offset + 52 + (amount_of_vertices * 12) +
			4 + (amount_of_vertices * 4) +
			4 + (amount_of_vertices * 8) +
			4 + (i * 4);

		float x = byte_to_single(buffer, p);
		float y = byte_to_single(buffer, p + 1);
		float z = byte_to_single(buffer, p + 2);
		float w = byte_to_single(buffer, p + 3);

		vertices[i].colour[0] = x;
		vertices[i].colour[1] = y;
		vertices[i].colour[2] = z;
		vertices[i].colour[3] = w;
	}

	return { vertices };
}

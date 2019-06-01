#pragma once

#include <string>
#include <vector>

class mdl2
{
public:
	struct Bounds;
	struct Vertex;
	struct Mesh;
	struct Subobject;


	struct Bounds
	{
		float x, y, z;		// POSITION
		float sx, sy, sz;	// SIZE
		float ox, oy, oz;	// ORIGIN / UNKNOWN
	};

	struct Vertex
	{
		float position[3]; // XYZ
		float normal[3]; // XYZ
		float texcoord[2]; // UV (XY)
		float skin[3]; // MODIFIER; BONE_A; BONE_B
		float colour[4]; // RGBA
	};

	struct Segment
	{
		std::vector<Vertex> vertices;
	};

	struct Mesh
	{
		std::string material;
		std::vector<Segment> segments;
	};

	struct Subobject
	{
		Bounds bounds;

		std::string name;
		std::string material;

		size_t triangle_count;

		std::vector<Mesh> meshes;
	};

public:
	bool load(const char* buffer, size_t offset);

public:
	Bounds bounds;
	std::string name;

	std::vector<Subobject> subobjects;

private:
	Subobject parse_subobject(const char* buffer, size_t offset);
	Mesh parse_mesh(const char* buffer, size_t offset);
	Segment parse_segment(const char* buffer, size_t offset, size_t& size);
};
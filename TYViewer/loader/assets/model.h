#pragma once

#include <string>
#include <vector>

#include "loader/asset.h"

#define MDL2_SUBMESH_SIZE 80

#define MDL2_SEGMENT_VERTEX_LIST_COUNT_OFFSET	12
#define MDL2_SEGMENT_VERTEX_LIST_OFFSET 52

namespace Loader
{
	struct Model : public Asset
	{
		virtual void loadFromFile(const std::string& filepath) override;
		virtual void loadFromMemory(const char* data, size_t size) override;

		// Version;
		// The version of the MDL file.
		//
		// Previous Krome games used the MDL1 format; not supported.
		//
		// MDL2 ->	Ty the Tasmanian Tiger 1
		// MDL3 ->	Ty the Tasmanian Tiger 2
		//
		enum Version
		{
			MDL2,
			MDL3
		};

		// Vertex;
		// Vertex is the smallest data structure of the model. Each vertex holds a position,
		// texcoords, normals, and skin-data. Skin-data will be described as animations
		// are implemented.
		//
		// x, y, z : 
		//		Position of vertex
		//
		// normalX, normalY, normalZ :
		//		Normals of vertex; lighting and stuff.
		//
		// u, v :
		//		Texcoords of vertex.
		//
		// skinX, skinY, skinZ :
		//		Skin of the vertex; skinX is a float (0.0f - 1.0f) that interpolates between
		//		skinZ and skinY respectively. This means that a vertex can have two attached
		//		bones while skinX controls the strength of each bone.
		//			The reason that they are named x, y, z is to not mistakenly place them
		//		differently when making them into a 3D vector later on.
		//
		struct Vertex
		{
			float x, y, z;
			float normalX, normalY, normalZ;
			float u, v;
			float skinX, skinY, skinZ;
		};

		// Bounds;
		// Used by 'Subobject' to define bounds.
		struct Bounds
		{
			float x, y, z;
			float sizeX, sizeY, sizeZ;
			float originX, originY, originZ;
		};


		// Submesh;
		// Submeshes are the divisions of a subobject used for holding LODs for an
		// object. <-- [FACT CHECK]
		// It also holds different meshes that a subobject should switch between
		// for example Ty's head when he bites.
		//
		// ident : 
		//		The identifier is used to load texture and data related to
		//		texcoords and drawing. It's identifier is specified in the
		//		'global.mad'. (Ty 1)
		//
		struct Submesh
		{
			std::string ident;


			std::vector<Vertex> vertices;
			std::vector<unsigned int> indices;
		};


		// Subobject;
		// Subobject are the divisions of the model.
		// 
		// name :	
		//		The name of the subobject. Mentioned in 'global.model'
		//		where it's used by the LOD functionality to say which
		//		models should currently be loaded.
		//
		// ident :	
		//		The identifier is used by the game to identify what an
		//		object should act as (drawable, collider, etc) and it's
		//		data is stored in 'global.model'. (Ty 1)
		//
		// bounds :
		//		The bounds of the subobject where all submeshes are
		//		contained withint. Probably used to determine if
		//		the object is seen by the camera and should be drawn.
		//
		struct Subobject
		{
			std::string name;
			std::string ident;

			Bounds bounds;

			std::vector<Submesh> submeshes;
		};


		std::vector<Subobject> subobjects; // Subobjects; holds all subobjects of model.
	};
}
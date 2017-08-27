#pragma once
#include "stdafx.h"
#include <scene.h>
/*
struct Material
{
	Material()
		: Ambient(0, 0, 0, 0), Diffuse(0, 0, 0, 0), Specular(0, 0, 0, 0), Reflect(0, 0, 0, 0)
	{}

	DirectX::XMFLOAT4 Ambient;
	DirectX::XMFLOAT4 Diffuse;
	DirectX::XMFLOAT4 Specular;
	DirectX::XMFLOAT4 Reflect;
};
*/
struct modelBone
{
	modelBone()
	{
		ZeroMemory(this, sizeof(modelBone));
		XMStoreFloat4x4(&Offset, XMMatrixIdentity());
	}
	string Name;

	XMFLOAT4X4 Offset;
};

// Holds mesh subset data
struct modelMesh
{
	modelMesh()
	{
		ZeroMemory(this, sizeof(modelMesh));
	}

	// The count of vertices that make up the mesh
	UINT VertexCount;

	// The count of faces that makes up the mesh
	UINT FaceCount;

	// The count of indices that makes up the mesh
	UINT IndexCount;

	// The diffuse texture for this mesh
	wstring DiffuseTexturePath;

	// The normal texture for this mesh
	wstring NormalTexturePath;

	// The material for this node
	//Material MeshMaterial;

	// The names of the bone nodes that affect this mesh
	vector<modelBone> Bones;

	// Animated vertex buffer
	//vector<vertex_ptn_skinned_xm> AnimatedVertices;

	// Static vertex buffer
	//vector<vertex_ptn_xm> StaticVertices;

	// Indices
	vector<UINT> Indices;

	// If there are less than 65536 indices we can save the indices as 16 bit instead of 32
	bool SaveIndicesAs16Bit;

	// opacity for the material
	float MaterialOpacity;

	// shininess for the material
	float MaterialShininess;

	// strength of the specular highlight
	float SpecularStrength;

	// The number of bones associated with this mesh
	UINT NumBones;

	// The material for this node
	//Material MeshMaterial;

	// The bounding box surrounding the vertices in this mesh
	
};

struct modelNode
{
	modelNode()
	{
		ZeroMemory(this, sizeof(modelNode));

		XMStoreFloat4x4(&LocalTransform, XMMatrixIdentity());
		XMStoreFloat4x4(&GlobalTransform, XMMatrixIdentity());

	}
	modelNode(_In_ aiNode* node)
	{
		ZeroMemory(this, sizeof(modelNode));

		XMStoreFloat4x4(&LocalTransform, XMMatrixIdentity());
		XMStoreFloat4x4(&GlobalTransform, XMMatrixIdentity());
		
		this->Name = node->mName.C_Str();

		// Convert LocalTransform from aiMatrix to XMFLOAT4X4
		DirectX::XMMATRIX* tm = reinterpret_cast<DirectX::XMMATRIX*>(&node->mTransformation);
		DirectX::XMMATRIX om(*tm);
		XMStoreFloat4x4(&LocalTransform, om);
	}

	

	// Holds the starting vertex for each mesh subset
	vector<modelMesh> Meshs;

	// The child nodes to this one
	std::vector<modelNode*> Children;

	// The parent node to this one
	modelNode* Parent;

	// Most recently calculated local transform
	XMFLOAT4X4 LocalTransform;

	// Most recently calculated global transform. This includes the local bone transform if applicable
	XMFLOAT4X4 GlobalTransform;

	// Index in the current animation's channel array. -1 if not animated.
	size_t ChannelIndex;

	// The name of this node
	string Name;

};


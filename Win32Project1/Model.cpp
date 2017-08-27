#include "stdafx.h"
#include "Model.h"
#include <vector>
#include "animationData.h"

/*
struct DirectionalLight {
	D3DXVECTOR4 Ambient;
	D3DXVECTOR4 Diffuse;
	D3DXVECTOR4 Specular;
	D3DXVECTOR3 Direction;
};
DirectionalLight DirLights[3];

struct Material {
	D3DXVECTOR4 Ambient;
	D3DXVECTOR4 Diffuse;
	D3DXVECTOR4 Specular;
};

Material Mat;
*/

Model::Model()
{
	/*
	DirLights[0].Ambient = D3DXVECTOR4(0.2f, 0.2f, 0.2f, 1.0f);
	DirLights[0].Diffuse = D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.0f);
	DirLights[0].Specular = D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.0f);
	DirLights[0].Direction = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	Mat.Ambient = D3DXVECTOR4(0.48f, 0.77f, 0.46f, 1.0f);
	Mat.Diffuse = D3DXVECTOR4(0.48f, 0.77f, 0.46f, 1.0f);
	Mat.Specular = D3DXVECTOR4(0.2f, 0.2f, 0.2f, 16.0f);*/
}

Model::~Model()
{
	SAFE_RELEASE(m_LightBuffer);

	std::vector<MeshInfo>::iterator itMeshInfo;
	for (itMeshInfo = m_vecMesh.begin(); itMeshInfo != m_vecMesh.end(); ++itMeshInfo)
		SAFE_DELETE(itMeshInfo->ptexture);

	std::map<string, modelNode*>::iterator itNode;
	for (itNode = m_mapNode.begin(); itNode != m_mapNode.end(); ++itNode)
		SAFE_DELETE(itNode->second);
}

bool Model::checkSaveFileExists(string fileName)
{
	fileName += ".model";

	ifstream fin;
	fin.open(fileName.c_str(), ios::in | ios::binary);

	fin.seekg(0, ios::end);
	int size = fin.tellg();
	fin.seekg(0, ios::beg);

	fin.close();

	if (size > 0)
	{
		return true;
	}


	return false;
}

void Model::save(string fileName)
{
	fileName += ".model";

	ofstream fout;
	fout.open(fileName.c_str(), ios::out | ios::binary);

	int clipSize = m_vecCacheClips.size();
	fout.write((char*)&clipSize, sizeof(int));

	for (int clipIdx = 0; clipIdx < clipSize; ++clipIdx)
	{
		int frameSize = m_vecCacheClips[clipIdx].size();
		fout.write((char*)&frameSize, sizeof(int));

		for (int frameIdx = 0; frameIdx < frameSize; ++frameIdx)
		{
			int meshSize = m_vecCacheClips[clipIdx][frameIdx].size();
			fout.write((char*)&meshSize, sizeof(int));

			for (int meshIdx = 0; meshIdx < meshSize; ++meshIdx)
			{
				int boneTMSize = m_vecCacheClips[clipIdx][frameIdx][meshIdx].size();
				fout.write((char*)&boneTMSize, sizeof(int));
				if (boneTMSize > 0)
				{
					fout.write((char*)&(m_vecCacheClips[clipIdx][frameIdx][meshIdx][0]), sizeof(XMFLOAT4X4)*boneTMSize);
				}
			}
		}
	}

	int size = m_mapAnimClipNameIdx.size();
	fout.write((char*)&size, sizeof(int));


	map<string, int>::iterator it_map;
	for (it_map = m_mapAnimClipNameIdx.begin(); it_map != m_mapAnimClipNameIdx.end(); ++it_map)
	{
		int stringlen = it_map->first.length();
		fout.write((char*)&stringlen, sizeof(int));
		fout.write((char*)&(it_map->first[0]), it_map->first.length());
		fout.write((char*)&(it_map->second), sizeof(int));
	}

	size = m_indices.size();
	fout.write((char*)&size, sizeof(int));


	for (int i = 0; i < m_indices.size(); ++i)
	{
		fout.write((char*)&(m_indices[i]), sizeof(UINT));
	}

	size = m_vecMesh.size();
	fout.write((char*)&size, sizeof(int));


	for (int i = 0; i < m_vecMesh.size(); ++i)
	{
		fout.write((char*)&(m_vecMesh[i].cntIndex), sizeof(int));
		fout.write((char*)&(m_vecMesh[i].cntVertex), sizeof(int));
		fout.write((char*)&(m_vecMesh[i].indexStart), sizeof(int));
		fout.write((char*)&(m_vecMesh[i].vertexStart), sizeof(int));
	}

	size = m_vertex_skinned_xm.size();
	fout.write((char*)&size, sizeof(int));


	for (int i = 0; i < m_vertex_skinned_xm.size(); ++i)
	{
		fout.write((char*)&(m_vertex_skinned_xm[i]), sizeof(vertex_ptn_skinned_xm));
	}


	size = m_AnimationClips.size();
	fout.write((char*)&size, sizeof(int));


	for (int i = 0; i < m_AnimationClips.size(); ++i)
	{
		int stringlen = m_AnimationClips[i].Name.length();

		fout.write((char*)&(m_AnimationClips[i].TicksPerSecond), sizeof(float));
		fout.write((char*)&(m_AnimationClips[i].Duration), sizeof(float));
		fout.write((char*)&stringlen, sizeof(int));
		fout.write((char*)m_AnimationClips[i].Name.c_str(), m_AnimationClips[i].Name.length());
		fout.write((char*)&(m_AnimationClips[i].TotalFrames), sizeof(UINT));

	}


	size = m_mapTextureFileNames.size();
	fout.write((char*)&size, sizeof(int));

	map<int, string>::iterator it_mapTexture=m_mapTextureFileNames.begin();

	for (; it_mapTexture != m_mapTextureFileNames.end(); ++it_mapTexture)
	{
		int stringlen = it_mapTexture->second.length();
		string data = it_mapTexture->second;

		int first = it_mapTexture->first;
		
		fout.write((char*)&first, 4);

		fout.write((char*)&stringlen, 4);
		fout.write((char*)&(data[0]), stringlen);
	}


	fout.close();

}

void Model::load(string fileName)
{

	fileName += ".model";

	int clipSize = 0;

	m_vecCacheClips.clear();

	ifstream fin;
	fin.open(fileName.c_str(), ios::in | ios::binary);

	fin.read((char*)&clipSize, sizeof(int));
	m_vecCacheClips.resize(clipSize);

	for (int clipIdx = 0; clipIdx < clipSize; ++clipIdx)
	{
		int frameSize;
		fin.read((char*)&frameSize, sizeof(int));


		for (int frameIdx = 0; frameIdx < frameSize; ++frameIdx)
		{
			int meshSize;
			fin.read((char*)&meshSize, sizeof(int));

			m_vecCacheClips[clipIdx][frameIdx].resize(meshSize);

			for (int meshIdx = 0; meshIdx < meshSize; ++meshIdx)
			{
				int boneTMSize;
				fin.read((char*)&boneTMSize, sizeof(int));

				m_vecCacheClips[clipIdx][frameIdx][meshIdx].resize(boneTMSize);
				if (boneTMSize > 0)
				{
					fin.read((char*)&(m_vecCacheClips[clipIdx][frameIdx][meshIdx][0]), sizeof(XMFLOAT4X4)*boneTMSize);
				}
			}
		}
	}

	//m_mapAnimClipNameIdx
	int size;
	fin.read((char*)&size, 4);

	for (int i = 0; i < size; ++i)
	{
		int stringlen;
		string first;
		int second;

		fin.read((char*)&(stringlen), 4);
		first.resize(stringlen);
		fin.read((char*)&(first[0]), stringlen);
		fin.read((char*)&second, 4);

		m_mapAnimClipNameIdx[first] = second;
	}

	//m_indices
	fin.read((char*)&size, 4);
	m_indices.resize(size);

	for (int i = 0; i < size; ++i)
	{
		int index;
		fin.read((char*)&index, 4);
		m_indices[i] = index;
	}

	//m_vecMesh

	fin.read((char*)&size, 4);
	m_vecMesh.resize(size);

	for (int i = 0; i < size; ++i)
	{
		MeshInfo meshinfo = {};


		fin.read((char*)&(meshinfo.cntIndex), sizeof(int));
		fin.read((char*)&(meshinfo.cntVertex), sizeof(int));
		fin.read((char*)&(meshinfo.indexStart), sizeof(int));
		fin.read((char*)&(meshinfo.vertexStart), sizeof(int));

		m_vecMesh[i] = meshinfo;
	}

	//m_vertex_skinned_xm

	fin.read((char*)&size, 4);
	m_vertex_skinned_xm.resize(size);

	for (int i = 0; i < size; ++i)
	{
		vertex_ptn_skinned_xm vertex = {};

		fin.read((char*)(&vertex), sizeof(vertex_ptn_skinned_xm));

		m_vertex_skinned_xm[i] = vertex;
	}

	//m_AnimationClips

	fin.read((char*)&size, 4);
	m_AnimationClips.resize(size);

	for (int i = 0; i < size; ++i)
	{
		float tickspersecond, duration;
		int stringlen;
		string name;
		UINT totalframes;

		fin.read((char*)&(tickspersecond), sizeof(float));
		fin.read((char*)&(duration), sizeof(float));
		fin.read((char*)&stringlen, sizeof(int));

		name.resize(stringlen);
		fin.read((char*)&(name[0]), stringlen);

		fin.read((char*)&(totalframes), sizeof(UINT));

		m_AnimationClips[i].TicksPerSecond = tickspersecond;
		m_AnimationClips[i].Duration = duration;
		m_AnimationClips[i].Name = name;
		m_AnimationClips[i].TotalFrames = totalframes;

	}
	
	//m_vecTextureFileNames
	//also!!! setting textures 

	fin.read((char*)&size, 4);
	
	for (int i = 0; i < size; ++i)
	{
		int stringlen;
		string data;

		int first;
		fin.read((char*)&first, 4);

		fin.read((char*)&stringlen, 4);
		data.resize(stringlen);
		fin.read((char*)(&data[0]), stringlen);

		m_mapTextureFileNames[first] = data;

		m_vecMesh[first].ptexture = new textureclass;
		m_vecMesh[first].ptexture->load(data);
	}



	fin.close();
}



void Model::init(char * dir, char * FileName)
{

	Init_compileShader("./cube_normal_vs.hlsl", "./cube_normal_ps.hlsl");
	Init_polygonLayout(EPolygonLayout::LAYOUT_PTN_SKINNED);
	Init_CreateConstantBuffer(&m_LightBuffer, sizeof(LightBuffer));
	Init_CreateConstantBuffer(&m_boneBuffer, sizeof(XMFLOAT4X4) * 96);
	Init_CreateConstantBuffer(&m_AlphaBlendBuffer, sizeof(AlphaBlendBuffer));

	m_dir = dir;
	m_fileName = FileName;

	if (checkSaveFileExists(FileName) == false) //no save file found.
	{


		Assimp::Importer importer;
		_aiScene = importer.ReadFile(m_dir + m_fileName,
			aiProcess_JoinIdenticalVertices |		// join identical vertices/ optimize indexing
			aiProcess_ValidateDataStructure |		// perform a full validation of the loader's output
			aiProcess_ImproveCacheLocality |		// improve the cache locality of the output vertices
			aiProcess_RemoveRedundantMaterials |	// remove redundant materials
			aiProcess_GenUVCoords |					// convert spherical, cylindrical, box and planar mapping to proper UVs
			aiProcess_TransformUVCoords |			// pre-process UV transformations (scaling, translation ...)
			aiProcess_FindInstances |				// search for instanced meshes and remove them by references to one master
			aiProcess_LimitBoneWeights |			// limit bone weights to 4 per vertex
			aiProcess_OptimizeMeshes |				// join small meshes, if possible;
			aiProcess_GenSmoothNormals |			// generate smooth normal vectors if not existing
			aiProcess_SplitLargeMeshes |			// split large, unrenderable meshes into sub-meshes
			aiProcess_Triangulate |					// triangulate polygons with more than 3 edges
			//aiProcess_ConvertToLeftHanded |			// convert everything to D3D left handed space
			aiProcess_SortByPType);					// make 'clean' meshes which consist of a single type of primitives

		// If the import failed, report it
		if (!_aiScene)
		{
			ERROR_MSGBOX(true, "model import error");
		}

		//
		// Read in global stats
		m_NumMeshes = _aiScene->mNumMeshes;
		m_NumMaterials = _aiScene->mNumMaterials;
		m_NumTextures = _aiScene->mNumTextures;
		m_NumAnimations = _aiScene->mNumAnimations;
		m_HasAnimations = m_NumAnimations > 0;

		m_tmpBaseVertexidx = 0;
		m_tmpBaseFaceIdx = 0;
		m_totalBoneNum = 0;

		RootNode = createNodeTree(_aiScene->mRootNode, nullptr);

		//loadMaterial()

		if (m_HasAnimations)
		{
			extractAnimations();

			cache();
		}

		settingTextures(_aiScene, m_dir);

		save(FileName);
	}
	else //save file found
	{
		load(FileName);
		if (m_vecCacheClips.size() > 0)
		{
			m_HasAnimations = true;
		}
		else
		{
			m_HasAnimations = false;
		}
	}

	m_AnimName = "Idle";


	Init_CreateVertexBuffer(&m_vertex_skinned_xm[0], sizeof(vertex_ptn_skinned_xm), m_vertex_skinned_xm.size(),
		D3D11_USAGE_DEFAULT, D3D11_BIND_VERTEX_BUFFER, 0);

	Init_option_CreateIndexBuffer(&m_indices[0], sizeof(DWORD), m_indices.size(),
		D3D11_USAGE_DEFAULT, D3D11_BIND_INDEX_BUFFER, 0);


	m_lightProperties.diffuseColor = D3DXVECTOR4(1, 1, 1, 1);
	m_lightProperties.lightDirection = D3DXVECTOR3(0.0f, -1.f, 0.f);

	m_alphaProperty.alpha = 1.f;

	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	gameUtil.getDevice()->CreateSamplerState(&samplerDesc, &m_sampleState);

//	D3DXComputeBoundingBox()
}

/*
void Model::loadAnimation(string fulldir)
{

	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(fulldir,
		aiProcess_JoinIdenticalVertices |		// join identical vertices/ optimize indexing
		aiProcess_ValidateDataStructure |		// perform a full validation of the loader's output
		aiProcess_ImproveCacheLocality |		// improve the cache locality of the output vertices
		aiProcess_RemoveRedundantMaterials |	// remove redundant materials
		aiProcess_GenUVCoords |					// convert spherical, cylindrical, box and planar mapping to proper UVs
		aiProcess_TransformUVCoords |			// pre-process UV transformations (scaling, translation ...)
		aiProcess_FindInstances |				// search for instanced meshes and remove them by references to one master
		aiProcess_LimitBoneWeights |			// limit bone weights to 4 per vertex
		aiProcess_OptimizeMeshes |				// join small meshes, if possible;
		aiProcess_GenSmoothNormals |			// generate smooth normal vectors if not existing
		aiProcess_SplitLargeMeshes |			// split large, unrenderable meshes into sub-meshes
		aiProcess_Triangulate |					// triangulate polygons with more than 3 edges
		aiProcess_ConvertToLeftHanded |			// convert everything to D3D left handed space
		aiProcess_SortByPType);					// make 'clean' meshes which consist of a single type of primitives

	m_HasAnimations = scene->HasAnimations() > 0;

	for (size_t j = 0; j < scene->mNumAnimations; j++)
	{
		const aiAnimation* sourceAnimation = scene->mAnimations[j];
		AnimationClip animation;

		animation.TicksPerSecond = static_cast<float>(sourceAnimation->mTicksPerSecond);
		animation.Duration = static_cast<float>(sourceAnimation->mDuration);
		string name = sourceAnimation->mName.C_Str();
		animation.Name = name;

		for (UINT a = 0; a < sourceAnimation->mNumChannels; a++)
		{
			BoneAnimation boneAnimation;

			string tmp(sourceAnimation->mChannels[a]->mNodeName.data);
			boneAnimation.Name = tmp;

			for (unsigned int i = 0; i < sourceAnimation->mChannels[a]->mNumPositionKeys; i++)
			{
				Keyframe keyFrame;
				keyFrame.Translation.x = sourceAnimation->mChannels[a]->mPositionKeys[i].mValue.x;
				keyFrame.Translation.y = sourceAnimation->mChannels[a]->mPositionKeys[i].mValue.y;
				keyFrame.Translation.z = sourceAnimation->mChannels[a]->mPositionKeys[i].mValue.z;

				keyFrame.RotationQuat.x = sourceAnimation->mChannels[a]->mRotationKeys[i].mValue.x;
				keyFrame.RotationQuat.y = sourceAnimation->mChannels[a]->mRotationKeys[i].mValue.y;
				keyFrame.RotationQuat.z = sourceAnimation->mChannels[a]->mRotationKeys[i].mValue.z;
				keyFrame.RotationQuat.w = sourceAnimation->mChannels[a]->mRotationKeys[i].mValue.w;

				keyFrame.Scale.x = sourceAnimation->mChannels[a]->mScalingKeys[i].mValue.x;
				keyFrame.Scale.y = sourceAnimation->mChannels[a]->mScalingKeys[i].mValue.y;
				keyFrame.Scale.z = sourceAnimation->mChannels[a]->mScalingKeys[i].mValue.z;

				keyFrame.TimePos = static_cast<float>(sourceAnimation->mChannels[a]->mPositionKeys[i].mTime);

				boneAnimation.Keyframes.push_back(keyFrame);
			}

			animation.TotalFrames = animation.Duration*30.0f;

			animation.BoneAnimations.push_back(boneAnimation);
		}
		m_AnimationClips.push_back(animation);
	}

	int a = 0;
}
*/


modelNode* Model::createNodeTree(aiNode * node, modelNode* parent)
{
	// create a KsmModelNode from the aiNode
	modelNode* ModelNode = new modelNode(node);
	ModelNode->Parent = parent;

	// store the node by name for easy reference and counting total nodes
	m_mapNode[node->mName.C_Str()] = ModelNode;

	calculateGlobalTransform(ModelNode);

	// find the index of the animation track affecting this node, if any
	ModelNode->ChannelIndex = -1;
	bool channelFound = false;
	for (UINT i = 0; i < _aiScene->mNumAnimations; ++i)
	{
		if (channelFound)
		{
			break;
		}
		const aiAnimation* currentAnim = _aiScene->mAnimations[i];
		for (unsigned int c = 0; c < currentAnim->mNumChannels; c++)
		{
			if (!strcmp(currentAnim->mChannels[c]->mNodeName.C_Str(), ModelNode->Name.c_str()))
			{
				ModelNode->ChannelIndex = c;
				channelFound = true;
				break;
			}
		}
	}

	// Load the meshes that belong to this node
	for (UINT m = 0; m < node->mNumMeshes; ++m)
	{
		loadMeshData(ModelNode, _aiScene->mMeshes[node->mMeshes[m]]);
	}

	// continue for all child nodes and assign the created internal nodes as our children
	for (unsigned int c = 0; c < node->mNumChildren; c++)
	{
		modelNode* childNode = createNodeTree(node->mChildren[c], ModelNode);
		ModelNode->Children.push_back(childNode);
	}

	return ModelNode;
}


void Model::loadMeshData(modelNode * ownerNode, const aiMesh * mesh)
{
	modelMesh ModelMesh;

	// Read global data
	ModelMesh.NumBones = mesh->mNumBones;
	ModelMesh.FaceCount = mesh->mNumFaces;
	ModelMesh.VertexCount = mesh->mNumVertices;

	MeshInfo meshinfo = {};

	// create the material for the mesh
	//loadMaterial(&ModelMesh, mesh);

	unsigned int nidx;
	switch (mesh->mPrimitiveTypes) {
	case aiPrimitiveType_POINT:
		nidx = 1; break;
	case aiPrimitiveType_LINE:
		nidx = 2; break;
	case aiPrimitiveType_TRIANGLE:
		nidx = 3; break;
	default:
		nidx = 3;
	};

	// check whether we can use 16 bit indices
	ModelMesh.SaveIndicesAs16Bit = true;
	if (mesh->mNumFaces * nidx >= 65536)
	{
		ModelMesh.SaveIndicesAs16Bit = false;
	}

	// now fill the index buffer
	for (unsigned int x = 0; x < mesh->mNumFaces; ++x)
	{
		for (unsigned int a = 0; a < nidx; ++a)
		{
			ModelMesh.Indices.push_back(mesh->mFaces[x].mIndices[a]);
			m_indices.push_back(mesh->mFaces[x].mIndices[a]);
		}
	}

	ModelMesh.IndexCount = static_cast<UINT>(ModelMesh.Indices.size());

	// collect weights on all vertices
	vector<vector<aiVertexWeight> > weightsPerVertex(mesh->mNumVertices);
	for (unsigned int a = 0; a < mesh->mNumBones; a++)
	{
		const aiBone* bone = mesh->mBones[a];
		for (unsigned int b = 0; b < bone->mNumWeights; b++)
		{
			weightsPerVertex[bone->mWeights[b].mVertexId].push_back(aiVertexWeight(a, bone->mWeights[b].mWeight));
		}
	}

	/*
	// now fill the vertex buffer
	if (m_HasAnimations)
	{
		ModelMesh.AnimatedVertices.resize(mesh->mNumVertices);
	}
	else
	{
		ModelMesh.StaticVertices.resize(mesh->mNumVertices);
	}*/

	for (unsigned int x = 0; x < mesh->mNumVertices; ++x)
	{
		XMFLOAT3 position;
		XMFLOAT3 normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
		XMFLOAT2 texture = XMFLOAT2(0.5f, 0.5f);
		unsigned char boneIndices[4] = { 0, 0, 0, 0 };
		float boneWeights[4] = { 0, 0, 0, 0 };

		position.x = mesh->mVertices[x].x;
		position.y = mesh->mVertices[x].y;
		position.z = mesh->mVertices[x].z;

		if (NULL != mesh->mNormals)
		{
			normal.x = mesh->mNormals[x].x;
			normal.y = mesh->mNormals[x].y;
			normal.z = mesh->mNormals[x].z;
		}

		if (mesh->HasTextureCoords(0))
		{
			texture.x = mesh->mTextureCoords[0][x].x;
			texture.y = mesh->mTextureCoords[0][x].y;
		}

		// Bone indices and weights
		if (mesh->HasBones())
		{
			assert(weightsPerVertex[x].size() <= 4);
			for (unsigned int a = 0; a < weightsPerVertex[x].size(); a++)
			{
				boneIndices[a] = weightsPerVertex[x][a].mVertexId;
				boneWeights[a] = weightsPerVertex[x][a].mWeight;
			}
		}

		//load vertex
		vertex_ptn_skinned_xm vertex = {};
		vertex.pos = position;
		vertex.normal = normal;
		vertex.uv = texture;
		vertex.weights[0] = boneWeights[0];
		vertex.weights[1] = boneWeights[1];
		vertex.weights[2] = boneWeights[2];
		vertex.weights[3] = 1.0f - (boneWeights[0] + boneWeights[1] + boneWeights[2]);
		//vertex.weights[3] = vertex.weights[3] < 0.0f ? 0.0f : vertex.weights[3];

		vertex.boneId[0] = boneIndices[0];
		vertex.boneId[1] = boneIndices[1];
		vertex.boneId[2] = boneIndices[2];
		vertex.boneId[3] = boneIndices[3];

		//ModelMesh.AnimatedVertices[x] = vertex;

		m_vertex_skinned_xm.push_back(vertex);

		/*
		if (m_HasAnimations)
		{
			vertex_ptn_skinned_xm vertex = {};
			vertex.pos = position;
			vertex.normal = normal;
			vertex.uv = texture;
			vertex.weights[0] = boneWeights[0];
			vertex.weights[1] = boneWeights[1];
			vertex.weights[2] = boneWeights[2];
			vertex.weights[3] = 1.0f - (boneWeights[0] + boneWeights[1] + boneWeights[2]);
			//vertex.weights[3] = vertex.weights[3] < 0.0f ? 0.0f : vertex.weights[3];

			vertex.boneId[0] = boneIndices[0];
			vertex.boneId[1] = boneIndices[1];
			vertex.boneId[2] = boneIndices[2];
			vertex.boneId[3] = boneIndices[3];

			ModelMesh.AnimatedVertices[x] = vertex;

			m_vertex_skinned_xm.push_back(vertex);
		}
		else
		{
			vertex_ptn_xm vertex = {};
			vertex.pos = position;
			vertex.normal = normal;
			vertex.uv = texture;

			ModelMesh.StaticVertices[x] = vertex;

			m_vertices_static.push_back(vertex);
		}*/
	}

	//
	// Load Bones
	for (UINT b = 0; b < mesh->mNumBones; ++b)
	{
		modelBone ModelBone;
		aiBone* bone = mesh->mBones[b];

		ModelBone.Name = bone->mName.C_Str();


		XMMATRIX* tm = reinterpret_cast<XMMATRIX*>(&bone->mOffsetMatrix);
		XMMATRIX om(*tm);
		XMStoreFloat4x4(&ModelBone.Offset, om);

		ModelMesh.Bones.push_back(ModelBone);

		m_mapBoneOffsetTM[ModelBone.Name] = ModelBone.Offset;
		m_totalBoneNum++;
		/*
		if (m_mapBoneOffsetTM.find(ModelBone.Name) == m_mapBoneOffsetTM.end())
		{
			m_mapBoneOffsetTM[ModelBone.Name] = ModelBone.Offset;
			m_totalBoneNum++;
		}*/

		meshinfo.vecBoneNames.push_back(ModelBone.Name);
	}

	ownerNode->Meshs.push_back(ModelMesh);

	meshinfo.vecBoneTransforms.resize(meshinfo.vecBoneNames.size());

	meshinfo.cntVertex = mesh->mNumVertices;
	meshinfo.cntIndex = mesh->mNumFaces * 3;

	meshinfo.indexStart = m_tmpBaseFaceIdx;
	meshinfo.vertexStart = m_tmpBaseVertexidx;

	m_tmpBaseVertexidx += mesh->mNumVertices;
	m_tmpBaseFaceIdx += mesh->mNumFaces * 3;

	aiString aiPath;
	aiMaterial* material = _aiScene->mMaterials[mesh->mMaterialIndex];
	if ((material->GetTexture(aiTextureType_DIFFUSE, 0, &aiPath)) == AI_SUCCESS)
	{
		meshinfo.ptexture = new textureclass();
	}

	m_vecMesh.push_back(meshinfo);

}

void Model::calculateGlobalTransform(modelNode * ModelModelNode)
{
	// Concatenate all parent transforms to get the global transform for this node
	ModelModelNode->GlobalTransform = ModelModelNode->LocalTransform;
	modelNode* parent = ModelModelNode->Parent;

	while (parent)
	{
		XMMATRIX lt = XMLoadFloat4x4(&parent->LocalTransform);
		XMMATRIX gt = XMLoadFloat4x4(&ModelModelNode->GlobalTransform);
		XMMATRIX result = lt * gt;
		XMStoreFloat4x4(&ModelModelNode->GlobalTransform, result);

		parent = parent->Parent;
	}
}

void Model::extractAnimations()
{

	for (size_t j = 0; j < _aiScene->mNumAnimations; j++)
	{
		int max = 0;
		const aiAnimation* sourceAnimation = _aiScene->mAnimations[j];
		AnimationClip animation;

		animation.TicksPerSecond = static_cast<float>(sourceAnimation->mTicksPerSecond);
		animation.Duration = static_cast<float>(sourceAnimation->mDuration);
		string name = sourceAnimation->mName.C_Str();
		animation.Name = name;

		for (UINT a = 0; a < sourceAnimation->mNumChannels; a++)
		{
			BoneAnimation boneAnimation;

			string tmp(sourceAnimation->mChannels[a]->mNodeName.data);
			boneAnimation.Name = tmp;

			for (unsigned int i = 0; i < sourceAnimation->mChannels[a]->mNumPositionKeys; i++)
			{
				Keyframe keyFrame;
				keyFrame.Translation.x = sourceAnimation->mChannels[a]->mPositionKeys[i].mValue.x;
				keyFrame.Translation.y = sourceAnimation->mChannels[a]->mPositionKeys[i].mValue.y;
				keyFrame.Translation.z = sourceAnimation->mChannels[a]->mPositionKeys[i].mValue.z;

				keyFrame.RotationQuat.x = sourceAnimation->mChannels[a]->mRotationKeys[i].mValue.x;
				keyFrame.RotationQuat.y = sourceAnimation->mChannels[a]->mRotationKeys[i].mValue.y;
				keyFrame.RotationQuat.z = sourceAnimation->mChannels[a]->mRotationKeys[i].mValue.z;
				keyFrame.RotationQuat.w = sourceAnimation->mChannels[a]->mRotationKeys[i].mValue.w;

				keyFrame.Scale.x = sourceAnimation->mChannels[a]->mScalingKeys[i].mValue.x;
				keyFrame.Scale.y = sourceAnimation->mChannels[a]->mScalingKeys[i].mValue.y;
				keyFrame.Scale.z = sourceAnimation->mChannels[a]->mScalingKeys[i].mValue.z;

				keyFrame.TimePos = static_cast<float>(sourceAnimation->mChannels[a]->mPositionKeys[i].mTime);

				boneAnimation.Keyframes.push_back(keyFrame);
			}
			int numKeyFrames = boneAnimation.Keyframes.size();
			//animation.TotalFrames = max < numKeyFrames ? numKeyFrames : max;
			animation.TotalFrames = (animation.Duration*30.0f) / animation.TicksPerSecond;
			animation.BoneAnimations.push_back(boneAnimation);
		}
		m_AnimationClips.push_back(animation);
	}
}


void Model::updateNodeTree(modelNode * node)
{

	calculateGlobalTransform(node);


	for (unsigned int c = 0; c < node->Children.size(); c++)
	{
		updateNodeTree(node->Children[c]);
	}
}

void Model::updateBoneFinalTM()
{

	for (int i = 0; i < m_vecMesh.size(); ++i)
	{
		for (int j = 0; j < m_vecMesh[i].vecBoneNames.size(); ++j)
		{
			string name = m_vecMesh[i].vecBoneNames[j];

			XMMATRIX gt = XMLoadFloat4x4(&m_mapNode[name]->GlobalTransform);
			XMMATRIX offset = XMLoadFloat4x4(&m_mapBoneOffsetTM[name]);


			XMStoreFloat4x4(&m_vecMesh[i].vecBoneTransforms[j], XMMatrixMultiply(gt, offset));
		}
	}

}

void Model::cache()
{

	for (int clipIdx = 0; clipIdx < m_AnimationClips.size(); ++clipIdx)
	{
		map<int, vector<vector< XMFLOAT4X4>> > mapCacheFinalTM;
		vector<vector<XMFLOAT4X4>> tmpTransforms;

		m_mapAnimClipNameIdx[m_AnimationClips[clipIdx].Name] = clipIdx;

		int totalFrames = m_AnimationClips[clipIdx].TotalFrames;

		for (int frame = 0; frame <= totalFrames; ++frame)
		{
			vector<XMFLOAT4X4> tmp;
			m_AnimationClips[clipIdx].Interpolate(m_AnimationClips[clipIdx].GetTimeStepAtFrame(frame), tmp);

			tmpTransforms.push_back(tmp);
		}

		//loop all frames of one animation
		for (int frame = 0; frame <= totalFrames; ++frame)
		{
			for (int j = 0; j < tmpTransforms[frame].size(); ++j)
			{
				string boneName = m_AnimationClips[clipIdx].getBoneNameAt(j);
				m_mapNode[boneName]->LocalTransform = tmpTransforms[frame][j];
			}
			updateNodeTree(m_mapNode[RootNode->Name]);
			updateBoneFinalTM();

			vector<vector<XMFLOAT4X4>> boneTransforms;

			for (int c = 0; c < m_vecMesh.size(); ++c)
			{
				boneTransforms.push_back(m_vecMesh[c].vecBoneTransforms);
			}
			mapCacheFinalTM[frame] = boneTransforms;

		}
		m_vecCacheClips.push_back(mapCacheFinalTM);
	}

}

void Model::update()
{
	
	

	if (m_HasAnimations)
	{
		//animate
		m_frame += gameTimer.getDeltaTime() * 30;

		if (m_frame > m_AnimationClips[m_mapAnimClipNameIdx[m_AnimName]].TotalFrames)
			m_frame = 0;

	}
}

void Model::render()
{

	gameDeviceContext->PSSetSamplers(0, 1, &m_sampleState);

	unsigned int stride;
	unsigned int offset;

	stride = m_sizeVertexType;
	offset = 0;



	transform.buildMatrixWVP();
	matrix_WorldViewProj wvp = transform.getWVP();

	D3DXMatrixTranspose(&wvp.world, &wvp.world);
	D3DXMatrixTranspose(&wvp.view, &wvp.view);
	D3DXMatrixTranspose(&wvp.proj, &wvp.proj);

	
	SetVSParameters<matrix_WorldViewProj>(m_WVPBuffer, wvp);
	SetPSParameters<LightBuffer>(m_LightBuffer, m_lightProperties);
	SetPSParameters<AlphaBlendBuffer>(m_AlphaBlendBuffer, m_alphaProperty,1);

	




	gameUtil.getDeviceContext()->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	gameUtil.getDeviceContext()->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	gameUtil.getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	gameUtil.getDeviceContext()->IASetInputLayout(m_layout);

	gameUtil.getDeviceContext()->VSSetShader(m_vs, 0, 0);
	gameUtil.getDeviceContext()->PSSetShader(m_ps, 0, 0);

	for (int i = 0; i < m_vecMesh.size(); ++i)
	{
		//loading bone final transforms
		if (m_HasAnimations)
		{
			boneTMBuffer* dataPtr = nullptr;
			D3D11_MAPPED_SUBRESOURCE mappedResource = {};

			HRESULT result = gameUtil.getDeviceContext()->Map(m_boneBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

			dataPtr = (boneTMBuffer*)mappedResource.pData;
			ZeroMemory(dataPtr->transform, sizeof(XMFLOAT4X4) * 96);
			int idx = m_mapAnimClipNameIdx[m_AnimName];
			
			int boneTMSize = m_vecCacheClips[m_mapAnimClipNameIdx[m_AnimName]][m_frame][i].size();

			//some meshes have zero bone TMs.
			if (boneTMSize == 0)
			{
				for (int i = 0; i < 96; ++i)
				{
					XMStoreFloat4x4(&dataPtr->transform[i], XMMatrixIdentity());
				}
			}
			else
			{
				memcpy(dataPtr->transform,
					&(m_vecCacheClips[m_mapAnimClipNameIdx[m_AnimName]][m_frame][i][0]),
					sizeof(XMFLOAT4X4)*m_vecCacheClips[m_mapAnimClipNameIdx[m_AnimName]][m_frame][i].size());
			}

			gameUtil.getDeviceContext()->Unmap(m_boneBuffer, 0);

			gameUtil.getDeviceContext()->VSSetConstantBuffers(1, 1, &m_boneBuffer);
		}
		else
		{
			boneTMBuffer* dataPtr = nullptr;
			D3D11_MAPPED_SUBRESOURCE mappedResource = {};

			HRESULT result = gameUtil.getDeviceContext()->Map(m_boneBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

			dataPtr = (boneTMBuffer*)mappedResource.pData;

			for (int i = 0; i < 96; ++i)
			{
				XMStoreFloat4x4(&dataPtr->transform[i], XMMatrixIdentity());
			}


			gameUtil.getDeviceContext()->Unmap(m_boneBuffer, 0);

			gameUtil.getDeviceContext()->VSSetConstantBuffers(1, 1, &m_boneBuffer);
		}

		//texturing
		if (m_vecMesh[i].ptexture)
		{
			m_vecMesh[i].ptexture->render();
		}

		gameDeviceContext->DrawIndexed(m_vecMesh[i].cntIndex, m_vecMesh[i].indexStart, m_vecMesh[i].vertexStart);

	}

	/*
	drawIndexed 함수와 vssetconstantbuffer 함수만으로 반복문을 돌았더니 메쉬가 이동되었다.
	이것을 보았을때 셋팅한 값들을 토대로 렌더링 파이프라인이 수행되는 시점은 drawIndexed 함수를
	호출하는 시점인것같다. 그러니까 각각의 메쉬를 이동시키고싶다면 각각의 메쉬에 대해서 drawIndexed 함수 이전 까지
	값 셋팅을 마치면 된다.
	*/


}


void Model::settingTextures(const aiScene* pScene, string dir)
{
	aiString aiPath;
	string fileName;
	int pos = 0;

	for (int i = 0; i < pScene->mNumMeshes; ++i)
	{
		aiMesh* mesh = pScene->mMeshes[i];
		aiMaterial* material = pScene->mMaterials[mesh->mMaterialIndex];

		if ((material->GetTexture(aiTextureType_DIFFUSE, 0, &aiPath)) == AI_SUCCESS)
		{
			
			string path = aiPath.C_Str();
			pos = path.rfind("/");

			//not found "/"
			if (pos < 0)
			{
				pos = path.rfind("\\");
				//not found "\\"
				if (pos < 0)
				{
					fileName = path;
				}
				//found "\\"
				else {
					fileName = &path[pos] + 1;
				}
			}
			//found "/"
			else {
				fileName = &path[pos] + 1;
			}

			if (fileName[0] == '.')
			{
				fileName = &fileName[2];
			}


			if (!strcmp(fileName.c_str(), "Spinnen_Bein_tex_COLOR_"))
			{
				fileName += ".png";
			}

			string result = dir + fileName;

			m_mapTextureFileNames[i] = result;

			m_vecMesh[i].ptexture->load( result.c_str() );
		}
	}

}
string Model::getFileNameOnly(const string src)
{
	string fileName;

	int pos = src.rfind("/");
	if (pos < 0)
	{
		pos = src.rfind("\\");
		if (pos < 0)
		{
			fileName = src;
		}
		else
		{
			fileName = src[pos + 1];
		}
	}
	else
	{
		fileName = src[pos + 1];
	}

	return fileName;
}



/*
void Model::loadMaterial(KsmModelMesh * ModelMesh, const aiMesh * sourceMesh)
{	// extract all properties from the ASSIMP material structure
	const aiMaterial* sourceMaterial = _aiScene->mMaterials[sourceMesh->mMaterialIndex];

	//
	// DIFFUSE COLOR
	//
	aiColor4D materialColor;
	if (AI_SUCCESS == aiGetMaterialColor(sourceMaterial, AI_MATKEY_COLOR_DIFFUSE, &materialColor))
	{
		ModelMesh->MeshMaterial.Diffuse.x=
		ModelMesh->MeshMaterial.Diffuse.x = materialColor.r;
		ModelMesh->MeshMaterial.Diffuse.y = materialColor.g;
		ModelMesh->MeshMaterial.Diffuse.z = materialColor.b;
		ModelMesh->MeshMaterial.Diffuse.w = materialColor.a;
	}
	else
	{
		ModelMesh->MeshMaterial.Diffuse.x = 1.0f;
		ModelMesh->MeshMaterial.Diffuse.y = 1.0f;
		ModelMesh->MeshMaterial.Diffuse.z = 1.0f;
		ModelMesh->MeshMaterial.Diffuse.w = 1.0f;
	}
	//
	// SPECULAR COLOR
	//
	if (AI_SUCCESS == aiGetMaterialColor(sourceMaterial, AI_MATKEY_COLOR_SPECULAR, &materialColor))
	{
		ModelMesh->MeshMaterial.Specular.x = materialColor.r;
		ModelMesh->MeshMaterial.Specular.y = materialColor.g;
		ModelMesh->MeshMaterial.Specular.z = materialColor.b;
		ModelMesh->MeshMaterial.Specular.w = materialColor.a;
	}
	else
	{
		ModelMesh->MeshMaterial.Specular.x = 1.0f;
		ModelMesh->MeshMaterial.Specular.y = 1.0f;
		ModelMesh->MeshMaterial.Specular.z = 1.0f;
		ModelMesh->MeshMaterial.Specular.w = 1.0f;
	}

	//
	// AMBIENT COLOR
	//
	if (AI_SUCCESS == aiGetMaterialColor(sourceMaterial, AI_MATKEY_COLOR_AMBIENT, &materialColor))
	{
		ModelMesh->MeshMaterial.Ambient.x = materialColor.r;
		ModelMesh->MeshMaterial.Ambient.y = materialColor.g;
		ModelMesh->MeshMaterial.Ambient.z = materialColor.b;
		ModelMesh->MeshMaterial.Ambient.w = materialColor.a;
	}
	else
	{
		ModelMesh->MeshMaterial.Ambient.x = 1.0f;
		ModelMesh->MeshMaterial.Ambient.y = 1.0f;
		ModelMesh->MeshMaterial.Ambient.z = 1.0f;
		ModelMesh->MeshMaterial.Ambient.w = 1.0f;
	}

	//
	// EMISSIVE COLOR
	//

	if (AI_SUCCESS == aiGetMaterialColor(sourceMaterial, AI_MATKEY_COLOR_EMISSIVE, &materialColor))
	{
		ModelMesh->MeshMaterial.Reflect.x = materialColor.r;
		ModelMesh->MeshMaterial.Reflect.y = materialColor.g;
		ModelMesh->MeshMaterial.Reflect.z = materialColor.b;
		ModelMesh->MeshMaterial.Reflect.w = materialColor.a;
	}
	else
	{
		ModelMesh->MeshMaterial.Reflect.x = 1.0f;
		ModelMesh->MeshMaterial.Reflect.y = 1.0f;
		ModelMesh->MeshMaterial.Reflect.z = 1.0f;
		ModelMesh->MeshMaterial.Reflect.w = 1.0f;
	}

	//
	// Opacity
	//
	if (AI_SUCCESS != aiGetMaterialFloat(sourceMaterial, AI_MATKEY_OPACITY, &ModelMesh->MaterialOpacity))
	{
		ModelMesh->MaterialOpacity = 1.0f;
	}

	//
	// Shininess
	//
	if (AI_SUCCESS != aiGetMaterialFloat(sourceMaterial, AI_MATKEY_SHININESS, &ModelMesh->MaterialShininess))
	{
		// assume 15 as default shininess
		ModelMesh->MaterialShininess = 15.0f;
	}

	//
	// Shininess strength
	//
	if (AI_SUCCESS != aiGetMaterialFloat(sourceMaterial, AI_MATKEY_SHININESS_STRENGTH, &ModelMesh->SpecularStrength))
	{
		// assume 1.0 as default shininess strength
		ModelMesh->SpecularStrength = 1.0f;
	}

	aiString szPath;
	aiTextureMapMode mapU(aiTextureMapMode_Wrap), mapV(aiTextureMapMode_Wrap);

	//bool bib = false;
	if (sourceMesh->mTextureCoords[0])
	{
		//
		// Diffuse texture
		//
		if (AI_SUCCESS == aiGetMaterialString(sourceMaterial, AI_MATKEY_TEXTURE_DIFFUSE(0), &szPath))
		{
			// strip off any paths and get the filename only
			string texturePath(szPath.C_Str());

			string filenameOnly=getFileNameOnly(texturePath);


			// TODO: Replace this with a relative path that makes sense for where you store model textures
			string tmppath(m_dir);
			tmppath.append(filenameOnly);
			string path(tmppath.c_str());

			wstring convert = wstring(path.begin(), path.end());
			ModelMesh->DiffuseTexturePath = convert;
		}

		//
		// Normal texture
		//
		if (AI_SUCCESS == aiGetMaterialString(sourceMaterial, AI_MATKEY_TEXTURE_NORMALS(0), &szPath))
		{
			// strip off any paths and get the filename only
			string filenameOnly = getFileNameOnly(szPath.C_Str());

			// TODO: Replace this with a relative path that makes sense for where you store model textures
			string tmppath(m_dir);
			tmppath.append(filenameOnly);
			string path(tmppath.c_str());

			wstring convert = wstring(path.begin(), path.end());
			ModelMesh->NormalTexturePath = convert;
		}
	}
}
*/

/*
static float t = 0.f;
t += gameTimer.getDeltaTime() * 60;

if (t > m_AnimationClips[0].TotalFrames)
t = 0;

vector<XMFLOAT4X4> tmpTransforms;
m_AnimationClips[0].Interpolate(m_AnimationClips[0].GetTimeStepAtFrame((UINT)t), tmpTransforms);

//update local transform
for (int i = 0; i < tmpTransforms.size(); ++i)
{
string boneName = m_AnimationClips[0].getBoneNameAt(i);
m_mapNode[boneName]->LocalTransform = tmpTransforms[i];
}

static float t = 0.f;
t += gameTimer.getDeltaTime() * 30;

if (t > m_AnimationClips[0].TotalFrames)
t = 0;

for (int i = 0; i < m_ttmpTransforms[(UINT)t].size(); ++i)
{
string boneName = m_AnimationClips[0].getBoneNameAt(i);
m_mapNode[boneName]->LocalTransform = m_ttmpTransforms[(UINT)t][i];
}

//update global transform, calculate final transform
updateNodeTree(m_mapNode[RootNode->Name]);
updateBoneFinalTM();
*/
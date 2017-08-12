#pragma once
#include <Importer.hpp>
#include <scene.h>
#include <postprocess.h>

#include "gameObject.h"
#include "animationData.h"
#include "modelNodes.h"

class Model : public gameObject
{
private:
 
	vector< map<int, vector<vector<XMFLOAT4X4>> > > m_vecCacheClips; 
	map<string,int> m_mapAnimClipNameIdx; 
	
public:
	Model();
	~Model();

	void init(char* dir, char* FileName);
	void update();
	virtual void render() override;

	//void loadAnimation(string fulldir);

	bool checkSaveFileExists(string fileName);
	void save(string fileName);
	void load(string fileName);

	struct MeshInfo {
		int cntVertex;
		int cntIndex;
		int indexStart;
		int vertexStart;

		textureclass* ptexture = nullptr;

		vector<XMFLOAT4X4> vecBoneTransforms;
		vector<string> vecBoneNames;
	};

	vector<MeshInfo> m_vecMesh;
	vector<vertex_ptn_skinned_xm> m_vertex_skinned_xm;
	vector<UINT> m_indices;
	BoundingBox m_box;
	string m_AnimName;

private:
	void settingTextures(const aiScene* pScene, string dir);
	//void loadMaterial(_In_ KsmModelMesh* ksmMesh,_In_ const aiMesh* sourceMesh);
	string getFileNameOnly(const string src);

public:

	LightBuffer m_lightProperties = {};
	AlphaBlendBuffer m_alphaProperty;
private:

	ID3D11Buffer* m_boneBuffer = nullptr;
	ID3D11Buffer* m_LightBuffer = nullptr;
	ID3D11Buffer* m_AlphaBlendBuffer = nullptr;

private:
	string m_dir, m_fileName;

	const aiScene* _aiScene;

	UINT m_NumMeshes = 0;

	UINT m_NumMaterials = 0;

	UINT m_NumTextures = 0;

	UINT m_NumAnimations=0;

	bool m_HasAnimations=false;

	int m_tmpBaseVertexidx = 0, m_tmpBaseFaceIdx = 0;

	int m_totalBoneNum = 0;

	int m_tmpBoneIdx = 0;

	map<string, modelNode*> m_mapNode;
	map<string, XMFLOAT4X4> m_mapBoneOffsetTM;
	
	modelNode* RootNode;

	map<int,string> m_mapTextureFileNames;
	vector<AnimationClip> m_AnimationClips;
	float m_frame=0;

	string FileName;

	modelNode* createNodeTree(
		_In_ aiNode* node,
		_In_ modelNode* parent);

	void extractAnimations();

	void loadMeshData(
		_In_ modelNode* ownerNode,
		_In_ const aiMesh* mesh);

	void calculateGlobalTransform(_In_ modelNode* node);

	void updateNodeTree(_In_ modelNode* node);
	void updateBoneFinalTM();
	void cache();
};


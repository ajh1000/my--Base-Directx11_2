#pragma once
#include "gameObject.h"

class Terrain :public gameObject
{
public:
	Terrain();
	Terrain(float x,float y, float z);
	virtual ~Terrain();

	void init();
	void update();
	void render();
private:
	void CreateTerrain(float w, float d, UINT m, UINT n);

private:
	textureclass texture;

	vector<vertex_ptn> m_vertices;
	vector<UINT> m_indices;
	
	ID3D11Buffer* m_LightBuffer = 0;
	LightBuffer m_lightProperties = {};
};


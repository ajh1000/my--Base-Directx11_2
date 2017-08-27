#include "stdafx.h"
#include "Terrain.h"


Terrain::Terrain()
{
}

Terrain::Terrain(float x, float y, float z)
{
	transform.setPos(x, y, z);
}


Terrain::~Terrain()
{

}

void Terrain::init()
{

	Init_compileShader("./cube_normal_vs.hlsl", "./cube_normal_ps.hlsl");
	Init_polygonLayout(EPolygonLayout::LAYOUT_PTN);
	Init_CreateConstantBuffer(&m_LightBuffer, sizeof(LightBuffer));

	CreateTerrain(5,5,4,4);


	gameObject::Init_CreateVertexBuffer(&m_vertices.front(), sizeof(vertex_ptn), m_vertices.size(), D3D11_USAGE_DEFAULT,
		D3D11_BIND_VERTEX_BUFFER, 0);
	Init_option_CreateIndexBuffer(&m_indices.front(), sizeof(UINT), m_indices.size(), D3D11_USAGE_DEFAULT,
		D3D11_BIND_INDEX_BUFFER, 0);


	//texture.load("./DATA/seafloor.dds");
	//texture.loadHttpDir("http://www.google.com");
	//texture.loadHttpDir("file:///./asd.html");

	m_lightProperties.diffuseColor = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);

	D3DXVECTOR3 lightDir = D3DXVECTOR3(0, -1,0);
	//D3DXVec3Normalize(&lightDir, &lightDir);

	m_lightProperties.lightDirection = lightDir;

}

void Terrain::update()
{



}

void Terrain::render()
{

	SetPSParameters<LightBuffer>(m_LightBuffer, m_lightProperties);

	texture.render();

	gameObject::renderIndexed();
}

void Terrain::CreateTerrain(float w,float d,UINT m,UINT n)
{
	UINT vertexCount = m*n;
	UINT faceCount = (m - 1)*(n - 1) * 2;

	float halfWidth = 0.5f*w;
	float halfDepth = 0.5f*d;

	float dx = w / (n - 1);
	float dz = d / (m - 1);

	float du = 1.f / (n - 1);
	float dv = 1.f / (m - 1);

	m_vertices.resize(vertexCount);
	m_indices.resize(faceCount*3);

	for (UINT i = 0; i < m; ++i)
	{
		float z = halfDepth - i*dz;

		for (UINT j = 0; j < n; ++j)
		{
			float x = -halfWidth + j*dx;

			m_vertices[i*n + j].pos = D3DXVECTOR3(x, 0.f, z);
			m_vertices[i*n + j].normal = D3DXVECTOR3(0,1, 0);
			m_vertices[i*n + j].uv.x = j*du;
			m_vertices[i*n + j].uv.y = i*dv;


		}
	}

	UINT k = 0;

	for (UINT i = 0; i < m - 1; ++i)
	{
		for (UINT j = 0; j < n - 1; ++j)
		{
			m_indices[k] = i*n + j;
			m_indices[k+1] = i*n + j+1;
			m_indices[k+2] = (i+1)*n + j;

			m_indices[k+3] = (i+1)*n + j;
			m_indices[k+4] = i*n + j+1;
			m_indices[k+5] = (i+1)*n + j+1;

			k += 6;

		}
	}
}

#include "stdafx.h"
#include "cube_normal.h"


cube_normal::cube_normal()
{
}

cube_normal::cube_normal(float x, float y, float z)
{
	transform.setPos(x, y, z);
}


cube_normal::~cube_normal()
{
	SAFE_RELEASE(m_LightBuffer);
}

void cube_normal::init()
{
	Init_compileShader("./cube_normal_vs.hlsl", "./cube_normal_ps.hlsl");
	Init_polygonLayout(EPolygonLayout::LAYOUT_PTN);
	Init_CreateConstantBuffer(&m_LightBuffer, sizeof(LightBuffer));

	vertex_ptn vertices[3] = {};
	vertices[0].pos = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);  // Bottom left.
	vertices[0].uv = D3DXVECTOR2(0.0f, 1.0f);
	vertices[0].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	vertices[1].pos = D3DXVECTOR3(0.0f, 1.0f, 0.0f);  // Top middle.
	vertices[1].uv = D3DXVECTOR2(0.5f, 0.0f);
	vertices[1].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	vertices[2].pos = D3DXVECTOR3(1.0f, -1.0f, 0.0f);  // Bottom right.
	vertices[2].uv = D3DXVECTOR2(1.0f, 1.0f);
	vertices[2].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	super::Init_CreateVertexBuffer(vertices,sizeof(vertex_ptn), 3, D3D11_USAGE_DEFAULT,
		D3D11_BIND_VERTEX_BUFFER, 0);


	texture.load("./DATA/seafloor.dds");


	m_lightProperties.diffuseColor=D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	m_lightProperties.lightDirection=D3DXVECTOR3(0.0f, 0.0f, 1.0f);
}

void cube_normal::update()
{
	//transform.translate(gameTimer.getDeltaTime(), 0, 0);
	transform.rotate(0, gameTimer.getDeltaTime()*100.f, 0);
}

void cube_normal::render()
{
	SetPSParameters<LightBuffer>(m_LightBuffer, m_lightProperties);

	texture.render();

	gameObject::render();
}

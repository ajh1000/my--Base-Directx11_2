#include "stdafx.h"
#include "cube.h"


cube::cube()
{
}


cube::~cube()
{
}

void cube::init()
{
	gameObject::Init_compileShader("./cube_vs.hlsl", "./cube_ps.hlsl");
	gameObject::Init_polygonLayout(EPolygonLayout::LAYOUT_PC);
	

	vertex_pc some_polygon[3] = {

	{D3DXVECTOR3(-1.0f, -1.0f, 0.0f),
	D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f) },

	{D3DXVECTOR3(0.0f, 1.0f, 0.0f),
	D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f)},

	{D3DXVECTOR3(1.0f, -1.0f, 0.0f),
	D3DXVECTOR4(1.0f, 0.f, 0.0f, 1.0f)}
	};

	gameObject::Init_CreateVertexBuffer(some_polygon,sizeof(vertex_pc), 3, D3D11_USAGE_DEFAULT, 
		D3D11_BIND_VERTEX_BUFFER, 0);

	int a = 0;

}

void cube::update()
{
}

void cube::render()
{
	gameObject::render();
}



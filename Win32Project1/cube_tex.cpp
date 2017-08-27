#include "stdafx.h"
#include "cube_tex.h"


cube_tex::cube_tex()
{
}


cube_tex::~cube_tex()
{
}

void cube_tex::init()
{
	
	gameObject::Init_compileShader("./cube_tex_vs.hlsl", "./cube_tex_ps.hlsl");
	gameObject::Init_polygonLayout(gameObject::EPolygonLayout::LAYOUT_PT);
	
	vertex_pt vertices[3] = {};
	vertices[0].pos = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);  // Bottom left.
	vertices[0].uv = D3DXVECTOR2(0.0f, 1.0f);

	vertices[1].pos = D3DXVECTOR3(0.0f, 1.0f, 0.0f);  // Top middle.
	vertices[1].uv = D3DXVECTOR2(0.5f, 0.0f);

	vertices[2].pos = D3DXVECTOR3(1.0f, -1.0f, 0.0f);  // Bottom right.
	vertices[2].uv = D3DXVECTOR2(1.0f, 1.0f);

	gameObject::Init_CreateVertexBuffer(vertices, sizeof(vertex_pt), 3, D3D11_USAGE_DEFAULT,
		D3D11_BIND_VERTEX_BUFFER, 0);



	texture.load("./DATA/seafloor.dds");
}

void cube_tex::update()
{
	transform.translate(gameTimer.getDeltaTime(), 0, 0);
}

void cube_tex::render()
{
	texture.render();

	gameObject::render();
}

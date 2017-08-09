#include "stdafx.h"
#include "GameUIObject.h"



GameUIObject::GameUIObject()
{
}


GameUIObject::~GameUIObject()
{
}

void GameUIObject::init(int xpos, int ypos, int vertexWidth, int vertexHeight,
	int textureWidth, int textureHeight, string URL)
{
	super::Init_compileShader("./cube_tex_vs.hlsl", "./cube_tex_ps.hlsl");
	super::Init_polygonLayout(super::EPolygonLayout::LAYOUT_PT);

	//NORMALIZING
	RECT rect;
	GetClientRect(gameUtil.GetHWND(), &rect);

	float left = xpos - rect.right / 2.f;
	float top = ypos + rect.bottom / 2.f;
	float right = left + vertexWidth;
	float bottom = top - vertexHeight;

	/*int left =  xpos;
	int top = ypos;
	int right = left + vertexWidth;
	int bottom = top - vertexHeight;*/

	vertex_pt vertices[4] = {};
	vertices[0].pos = D3DXVECTOR3(left, top, 0.0f);  //  left Top
	vertices[0].uv = D3DXVECTOR2(0.0f, 0);

	vertices[1].pos = D3DXVECTOR3(right, top, 0.0f);  // Top right
	vertices[1].uv = D3DXVECTOR2(1.0f, 0.0f);

	vertices[2].pos = D3DXVECTOR3(left, bottom, 0.0f);  // Bottom left
	vertices[2].uv = D3DXVECTOR2(0, 1.0f);

	vertices[3].pos = D3DXVECTOR3(right, bottom, 0.0f);  // Bottom right
	vertices[3].uv = D3DXVECTOR2(1.0f, 1.0f);

	int indices[] = { 0,1,2,
						1,3,2 };

	super::Init_CreateVertexBuffer(vertices, sizeof(vertex_pt), 4, D3D11_USAGE_DEFAULT,
		D3D11_BIND_VERTEX_BUFFER, 0);
	gameObject::Init_option_CreateIndexBuffer(indices, sizeof(int), 6, D3D11_USAGE_DEFAULT,
		D3D11_BIND_INDEX_BUFFER, 0);

	transform.m_isScreenSpace = true;
	m_htmlTexture.load(URL, xpos, ypos, vertexWidth, vertexHeight, textureWidth, textureHeight);
}

void GameUIObject::update()
{
}

void GameUIObject::render()
{
	m_htmlTexture.render();
	gameObject::renderIndexed();
}

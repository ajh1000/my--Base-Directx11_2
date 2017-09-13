#include "stdafx.h"
#include "GameUIObject.h"



GameUIObject::GameUIObject()
{
	m_top = 0;
	m_width = 0;
	m_height = 0;
	m_left=0;
}


GameUIObject::~GameUIObject()
{
}

void GameUIObject::init(int xpos, int ypos, int vertexWidth, int vertexHeight,
	int textureWidth, int textureHeight, string URL)
{
	gameObject::Init_compileShader("./cube_tex_vs.hlsl", "./cube_tex_ps.hlsl");
	gameObject::Init_polygonLayout(gameObject::EPolygonLayout::LAYOUT_PT);

	//integrate 'd3d screen space coord' To 'window client space coord'.
	/*
	RECT rect;
	GetClientRect(gameUtil.GetHWND(), &rect);

	float left = xpos - rect.right / 2;		//xpos
	float top = rect.bottom / 2 - ypos;		//ypos
	float width = left + vertexWidth;		//width
	float height = top - vertexHeight;       //height 
	*/
	/*
		0---1
		| / |
		2---3
	*/

	RECT rect;
	GetClientRect(gameUtil.GetHWND(), &rect);

	m_left = (xpos - rect.right / 2) - vertexWidth / 2;		//xpos
	m_top = (rect.bottom / 2 - ypos) + vertexHeight / 2;		//ypos
	m_width = m_left + vertexWidth;		//width
	m_height = m_top - vertexHeight;       //height 


	vertex_pt vertices[4] = {};
	vertices[0].pos = D3DXVECTOR3(m_left, m_top, 0.0f);
	vertices[0].uv = D3DXVECTOR2(0.0f, 0);

	vertices[1].pos = D3DXVECTOR3(m_width, m_top, 0.0f);
	vertices[1].uv = D3DXVECTOR2(1.0f, 0.0f);

	vertices[2].pos = D3DXVECTOR3(m_left, m_height, 0.0f);
	vertices[2].uv = D3DXVECTOR2(0, 1.0f);

	vertices[3].pos = D3DXVECTOR3(m_width, m_height, 0.0f);
	vertices[3].uv = D3DXVECTOR2(1.0f, 1.0f);

	int indices[] = { 0,1,2,
						1,3,2 };

	gameObject::Init_CreateVertexBuffer(vertices, sizeof(vertex_pt), 4, D3D11_USAGE_DYNAMIC,
		D3D11_BIND_VERTEX_BUFFER, D3D11_CPU_ACCESS_WRITE);
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

void GameUIObject::setEnable(bool enable)
{
	m_isEnabled = enable;

}

void GameUIObject::setPos(int x, int y)
{
	RECT rect;
	GetClientRect(gameUtil.GetHWND(), &rect);

	m_left = (x - rect.right / 2) - m_htmlTexture.m_rect.right /2;		//xpos
	m_top = (rect.bottom / 2 - y)+ m_htmlTexture.m_rect.bottom /2;		//ypos
	m_width= m_htmlTexture.m_rect.right+m_left;		//width
	m_height = m_top- m_htmlTexture.m_rect.bottom;       //height 

	vertex_pt vertices[4] = {};
	vertices[0].pos = D3DXVECTOR3(m_left, m_top, 0.0f);
	vertices[0].uv = D3DXVECTOR2(0.0f, 0);

	vertices[1].pos = D3DXVECTOR3(m_width, m_top, 0.0f);
	vertices[1].uv = D3DXVECTOR2(1.0f, 0.0f);

	vertices[2].pos = D3DXVECTOR3(m_left, m_height, 0.0f);
	vertices[2].uv = D3DXVECTOR2(0, 1.0f);

	vertices[3].pos = D3DXVECTOR3(m_width, m_height, 0.0f);
	vertices[3].uv = D3DXVECTOR2(1.0f, 1.0f);

	D3D11_MAPPED_SUBRESOURCE mappedResource = {};

	HRESULT result = gameUtil.getDeviceContext()->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	memcpy_s(mappedResource.pData, sizeof(vertex_pt)*4, vertices, sizeof(vertex_pt) * 4);


	gameUtil.getDeviceContext()->Unmap(m_vertexBuffer, 0);
}

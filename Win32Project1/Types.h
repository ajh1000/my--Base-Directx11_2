#pragma once
#include <d3dx10math.h>


static D3DXMATRIX& identityMat()
{
	static D3DXMATRIX identity = D3DXMATRIX(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);
	return identity;
}

struct vertex_pc
{
	vertex_pc() {}
	vertex_pc(D3DXVECTOR3 _p, D3DXVECTOR4 _c)
	{
		pos = _p;
		color = _c;
	}
	D3DXVECTOR3 pos;
	D3DXVECTOR4 color;
};



struct vertex_pt
{
	vertex_pt() {}
	vertex_pt(D3DXVECTOR3 _pos, D3DXVECTOR2 _uv)
	{
		pos = _pos;
		uv = _uv;
	}
	D3DXVECTOR3 pos;
	D3DXVECTOR2 uv;
};

struct vertex_ptn
{
	vertex_ptn(){}
	vertex_ptn(D3DXVECTOR3 _p, D3DXVECTOR2 _uv, D3DXVECTOR3 _n)
	{
		pos = _p; 
		uv = _uv; 
		normal = _n;
	}
	D3DXVECTOR3 pos;
	D3DXVECTOR2 uv;
	D3DXVECTOR3 normal;
};


struct vertex_ptn_xm
{
	vertex_ptn_xm() {}
	vertex_ptn_xm(XMFLOAT3 _p, XMFLOAT2 _uv, XMFLOAT3 _n)
	{
		pos = _p;
		uv = _uv;
		normal = _n;
	}
	XMFLOAT3 pos;
	XMFLOAT2 uv;
	XMFLOAT3 normal;
};
struct vertex_ptn_skinned
{

	vertex_ptn_skinned()
	{
		ZeroMemory(pos, sizeof(D3DXVECTOR3));
		ZeroMemory(uv, sizeof(D3DXVECTOR2));
		ZeroMemory(normal, sizeof(D3DXVECTOR3));
		ZeroMemory(boneId, sizeof(unsigned int)*4);
		ZeroMemory(weights, sizeof(float)*4);
		
	}
	D3DXVECTOR3 pos;
	D3DXVECTOR2 uv;
	D3DXVECTOR3 normal;
	
	float weights[4];
	unsigned int boneId[4];
	
};

struct vertex_ptn_skinned_xm
{

	vertex_ptn_skinned_xm()
	{
		ZeroMemory(&pos, sizeof(XMFLOAT3));
		ZeroMemory(&uv, sizeof(XMFLOAT2));
		ZeroMemory(&normal, sizeof(XMFLOAT3));
		ZeroMemory(boneId, sizeof(unsigned int) * 4);
		ZeroMemory(weights, sizeof(float) * 4);

	}
	XMFLOAT3 pos;
	XMFLOAT2 uv;
	XMFLOAT3 normal;

	float weights[4];
	unsigned int boneId[4];

};

////////////////////
////  CBUFFERS   ///
////////////////////


struct AlphaBlendBuffer
{
	AlphaBlendBuffer() { ZeroMemory(padding, sizeof(float) * 3); alpha = 1.f; }
	float alpha;
	float padding[3];
};


struct matrix_WorldViewProj
{
	D3DXMATRIX world;
	D3DXMATRIX  view;
	D3DXMATRIX  proj;

};

struct matrix_WorldViewProjBones
{
	XMFLOAT4X4 world;
	XMFLOAT4X4 view;
	XMFLOAT4X4 proj;
	XMFLOAT4X4 transform[96];
};

struct LightBuffer
{
	D3DXVECTOR4 diffuseColor;
	D3DXVECTOR3 lightDirection;
	float padding;
};

struct boneTransformsBuffer
{
	boneTransformsBuffer()
	{
		for (int i = 0; i < 96; ++i)
		{
			transform[i] = identityMat();
		}
	}
	D3DXMATRIX transform[96];
};


struct boneTMBuffer
{
	boneTMBuffer()
	{
		for (int i = 0; i <96; ++i)
		{
			XMStoreFloat4x4(&transform[i], XMMatrixIdentity());
		}
	}
	XMFLOAT4X4 transform[96];
};



/*
하나의 변수로 공유를 하는것이 아님.
각각의 cpp파일에서 객체를 생성하게됨. 
extern을 써주면 됨. 다만 이런 값이 변해서는 안되는 변수들에대해서는 구조체 생성자로 그냥 일일히 리턴해줘야함.
static vertex_pc vertex_pc_blank = {};
static vertex_pt vertex_pt_blank = {};
static vertex_ptn vertex_ptn_blank = {};
*/

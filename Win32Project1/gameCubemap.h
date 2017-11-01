#pragma once
#include "gameObject.h"
class gameCubemap :
	public gameObject
{
public:
	gameCubemap();
	~gameCubemap();

	void init();
	void update();
	void render();
public:
	ID3D11Buffer* sphereIndexBuffer=0;
	ID3D11Buffer* sphereVertBuffer = 0;
	ID3D11SamplerState* CubesTexSamplerState=0;
	ID3D11VertexShader* SKYMAP_VS = 0;
	ID3D11PixelShader* SKYMAP_PS = 0;
	ID3D10Blob* SKYMAP_VS_Buffer = 0;
	ID3D10Blob* SKYMAP_PS_Buffer = 0;

	ID3D11ShaderResourceView* smrv = 0;

	ID3D11DepthStencilState* DSLessEqual = 0;
	ID3D11RasterizerState* RSCullNone = 0;

	int NumSphereVertices = 0;
	int NumSphereFaces = 0;

	XMMATRIX sphereWorld;

	void CreateSphere(int LatLines, int LongLines);

	textureclass m_texture;

	struct cbPerObject
	{
		XMMATRIX  WVP;
		XMMATRIX World;
	};

	cbPerObject cbPerObj;

	ID3D11Buffer* cbPerObjectBuffer=0;
};


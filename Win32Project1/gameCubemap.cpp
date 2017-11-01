#include "stdafx.h"
#include "gameCubemap.h"


gameCubemap::gameCubemap()
{
}


gameCubemap::~gameCubemap()
{
}

void gameCubemap::init()
{
	/*
	gameObject::Init_compileShader("cubemap_vs.hlsl", "cubemap_ps.hlsl");
	gameObject::Init_polygonLayout(gameObject::EPolygonLayout::LAYOUT_PT);

	vertex_pt vertices[] =
	{
		// Front Face
		vertex_pt(D3DXVECTOR3(-1.0f, -1.0f, -1.0f),D3DXVECTOR2(0,0)),
		vertex_pt(D3DXVECTOR3(-1.0f,  1.0f, -1.0f),D3DXVECTOR2(0,0)),
		vertex_pt(D3DXVECTOR3(1.0f,  1.0f, -1.0f) ,D3DXVECTOR2(0,0)),
		vertex_pt(D3DXVECTOR3(1.0f, -1.0f, -1.0f) ,D3DXVECTOR2(0,0)),

		// Back Face            
		vertex_pt(D3DXVECTOR3(-1.0f, -1.0f, 1.0f),D3DXVECTOR2(0,0)),
		vertex_pt(D3DXVECTOR3(1.0f, -1.0f, 1.0f) ,D3DXVECTOR2(0,0)),
		vertex_pt(D3DXVECTOR3(1.0f,  1.0f, 1.0f) ,D3DXVECTOR2(0,0)),
		vertex_pt(D3DXVECTOR3(-1.0f,  1.0f, 1.0f),D3DXVECTOR2(0,0)),

		// Top Face                
		vertex_pt(D3DXVECTOR3(-1.0f, 1.0f, -1.0f),D3DXVECTOR2(0,0)),
		vertex_pt(D3DXVECTOR3(-1.0f, 1.0f,  1.0f),D3DXVECTOR2(0,0)),
		vertex_pt(D3DXVECTOR3(1.0f, 1.0f,  1.0f) ,D3DXVECTOR2(0,0)),
		vertex_pt(D3DXVECTOR3(1.0f, 1.0f, -1.0f) ,D3DXVECTOR2(0,0)),

		// Bottom Face            
		vertex_pt(D3DXVECTOR3(-1.0f, -1.0f, -1.0f),D3DXVECTOR2(0,0)),
		vertex_pt(D3DXVECTOR3(1.0f, -1.0f, -1.0f) ,D3DXVECTOR2(0,0)),
		vertex_pt(D3DXVECTOR3(1.0f, -1.0f,  1.0f) ,D3DXVECTOR2(0,0)),
		vertex_pt(D3DXVECTOR3(-1.0f, -1.0f,  1.0f),D3DXVECTOR2(0,0)),

		// Left Face            
		vertex_pt(D3DXVECTOR3(-1.0f, -1.0f,  1.0f),D3DXVECTOR2(0,0)),
		vertex_pt(D3DXVECTOR3(-1.0f,  1.0f,  1.0f),D3DXVECTOR2(0,0)),
		vertex_pt(D3DXVECTOR3(-1.0f,  1.0f, -1.0f),D3DXVECTOR2(0,0)),
		vertex_pt(D3DXVECTOR3(-1.0f, -1.0f, -1.0f),D3DXVECTOR2(0,0)),

		// Right Face            
		vertex_pt(D3DXVECTOR3(1.0f, -1.0f, -1.0f),D3DXVECTOR2(0,0)),
		vertex_pt(D3DXVECTOR3(1.0f,  1.0f, -1.0f),D3DXVECTOR2(0,0)),
		vertex_pt(D3DXVECTOR3(1.0f,  1.0f,  1.0f),D3DXVECTOR2(0,0)),
		vertex_pt(D3DXVECTOR3(1.0f, -1.0f,  1.0f),D3DXVECTOR2(0,0)),
	};
	DWORD Indices[] =
	{
		// Front Face
		0,  1,  2,
		0,  2,  3,

		// Back Face
		4,  5,  6,
		4,  6,  7,

		// Top Face
		8,  9, 10,
		8, 10, 11,

		// Bottom Face
		12, 13, 14,
		12, 14, 15,

		// Left Face
		16, 17, 18,
		16, 18, 19,

		// Right Face
		20, 21, 22,
		20, 22, 23
	};
	gameObject::Init_CreateVertexBuffer(vertices, sizeof(vertex_pt), 24,
		D3D11_USAGE_DEFAULT,
		D3D11_BIND_VERTEX_BUFFER, 0);


	Init_option_CreateIndexBuffer(Indices, sizeof(DWORD), sizeof(Indices)/sizeof(DWORD),
		D3D11_USAGE_DEFAULT, D3D11_BIND_INDEX_BUFFER, 0);

	m_texture.loadCubemap("./data/cubemap/cubemap.dds");

	transform.setScale(100, 100, 100);*/

	sphereWorld = XMMatrixIdentity();
	CreateSphere(10, 10);

	HRESULT hr;
	hr = D3DX11CompileFromFile("cubemap_vs.hlsl", 0, 0, "SKYMAP_VS", "vs_5_0", 0, 0, 0, &SKYMAP_VS_Buffer, 0, 0);
	hr = D3DX11CompileFromFile("cubemap_ps.hlsl", 0, 0, "SKYMAP_PS", "ps_5_0", 0, 0, 0, &SKYMAP_PS_Buffer, 0, 0);

	hr = gameUtil.getDevice()->CreateVertexShader(SKYMAP_VS_Buffer->GetBufferPointer(), SKYMAP_VS_Buffer->GetBufferSize(), NULL, &SKYMAP_VS);
	hr = gameUtil.getDevice()->CreatePixelShader(SKYMAP_PS_Buffer->GetBufferPointer(), SKYMAP_PS_Buffer->GetBufferSize(), NULL, &SKYMAP_PS);

	D3DX11_IMAGE_LOAD_INFO loadSMInfo;
	loadSMInfo.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

	ID3D11Texture2D* SMTexture = 0;
	hr = D3DX11CreateTextureFromFile(gameUtil.getDevice(), "data/cubemap/cubemap.dds",
		&loadSMInfo, 0, (ID3D11Resource**)&SMTexture, 0);

	D3D11_TEXTURE2D_DESC SMTextureDesc;
	SMTexture->GetDesc(&SMTextureDesc);

	D3D11_SHADER_RESOURCE_VIEW_DESC SMViewDesc;
	SMViewDesc.Format = SMTextureDesc.Format;
	SMViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	SMViewDesc.TextureCube.MipLevels = SMTextureDesc.MipLevels;
	SMViewDesc.TextureCube.MostDetailedMip = 0;

	hr = gameUtil.getDevice()->CreateShaderResourceView(SMTexture, &SMViewDesc, &smrv);
	D3D11_RASTERIZER_DESC cmdesc;
	cmdesc.CullMode = D3D11_CULL_NONE;
	hr = gameUtil.getDevice()->CreateRasterizerState(&cmdesc, &RSCullNone);

	D3D11_DEPTH_STENCIL_DESC dssDesc = {};
	dssDesc.DepthEnable = true;
	dssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dssDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	gameUtil.getDevice()->CreateDepthStencilState(&dssDesc, &DSLessEqual);

	D3D11_BUFFER_DESC cbbd;
	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));

	cbbd.Usage = D3D11_USAGE_DEFAULT;
	cbbd.ByteWidth = sizeof(cbPerObject);
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = 0;
	cbbd.MiscFlags = 0;
	gameUtil.getDevice()->CreateBuffer(&cbbd, NULL, &cbPerObjectBuffer);

	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	//Create the Sample State
	hr = gameUtil.getDevice()->CreateSamplerState(&sampDesc, &CubesTexSamplerState);

}

void gameCubemap::update()
{
	sphereWorld = XMMatrixIdentity();

	//Define sphereWorld's world space matrix
	XMMATRIX Scale = XMMatrixScaling(500.0f, 500.0f, 500.0f);
	//Make sure the sphere is always centered around camera
	//XMMATRIX Translation = XMMatrixTranslation(gameUtil.GetMainCamera()->GetEye., XMVectorGetY(camPosition), XMVectorGetZ(camPosition));
	XMMATRIX Translation = XMMatrixIdentity();
	//Set sphereWorld's world space using the transformations
	sphereWorld = Scale * Translation;

}

void gameCubemap::render()
{
	gameUtil.getDeviceContext()->IASetIndexBuffer(sphereIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	UINT stride = sizeof(vertex_pt);
	UINT offset = 0;
	gameUtil.getDeviceContext()->IASetVertexBuffers(0, 1, &sphereVertBuffer, &stride, &offset);

	D3DXMATRIX view,proj;
	view = gameUtil.GetMainCamera()->GetViewMat();
	proj= gameUtil.GetMainCamera()->GetProjMat();

	XMFLOAT4X4 tmpView, tmpProj;
	memcpy(tmpView.m, view.m, sizeof(float) * 16);
	memcpy(tmpProj.m, proj.m, sizeof(float) * 16);

	XMMATRIX xmView, xmProj;
	xmView = XMLoadFloat4x4(&tmpView);
	xmProj = XMLoadFloat4x4(&tmpProj);
		 

	XMMATRIX WVP = sphereWorld * xmView * xmProj;
	cbPerObj.WVP = XMMatrixTranspose(WVP);
	cbPerObj.World = XMMatrixTranspose(sphereWorld);
	gameUtil.getDeviceContext()->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
	gameUtil.getDeviceContext()->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);
	gameUtil.getDeviceContext()->PSSetShaderResources(0, 1, &smrv);
	gameUtil.getDeviceContext()->PSSetSamplers(0, 1, &CubesTexSamplerState);

	gameUtil.getDeviceContext()->VSSetShader(SKYMAP_VS, 0, 0);
	gameUtil.getDeviceContext()->PSSetShader(SKYMAP_PS, 0, 0);
	gameUtil.getDeviceContext()->OMSetDepthStencilState(DSLessEqual, 0);
	gameUtil.getDeviceContext()->RSSetState(RSCullNone);
	gameUtil.getDeviceContext()->DrawIndexed(NumSphereFaces * 3, 0, 0);
	
	//gameUtil.getDeviceContext()->VSSetShader(VS, 0, 0);
	gameUtil.getDeviceContext()->OMSetDepthStencilState(NULL, 0);
}

void gameCubemap::CreateSphere(int LatLines, int LongLines)
{
	NumSphereVertices = ((LatLines - 2) * LongLines) + 2;
	NumSphereFaces = ((LatLines - 3)*(LongLines) * 2) + (LongLines * 2);

	float sphereYaw = 0.0f;
	float spherePitch = 0.0f;

	std::vector<vertex_pt> vertices(NumSphereVertices);

	XMVECTOR currVertPos = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	vertices[0].pos.x = 0.0f;
	vertices[0].pos.y = 0.0f;
	vertices[0].pos.z = 1.0f;

	XMMATRIX Rotationx;
	XMMATRIX Rotationy;
	for (DWORD i = 0; i < LatLines - 2; ++i)
	{
		spherePitch = (i + 1) * (3.14 / (LatLines - 1));
		Rotationx = XMMatrixRotationX(spherePitch);
		for (DWORD j = 0; j < LongLines; ++j)
		{
			sphereYaw = j * (6.28 / (LongLines));
			Rotationy = XMMatrixRotationZ(sphereYaw);
			currVertPos = XMVector3TransformNormal(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), (Rotationx * Rotationy));
			currVertPos = XMVector3Normalize(currVertPos);
			vertices[i*LongLines + j + 1].pos.x = XMVectorGetX(currVertPos);
			vertices[i*LongLines + j + 1].pos.y = XMVectorGetY(currVertPos);
			vertices[i*LongLines + j + 1].pos.z = XMVectorGetZ(currVertPos);
		}
	}

	vertices[NumSphereVertices - 1].pos.x = 0.0f;
	vertices[NumSphereVertices - 1].pos.y = 0.0f;
	vertices[NumSphereVertices - 1].pos.z = -1.0f;


	D3D11_BUFFER_DESC vertexBufferDesc = {};

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(vertex_pt) * NumSphereVertices;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData = {};
	vertexBufferData.pSysMem = &vertices[0];
	gameUtil.getDevice()->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &sphereVertBuffer);


	std::vector<DWORD> indices(NumSphereFaces * 3);

	int k = 0;
	for (DWORD l = 0; l < LongLines - 1; ++l)
	{
		indices[k] = 0;
		indices[k + 1] = l + 1;
		indices[k + 2] = l + 2;
		k += 3;
	}

	indices[k] = 0;
	indices[k + 1] = LongLines;
	indices[k + 2] = 1;
	k += 3;

	for (DWORD i = 0; i < LatLines - 3; ++i)
	{
		for (DWORD j = 0; j < LongLines - 1; ++j)
		{
			indices[k] = i*LongLines + j + 1;
			indices[k + 1] = i*LongLines + j + 2;
			indices[k + 2] = (i + 1)*LongLines + j + 1;

			indices[k + 3] = (i + 1)*LongLines + j + 1;
			indices[k + 4] = i*LongLines + j + 2;
			indices[k + 5] = (i + 1)*LongLines + j + 2;

			k += 6; // next quad
		}

		indices[k] = (i*LongLines) + LongLines;
		indices[k + 1] = (i*LongLines) + 1;
		indices[k + 2] = ((i + 1)*LongLines) + LongLines;

		indices[k + 3] = ((i + 1)*LongLines) + LongLines;
		indices[k + 4] = (i*LongLines) + 1;
		indices[k + 5] = ((i + 1)*LongLines) + 1;

		k += 6;
	}

	for (DWORD l = 0; l < LongLines - 1; ++l)
	{
		indices[k] = NumSphereVertices - 1;
		indices[k + 1] = (NumSphereVertices - 1) - (l + 1);
		indices[k + 2] = (NumSphereVertices - 1) - (l + 2);
		k += 3;
	}

	indices[k] = NumSphereVertices - 1;
	indices[k + 1] = (NumSphereVertices - 1) - LongLines;
	indices[k + 2] = NumSphereVertices - 2;

	D3D11_BUFFER_DESC indexBufferDesc = {};

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * NumSphereFaces * 3;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData;

	iinitData.pSysMem = &indices[0];
	gameUtil.getDevice()->CreateBuffer(&indexBufferDesc, &iinitData, &sphereIndexBuffer);

}

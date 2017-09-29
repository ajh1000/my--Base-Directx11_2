#include "stdafx.h"
#include "gameObject.h"


gameObject::gameObject()
{
	settingDeafaultLayouts();

}


gameObject::~gameObject()
{
	
			SAFE_RELEASE(m_vs);

		SAFE_RELEASE(m_ps);
		SAFE_RELEASE(m_layout);
		SAFE_RELEASE(m_vertexBuffer);
		SAFE_RELEASE(m_indexBuffer);
		SAFE_RELEASE(m_WVPBuffer);

		SAFE_RELEASE(m_vsblob);
		SAFE_RELEASE(m_psblob);
	
	
	


}

void gameObject::settingDeafaultLayouts()
{
	//.layout_pc
	layout_pc[0].SemanticName = "POSITION";
	layout_pc[0].SemanticIndex = 0;
	layout_pc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	layout_pc[0].InputSlot = 0;
	layout_pc[0].AlignedByteOffset = 0;
	layout_pc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	layout_pc[0].InstanceDataStepRate = 0;

	layout_pc[1].SemanticName = "COLOR";
	layout_pc[1].SemanticIndex = 0;
	layout_pc[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	layout_pc[1].InputSlot = 0;
	layout_pc[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	layout_pc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	layout_pc[1].InstanceDataStepRate = 0;

	//.layout_pt
	layout_pt[0].SemanticName = "POSITION";
	layout_pt[0].SemanticIndex = 0;
	layout_pt[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	layout_pt[0].InputSlot = 0;
	layout_pt[0].AlignedByteOffset = 0;
	layout_pt[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	layout_pt[0].InstanceDataStepRate = 0;

	layout_pt[1].SemanticName = "TEXCOORD";
	layout_pt[1].SemanticIndex = 0;
	layout_pt[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	layout_pt[1].InputSlot = 0;
	layout_pt[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	layout_pt[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	layout_pt[1].InstanceDataStepRate = 0;


	//.layout_ptn	
	layout_ptn[0].SemanticName = "POSITION";
	layout_ptn[0].SemanticIndex = 0;
	layout_ptn[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	layout_ptn[0].InputSlot = 0;
	layout_ptn[0].AlignedByteOffset = 0;
	layout_ptn[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	layout_ptn[0].InstanceDataStepRate = 0;

	layout_ptn[1].SemanticName = "TEXCOORD";
	layout_ptn[1].SemanticIndex = 0;
	layout_ptn[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	layout_ptn[1].InputSlot = 0;
	layout_ptn[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	layout_ptn[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	layout_ptn[1].InstanceDataStepRate = 0;

	layout_ptn[2].SemanticName = "NORMAL";
	layout_ptn[2].SemanticIndex = 0;
	layout_ptn[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	layout_ptn[2].InputSlot = 0;
	layout_ptn[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	layout_ptn[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	layout_ptn[2].InstanceDataStepRate = 0;

	//.layout_ptn_skinned
	layout_ptn_skinned[0].SemanticName = "POSITION";
	layout_ptn_skinned[0].SemanticIndex = 0;
	layout_ptn_skinned[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	layout_ptn_skinned[0].InputSlot = 0;
	layout_ptn_skinned[0].AlignedByteOffset = 0;
	layout_ptn_skinned[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	layout_ptn_skinned[0].InstanceDataStepRate = 0;

	layout_ptn_skinned[1].SemanticName = "TEXCOORD";
	layout_ptn_skinned[1].SemanticIndex = 0;
	layout_ptn_skinned[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	layout_ptn_skinned[1].InputSlot = 0;
	layout_ptn_skinned[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	layout_ptn_skinned[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	layout_ptn_skinned[1].InstanceDataStepRate = 0;

	layout_ptn_skinned[2].SemanticName = "NORMAL";
	layout_ptn_skinned[2].SemanticIndex = 0;
	layout_ptn_skinned[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	layout_ptn_skinned[2].InputSlot = 0;
	layout_ptn_skinned[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	layout_ptn_skinned[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	layout_ptn_skinned[2].InstanceDataStepRate = 0;

	layout_ptn_skinned[3].SemanticName = "WEIGHTS";
	layout_ptn_skinned[3].SemanticIndex = 0;
	layout_ptn_skinned[3].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	layout_ptn_skinned[3].InputSlot = 0;
	layout_ptn_skinned[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	layout_ptn_skinned[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	layout_ptn_skinned[3].InstanceDataStepRate = 0;

	layout_ptn_skinned[4].SemanticName = "BONEINDICES";
	layout_ptn_skinned[4].SemanticIndex = 0;
	layout_ptn_skinned[4].Format = DXGI_FORMAT_R32G32B32A32_UINT;
	layout_ptn_skinned[4].InputSlot = 0;
	layout_ptn_skinned[4].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	layout_ptn_skinned[4].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	layout_ptn_skinned[4].InstanceDataStepRate = 0;

}

void gameObject::Init_compileShader(char * vsDir, char * psDir)
{
	if (m_vs != nullptr && m_ps != nullptr)
		return;

	HRESULT result, result2;
	result = D3DX11CompileFromFile(vsDir, NULL, NULL, "main", "vs_4_0", D3D10_SHADER_ENABLE_STRICTNESS | D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION, 0, NULL,
		&m_vsblob, NULL, NULL);



	result2 = D3DX11CompileFromFile(psDir, NULL, NULL, "main", "ps_4_0", D3D10_SHADER_ENABLE_STRICTNESS | D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION, 0, NULL,
		&m_psblob, NULL, NULL);

	//assert는 디버그 빌드일때만 작동.
	//릴리즈 빌드일때도 작동할려면 메시지박스로 띄우고 시스템종료하든가해야함.
	//false일때만 작동.
	//assert(!(m_vsblob == nullptr || m_psblob == nullptr)); 

	ERROR_MSGBOX(m_vsblob == nullptr || m_psblob == nullptr, "shader compile error");

	gameUtil.getDevice()->CreateVertexShader(m_vsblob->GetBufferPointer(), m_vsblob->GetBufferSize(), 0, &m_vs);
	gameUtil.getDevice()->CreatePixelShader(m_psblob->GetBufferPointer(), m_psblob->GetBufferSize(), 0, &m_ps);


}

void gameObject::Init_polygonLayout(EPolygonLayout eType)
{
	if (eType == EPolygonLayout::LAYOUT_PC)
	{
		gameUtil.getDevice()->CreateInputLayout(layout_pc, 2, m_vsblob->GetBufferPointer(),
			m_vsblob->GetBufferSize(), &m_layout);
	}
	else if (eType == EPolygonLayout::LAYOUT_PT)
	{
		gameUtil.getDevice()->CreateInputLayout(layout_pt, 2, m_vsblob->GetBufferPointer(),
			m_vsblob->GetBufferSize(), &m_layout);

		HRESULT result;
		D3D11_SAMPLER_DESC samplerDesc = {};
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 1;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.BorderColor[0] = 0;
		samplerDesc.BorderColor[1] = 0;
		samplerDesc.BorderColor[2] = 0;
		samplerDesc.BorderColor[3] = 0;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		result = gameDevice->CreateSamplerState(&samplerDesc, &m_sampleState);
	}
	else if (eType == EPolygonLayout::LAYOUT_PTN)
	{
		gameUtil.getDevice()->CreateInputLayout(layout_ptn, 3, m_vsblob->GetBufferPointer(),
			m_vsblob->GetBufferSize(), &m_layout);
	}
	else if (eType == EPolygonLayout::LAYOUT_PTN_SKINNED)
	{
		gameUtil.getDevice()->CreateInputLayout(layout_ptn_skinned, 5, m_vsblob->GetBufferPointer(),
			m_vsblob->GetBufferSize(), &m_layout);
	}

	gameObject::Init_CreateConstantBuffer(&m_WVPBuffer, sizeof(matrix_WorldViewProj));
}

void gameObject::Init_CreateVertexBuffer(void* pdata, int ByteWidth, UINT count, D3D11_USAGE usage,
	D3D11_BIND_FLAG bindFlags, UINT CPUAccessFlags)
{
	D3D11_BUFFER_DESC vertexBufferDesc = {};
	D3D11_SUBRESOURCE_DATA vertexData = {};
	m_vertexCount = count;
	m_sizeVertexType = ByteWidth;

	vertexBufferDesc.Usage = usage;
	vertexBufferDesc.ByteWidth = m_sizeVertexType * m_vertexCount;
	vertexBufferDesc.BindFlags = bindFlags;
	vertexBufferDesc.CPUAccessFlags = CPUAccessFlags;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = pdata;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	HRESULT result = gameUtil.getDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);

}

void gameObject::Init_option_CreateIndexBuffer(void * pdata, int ByteWidth,
	UINT count, D3D11_USAGE usage, D3D11_BIND_FLAG bindFlags, UINT CPUAccessFlags)
{
	D3D11_BUFFER_DESC desc = {};
	desc.Usage = usage;
	desc.ByteWidth = sizeof(DWORD) * count;
	desc.BindFlags = bindFlags;
	desc.CPUAccessFlags = CPUAccessFlags;
	desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA indexData = {};
	indexData.pSysMem = pdata;

	gameUtil.getDevice()->CreateBuffer(&desc, &indexData, &m_indexBuffer);

	m_numFaces = count / 3;
	m_indexCount = count;
}

void gameObject::Init_CreateConstantBuffer(ID3D11Buffer** ppbuffer, UINT ByteWidth,
	D3D11_USAGE Usage, UINT CPUAccessFlags)
{
	D3D11_BUFFER_DESC desc = {};
	desc.Usage = Usage;
	desc.ByteWidth = ByteWidth;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = CPUAccessFlags;

	HRESULT result = gameUtil.getDevice()->CreateBuffer(&desc, NULL, ppbuffer);

}


void gameObject::turnOnAlphaBlending()
{
	float blendFactor[4];


	// Setup the blend factor.
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	// Turn on the alpha blending.
	gameUtil.getDeviceContext()->OMSetBlendState(gameUtil.m_alphaEnableBlendingState, blendFactor, 0xffffffff);

}

void gameObject::turnOffAlphaBlending()
{
	float blendFactor[4];


	// Setup the blend factor.
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	// Turn off the alpha blending.
	gameUtil.getDeviceContext()->OMSetBlendState(gameUtil.m_alphaDisableBlendingState, blendFactor, 0xffffffff);

}

void gameObject::init()
{
}

void gameObject::update()
{
}

void gameObject::render()
{
	//every object have transform. so automate code.
	transform.buildMatrixWVP();
	matrix_WorldViewProj wvp = transform.getWVP();

	D3DXMatrixTranspose(&wvp.world, &wvp.world);
	D3DXMatrixTranspose(&wvp.view, &wvp.view);
	D3DXMatrixTranspose(&wvp.proj, &wvp.proj);

	SetVSParameters<matrix_WorldViewProj>(m_WVPBuffer, wvp);


	//render
	unsigned int stride;
	unsigned int offset;


	stride = m_sizeVertexType;
	offset = 0;


	gameUtil.getDeviceContext()->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);


	gameUtil.getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	gameUtil.getDeviceContext()->IASetInputLayout(m_layout);
	gameUtil.getDeviceContext()->VSSetShader(m_vs, 0, 0);
	gameUtil.getDeviceContext()->PSSetShader(m_ps, 0, 0);

	gameUtil.getDeviceContext()->Draw(m_vertexCount, 0);


}

void gameObject::renderIndexed()
{


	//every object have transform. so automate the code.
	transform.buildMatrixWVP();
	matrix_WorldViewProj wvp = transform.getWVP();

	D3DXMatrixTranspose(&wvp.world, &wvp.world);
	D3DXMatrixTranspose(&wvp.view, &wvp.view);
	D3DXMatrixTranspose(&wvp.proj, &wvp.proj);

	SetVSParameters<matrix_WorldViewProj>(m_WVPBuffer, wvp);


	//render
	unsigned int stride;
	unsigned int offset;


	stride = m_sizeVertexType;
	offset = 0;

	gameUtil.getDeviceContext()->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	gameUtil.getDeviceContext()->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	gameUtil.getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	gameUtil.getDeviceContext()->IASetInputLayout(m_layout);
	gameUtil.getDeviceContext()->VSSetShader(m_vs, 0, 0);
	gameUtil.getDeviceContext()->PSSetShader(m_ps, 0, 0);

	gameDeviceContext->DrawIndexed(m_indexCount, 0, 0);

}
#pragma once

#include "Transform.h"
#include "textureclass.h"

#include <map>
#include <string>

using namespace std;



class gameObject
{
public:
	gameObject();
	virtual ~gameObject();

private:

	D3D11_INPUT_ELEMENT_DESC layout_pc[2] = {};
	D3D11_INPUT_ELEMENT_DESC layout_pt[2] = {};
	D3D11_INPUT_ELEMENT_DESC layout_ptn[3] = {};
	D3D11_INPUT_ELEMENT_DESC layout_ptn_skinned[5] = {};


	void settingDeafaultLayouts();

protected:
	enum EPolygonLayout { LAYOUT_PC = 0, LAYOUT_PT, LAYOUT_PTN,LAYOUT_PTN_SKINNED };

protected: //MUST USE
	void Init_compileShader(char* vsDir, char* psDir);
	void Init_polygonLayout(EPolygonLayout eType);

	void Init_CreateConstantBuffer(ID3D11Buffer** ppbuffer, UINT ByteWidth,
		D3D11_USAGE Usage = D3D11_USAGE_DYNAMIC, UINT CPUAccessFlags = D3D11_CPU_ACCESS_WRITE);

	void Init_CreateVertexBuffer(void* pdata, int ByteWidth, UINT count, D3D11_USAGE usage,
		D3D11_BIND_FLAG bindFlags, UINT CPUAccessFlags);
	void Init_option_CreateIndexBuffer(void* pdata, int ByteWidth, UINT count, D3D11_USAGE usage,
		D3D11_BIND_FLAG bindFlags, UINT CPUAccessFlags);

	template<typename  Type>
	void SetVSParameters(ID3D11Buffer* buffer, Type& data);

	template<typename  Type>
	void SetPSParameters(ID3D11Buffer* buffer, Type& data);

public:
	virtual void init();
	virtual void update();
	virtual void render();
	virtual void renderIndexed();

public:
	MyTransform transform;

protected:
	ID3D11VertexShader* m_vs = nullptr;
	ID3D11PixelShader* m_ps = nullptr;
	ID3D11InputLayout* m_layout = nullptr;
	ID3D10Blob* m_vsblob = nullptr, *m_psblob = nullptr; //release in Init_polygonLayout


	ID3D11Buffer* m_vertexBuffer = nullptr;
	unsigned int m_sizeVertexType = 0;
	unsigned int m_vertexCount = 0;

	ID3D11Buffer* m_indexBuffer = nullptr;
	unsigned int m_numFaces = 0;
	unsigned int m_indexCount = 0;

	ID3D11SamplerState* m_sampleState = nullptr;

	ID3D11Buffer* m_WVPBuffer = 0;

};
#define super gameObject
#define gameUtil d3dutil::GetInstance()



template<typename  Type>
inline void gameObject::SetVSParameters(ID3D11Buffer* buffer, Type& data)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource = {};
	unsigned int bufferNumber;

	HRESULT result = gameUtil.getDeviceContext()->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	memcpy_s(mappedResource.pData, sizeof(Type), &data, sizeof(Type));


	gameUtil.getDeviceContext()->Unmap(buffer, 0);
	
	bufferNumber =0;
	gameUtil.getDeviceContext()->VSSetConstantBuffers(bufferNumber, 1, &buffer);
}

template<typename Type>
inline void gameObject::SetPSParameters(ID3D11Buffer* buffer, Type & data)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource = {};
	unsigned int bufferNumber;

	HRESULT result = gameUtil.getDeviceContext()->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);


	memcpy_s(mappedResource.pData, sizeof(Type), &data, sizeof(Type));


	gameUtil.getDeviceContext()->Unmap(buffer, 0);

	bufferNumber = 0;
	gameUtil.getDeviceContext()->PSSetConstantBuffers(bufferNumber, 1, &buffer);
}

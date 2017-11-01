#include "stdafx.h"
#include "textureclass.h"

#include <DirectXTex.h>

textureclass::textureclass()
{
	m_textureView = nullptr;
}


textureclass::~textureclass()
{
	
	SAFE_RELEASE(m_texture)
	SAFE_RELEASE(m_textureView);
}


bool textureclass::load(string dir)
{
	// Load the texture in.
	wstring convert = wstring(dir.begin(), dir.end());
	HRESULT result;

	ScratchImage srcImg;

	result = LoadFromWICFile(convert.c_str(), WIC_FLAGS_IGNORE_SRGB , nullptr, srcImg);


	//then try dds
	if (FAILED(result))
	{
		result = LoadFromDDSFile(convert.c_str(), WIC_FLAGS_IGNORE_SRGB, nullptr, srcImg);

		//then try tga
		if (FAILED(result))
		{
			result = LoadFromTGAFile(convert.c_str(), nullptr, srcImg);

			//all failed. do assert
			if (FAILED(result))
			{
				bool failedTexLoading = 0;
				MessageBox(0, "failedTexLoading", "error", 0);
				//assert(failedTexLoading);
			}
		}
	}
	result = CreateShaderResourceView(gameUtil.getDevice(), srcImg.GetImages(), srcImg.GetImageCount(),
		srcImg.GetMetadata(), &m_textureView);

	return true;
}

void textureclass::loadCubemap(string dir)
{
	HRESULT hr;
	D3DX11_IMAGE_LOAD_INFO loadInfo;
	loadInfo.MiscFlags = D3D11_RESOURCE_MISC_FLAG::D3D11_RESOURCE_MISC_TEXTURECUBE;
	ID3D11Texture2D *SphereTexture = nullptr;
	hr = D3DX11CreateTextureFromFile(gameUtil.getDevice(), "./data/cubemap/cubemap.dds",
		&loadInfo, NULL, (ID3D11Resource**)&SphereTexture, NULL);
	if (FAILED(hr))
		int a = 0;
	D3D11_TEXTURE2D_DESC texDesc = { 0 };
	SphereTexture->GetDesc(&texDesc);
	D3D11_SHADER_RESOURCE_VIEW_DESC shadDesc;
	shadDesc.Format = texDesc.Format;
	shadDesc.ViewDimension = D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURECUBE;
	shadDesc.TextureCube.MipLevels = texDesc.MipLevels;
	shadDesc.TextureCube.MostDetailedMip = 0;
	hr = gameUtil.getDevice()->CreateShaderResourceView(SphereTexture, &shadDesc, &m_textureView);
	SphereTexture->Release();
}

ID3D11ShaderResourceView * textureclass::GetTexture()
{
	return m_textureView;
}

void textureclass::render()
{

		gameDeviceContext->PSSetShaderResources(0, 1, &m_textureView);
	

}

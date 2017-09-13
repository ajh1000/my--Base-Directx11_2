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

ID3D11ShaderResourceView * textureclass::GetTexture()
{
	return m_textureView;
}

void textureclass::render()
{

		gameDeviceContext->PSSetShaderResources(0, 1, &m_textureView);
	

}

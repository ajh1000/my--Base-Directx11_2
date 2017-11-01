#pragma once




class textureclass
{
public:
	textureclass();
	virtual ~textureclass();
	
	bool load(std::string dir);
	void loadCubemap(string dir);
	ID3D11ShaderResourceView* GetTexture();
	void render();
private:
	ID3D11Texture2D* m_texture =nullptr;
	ID3D11ShaderResourceView*  m_textureView=nullptr;
};


#pragma once

#include <Awesomium/WebCore.h>
#include <Awesomium/BitmapSurface.h>
#include <Awesomium/STLHelpers.h>
using namespace Awesomium;


class WebInstance
{
public:
	WebInstance();

	~WebInstance();
	void release();

	singleton_class(WebInstance)
public:
	WebCore* m_webCore = nullptr;
};
#define getWebInstance WebInstance::GetInstance()

class HTMLTexture
{
public:
	HTMLTexture();
	virtual ~HTMLTexture();



	void load(string url,int xpos,int ypos,int width,int height,int textureWidth,int textureHeight);
	void render();
	void setAlwaysRender(bool _always);
public:
	WebView* m_view=nullptr; //same as Tab of browser

	ID3D11Texture2D* m_texture;
	ID3D11ShaderResourceView*  m_textureView;

	RECT m_rect;
	bool m_isAlwaysRender=false;
};


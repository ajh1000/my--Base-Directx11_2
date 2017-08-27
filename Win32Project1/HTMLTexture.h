#pragma once

#include <Awesomium/WebCore.h>
#include <Awesomium/BitmapSurface.h>
#include <Awesomium/STLHelpers.h>
using namespace Awesomium;

class HTMLTexture
{
public:
	HTMLTexture();
	virtual ~HTMLTexture();



	void load(string url,int xpos,int ypos,int width,int height,int textureWidth,int textureHeight);
	void render();
	void setAlwaysRender(bool _always);
	void injectLButtonDown(int x, int y);
	void injectLButtonUp(int x, int y);
public:
	WebView* m_view=nullptr; //same as Tab of browser
	WebSession* m_webSession = nullptr;
	ID3D11Texture2D* m_texture;
	ID3D11ShaderResourceView*  m_textureView;

	RECT m_rect;
private:

	bool m_isAlwaysRender=false;
};


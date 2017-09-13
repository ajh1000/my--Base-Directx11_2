#include "stdafx.h"
#include "HTMLTexture.h"


HTMLTexture::HTMLTexture() :
	m_texture(nullptr),
	m_textureView(nullptr),
	m_view(nullptr)
{
}


HTMLTexture::~HTMLTexture()
{
	SAFE_RELEASE(m_texture);
	SAFE_RELEASE(m_textureView);

}

void HTMLTexture::load(string url, int xpos, int ypos, int width, int height, int textureWidth, int textureHeight)
{
	m_rect.left = xpos - width / 2;
	m_rect.top = ypos - height / 2;
	//m_rect.right = xpos + width;
	//m_rect.bottom = ypos + height;
	m_rect.right = width;
	m_rect.bottom = height;
	WebPreferences pref;
	pref.enable_web_audio = true;
	pref.enable_web_gl = true;
	pref.enable_gpu_acceleration = true;
	WebString my_string = WebString::CreateFromUTF8("", strlen(""));

	m_webSession = WebCore::instance()->CreateWebSession(my_string, pref);

	m_view = WebCore::instance()->CreateWebView(textureWidth + 50, textureHeight + 50, m_webSession);

	m_view->SetTransparent(true);

	WebURL webUrl(WSLit(url.c_str()));
	m_view->LoadURL(webUrl);

	BitmapSurface* surface = (BitmapSurface*)m_view->surface();


	CD3D11_TEXTURE2D_DESC desc = CD3D11_TEXTURE2D_DESC(DXGI_FORMAT_B8G8R8A8_UNORM, textureWidth, textureHeight, 1, 1);
	gameDevice->CreateTexture2D(&desc, nullptr, &m_texture);
	gameDevice->CreateShaderResourceView(m_texture, 0, &m_textureView);
}

void HTMLTexture::render()
{
	
	POINT pt = { gameUtil.m_mouseX,gameUtil.m_mouseY };
	RECT rect = m_rect;
	rect.right += m_rect.left;
	rect.bottom += m_rect.top;

	if (m_isAlwaysRender || PtInRect(&rect, pt))
	{
		if (m_view) {
			WebCore::instance()->Update();
			Awesomium::BitmapSurface * surface = static_cast<Awesomium::BitmapSurface *>(m_view->surface());
			if (surface != nullptr)
			{
				gameUtil.getDeviceContext()->UpdateSubresource(m_texture, 0, 0, surface->buffer(), surface->row_span(), 0);
			}

		}
	}


	gameDeviceContext->PSSetShaderResources(0, 1, &m_textureView);
}

void HTMLTexture::setAlwaysRender(bool _always)
{
	m_isAlwaysRender = _always;
}

void HTMLTexture::injectLButtonDown(int x, int y)
{
	int mouseX, mouseY;
	POINT pt = { x, y };
	RECT rect = m_rect;
	rect.right += m_rect.left;
	rect.bottom += m_rect.top;

	if (PtInRect(&rect, pt))
	{
		mouseX = pt.x - m_rect.left;
		mouseY = pt.y - m_rect.top;

		m_view->InjectMouseMove(mouseX, mouseY);
		m_view->InjectMouseDown(MouseButton::kMouseButton_Left);
	}
}

void HTMLTexture::injectLButtonUp(int x, int y)
{
	int mouseX, mouseY;
	POINT pt = { x, y };
	RECT rect = m_rect;
	rect.right += m_rect.left;
	rect.bottom += m_rect.top;

	if (PtInRect(&rect, pt))
	{
		mouseX = pt.x - m_rect.left;
		mouseY = pt.y - m_rect.top;

		m_view->InjectMouseMove(mouseX, mouseY);
		m_view->InjectMouseUp(MouseButton::kMouseButton_Left);
	}
}


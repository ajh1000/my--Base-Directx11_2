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
	m_rect.left = xpos;
	m_rect.top = ypos;
	m_rect.right = xpos+ width;
	m_rect.bottom = ypos+ height;

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

	if (m_isAlwaysRender || PtInRect(&m_rect, pt))
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

	if (PtInRect(&m_rect, pt))
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

	if (PtInRect(&m_rect, pt))
	{
		mouseX = pt.x - m_rect.left;
		mouseY = pt.y - m_rect.top;

		m_view->InjectMouseMove(mouseX, mouseY);
		m_view->InjectMouseUp(MouseButton::kMouseButton_Left);
	}
}


/*

void textureclass::loadHttpDir(char* dir)
{
WebConfig config = WebConfig();
config.user_agent = WSLit("Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/41.0.2228.0 Safari/537.36");

string useragentName = ToString(config.user_agent);
string userscript = ToString(config.user_script); //진짜 자바 스크립트 내용을 삽입하는 부분.
string userstyle = ToString(config.user_stylesheet); // 마찬가지.

m_webCore = WebCore::Initialize(config);


m_view = m_webCore->CreateWebView(800, 600);

WebURL url(WSLit(dir));
m_view->LoadURL(url);

while (m_view->IsLoading())
m_webCore->Update();

// Sleep a bit and update once more to give scripts and plugins
// on the page a chance to finish loading.
//Sleep(600);
m_webCore->Update();

BitmapSurface* surface = (BitmapSurface*)m_view->surface();


//D3D11_TEXTURE2D_DESC td;
//td.ArraySize = 1;
//td.BindFlags = D3D11_BIND_SHADER_RESOURCE;
//td.Usage = D3D11_USAGE_IMMUTABLE;
//td.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
//td.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
//td.Height = 800;
//td.Width = 600;
//td.MipLevels = 1;
//td.MiscFlags = 0;
//td.SampleDesc.Count = 1;
//td.SampleDesc.Quality = 0;
//
//D3D11_SUBRESOURCE_DATA srd;
//srd.pSysMem = surface->buffer();
//srd.SysMemPitch = surface->row_span();
//srd.SysMemSlicePitch = 0;

//gameDevice->CreateTexture2D(&td, &srd, &m_texture);

CD3D11_TEXTURE2D_DESC desc = CD3D11_TEXTURE2D_DESC(DXGI_FORMAT_B8G8R8A8_UNORM, 800, 600, 1, 1);
gameDevice->CreateTexture2D(&desc, nullptr, &m_texture);
gameDevice->CreateShaderResourceView(m_texture, 0, &m_textureView);

}

//render

//
//if (m_view) {

int x = gameUtil.m_mouseX;
int y = gameUtil.m_mouseY;


m_view->InjectMouseMove(gameUtil.m_mouseX, gameUtil.m_mouseY);


m_webCore->Update();
Awesomium::BitmapSurface * surface = static_cast<Awesomium::BitmapSurface *>(m_view->surface());
//
//int a = surface->row_span();  3200
//int b = surface->width()* surface->height(); 480000
//int c = surface->width() * 4; 3200
//int d = surface->width(); 800
//int e = surface->height(); 600


gameUtil.getDeviceContext()->UpdateSubresource(m_texture, 0, 0, surface->buffer(), surface->row_span(), 0);

//
//UpdateSubresource는 밑에 코드를 자동으로 수행한다. 다만 버퍼의 usage는 반드시 default로 설정해야한다.
//map()~unmap()을 할려면 dynamic 으로 설정해야한다.
//
//D3D11_MAPPED_SUBRESOURCE mappedResource;
//gameDeviceContext->Map(m_texture, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
//memcpy(mappedResource.pData, surface->buffer(), surface->row_span()*surface->height());
//gameDeviceContext->Unmap(m_texture, 0);


gameDeviceContext->PSSetShaderResources(0, 1, &m_textureView);
}
*/

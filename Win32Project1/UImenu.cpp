#include "stdafx.h"
#include "UImenu.h"


UImenu::UImenu()
{
}


UImenu::~UImenu()
{
}

void UImenu::init(int xpos, int ypos, int vertexWidth, int vertexHeight,
	int textureWidth, int textureHeight, string URL)
{
	GameUIObject::init(xpos, ypos, vertexWidth, vertexHeight, textureWidth, textureHeight, URL);

	WebView* webView = m_htmlTexture.m_view;

	if (webView)
	{
		JSObject myObject = webView->CreateGlobalJavascriptObject(WSLit("myObject")).ToObject();


		myObject.SetCustomMethod(WSLit("play"), false);
		myObject.SetCustomMethod(WSLit("quit"), false);


		webView->set_js_method_handler(&m_dispatcher);
		
	}
	m_dispatcher.m_menu = this;
}

void UImenu::update()
{
	if (keyMgr.IsPressed(VK_LBUTTON))
	{
		m_htmlTexture.injectLButtonDown(gameUtil.m_mouseX, gameUtil.m_mouseY);

	}

	if (keyMgr.IsUp(VK_LBUTTON))
	{
		m_htmlTexture.injectLButtonUp(gameUtil.m_mouseX, gameUtil.m_mouseY);

	}


}

void UImenu::render()
{
	GameUIObject::render();
	

}

void MenuDispatcher::OnMethodCall(Awesomium::WebView * caller, unsigned int remote_object_id, const Awesomium::WebString & method_name, const Awesomium::JSArray & args)
{
	string methodName(ToString(method_name));

	if (!methodName.compare("quit"))
	{
		PostQuitMessage(0);
	}
	else if (!methodName.compare("play"))
	{
		m_menu->setEnable(false);
		gameUtil.m_gameUIScene->m_mapUI["healthBar"]->setEnable(true);
	}

}

Awesomium::JSValue MenuDispatcher::OnMethodCallWithReturnValue(Awesomium::WebView * caller, unsigned int remote_object_id, const Awesomium::WebString & method_name, const Awesomium::JSArray & args)
{
	return Awesomium::JSValue();
}

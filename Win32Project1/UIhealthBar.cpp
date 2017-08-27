#include "stdafx.h"
#include "UIhealthBar.h"


UIhealthBar::UIhealthBar()
{
}


UIhealthBar::~UIhealthBar()
{
}

void UIhealthBar::init(int xpos, int ypos, int vertexWidth, int vertexHeight, int textureWidth, int textureHeight, string URL)
{
	GameUIObject::init(xpos, ypos, vertexWidth, vertexHeight, textureWidth, textureHeight, URL);

	
}

void UIhealthBar::update()
{
	if (keyMgr.IsPressed(VK_SPACE))
	{
		if (m_htmlTexture.m_view)
		{

			JSValue window = m_htmlTexture.m_view->ExecuteJavascriptWithResult(
				WSLit("window"), WSLit(""));

			if (window.IsObject()) {
				JSArray args;
				args.Push(JSValue(0.2));

				window.ToObject().Invoke(WSLit("damage"), args);
			}

		}

	}

}

void UIhealthBar::render()
{
	GameUIObject::render();
}
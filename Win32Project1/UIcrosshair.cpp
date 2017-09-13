#include "stdafx.h"
#include "UIcrosshair.h"


UIcrosshair::UIcrosshair()
{
}


UIcrosshair::~UIcrosshair()
{
}

void UIcrosshair::init(int xpos, int ypos, int vertexWidth, int vertexHeight, int textureWidth, int textureHeight, string URL)
{
	GameUIObject::init(xpos, ypos, vertexWidth, vertexHeight, textureWidth, textureHeight, URL);

}

void UIcrosshair::update()
{
}

void UIcrosshair::render()
{
	GameUIObject::render();
}

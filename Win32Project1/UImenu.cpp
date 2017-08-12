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
}

void UImenu::update()
{

}

void UImenu::render()
{
	GameUIObject::render();
}

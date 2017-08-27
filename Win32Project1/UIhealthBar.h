#pragma once
#include "GameUIObject.h"

class UIhealthBar :
	public GameUIObject
{
public:
	UIhealthBar();
	virtual ~UIhealthBar();

	void init(int xpos, int ypos, int vertexWidth, int vertexHeight,
		int textureWidth, int textureHeight, string URL);
	virtual void update();
	virtual void render();
private:
};


#pragma once
#include "gameObject.h"
#include "HTMLTexture.h"
/*
	FOR 2D SPACE UI CLASS
*/
class GameUIObject : public gameObject
{
public:
	GameUIObject();
	virtual ~GameUIObject();

public:

	void init(int xpos, int ypos, int vertexWidth, int vertexHeight,
		int textureWidth, int textureHeight,string URL);
	virtual void update();
	virtual void render();

	void setEnable(bool enable);

public:
	HTMLTexture m_htmlTexture;

public:
	bool m_isEnabled = true;
};


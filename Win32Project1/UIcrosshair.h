#pragma once
#include "gamePlayer.h"
#include "gameCameraThirdPerson.h"
class UIcrosshair :
	public GameUIObject
{
public:
	UIcrosshair();
	virtual ~UIcrosshair(); 
	
	void init(int xpos, int ypos, int vertexWidth, int vertexHeight,
		int textureWidth, int textureHeight, string URL);
	virtual void update();
	virtual void render();
	D3DXVECTOR3 screenToWorld(D3DXVECTOR3 vIn);
private:
	
};


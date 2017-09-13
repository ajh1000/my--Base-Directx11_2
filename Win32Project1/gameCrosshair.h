#pragma once
#include "gamePlayer.h"
#include "gameCameraThirdPerson.h"

class gameCrosshair : public gameObject
{
public:
	gameCrosshair();
	virtual ~gameCrosshair();
	
	virtual void init();
	virtual void update();
	void lateUpdate();
	virtual void render();

private:
	textureclass m_texture;
	gamePlayer* m_pPlayer = 0;
	gameCameraThirdPerson* m_pCamera = 0;
};


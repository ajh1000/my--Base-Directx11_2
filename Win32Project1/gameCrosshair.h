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
	void initDepthStencilState();
	void TurnZBufferOn();
	void TurnZBufferOff();
private:
	textureclass m_texture;
	gamePlayer* m_pPlayer = 0;
	gameCameraThirdPerson* m_pCamera = 0;
	ID3D11DepthStencilState* m_depthStencilState = nullptr;
	ID3D11DepthStencilState* m_depthDisabledStencilState = nullptr;
};


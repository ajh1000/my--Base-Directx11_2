#pragma once
#include "GameUIObject.h"

class GameUIScene
{
public:
	GameUIScene();
	virtual ~GameUIScene();

	void init();
	void initDepthStencilState();
	void update();
	void render();
private:
	void TurnZBufferOn();
	void TurnZBufferOff();
private:
	vector<GameUIObject*> m_vecGameUIObjects;
	
	ID3D11DepthStencilState* m_depthStencilState = nullptr;
	ID3D11DepthStencilState* m_depthDisabledStencilState=nullptr;

};


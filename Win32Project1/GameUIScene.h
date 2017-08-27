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
public:
	vector<GameUIObject*> m_vecGameUIObjects;
	map<string, GameUIObject*> m_mapUI;
private:
	ID3D11DepthStencilState* m_depthStencilState = nullptr;
	ID3D11DepthStencilState* m_depthDisabledStencilState=nullptr;

};


#pragma once

#include "stdafx.h"
#include <vector>
using namespace std;

#include "gameCamera.h"
#include "gameObject.h"
#include "gameCamera.h"
#include "Model.h"
#include "MyPathfinder.h"
#include "GameUIScene.h"

class d3dutil
{
public:
	d3dutil();
	~d3dutil();

public:
	singleton_class(d3dutil)
	
		/*
	static d3dutil& GetInstance()
	{
		static d3dutil instance;

		return instance;
	}*/

	

	void SetDevice(ID3D11Device* _device);
	ID3D11Device*& getDevice();
	void SetDeviceContext(ID3D11DeviceContext* _context);
	ID3D11DeviceContext*& getDeviceContext();

	void SetMainCamera(GameCamera* camera);
	class GameCamera* GetMainCamera();


	void SetFeatureLevel(D3D_FEATURE_LEVEL _level);
	int GetFeatureLevel();

	HWND GetHWND();
	void SetHWND(HWND _hWND);

	

public:
	int m_mouseX=0, m_mouseY =0;
	bool m_windowActive = true;

	MyPathfinder* myPath = 0;

	std::vector<gameObject*> m_vecGameObjects;
	std::vector<Model*> m_vecEnemys;
	map<string, Model*> m_mapTag; //This is just pointing m_vecEnemys, m_vecGameObjects. so no need to clear contents.

	GameUIScene* m_gameUIScene=nullptr;
private:
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;



	D3D_FEATURE_LEVEL m_featureLevel;

	HWND m_hWnd;

	GameCamera* mainCamera = nullptr;


};

#define gameUtil d3dutil::GetInstance()
#define getGameCamera gameUtil.GetMainCamera()
#define getFeatureLevel gameUtil.GetFeatureLevel()
#define gameDevice gameUtil.getDevice()
#define gameDeviceContext gameUtil.getDeviceContext()
#define SAFE_RELEASE(t) if(t) { t->Release(); t=0;}
#define SAFE_DELETE(t) if(t) { delete t; t=nullptr;}
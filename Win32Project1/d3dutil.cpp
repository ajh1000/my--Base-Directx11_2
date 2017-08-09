#include "stdafx.h"
#include "gameCamera.h"
#include "d3dutil.h"



d3dutil::d3dutil()
{
}


d3dutil::~d3dutil()
{
	for (int i = 0; i < m_vecEnemys.size(); ++i)
	{
		SAFE_DELETE(m_vecEnemys[i]);
	}

	for (int i = 0; i < m_vecGameObjects.size(); ++i)
	{
		SAFE_DELETE(m_vecGameObjects[i]);
	}



}


void d3dutil::SetDevice(ID3D11Device * _device)
{
	m_device = _device;
}

ID3D11Device *& d3dutil::getDevice()
{
	return m_device;
}

void d3dutil::SetDeviceContext(ID3D11DeviceContext * _context)
{
	m_deviceContext = _context;
}

ID3D11DeviceContext *& d3dutil::getDeviceContext()
{
	return m_deviceContext;
}

void d3dutil::SetMainCamera(GameCamera * camera)
{
	mainCamera = camera;
}

GameCamera * d3dutil::GetMainCamera()
{
	return mainCamera;
}


void d3dutil::SetFeatureLevel(D3D_FEATURE_LEVEL _level)
{
	m_featureLevel = _level;
}

int d3dutil::GetFeatureLevel()
{
	return m_featureLevel;
}

HWND d3dutil::GetHWND()
{
	return m_hWnd;
}

void d3dutil::SetHWND(HWND _hWND)
{
	m_hWnd = _hWND;
}

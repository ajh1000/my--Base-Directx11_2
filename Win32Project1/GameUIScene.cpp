#include "stdafx.h"
#include "GameUIScene.h"
#include "UImenu.h"
#include "UIhealthBar.h"
#include "UIcrosshair.h"
GameUIScene::GameUIScene()
{
}


GameUIScene::~GameUIScene()
{
	for (int i = 0; i < m_vecGameUIObjects.size(); ++i)
	{
		SAFE_DELETE(m_vecGameUIObjects[i]);
	}

	WebCore::Shutdown();

	SAFE_RELEASE(m_depthDisabledStencilState)
	SAFE_RELEASE(m_depthStencilState)
}

void GameUIScene::init()
{
	initDepthStencilState();

	//http://35.194.254.46/ Apache2 WebServer running on the google cloud. Base OS : ubuntu. 24hour 365day.
	WebConfig config = WebConfig();
	config.user_agent = WSLit("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/59.0.3071.115 Safari/537.36");
	config.log_level = Awesomium::LogLevel::kLogLevel_Verbose;
	

	WebCore::Initialize(config);


	RECT rect;
	GetClientRect(gameUtil.GetHWND(), &rect);
	int centerX = rect.right  / 2;
	int centerY = rect.bottom / 2;
	
	UImenu * titleMenu = new UImenu();
	titleMenu->init(centerX, centerY, 320, 190, 320, 190, "http://35.194.254.46/ui/TitleMenu.html");
	titleMenu->setEnable(true);
	titleMenu->m_htmlTexture.setAlwaysRender(true);
	m_mapUI["titleMenu"] = titleMenu;

	UIhealthBar* healthBar = new UIhealthBar();
	healthBar->init(0+150, 0+35, 300, 70, 300, 70, "http://35.194.254.46/ui/healthBar.html");
	healthBar->setEnable(true);
	healthBar->m_htmlTexture.setAlwaysRender(true);
	m_mapUI["healthBar"] = healthBar;
	/*
	UIcrosshair* crosshair = new UIcrosshair();
	crosshair->init(centerX, centerY, 48, 48, 106,106, "http://35.194.254.46/ui/crosshair.html"); 
	crosshair->setEnable(true);
	crosshair->m_htmlTexture.setAlwaysRender(true);
	m_mapUI["crosshair"] = crosshair;
	*/
	m_vecGameUIObjects.push_back(titleMenu);
	m_vecGameUIObjects.push_back(healthBar);
	//m_vecGameUIObjects.push_back(crosshair);


}

void GameUIScene::update()
{

	for (int i = 0; i < m_vecGameUIObjects.size(); ++i)
	{
		if (m_vecGameUIObjects[i]->m_isEnabled == true)
		{
			m_vecGameUIObjects[i]->update();
		}
		
	}
}

void GameUIScene::render()
{
	TurnZBufferOff();
	
	for (int i = 0; i < m_vecGameUIObjects.size(); ++i)
	{
		if (m_vecGameUIObjects[i]->m_isEnabled == true)
		{
			m_vecGameUIObjects[i]->render();
		}
	}

	TurnZBufferOn();
}

void GameUIScene::TurnZBufferOn()
{
	gameUtil.getDeviceContext()->OMSetDepthStencilState(m_depthStencilState, 1);
}

void GameUIScene::TurnZBufferOff()
{
	gameUtil.getDeviceContext()->OMSetDepthStencilState(m_depthDisabledStencilState, 1);
}




void GameUIScene::initDepthStencilState()
{
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state.
	HRESULT result = gameUtil.getDevice()->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);

	D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;

	// Clear the second depth stencil state before setting the parameters.
	ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));

	// Now create a second depth stencil state which turns off the Z buffer for 2D rendering.  The only difference is 
	// that DepthEnable is set to false, all other parameters are the same as the other depth stencil state.
	depthDisabledStencilDesc.DepthEnable = false;
	depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthDisabledStencilDesc.StencilEnable = true;
	depthDisabledStencilDesc.StencilReadMask = 0xFF;
	depthDisabledStencilDesc.StencilWriteMask = 0xFF;
	depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the state using the device.
	result = gameUtil.getDevice()->CreateDepthStencilState(&depthDisabledStencilDesc, &m_depthDisabledStencilState);

}

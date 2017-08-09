#include "stdafx.h"
#include "GameUIScene.h"


GameUIScene::GameUIScene()
{
}


GameUIScene::~GameUIScene()
{
	for (int i = 0; i < m_vecGameUIObjects.size(); ++i)
	{
		SAFE_DELETE(m_vecGameUIObjects[i]);
	}
	WebInstance::GetInstance().release();

	SAFE_RELEASE(m_depthDisabledStencilState)
	SAFE_RELEASE(m_depthStencilState)
}

void GameUIScene::init()
{
	initDepthStencilState();

	GameUIObject* test = new GameUIObject();
	test->init(0, 0, 800, 450,1920,800, "https://www.google.co.kr/");
	m_vecGameUIObjects.push_back(test);
}

void GameUIScene::update()
{
	for (int i = 0; i < m_vecGameUIObjects.size(); ++i)
	{
		m_vecGameUIObjects[i]->update();
	}
}

void GameUIScene::render()
{
	TurnZBufferOff();
	
	for (int i = 0; i < m_vecGameUIObjects.size(); ++i)
	{
		m_vecGameUIObjects[i]->render();
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

#include "stdafx.h"
#include "gameCameraThirdPerson.h"


gameCameraThirdPerson::gameCameraThirdPerson()
{
}


gameCameraThirdPerson::~gameCameraThirdPerson()
{
}

void gameCameraThirdPerson::update()
{
	if (keyMgr.IsPressing(VK_W))
	{
		MoveLocalZ(gameTimer.getDeltaTime() * 60);
	}
	if (keyMgr.IsPressing(VK_S))
	{
		MoveLocalZ(gameTimer.getDeltaTime() * -60);
	}
	if (keyMgr.IsPressing(VK_D))
	{
		MoveLocalX(gameTimer.getDeltaTime() * 60);
	}

	if (keyMgr.IsPressing(VK_A))
	{
		MoveLocalX(gameTimer.getDeltaTime() * -60);

	}
	static int curX = 0, curY = 0;
	static int oldX = 0, oldY = 0;
	float deltaX = 0, deltaY = 0;

	float sensitivity = 0.001f;

	//POINT pt;
	//GetCursorPos(&pt);
	curX = gameUtil.m_mouseX;
	curY = gameUtil.m_mouseY;

	deltaX = (curX - oldX)* sensitivity;
	deltaY = (curY - oldY)* sensitivity;



	if (keyMgr.IsPressing(VK_LBUTTON))
	{
		RotateLocalX(deltaY);
		RotateLocalY(deltaX);
	}

	/*
	//마우스 화면 중앙에 가둬놓기
	RECT rc;
	GetClientRect(gameUtil.GetHWND(), &rc);
	pt.x = (rc.right - rc.left) / 2;
	pt.y = (rc.bottom - rc.top) / 2;
	ClientToScreen(gameUtil.GetHWND(), &pt);
	SetCursorPos(pt.x, pt.y);
	*/

	oldX = curX;
	oldY = curY;
}


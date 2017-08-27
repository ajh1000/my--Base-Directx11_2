#include "stdafx.h"
#include "TestCamera.h"


TestCamera::TestCamera()
{
}


TestCamera::~TestCamera()
{
}

void TestCamera::update()
{
	/**/
	if (keyMgr.IsPressing(VK_W))
	{
		Walk(gameTimer.getDeltaTime() * 60);
	}
	if (keyMgr.IsPressing(VK_S))
	{
		Walk(gameTimer.getDeltaTime() * -60);
	}
	if (keyMgr.IsPressing(VK_D))
	{
		Strafe(gameTimer.getDeltaTime() * 60);
	}

	if (keyMgr.IsPressing(VK_A))
	{
		Strafe(gameTimer.getDeltaTime() * -60);

	}
	/*
	static int curX = 0, curY = 0;
	static int oldX = 0, oldY = 0;
	float deltaX = 0, deltaY = 0;

	float sensitivity = 0.001f;

	curX = gameUtil.m_mouseX;
	curY = gameUtil.m_mouseY;

	deltaX = (curX - oldX)* sensitivity;
	deltaY = (curY - oldY)* sensitivity;

	if (keyMgr.IsPressing(VK_LBUTTON))
	{
		Pitch(deltaY);
		RotateY(deltaX);
	}

	oldX = curX;
	oldY = curY;
	*/
	//UpdateViewMatrix();
}

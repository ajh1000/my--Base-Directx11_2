#include "stdafx.h"
#include "KeyManager.h"

KeyManager::KeyManager()
{
	ZeroMemory(m_key, sizeof(struct_key) * 255);

	POINT pt;
	RECT rc;
	GetClientRect(gameUtil.GetHWND(), &rc);
	pt.x = (rc.right - rc.left) / 2;
	pt.y = (rc.bottom - rc.top) / 2;

	gameUtil.m_centerX = pt.x;
	gameUtil.m_centerY = pt.y;
}

KeyManager::~KeyManager()
{
}


void KeyManager::update()
{
	if (gameUtil.m_windowActive == true)
	{
		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(gameUtil.GetHWND(), &pt);
		gameUtil.m_mouseX = pt.x;
		gameUtil.m_mouseY = pt.y;

		


		for (int i = 1; i <= 254; ++i)
		{
			if (GetAsyncKeyState(i) & 0x8000)
			{

				if (m_key[i].pressing == false)
				{
					m_key[i].pressed = true;
				}
				else
				{
					m_key[i].pressed = false;
				}

				m_key[i].pressing = true;

			}
			else
			{
				if (m_key[i].pressing == true)
				{
					m_key[i].up = true;
				}
				else
				{
					m_key[i].up = false;
				}
				m_key[i].pressing = false;
				m_key[i].pressed = false;
			}
		}
	}
}

bool KeyManager::IsPressing(int vk_code)
{
	return m_key[vk_code].pressing;
}

bool KeyManager::IsUp(int vk_code)
{
	return m_key[vk_code].up;
}

bool KeyManager::IsPressed(int vk_code)
{
	return m_key[vk_code].pressed;
}

#include "stdafx.h"
#include "KeyManager.h"

KeyManager::KeyManager()
{
	ZeroMemory(m_key, sizeof(struct_key) * 255);
}

KeyManager::~KeyManager()
{
}


void KeyManager::update()
{
	if (gameUtil.m_windowActive == true)
	{
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

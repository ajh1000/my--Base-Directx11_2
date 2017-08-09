#pragma once

#include "d3dclass.h"

class CustomWindow
{
public:
	CustomWindow(int width,int height,HINSTANCE hInst);

	virtual ~CustomWindow();

	HWND& GetHandle();
	
	LRESULT CustomMsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	

	HWND getHWND();
private:
	HWND m_hWnd;
	HINSTANCE m_hInst;

};
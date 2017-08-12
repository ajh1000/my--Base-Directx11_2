#include "stdafx.h"
#include "CustomWindow.h"

#include "resource1.h"


CustomWindow * g_window;


LRESULT CALLBACK msgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	return g_window->CustomMsgProc(hWnd, message, wParam, lParam);
}

CustomWindow::CustomWindow(int width, int height, HINSTANCE hInst)
{
	m_hInst = hInst;

	WNDCLASS wc = {};
	wc.style = CS_OWNDC;
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.lpfnWndProc = msgProc;
	wc.lpszClassName = "direct11";

	RegisterClass(&wc);

	int ScreenX = GetSystemMetrics(SM_CXSCREEN);
	int ScreenY = GetSystemMetrics(SM_CYSCREEN);


	width += 16;
	height += 39;

	int centerX = (ScreenX*0.5) - (width*0.5);
	int centerY = (ScreenY*0.5) - (height*0.5);

	m_hWnd = CreateWindow("direct11", "directx11", WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_VISIBLE,
		centerX, centerY, width, height, 0, 0, 0, 0);
	gameUtil.SetHWND(m_hWnd);

	//RECT clientRect;
	//GetClientRect(m_hWnd, &clientRect);


	UpdateWindow(m_hWnd);

	g_window = this;


}



CustomWindow::~CustomWindow()
{

	g_window = 0;
}

HWND& CustomWindow::GetHandle()
{
	return m_hWnd;
}

LRESULT CustomWindow::CustomMsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	int a = 0;
	POINT pt = {};

	GetCursorPos(&pt);


	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_MOVE:
		break;
	case WM_PAINT:
		break;

	case WM_MOUSEMOVE:
		ScreenToClient(m_hWnd, &pt);
		gameUtil.m_mouseX = pt.x;
		gameUtil.m_mouseY = pt.y;
		break;
	case WM_ACTIVATE:
		if (LOWORD(wParam) == WA_ACTIVE)
			gameUtil.m_windowActive = true;
		else
			gameUtil.m_windowActive = false;
		break;
	default:
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}


HWND CustomWindow::getHWND()
{
	return m_hWnd;
}

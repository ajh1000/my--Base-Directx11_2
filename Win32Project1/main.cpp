#include "stdafx.h"
#include "CustomWindow.h"
#include "d3dclass.h"


int CALLBACK WinMain(HINSTANCE appInstance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdCount)
{
	CustomWindow window(1600,900,appInstance);
	
	D3DClass direct3d;
	direct3d.Initialize(window.GetHandle(),true);

	MSG msg = {};

	gameTimer.update();

	while (1)
	{
		keyMgr.update();

		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
			{
				break;
			}
		}
		direct3d.update();
		direct3d.render();

		gameTimer.update();
	}

	direct3d.Shutdown();

	return 0;
}
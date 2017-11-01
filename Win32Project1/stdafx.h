
// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:

#define NOMINMAX
#include <windows.h>

// C/C++ 표준 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <iostream>
#include <vector>
#include <map>
using namespace std;

#include <fstream>


/////////////
// LINKING //
/////////////
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")


/////////////////
//   DEFINES   //
/////////////////

#define singleton_class(type) \
public : \
static type& GetInstance() \
{ \
	static type inst; \
	return inst; \
}

#define SAFE_RELEASE(t) if(&t) { t->release(); t=0;}
#define SAFE_DELETE(t) if(&t) { delete t; t=0;}
#define SAFE_ARR_DELETE(t) if(&t) {delete[] t; t=0; }
#define ERROR_MSGBOX(isTrue,text) {\
	if (isTrue)\
	{\
	MessageBox(0, text, "error", 0);\
	exit(0);\
	} }

//////////////
// INCLUDES //
//////////////
#include <dxgi1_2.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <d3dx11.h>
//#include <D3DX10math.h>



#include <DirectXMath.h>
using namespace DirectX;

#include "d3dutil.h"
#include "GameTimer.h"
#include "Types.h"
#include "KeyManager.h"



/*
XMFLOAT4X4& xmIdentityMat()
{
	static XMFLOAT4X4 identity;
	XMStoreFloat4x4(&identity, XMMatrixIdentity());

	return identity;
}*/
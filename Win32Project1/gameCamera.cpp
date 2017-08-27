#include "stdafx.h"
#include <d3d9.h> 
#include <d3dx9.h>
#include "gameCamera.h"
#include "Model.h"

// ������
GameCamera::GameCamera()
{
	D3DXVECTOR3	eye(0.0f, 50.0f, -20.0f);
	D3DXVECTOR3	lookat(0.0f, 0.0f, -1.0f);
	D3DXVECTOR3	up(0.0f, 1.0f, 0.0f);
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matBill);
	SetView(&eye, &lookat, &up);
}

GameCamera::~GameCamera()
{
}


void GameCamera::pick(int x, int y)
{
	/*

	RECT rect;
	GetClientRect(gameUtil.GetHWND(), &rect);
	int screenWidth = rect.right;
	int screenHeight = rect.bottom;
	FLOAT PointX;
	FLOAT PointY;


	// �������� ��Ʈ������ ���ɴϴ�.
	D3DXMATRIX Proj = GetProjMat();

	// ���콺 ����Ʈ�� ����â�� ��ǥ�� ��ȯ�մϴ�.
	PointX = (2.0f * x / screenWidth - 1.0f) / Proj(0, 0);
	PointY = (-2.0f * y / screenHeight + 1.0f) / Proj(1, 1);

	// ����â�� ��ǥ�� ��ȯ�� ����Ʈ�� ������ ������ �����մϴ�.

	D3DXVECTOR3 origin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 dir = D3DXVECTOR3(PointX, PointY, 1.0f);

	// �� ��Ʈ������ ���ɴϴ�.
	D3DXMATRIX View = GetViewMat();

	// �� ��Ʈ������ ������� ���մϴ�.
	D3DXMATRIX ViewInverse;
	D3DXMatrixInverse(&ViewInverse, 0, &View);

	// ����Ʈ�� ���� �����̽��� ��ǥ�� ��ȯ�մϴ�.
	D3DXVec3TransformCoord(&origin, &origin, &ViewInverse);
	D3DXVec3TransformNormal(&dir, &dir, &ViewInverse);
	D3DXVec3Normalize(&dir, &dir);

	float dist;

	XMFLOAT3 xmorigin, xmdir;

	memcpy(&xmorigin, &origin, sizeof(float) * 3);
	memcpy(&xmdir, &dir, sizeof(float) * 3);


	Model* mapAABB = (Model*)gameUtil.m_vecGameObjects[2];


	bool hitMap = mapAABB->m_box.Intersects(XMLoadFloat3(&xmorigin), XMLoadFloat3(&xmdir), dist);
	if (hitMap == true)
	{
		XMFLOAT3 hitpos;
		XMVECTOR xmvecOri, xmvecDir;

		xmvecOri = XMLoadFloat3(&xmorigin);
		xmvecDir = XMLoadFloat3(&xmdir);

		xmvecOri += xmvecDir*dist;
		XMStoreFloat3(&hitpos, xmvecOri);

		pickedPos[0] = hitpos.x;
		pickedPos[1] = hitpos.y;
		pickedPos[2] = hitpos.z;

	}*/
}

void GameCamera::update()
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
	//���콺 ȭ�� �߾ӿ� ���ֳ���
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

void GameCamera::setProjMatrix(D3DXMATRIX & mat)
{
	m_projMat = mat;
}

void GameCamera::setOrthoMatrix(D3DXMATRIX & mat)
{
	m_orthoMat = mat;
}

D3DXMATRIX GameCamera::GetProjMat()
{
	return m_projMat;
}

D3DXMATRIX GameCamera::GetOrthoMat()
{
	return m_orthoMat;
}

D3DXMATRIX GameCamera::GetViewMat()
{
	return m_matView;
}

/// ī�޶� ����� �����ϱ����� �⺻ ���Ͱ����� �����Ѵ�.
D3DXMATRIXA16*	GameCamera::SetView(D3DXVECTOR3* pvEye, D3DXVECTOR3* pvLookat, D3DXVECTOR3* pvUp)
{
	m_vEye = *pvEye;
	m_vLookat = *pvLookat;
	m_vUp = *pvUp;
	D3DXVec3Normalize(&m_vView, &(m_vLookat - m_vEye));
	D3DXVec3Cross(&m_vCross, &m_vUp, &m_vView);

	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vLookat, &m_vUp);
	D3DXMatrixInverse(&m_matBill, NULL, &m_matView);
	m_matBill._41 = 0.0f;
	m_matBill._42 = 0.0f;
	m_matBill._43 = 0.0f;

	return &m_matView;
}

/// ī�޶��� �ü����� �����Ѵ�.

inline void GameCamera::SetLookat(D3DXVECTOR3 * pv) { m_vLookat = *pv; }

/// ī�޶� ��ǥ���� X������ angle��ŭ ȸ���Ѵ�.
D3DXMATRIXA16* GameCamera::RotateLocalX(float angle)
{
	D3DXMATRIXA16 matRot;
	D3DXMatrixRotationAxis(&matRot, &m_vCross, angle);

	D3DXVECTOR3 vNewDst, vNewUp;
	D3DXVec3TransformCoord(&vNewDst, &m_vView, &matRot);	// view * rot�� ���ο� dst vector�� ���Ѵ�.
	D3DXVec3Cross( &vNewUp, &vNewDst, &m_vCross );			// cross( dst, x��)���� up vector�� ���Ѵ�.
	D3DXVec3Normalize( &vNewUp, &vNewUp );					// up vector�� unit vector��...
	vNewDst += m_vEye;										// ���� dst position =  eye Position + dst vector

	return SetView(&m_vEye, &vNewDst, &m_vUp);
}

/// ī�޶� ��ǥ���� Y������ angle��ŭ ȸ���Ѵ�.
D3DXMATRIXA16* GameCamera::RotateLocalY(float angle)
{
	D3DXMATRIXA16 matRot;
	D3DXMatrixRotationAxis(&matRot, &m_vUp, angle);

	D3DXVECTOR3 vNewDst;
	D3DXVec3TransformCoord(&vNewDst, &m_vView, &matRot);	// view * rot�� ���ο� dst vector�� ���Ѵ�.
	vNewDst += m_vEye;										// ���� dst position =  eye Position + dst vector

	return SetView(&m_vEye, &vNewDst, &m_vUp);
}

/// ī�޶� ��ǥ���� X��������� dist��ŭ �����Ѵ�.(������ -dist�� ������ �ȴ�.)
D3DXMATRIXA16* GameCamera::MoveLocalX(float dist)
{
	D3DXVECTOR3 vNewEye = m_vEye;
	D3DXVECTOR3 vNewDst = m_vLookat;

	D3DXVECTOR3 vMove;
	D3DXVec3Normalize(&vMove, &m_vCross);
	vMove *= dist;
	vNewEye += vMove;
	vNewDst += vMove;

	return SetView(&vNewEye, &vNewDst, &m_vUp);
}

/// ī�޶� ��ǥ���� Y��������� dist��ŭ �����Ѵ�.(������ -dist�� ������ �ȴ�.)
D3DXMATRIXA16* GameCamera::MoveLocalY(float dist)
{
	D3DXVECTOR3 vNewEye = m_vEye;
	D3DXVECTOR3 vNewDst = m_vLookat;

	D3DXVECTOR3 vMove;
	D3DXVec3Normalize(&vMove, &m_vUp);
	vMove *= dist;
	vNewEye += vMove;
	vNewDst += vMove;

	return SetView(&vNewEye, &vNewDst, &m_vUp);
}

/// ī�޶� ��ǥ���� Z��������� dist��ŭ �����Ѵ�.(������ -dist�� ������ �ȴ�.)
D3DXMATRIXA16* GameCamera::MoveLocalZ(float dist)
{
	D3DXVECTOR3 vNewEye = m_vEye;
	D3DXVECTOR3 vNewDst = m_vLookat;

	D3DXVECTOR3 vMove;
	D3DXVec3Normalize(&vMove, &m_vView);
	vMove *= dist;
	vNewEye += vMove;
	vNewDst += vMove;

	return SetView(&vNewEye, &vNewDst, &m_vUp);
}

/// ������ǥ���� *pv���� ��ġ�� ī�޶� �̵��Ѵ�.
D3DXMATRIXA16* GameCamera::MoveTo(D3DXVECTOR3* pv)
{
	D3DXVECTOR3	dv = *pv - m_vEye;
	m_vEye = *pv;
	m_vLookat += dv;
	return SetView(&m_vEye, &m_vLookat, &m_vUp);
}

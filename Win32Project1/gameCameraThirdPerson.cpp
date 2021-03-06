#include "stdafx.h"
#include "gameCameraThirdPerson.h"
#include <algorithm>


gameCameraThirdPerson::gameCameraThirdPerson()
{

	m_distance = 15.f;

	m_sensitivity = 0.1f;

	m_vEye = D3DXVECTOR3(200, 200, -200);
	m_vLookat = D3DXVECTOR3(0, 0, 0);
	m_vRight = D3DXVECTOR3(1, 0, 0);

	D3DXMatrixIdentity(&m_matView);
	m_desiredLookat = D3DXVECTOR3(0, 0, 0);
	m_desiredEyePos = D3DXVECTOR3(0, 0, 0);
	m_camMode = CAM_MODE::FPS_MODE;
}


gameCameraThirdPerson::~gameCameraThirdPerson()
{
}

void gameCameraThirdPerson::update()
{
	//마우스 입력
	int curX = 0, curY = 0;
	curX = gameUtil.m_mouseX;
	curY = gameUtil.m_mouseY;

	m_currentYaw += (curX - gameUtil.m_centerX)* m_sensitivity;
	m_currentPitch += (curY - gameUtil.m_centerY)* m_sensitivity;

	if (m_currentPitch > 89)
	{
		m_currentPitch = 89;
	}
	else if (m_currentPitch < -15)
	{
		m_currentPitch = -15;
	}


}

void gameCameraThirdPerson::lateUpdate()
{
	if (m_camMode == FPS_MODE)
	{
		m_desiredEyePos = m_vEye;

		if (keyMgr.IsPressing(VK_W))
		{
			m_desiredEyePos += m_vForward*gameTimer.getDeltaTime() * 600;
			
		}
		if (keyMgr.IsPressing(VK_S))
		{
			m_desiredEyePos += m_vForward*gameTimer.getDeltaTime() * -600;

		}
		if (keyMgr.IsPressing(VK_D))
		{
			m_desiredEyePos += m_vRight*gameTimer.getDeltaTime() * 600;

		}

		if (keyMgr.IsPressing(VK_A))
		{
			m_desiredEyePos += m_vRight*gameTimer.getDeltaTime() * -600;


		}

		D3DXMATRIXA16 matRot;
		D3DXMatrixRotationYawPitchRoll(&matRot, D3DXToRadian( m_currentYaw), D3DXToRadian( m_currentPitch), 0);

		D3DXVECTOR3 dir=D3DXVECTOR3(0,0,1);
		
		D3DXVec3TransformNormal(&dir, &dir, &matRot);

		m_desiredLookat = m_desiredEyePos + dir*10;
		
		
	}
	else if (m_camMode == TPS_MODE)
	{


		D3DXVECTOR3 vDistance = D3DXVECTOR3(0, 0, -m_distance);
		D3DXQUATERNION quat;
		D3DXQuaternionIdentity(&quat);
		D3DXQuaternionRotationYawPitchRoll(&quat, D3DXToRadian(m_currentYaw), D3DXToRadian(m_currentPitch), 0);

		D3DXMATRIX matRotation;
		D3DXMatrixIdentity(&matRotation);
		D3DXMatrixRotationQuaternion(&matRotation, &quat);

		D3DXVec3TransformNormal(&vDistance, &vDistance, &matRotation);

		if (keyMgr.IsPressing(VK_RBUTTON))
		{
			m_desiredLookat = player->transform.getPos();
			m_desiredLookat.y += 4;
			m_desiredLookat += m_vForward * 10;

			m_desiredEyePos = m_desiredLookat + vDistance;
		}
		else
		{

			m_desiredLookat = player->transform.getPos();
			m_desiredLookat.y += 3;
			m_desiredEyePos = m_desiredLookat + vDistance;
		}
	}
		D3DXVec3Lerp(&m_vEye, &m_vEye, &m_desiredEyePos, gameTimer.getDeltaTime() * 10);
		D3DXVec3Lerp(&m_vLookat, &m_vLookat, &m_desiredLookat, gameTimer.getDeltaTime() * 10);

		D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vLookat, &D3DXVECTOR3(0, 1, 0));

		m_vRight = D3DXVECTOR3(m_matView(0, 0), m_matView(1, 0), m_matView(2, 0));
		m_vUp = D3DXVECTOR3(m_matView(0, 1), m_matView(1, 1), m_matView(2, 1));
		m_vForward = D3DXVECTOR3(m_matView(0, 2), m_matView(1, 2), m_matView(2, 2));
	
}

void gameCameraThirdPerson::setTargetPos(gameObject* object)
{
	player = (gamePlayer*)object;

}

D3DXMATRIX gameCameraThirdPerson::GetViewMat()
{
	return m_matView;
}
/*
void gameCameraThirdPerson::updateViewMatrix()
{
	XMVECTOR L = XMVector3Normalize(XMVectorSubtract(target, pos));
	XMVECTOR R = XMVector3Normalize(XMVector3Cross(worldUp, L));
	XMVECTOR U = XMVector3Cross(L, R);

	D3DXVECTOR3 L;
	D3DXVECTOR3 R;
	D3DXVECTOR3 U;

	D3DXVec3Normalize(&L, &(m_vLookat - m_vEye));
	D3DXVec3Normalize(&R, D3DXVec3Cross(&R, &m_vUp, &L));
	D3DXVec3Cross(&U, &L, &R);

	//m_vLookat = L;
	m_vRight = R;
	m_vUp = U;

	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vLookat, &D3DXVECTOR3(0, 1, 0));
}
*/
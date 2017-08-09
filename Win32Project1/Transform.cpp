#include "stdafx.h"
#include "Transform.h"


MyTransform::MyTransform()
{
	m_worldPos = D3DXVECTOR3(0, 0, 0);
	m_worldRot = D3DXVECTOR3(0, 0, 0);
	m_worldScale = D3DXVECTOR3(1, 1, 1);

	m_localPos= D3DXVECTOR3(0, 0, 0);
	m_localRot= D3DXVECTOR3(0, 0, 0);
	m_localScale = D3DXVECTOR3(1, 1, 1);

	D3DXMatrixIdentity(&m_matWorldPos);
	D3DXMatrixIdentity(&m_matWorldRot);
	D3DXMatrixIdentity(&m_matWorldScale);


	D3DXMatrixIdentity(&m_wvp.world);
	D3DXMatrixIdentity(&m_wvp.view);
	D3DXMatrixIdentity(&m_wvp.proj);
	D3DXMatrixIdentity(&m_matLocalTransform);

	D3DXQuaternionIdentity(&m_quaternion);
}


MyTransform::~MyTransform()
{
}

matrix_WorldViewProj MyTransform::getWVP()
{
	return m_wvp;
}

void MyTransform::buildMatrixWVP()
{
	D3DXMatrixTranslation(&m_matWorldPos, m_worldPos.x, m_worldPos.y, m_worldPos.z);

	D3DXMatrixRotationQuaternion(&m_matWorldRot, &m_quaternion);

	//D3DXMatrixRotationYawPitchRoll(&m_matWorldRot, m_worldRot.y * (D3DX_PI / 180.f) ,
	//										m_worldRot.x * (D3DX_PI / 180.f),
	//										m_worldRot.z * (D3DX_PI / 180.f));
	
	D3DXMatrixScaling(&m_matWorldScale, m_worldScale.x, m_worldScale.y, m_worldScale.z);


	if (m_IsRotMat == true)
	{
		m_wvp.world = m_matLocalTransform *(m_matWorldScale*rotMat*m_matWorldPos);
	}
	else
	{
		m_wvp.world = m_matLocalTransform *(m_matWorldScale*m_matWorldRot*m_matWorldPos);
	}
	if (m_isScreenSpace)
	{
		D3DXMATRIX defaultViewMat;
		D3DXVECTOR3 eye=D3DXVECTOR3(0,0,-1);
		D3DXVECTOR3 lookat(0,0,0);
		D3DXVECTOR3 up(0,1,0);

		D3DXMatrixLookAtLH(&defaultViewMat, &eye, &lookat, &up);
		m_wvp.view = defaultViewMat;
		//m_wvp.view = identityMat();
		m_wvp.proj = gameUtil.GetMainCamera()->GetOrthoMat();
	}
	else
	{
		m_wvp.view = gameUtil.GetMainCamera()->GetViewMat();
		m_wvp.proj = gameUtil.GetMainCamera()->GetProjMat();
	}
	
}

void MyTransform::buildMatrixWorld()
{
	D3DXMatrixTranslation(&m_matWorldPos, m_worldPos.x, m_worldPos.y, m_worldPos.z);

	D3DXMatrixRotationQuaternion(&m_matWorldRot, &m_quaternion);
	/*
	D3DXMatrixRotationYawPitchRoll(&m_matWorldRot, m_worldRot.y * (D3DX_PI / 180.f),
		m_worldRot.x * (D3DX_PI / 180.f),
		m_worldRot.z * (D3DX_PI / 180.f));
		*/
	D3DXMatrixScaling(&m_matWorldScale, m_worldScale.x, m_worldScale.y, m_worldScale.z);

	m_wvp.world = m_matLocalTransform*(m_matWorldScale*m_matWorldRot*m_matWorldPos);
}

D3DXMATRIX MyTransform::getMatrixWorld()
{

	return m_wvp.world;
}


void MyTransform::setPos(D3DXVECTOR3 pos)
{
	m_worldPos = pos;
}

void MyTransform::setPos(float x, float y, float z)
{
	m_worldPos.x = x;
	m_worldPos.y = y;
	m_worldPos.z = z;
}

void MyTransform::translate(float x, float y, float z)
{
	m_worldPos.x += x;
	m_worldPos.y += y;
	m_worldPos.z += z;
}


void MyTransform::rotate(float x, float y, float z)
{

	m_worldRot.x += x ;
	m_worldRot.y += y ;
	m_worldRot.z += z ; 

	if (m_worldRot.x >= 360.0f)
	{
		m_worldRot.x -= 360.0f;
	}
	if (m_worldRot.y >= 360.0f)
	{
		m_worldRot.y -= 360.0f;
	}
	if (m_worldRot.z >= 360.0f)
	{
		m_worldRot.z -= 360.0f;
	}

	D3DXQuaternionRotationYawPitchRoll(&m_quaternion,
		m_worldRot.y* (D3DX_PI / 180.f),
		m_worldRot.x* (D3DX_PI / 180.f),
		m_worldRot.z* (D3DX_PI / 180.f));
}

void MyTransform::setRot(float x, float y, float z)
{

	m_worldRot.x = x;
	m_worldRot.y = y;
	m_worldRot.z = z;

	D3DXQuaternionRotationYawPitchRoll(&m_quaternion,
		m_worldRot.y * (D3DX_PI/180.f),
		m_worldRot.x* (D3DX_PI / 180.f),
		m_worldRot.z* (D3DX_PI / 180.f));
}

void MyTransform::setRot(D3DXQUATERNION quat)
{
	m_quaternion = quat;
		
}

void MyTransform::setScale(float x, float y, float z)
{
	m_worldScale.x = x;
	m_worldScale.y = y;
	m_worldScale.z = z;
}

D3DXVECTOR3 MyTransform::getPos()
{
	return m_worldPos;
}

#pragma once

#include "Types.h"

class MyTransform
{
public:
	MyTransform();
	virtual ~MyTransform();
public:
	void setPos(D3DXVECTOR3 pos);
	void setPos(float x, float y, float z);
	void setRot(float x, float y, float z); //input as degree. not radian.
	void setRot(D3DXQUATERNION quat);
	void setScale(float x, float y, float z);
	
	D3DXVECTOR3 getPos();

	void translate(float x, float y, float z);
	void rotate(float x, float y, float z); //input as degree. not radian.

	matrix_WorldViewProj getWVP();
	void buildMatrixWVP();
	void buildMatrixWorld();
	D3DXMATRIX getMatrixWorld();
public:
	bool m_IsRotMat = false;
	D3DXMATRIX rotMat;
	bool m_isScreenSpace = false;

private:
	D3DXVECTOR3 m_worldPos;
	D3DXVECTOR3 m_worldRot;
	D3DXVECTOR3 m_worldScale;

	D3DXMATRIX m_matWorldPos;
	D3DXMATRIX m_matWorldRot;
	D3DXMATRIX m_matWorldScale;

	D3DXVECTOR3 m_localPos;
	D3DXVECTOR3 m_localRot;
	D3DXVECTOR3 m_localScale;

	D3DXMATRIX m_matLocalPos;
	D3DXMATRIX m_matLocalRot;
	D3DXMATRIX m_matLocalScale;

	D3DXQUATERNION m_quaternion;

	D3DXMATRIX m_matLocalTransform;
	matrix_WorldViewProj m_wvp;

};


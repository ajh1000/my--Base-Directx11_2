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
	void setRot(float x, float y, float z); //input as euler degree. not radian.
	void setRot(D3DXQUATERNION quat);
	void setScale(float x, float y, float z);

	D3DXVECTOR3 getPos();

	void translate(float x, float y, float z);
	void rotate(float x, float y, float z); //input as euler degree. not radian.

	matrix_WorldViewProj getWVP();
	void buildMatrixWVP();
	void buildMatrixWorld();
	D3DXMATRIX getMatrixWorld();
public:
	bool m_isScreenSpace = false;

private:
	D3DXVECTOR3 m_worldPos;
	D3DXVECTOR3 m_worldRot;
	D3DXVECTOR3 m_worldScale;

	D3DXMATRIX m_matWorldPos;
	D3DXMATRIX m_matWorldRot;
	D3DXMATRIX m_matWorldScale;


	D3DXQUATERNION m_quaternion;

	matrix_WorldViewProj m_wvp;

};

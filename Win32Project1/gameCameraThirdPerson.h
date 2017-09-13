#pragma once
#include "gamePlayer.h"
class gameCameraThirdPerson : public GameCamera
{
public:
	gameCameraThirdPerson();
	virtual ~gameCameraThirdPerson();

	virtual void update();
	virtual void lateUpdate();
	void setTargetPos(gameObject* object);
	
	virtual D3DXMATRIX GetViewMat();
private:
	float m_currentPitch = 0, m_currentYaw = 0;
	float m_distance = 3.0f;
	float m_sensitivity = 0.001f;

	gamePlayer* player = nullptr;

public:
	D3DXVECTOR3		m_vEye;
	D3DXVECTOR3		m_vLookat;
	D3DXVECTOR3		m_vRight;
	D3DXVECTOR3		m_vUp;
	D3DXVECTOR3     m_vForward;

	D3DXMATRIX	m_matView;

	D3DXVECTOR3 m_desiredLookat;
	D3DXVECTOR3 m_desiredEyePos;
	
};


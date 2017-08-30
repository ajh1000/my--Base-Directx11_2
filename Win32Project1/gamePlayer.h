#pragma once
#include "Model.h"
#include <btBulletDynamicsCommon.h>

class gamePlayer :
	public Model
{
public:
	gamePlayer();
	virtual ~gamePlayer();

	virtual void init(char* dir,char* FileName);
	virtual void update();
	virtual void lateUpdate();
	virtual void render();

	void processingInput();
public:
	btRigidBody* m_rigidbody = nullptr;

private:
	D3DXQUATERNION m_desiredQuat;
	D3DXQUATERNION m_OriginalQuat;
	float m_slerpFactor;
};


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
	virtual void render();

	void updatePhysicsTransform();

public:
	btRigidBody* m_rigidbody = nullptr;

};


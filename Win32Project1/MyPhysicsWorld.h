#pragma once

#include <btBulletDynamicsCommon.h>

class MyPhysicsWorld
{
public:
	MyPhysicsWorld();
	virtual ~MyPhysicsWorld();

	void init();
	void update();

public:
	//must use after MyPhysicsWorld::init();
	btDiscreteDynamicsWorld* m_dynamicsWorld = nullptr;

private:
	btBroadphaseInterface* m_broadphase=nullptr;
	btCollisionDispatcher* m_dispatcher = nullptr;
	btDefaultCollisionConfiguration* m_collisionConfiguration = nullptr;
	btSequentialImpulseConstraintSolver* m_solver = nullptr;
	
	btTriangleMesh* m_TriangleMesh_map = 0;
	btCollisionShape* m_collisionShape_map = nullptr;
	btRigidBody* m_rigidbody_map = nullptr;

	btCollisionShape* m_collisionshape_player = 0;
	btRigidBody* m_rigidbody_player = 0;
};


#include "stdafx.h"
#include "gameBullet.h"


gameBullet::gameBullet()
{
	m_elapsedTime = 0;
}


gameBullet::~gameBullet()
{
}

void gameBullet::init()
{
		Model::init("./DATA/barrel/", "barrel.obj");
	transform.setScale(0.2, 0.2, 0.2);
	
	btCollisionShape* collisionShape = new btSphereShape(0.7);

	btDefaultMotionState* motionState =
		new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(1000, 1000, 1000)));

	btVector3 fallInertia(0, 0, 0);
	collisionShape->calculateLocalInertia(2, fallInertia);

	btRigidBody::btRigidBodyConstructionInfo
		rigidBodyCI(1, motionState, collisionShape, fallInertia);

	btRigidBody* rigidbody = new btRigidBody(rigidBodyCI);
	gameUtil.m_physicsWorld->m_mapRigidbody[m_tag] = rigidbody;
	
	m_rigidbody = gameUtil.m_physicsWorld->m_mapRigidbody[m_tag];
	m_strType = "bullet";
	m_rigidbody->setUserPointer(&m_strType);
}

void gameBullet::setEnable(D3DXVECTOR3 pos, D3DXVECTOR3 dir)
{
	if(m_isEnabled==false)
		gameUtil.m_physicsWorld->m_dynamicsWorld->addRigidBody(m_rigidbody);

	m_elapsedTime = 0;
	m_isEnabled = true;

	m_rigidbody->activate(true);

	//clear all velocity. 
	m_rigidbody->setAngularVelocity(btVector3(0, 0, 0));
	m_rigidbody->setLinearVelocity(btVector3(0, 0, 0));

	m_rigidbody->getWorldTransform().setIdentity();
	m_rigidbody->getWorldTransform().setOrigin(btVector3(pos.x, pos.y, pos.z));

	//발사
	m_power = 50.f;
	btVector3 btDir = btVector3(dir.x, dir.y , dir.z);
	m_rigidbody->applyCentralImpulse(btDir*m_power);

	transform.setPos(pos);
}

void gameBullet::update()
{
	
	if (m_elapsedTime >= 5.0f)
	{
		m_isEnabled = false;
		m_elapsedTime = 0;
		m_rigidbody->getWorldTransform().setOrigin(btVector3(1000, 1000, 1000));
		gameUtil.m_physicsWorld->m_dynamicsWorld->removeRigidBody(m_rigidbody);
	}

	m_elapsedTime += gameTimer.getDeltaTime();

	//애니메이션이 있는 객체가 아니므로 상위 클래스의 update함수를 호출하지않는다.
	//Model::update();

	m_rigidbody->activate(true);

	btTransform bttransform;
	m_rigidbody->getMotionState()->getWorldTransform(bttransform);


	//최종값을 player 트랜스폼에 입력.
	transform.setPos(bttransform.getOrigin().getX(),
		bttransform.getOrigin().getY(),
		bttransform.getOrigin().getZ());

	btQuaternion quat = bttransform.getRotation();
	D3DXQUATERNION d3dxquat = D3DXQUATERNION(quat.x(), quat.y(), quat.z(), quat.w());
	transform.setRot(d3dxquat);
}

void gameBullet::render()
{
	Model::render();
}

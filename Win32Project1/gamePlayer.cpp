#include "stdafx.h"
#include "gamePlayer.h"


gamePlayer::gamePlayer()
{
}


gamePlayer::~gamePlayer()
{
}

void gamePlayer::init(char* dir, char* FileName)
{
	Model::init(dir, FileName);
	gameUtil.m_physicsWorld->CreateCapsuleRigidbody(transform.getPos(), 0.5, 0.8, "player");
	
}

void gamePlayer::update()
{


	updatePhysicsTransform();
}

void gamePlayer::render()
{
	Model::render();
}

void gamePlayer::updatePhysicsTransform()
{
	btTransform bttransform;
	btRigidBody* rigidbody = gameUtil.m_physicsWorld->m_mapRigidbody["player"];

	rigidbody->getMotionState()->getWorldTransform(bttransform);
	btVector3 velocity = rigidbody->getAngularVelocity();
	velocity.setX(0);
	velocity.setZ(0);
	rigidbody->setAngularVelocity(velocity);

	transform.setPos(bttransform.getOrigin().getX(),
		bttransform.getOrigin().getY(),
		bttransform.getOrigin().getZ());

	btQuaternion quat = bttransform.getRotation();
	D3DXQUATERNION d3dxquat = D3DXQUATERNION(quat.x(), quat.y(), quat.z(), quat.w());

}

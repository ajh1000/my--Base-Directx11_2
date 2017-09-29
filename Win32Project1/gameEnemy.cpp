#include "stdafx.h"
#include "gameEnemy.h"
#include "BulletManager.h"
#include "gameCameraThirdPerson.h"

gameEnemy::gameEnemy()
{
}


gameEnemy::~gameEnemy()
{
}

void gameEnemy::init(char * dir, char * FileName)
{
	Model::init(dir, FileName);

	transform.setPos(0, 0, 0);
	transform.setScale(0.02, 0.02, 0.02);
	transform.setRot(0, 0, 0);
	setAnimation("Spider_Armature|run_ani_vor");
	//m_AnimName = "Spider_Armature|Attack";"Spider_Armature|normal"

	btCollisionShape* collisionShape = new btSphereShape(2);

	btDefaultMotionState* motionState =
		new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(1000, 1000, 1000)));

	btVector3 fallInertia(0, 0, 0);
	collisionShape->calculateLocalInertia(2, fallInertia);

	btRigidBody::btRigidBodyConstructionInfo
		rigidBodyCI(1, motionState, collisionShape, fallInertia);

	btRigidBody* rigidbody = new btRigidBody(rigidBodyCI);
	gameUtil.m_physicsWorld->m_mapRigidbody[m_tag] = rigidbody;

	m_rigidbody = gameUtil.m_physicsWorld->m_mapRigidbody[m_tag];
	m_strType = "enemy";
	m_rigidbody->setUserPointer(&m_strType);
	//m_rigidbody->setUserPointer(m_rigidbody);
}

void gameEnemy::setEnable(D3DXVECTOR3 pos)
{
	if (m_isEnabled == false)
		gameUtil.m_physicsWorld->m_dynamicsWorld->addRigidBody(m_rigidbody);
	m_elapsedTime = 0;
	m_isEnabled = true;

	m_rigidbody->activate(true);

	//clear all velocity. 
	m_rigidbody->setAngularVelocity(btVector3(0, 0, 0));
	m_rigidbody->setLinearVelocity(btVector3(0, 0, 0));

	m_rigidbody->getWorldTransform().setIdentity();
	m_rigidbody->getWorldTransform().setOrigin(btVector3(pos.x, pos.y, pos.z));

	transform.setPos(pos);

	m_fireElapsedTime = 0;


}

void gameEnemy::setPos(D3DXVECTOR3 pos)
{
	if (m_isEnabled == true && m_isDead == false)
	{
		transform.setPos(pos);
		//clear all velocity. 
		m_rigidbody->setAngularVelocity(btVector3(0, 0, 0));
		m_rigidbody->setLinearVelocity(btVector3(0, 0, 0));

		m_rigidbody->getWorldTransform().setIdentity();
		m_rigidbody->getWorldTransform().setOrigin(btVector3(pos.x, pos.y, pos.z));


	}

}


void gameEnemy::update()
{
	Model::update();
	gameCameraThirdPerson* camera = (gameCameraThirdPerson*)gameUtil.GetMainCamera();


	m_rigidbody->activate(true);

	if (m_isDead == true)
	{
		if (m_elapsedTime > 5.0f)
		{
			m_isEnabled = false;
			m_elapsedTime = 0;
			m_rigidbody->getWorldTransform().setOrigin(btVector3(1000, 1000, 1000));
			gameUtil.m_physicsWorld->m_dynamicsWorld->removeRigidBody(m_rigidbody);
			char cidx = m_tag.back();
			int idx = cidx - '0';

			gameUtil.myPath->crowd->removeAgent(idx);
		}
		m_elapsedTime += gameTimer.getDeltaTime();
	}


	if (m_isDead)
	{

		//m_rigidbody->setLinearFactor(btVector3(1, 1, 1));
		//m_rigidbody->setAngularFactor(btVector3(1, 1, 1));

		btTransform bttransform;
		m_rigidbody->getMotionState()->getWorldTransform(bttransform);

		//최종값을 player 트랜스폼에 입력.
		transform.setPos(bttransform.getOrigin().getX(),
			bttransform.getOrigin().getY(),
			bttransform.getOrigin().getZ());

		btQuaternion quat = bttransform.getRotation();
		D3DXQUATERNION d3dxquat = D3DXQUATERNION(quat.x(), quat.y(), quat.z(), quat.w());
		transform.setRot(d3dxquat);
		return;
	}

	if (camera->m_camMode == camera->TPS_MODE)
	{



		D3DXVECTOR3 pos;
		pos = D3DXVECTOR3(gameUtil.myPath->crowd->getAgent(m_tag.back() - '0')->npos[0],
			gameUtil.myPath->crowd->getAgent(m_tag.back() - '0')->npos[1],
			gameUtil.myPath->crowd->getAgent(m_tag.back() - '0')->npos[2]);


		setPos(pos);

		D3DXVECTOR3 playerPos = gameUtil.m_mapTag["player"]->transform.getPos();
		playerPos.y += 1;
		D3DXVECTOR3 dir =  playerPos- pos;
		float length = D3DXVec3Length(&dir);
		D3DXVec3Normalize(&dir, &dir);
		


		D3DXVECTOR3 rotdir = pos - playerPos; 
		D3DXVec3Normalize(&rotdir, &rotdir);
		float yaw = D3DXToDegree(atan2(rotdir.x, rotdir.z));
		transform.setRot(0, yaw, 0);
		

		if (length <= 30)
		{
			if (m_fireElapsedTime >= 2)
			{
				setAnimation("Spider_Armature|Attack");
				D3DXVECTOR3 firePoint = transform.getPos();
				BulletManager::GetInstance().instantiate(firePoint + dir * 3, dir);
				m_fireElapsedTime = 0;
			}
			m_fireElapsedTime += gameTimer.getDeltaTime();
			if (m_fireElapsedTime >= 1 && m_fireElapsedTime < 2)
			{
				setAnimation("Spider_Armature|run_ani_vor");
			}
		}






		btDynamicsWorld* dynamicsWorld = gameUtil.m_physicsWorld->m_dynamicsWorld;
		int numManifolds = dynamicsWorld->getDispatcher()->getNumManifolds();
		for (int i = 0; i < numManifolds; i++)
		{
			btPersistentManifold* contactManifold = dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
			const btCollisionObject* obA = contactManifold->getBody0();
			const btCollisionObject* obB = contactManifold->getBody1();

			int numContacts = contactManifold->getNumContacts();
			for (int j = 0; j < numContacts; j++)
			{
				btManifoldPoint& pt = contactManifold->getContactPoint(j);
				if (pt.getDistance() < 0.f)
				{
					if (obA->getUserPointer() != nullptr && obB->getUserPointer() != nullptr)
					{
						const btManifoldPoint& pt = contactManifold->getContactPoint(j);
						const btVector3& normalOnB = pt.m_normalWorldOnB;
						float power = 10;


						if (!((string*)obA->getUserPointer())->compare("bullet"))
						{
							if (!((string*)obB->getUserPointer())->compare("enemy"))
							{
								if ((btRigidBody*)obB == m_rigidbody)
								{

									m_rigidbody->applyCentralImpulse(normalOnB*power);
									m_isDead = true;
									setAnimation("Spider_Armature|normal");
								}
							}
						}
						else if (!((string*)obB->getUserPointer())->compare("bullet"))
						{
							if (!((string*)obA->getUserPointer())->compare("enemy"))
							{
								if ((btRigidBody*)obA == m_rigidbody)
								{
									m_rigidbody->applyCentralImpulse(normalOnB*power);
									m_isDead = true;
									setAnimation("Spider_Armature|normal");
								}
							}
						}
					}
				}
			}

		}

	}
}

void gameEnemy::lateUpdate()
{

}

void gameEnemy::render()
{
	Model::render();
}

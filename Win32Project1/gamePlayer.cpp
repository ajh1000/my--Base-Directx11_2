#include "stdafx.h"
#include "gamePlayer.h"
#include "gameCameraThirdPerson.h"
#include "BulletManager.h"
#include "UIhealthBar.h"

gamePlayer::gamePlayer()
{
}


gamePlayer::~gamePlayer()
{
}

void gamePlayer::init(char* dir, char* FileName)
{
	Model::init(dir, FileName);

	setAnimation("Idle_Aim");

	gameUtil.m_physicsWorld->CreateCapsuleRigidbody(transform.getPos(), 0.5, 0.8, 2, "player");
	m_rigidbody = gameUtil.m_physicsWorld->m_mapRigidbody["player"];

	m_strType = "player";
	m_rigidbody->setUserPointer(&m_strType);

	D3DXQuaternionIdentity(&m_desiredQuat);
	D3DXQuaternionIdentity(&m_OriginalQuat);
	m_slerpFactor = 0.0f;

}

void gamePlayer::update()
{
	//update animation frame
	Model::update();

}

void gamePlayer::lateUpdate()
{
	gameCameraThirdPerson* mainCamera = (gameCameraThirdPerson*)gameUtil.GetMainCamera();

	m_rigidbody->activate(true);

	if (mainCamera->m_camMode == mainCamera->TPS_MODE)
	{



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

		if (m_slerpFactor >= 1.0f)
			m_slerpFactor = 1.0f;

		transform.setRot(m_OriginalQuat);

		m_rigidbody->setLinearFactor(btVector3(1, 1, 1));
		m_rigidbody->setAngularFactor(btVector3(0, 1, 0));

		D3DXQuaternionSlerp(&m_OriginalQuat, &m_OriginalQuat, &m_desiredQuat, gameTimer.getDeltaTime()*10.f);

		processingInput();

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
							if (!((string*)obB->getUserPointer())->compare("player"))
							{
								m_rigidbody->applyCentralImpulse(normalOnB*power);
								((UIhealthBar*)gameUtil.m_gameUIScene->m_mapUI["healthBar"])->TakeDamage(0.05);

							}
						}
						else if (!((string*)obB->getUserPointer())->compare("bullet"))
						{
							if (!((string*)obA->getUserPointer())->compare("player"))
							{
								m_rigidbody->applyCentralImpulse(normalOnB*power);
								((UIhealthBar*)gameUtil.m_gameUIScene->m_mapUI["healthBar"])->TakeDamage(0.05);

							}
						}
					}
				}
			}

		}




		//회전 y축 고정.
		btTransform bttransform;

		m_rigidbody->getMotionState()->getWorldTransform(bttransform);
		btVector3 velocity = m_rigidbody->getAngularVelocity();
		velocity.setX(0);
		velocity.setZ(0);
		m_rigidbody->setAngularVelocity(velocity);

		//최종값을 player 트랜스폼에 입력.
		transform.setPos(bttransform.getOrigin().getX(),
			bttransform.getOrigin().getY() - 1,
			bttransform.getOrigin().getZ());

		btQuaternion quat = bttransform.getRotation();
		D3DXQUATERNION d3dxquat = D3DXQUATERNION(quat.x(), quat.y(), quat.z(), quat.w());
		//transform.setRot(d3dxquat);

	}
}


void gamePlayer::processingInput()
{
	D3DXVECTOR3 forward = ((gameCameraThirdPerson*)gameUtil.GetMainCamera())->m_vForward;
	D3DXVECTOR3 right = ((gameCameraThirdPerson*)gameUtil.GetMainCamera())->m_vRight;
	D3DXVECTOR3 diagonal = forward;

	float speed = 1500.f*gameTimer.getDeltaTime();

	float yaw = D3DXToDegree(atan2(forward.x, forward.z));
	D3DXQuaternionRotationYawPitchRoll(&m_desiredQuat, D3DXToRadian(yaw), 0, 0);

	/*
		전투
	*/
	if (keyMgr.IsPressing(VK_RBUTTON))
	{
		gameUtil.m_gameUIScene->m_mapUI["crosshair"]->setEnable(true);
		if (keyMgr.IsPressed(VK_LBUTTON))
		{
			D3DXVECTOR3 firePoint = transform.getPos();
			D3DXVECTOR3 fireDir = forward;
			firePoint.y += 3.5;
			firePoint += fireDir * 3;


			BulletManager::GetInstance().instantiate(firePoint, fireDir);
		}
	}
	if (keyMgr.IsUp(VK_RBUTTON))
	{
		gameUtil.m_gameUIScene->m_mapUI["crosshair"]->setEnable(false);
	}



	/*
		전방 대각선
	*/
	if (keyMgr.IsPressing(VK_W))
	{
		if (keyMgr.IsPressing(VK_D))
		{

			D3DXQUATERNION quat;
			D3DXQuaternionRotationYawPitchRoll(&quat, D3DXToRadian(45.f), 0, 0);

			D3DXMATRIX rotMat;
			D3DXMatrixRotationQuaternion(&rotMat, &quat);

			D3DXVec3TransformNormal(&diagonal, &diagonal, &rotMat);

			//never mess up with Y-velocity.
			btVector3 currentVel = m_rigidbody->getLinearVelocity();
			//m_rigidbody->setLinearVelocity(btVector3(diagonal.x*speed, currentVel.getY(), diagonal.z*speed));
			m_rigidbody->applyCentralForce(btVector3(diagonal.x*speed, currentVel.getY(), diagonal.z*speed));

			//rotation

			//Given unit (normalized) direction vector d
			//pitch = asin(-d.Y);
			float yaw = D3DXToDegree(atan2(diagonal.x, diagonal.z));

			D3DXQuaternionRotationYawPitchRoll(&m_desiredQuat, D3DXToRadian(yaw), 0, 0);

			//transform.setRot(0, yaw, 0);

			//anim name
			setAnimation("Aim_Running");

			return;
		}
	}

	if (keyMgr.IsPressing(VK_W))
	{
		if (keyMgr.IsPressing(VK_A))
		{
			D3DXQUATERNION quat;
			D3DXQuaternionRotationYawPitchRoll(&quat, D3DXToRadian(-45.f), 0, 0);

			D3DXMATRIX rotMat;
			D3DXMatrixRotationQuaternion(&rotMat, &quat);

			D3DXVec3TransformNormal(&diagonal, &diagonal, &rotMat);

			//never mess up with Y-velocity.
			btVector3 currentVel = m_rigidbody->getLinearVelocity();
			//m_rigidbody->setLinearVelocity(btVector3(diagonal.x*speed, currentVel.getY(), diagonal.z*speed));
			m_rigidbody->applyCentralForce(btVector3(diagonal.x*speed, currentVel.getY(), diagonal.z*speed));
			//rotation
			float yaw = D3DXToDegree(atan2(diagonal.x, diagonal.z));
			D3DXQuaternionRotationYawPitchRoll(&m_desiredQuat, D3DXToRadian(yaw), 0, 0);

			//transform.setRot(0, yaw, 0);

			//anim name
			setAnimation("Aim_Running");

			return;
		}
	}

	/*
		후방 대각선
	*/

	if (keyMgr.IsPressing(VK_S))
	{
		if (keyMgr.IsPressing(VK_D))
		{
			diagonal *= -1;

			D3DXQUATERNION quat;
			D3DXQuaternionRotationYawPitchRoll(&quat, D3DXToRadian(-45.f), 0, 0);

			D3DXMATRIX rotMat;
			D3DXMatrixRotationQuaternion(&rotMat, &quat);

			D3DXVec3TransformNormal(&diagonal, &diagonal, &rotMat);

			//never mess up with Y-velocity.
			btVector3 currentVel = m_rigidbody->getLinearVelocity();
			//m_rigidbody->setLinearVelocity(btVector3(diagonal.x*speed, currentVel.getY(), diagonal.z*speed));
			m_rigidbody->applyCentralForce(btVector3(diagonal.x*speed, currentVel.getY(), diagonal.z*speed));
			//rotation
			float yaw = D3DXToDegree(atan2(diagonal.x, diagonal.z));
			D3DXQuaternionRotationYawPitchRoll(&m_desiredQuat, D3DXToRadian(yaw), 0, 0);

			//transform.setRot(0, yaw, 0);

			//anim name
			setAnimation("Aim_Running");

			return;
		}
	}

	if (keyMgr.IsPressing(VK_S))
	{
		if (keyMgr.IsPressing(VK_A))
		{
			diagonal *= -1;

			D3DXQUATERNION quat;
			D3DXQuaternionRotationYawPitchRoll(&quat, D3DXToRadian(45.f), 0, 0);

			D3DXMATRIX rotMat;
			D3DXMatrixRotationQuaternion(&rotMat, &quat);

			D3DXVec3TransformNormal(&diagonal, &diagonal, &rotMat);

			//never mess up with Y-velocity.
			btVector3 currentVel = m_rigidbody->getLinearVelocity();
			//m_rigidbody->setLinearVelocity(btVector3(diagonal.x*speed, currentVel.getY(), diagonal.z*speed));
			m_rigidbody->applyCentralForce(btVector3(diagonal.x*speed, currentVel.getY(), diagonal.z*speed));

			//rotation
			float yaw = D3DXToDegree(atan2(diagonal.x, diagonal.z));
			D3DXQuaternionRotationYawPitchRoll(&m_desiredQuat, D3DXToRadian(yaw), 0, 0);


			//transform.setRot(0, yaw, 0);

			//anim name
			setAnimation("Aim_Running");

			return;
		}
	}

	/*
		W
	*/
	if (keyMgr.IsPressing(VK_W))
	{
		//never mess up with Y-velocity.
		btVector3 currentVel = m_rigidbody->getLinearVelocity();
		//m_rigidbody->setLinearVelocity(btVector3(currentVel.x()+ forward.x*speed, currentVel.getY(), currentVel.z()+ forward.z*speed));
		m_rigidbody->applyCentralForce(btVector3(forward.x*speed, currentVel.getY(), forward.z*speed));
		//rotation
		float yaw = D3DXToDegree(atan2(forward.x, forward.z));
		D3DXQuaternionRotationYawPitchRoll(&m_desiredQuat, D3DXToRadian(yaw), 0, 0);

		//transform.setRot(0, yaw, 0);

		//anim name
		setAnimation("Aim_Running");
	}
	if (keyMgr.IsUp(VK_W))
	{
		setAnimation("Idle_Aim");
		m_rigidbody->setLinearVelocity(btVector3(0, 0, 0));
	}

	/*
		S
	*/
	if (keyMgr.IsPressing(VK_S))
	{
		//never mess up with Y-velocity.
		btVector3 currentVel = m_rigidbody->getLinearVelocity();
		//m_rigidbody->setLinearVelocity(btVector3(-forward.x*speed, currentVel.getY(), -forward.z*speed));
		m_rigidbody->applyCentralForce(btVector3(-forward.x*speed, currentVel.getY(), -forward.z*speed));


		//rotation
		float yaw = D3DXToDegree(atan2(forward.x, forward.z));
		D3DXQuaternionRotationYawPitchRoll(&m_desiredQuat, D3DXToRadian(yaw), 0, 0);

		//transform.setRot(0, yaw, 0);

		//anim name
		setAnimation("Run_backwards");
	}
	if (keyMgr.IsUp(VK_S))
	{
		setAnimation("Idle_Aim");
		m_rigidbody->setLinearVelocity(btVector3(0, 0, 0));
	}

	/*
		A
	*/
	if (keyMgr.IsPressing(VK_A))
	{
		//never mess up with Y-velocity.
		btVector3 currentVel = m_rigidbody->getLinearVelocity();
		//m_rigidbody->setLinearVelocity(btVector3(-right.x*speed / 2, currentVel.getY(), -right.z*speed / 2));
		m_rigidbody->applyCentralForce(btVector3(-right.x*speed / 2, currentVel.getY(), -right.z*speed / 2));


		//rotation
		float yaw = D3DXToDegree(atan2(forward.x, forward.z));
		D3DXQuaternionRotationYawPitchRoll(&m_desiredQuat, D3DXToRadian(yaw), 0, 0);

		//transform.setRot(0, yaw, 0);

		//anim name
		setAnimation("Strafe_Left");
	}
	if (keyMgr.IsUp(VK_A))
	{
		setAnimation("Idle_Aim");
		m_rigidbody->setLinearVelocity(btVector3(0, 0, 0));
	}

	/*
		D
	*/
	if (keyMgr.IsPressing(VK_D))
	{
		//never mess up with Y-velocity.
		btVector3 currentVel = m_rigidbody->getLinearVelocity();
		//m_rigidbody->setLinearVelocity(btVector3(right.x*speed / 2, currentVel.getY(), right.z*speed / 2));
		m_rigidbody->applyCentralForce(btVector3(right.x*speed / 2, currentVel.getY(), right.z*speed / 2));


		//rotation
		float yaw = D3DXToDegree(atan2(forward.x, forward.z));
		D3DXQuaternionRotationYawPitchRoll(&m_desiredQuat, D3DXToRadian(yaw), 0, 0);

		//transform.setRot(0, yaw, 0);

		//anim name
		setAnimation("Strafe_Right");
	}
	if (keyMgr.IsUp(VK_D))
	{
		setAnimation("Idle_Aim");
		m_rigidbody->setLinearVelocity(btVector3(0, 0, 0));
	}
}

void gamePlayer::render()
{
	Model::render();
}

#include "stdafx.h"
#include "gamePlayer.h"
#include "gameCameraThirdPerson.h"


gamePlayer::gamePlayer()
{
}


gamePlayer::~gamePlayer()
{
}

void gamePlayer::init(char* dir, char* FileName)
{
	Model::init(dir, FileName);
	gameUtil.m_physicsWorld->CreateCapsuleRigidbody(transform.getPos(), 0.5, 0.8,2, "player");
	m_rigidbody = gameUtil.m_physicsWorld->m_mapRigidbody["player"];

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
	D3DXQuaternionSlerp(&m_OriginalQuat, &m_OriginalQuat, &m_desiredQuat, gameTimer.getDeltaTime()*10.f);
	

	if (m_slerpFactor >= 1.0f)
		m_slerpFactor = 1.0f;

	transform.setRot(m_OriginalQuat);


	m_rigidbody->activate(true);
	m_rigidbody->clearForces();
	

	//키보드 입력값 받고, 바로 강체에 움직임 적용.
	processingInput();

	//회전 y축 고정.
	btTransform bttransform;

	m_rigidbody->getMotionState()->getWorldTransform(bttransform);
	btVector3 velocity = m_rigidbody->getAngularVelocity();
	velocity.setX(0);
	velocity.setZ(0);
	m_rigidbody->setAngularVelocity(velocity);

	//최종값을 player 트랜스폼에 입력.
	transform.setPos(bttransform.getOrigin().getX(),
		bttransform.getOrigin().getY(),
		bttransform.getOrigin().getZ());

	btQuaternion quat = bttransform.getRotation();
	D3DXQUATERNION d3dxquat = D3DXQUATERNION(quat.x(), quat.y(), quat.z(), quat.w());
}


void gamePlayer::processingInput()
{
	D3DXVECTOR3 forward = ((gameCameraThirdPerson*)gameUtil.GetMainCamera())->m_vForward;
	D3DXVECTOR3 right = ((gameCameraThirdPerson*)gameUtil.GetMainCamera())->m_vRight;
	D3DXVECTOR3 diagonal = forward;

	float speed = 10.f;

	/*
		전방 대각선
	*/
	if(keyMgr.IsPressing(VK_W))
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
			m_rigidbody->setLinearVelocity(btVector3(diagonal.x*speed, currentVel.getY(), diagonal.z*speed));

			//rotation
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
			m_rigidbody->setLinearVelocity(btVector3(diagonal.x*speed, currentVel.getY(), diagonal.z*speed));

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
			m_rigidbody->setLinearVelocity(btVector3(diagonal.x*speed, currentVel.getY(), diagonal.z*speed));

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
			m_rigidbody->setLinearVelocity(btVector3(diagonal.x*speed, currentVel.getY(), diagonal.z*speed));

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
		m_rigidbody->setLinearVelocity(btVector3(forward.x*speed, currentVel.getY(), forward.z*speed));

		//rotation
		float yaw = D3DXToDegree(atan2(forward.x, forward.z));
		D3DXQuaternionRotationYawPitchRoll(&m_desiredQuat, D3DXToRadian(yaw), 0, 0);

		//transform.setRot(0, yaw, 0);

		//anim name
		setAnimation("Aim_Running");
	}
	if (keyMgr.IsUp(VK_W))
	{
		setAnimation("Idle");
		m_rigidbody->setLinearVelocity(btVector3(0, 0, 0));
	}

	/*
		S
	*/
	if (keyMgr.IsPressing(VK_S))
	{
		//never mess up with Y-velocity.
		btVector3 currentVel = m_rigidbody->getLinearVelocity();
		m_rigidbody->setLinearVelocity(btVector3(-forward.x*speed, currentVel.getY(), -forward.z*speed));

		//rotation
		float yaw = D3DXToDegree(atan2(forward.x, forward.z));
		D3DXQuaternionRotationYawPitchRoll(&m_desiredQuat, D3DXToRadian(yaw), 0, 0);

		//transform.setRot(0, yaw, 0);

		//anim name
		setAnimation("Run_backwards");
	}
	if (keyMgr.IsUp(VK_S))
	{
		setAnimation("Idle");
		m_rigidbody->setLinearVelocity(btVector3(0, 0, 0));
	}

	/*
		A
	*/
	if (keyMgr.IsPressing(VK_A))
	{
		//never mess up with Y-velocity.
		btVector3 currentVel = m_rigidbody->getLinearVelocity();
		m_rigidbody->setLinearVelocity(btVector3(-right.x*speed/2, currentVel.getY(), -right.z*speed/2));

		//rotation
		float yaw = D3DXToDegree(atan2(forward.x, forward.z));
		D3DXQuaternionRotationYawPitchRoll(&m_desiredQuat, D3DXToRadian(yaw), 0, 0);

		//transform.setRot(0, yaw, 0);

		//anim name
		setAnimation("Strafe_Left");
	}
	if (keyMgr.IsUp(VK_A))
	{
		setAnimation("Idle");
		m_rigidbody->setLinearVelocity(btVector3(0, 0, 0));
	}

	/*
		D
	*/
	if (keyMgr.IsPressing(VK_D))
	{
		//never mess up with Y-velocity.
		btVector3 currentVel = m_rigidbody->getLinearVelocity();
		m_rigidbody->setLinearVelocity(btVector3(right.x*speed/2, currentVel.getY(), right.z*speed/2));

		//rotation
		float yaw = D3DXToDegree(atan2(forward.x, forward.z));
		D3DXQuaternionRotationYawPitchRoll(&m_desiredQuat, D3DXToRadian(yaw), 0, 0);

		//transform.setRot(0, yaw, 0);

		//anim name
		setAnimation("Strafe_Right");
	}
	if (keyMgr.IsUp(VK_D))
	{
		setAnimation("Idle");
		m_rigidbody->setLinearVelocity(btVector3(0, 0, 0));
	}
}

void gamePlayer::render()
{
	Model::render();
}

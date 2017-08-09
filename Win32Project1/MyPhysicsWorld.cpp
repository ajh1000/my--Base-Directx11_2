#include "stdafx.h"
#include "MyPhysicsWorld.h"

MyPhysicsWorld::MyPhysicsWorld()
{




}


MyPhysicsWorld::~MyPhysicsWorld()
{
	SAFE_DELETE(m_dynamicsWorld);
	SAFE_DELETE(m_solver);
	SAFE_DELETE(m_dispatcher);
	SAFE_DELETE(m_collisionConfiguration);
	SAFE_DELETE(m_broadphase);

	SAFE_DELETE(m_TriangleMesh_map);
	SAFE_DELETE(m_collisionShape_map);
	delete m_rigidbody_map->getMotionState();
	SAFE_DELETE(m_rigidbody_map);

	SAFE_DELETE(m_collisionshape_player);
	delete m_rigidbody_player->getMotionState();
	SAFE_DELETE(m_rigidbody_map);
}

void MyPhysicsWorld::init()
{
	// Build the broadphase
	m_broadphase = new btDbvtBroadphase();

	// Set up the collision configuration and dispatcher
	m_collisionConfiguration = new btDefaultCollisionConfiguration();
	m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);


	// The actual physics solver
	m_solver = new btSequentialImpulseConstraintSolver;

	// The world.
	m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_broadphase, m_solver, m_collisionConfiguration);
	m_dynamicsWorld->setGravity(btVector3(0, -10, 0));

	// Do_everything_else_here

	//map
	m_TriangleMesh_map = new btTriangleMesh();
	Model* pmap = gameUtil.m_mapTag["map"];
	vector<Model::MeshInfo>& meshInfo = pmap->m_vecMesh;
	for (int cnt= 0; cnt< pmap->m_vecMesh.size(); ++cnt)
	{
		for (int idx = meshInfo[cnt].indexStart; idx <   meshInfo[cnt].indexStart + meshInfo[cnt].cntIndex; idx+=3)
		{
			int index1 = meshInfo[cnt].vertexStart + pmap->m_indices[idx];
			int index2 = meshInfo[cnt].vertexStart + pmap->m_indices[idx+1];
			int index3 = meshInfo[cnt].vertexStart + pmap->m_indices[idx+2];

			XMFLOAT3 pos1 = pmap->m_vertex_skinned_xm[index1].pos;
			btVector3 btPos1(pos1.x, pos1.y, pos1.z);
			XMFLOAT3 pos2 = pmap->m_vertex_skinned_xm[index2].pos;
			btVector3 btPos2(pos2.x, pos2.y, pos2.z);
			XMFLOAT3 pos3 = pmap->m_vertex_skinned_xm[index3].pos;
			btVector3 btPos3(pos3.x, pos3.y, pos3.z);

			m_TriangleMesh_map->addTriangle(btPos1, btPos2, btPos3, false);
		}
	}



	m_collisionShape_map = new btBvhTriangleMeshShape(m_TriangleMesh_map, true);
	m_collisionShape_map->setMargin(0.05f);

	btDefaultMotionState* groundMotionState =
		new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));
	btRigidBody::btRigidBodyConstructionInfo
		mapRigidBodyCI(0, groundMotionState, m_collisionShape_map);
	m_rigidbody_map = new btRigidBody(mapRigidBodyCI);
	m_dynamicsWorld->addRigidBody(m_rigidbody_map);

	
	//player
	m_collisionshape_player = new btSphereShape(0.5);
	btDefaultMotionState* playerMS = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 50, 0)));


	float mass = 2.f;

	btVector3 fallInertia(0, 0, 0);
	m_collisionshape_player->calculateLocalInertia(mass, fallInertia);

	btRigidBody::btRigidBodyConstructionInfo
		playerRigidCI(1, playerMS, m_collisionshape_player, fallInertia);
	m_rigidbody_player = new btRigidBody(playerRigidCI);
	m_dynamicsWorld->addRigidBody(m_rigidbody_player);
	
	//m_dynamicsWorld->setDebugDrawer()
	//
	////튕기게하고싶으면 두 줄의 코드를 추가. mass를 높게할수록 높이 뜀. 10을 주면 엄청 뜀.
	//m_rigidbody_map->setRestitution(1.0f);
	//m_rigidbody_player->setRestitution(1.0f);
	//
}

void MyPhysicsWorld::update()
{
	m_dynamicsWorld->stepSimulation(1 / 1000.f, 10);

	btTransform playerTransform;
	m_rigidbody_player->getMotionState()->getWorldTransform(playerTransform);


	Model* player = gameUtil.m_mapTag["player"];
	player->transform.setPos(playerTransform.getOrigin().getX(),
		playerTransform.getOrigin().getY(),
		playerTransform.getOrigin().getZ());

	btQuaternion quat = playerTransform.getRotation();
	D3DXQUATERNION d3dxquat = D3DXQUATERNION(quat.x(), quat.y(), quat.z(), quat.w());

	player->transform.setRot(d3dxquat);



	if (keyMgr.IsPressed(VK_1))
	{

		m_rigidbody_player->activate(true);

		//clear all velocity. 
		m_rigidbody_player->setAngularVelocity(btVector3(0, 0, 0));
		m_rigidbody_player->setLinearVelocity(btVector3(0, 0, 0));

		m_rigidbody_player->getWorldTransform().setIdentity();
		m_rigidbody_player->getWorldTransform().setOrigin(btVector3(0, 50, 0));
		
	}

	if (keyMgr.IsPressing(VK_2))
	{
		m_rigidbody_player->activate(true);

		m_rigidbody_player->applyCentralImpulse(btVector3(0, 0, 1.f));
	}
}

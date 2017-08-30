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

	for (int i = 0; i < m_vecTriangleMesh.size(); ++i)
	{
		SAFE_DELETE(m_vecTriangleMesh[i]);
	}

	for (int i = 0; i < m_vecCollisionShape.size(); ++i)
	{
		SAFE_DELETE(m_vecCollisionShape[i]);
	}

	for (int i = 0; i < m_vecRigidbody.size(); ++i)
	{
		delete m_vecRigidbody[i]->getMotionState();
		SAFE_DELETE(m_vecRigidbody[i]);
	}



}

void MyPhysicsWorld::CreateStaticRigidbody(vector<Model::MeshInfo>& meshInfo, 
	vector<vertex_ptn_skinned_xm>& vertices,
	vector<UINT>& indices, string tag)
{
	btTriangleMesh* staticMesh = new btTriangleMesh();

	for (int cnt = 0; cnt< meshInfo.size(); ++cnt)
	{
		for (int idx = meshInfo[cnt].indexStart; idx < meshInfo[cnt].indexStart + meshInfo[cnt].cntIndex; idx += 3)
		{
			int index1 = meshInfo[cnt].vertexStart + indices[idx];
			int index2 = meshInfo[cnt].vertexStart + indices[idx + 1];
			int index3 = meshInfo[cnt].vertexStart + indices[idx + 2];

			XMFLOAT3 pos1 = vertices[index1].pos;
			btVector3 btPos1(pos1.x, pos1.y, pos1.z);
			XMFLOAT3 pos2 = vertices[index2].pos;
			btVector3 btPos2(pos2.x, pos2.y, pos2.z);
			XMFLOAT3 pos3 = vertices[index3].pos;
			btVector3 btPos3(pos3.x, pos3.y, pos3.z);

			staticMesh->addTriangle(btPos1, btPos2, btPos3, false);
		}
	}

	m_vecTriangleMesh.push_back(staticMesh);

	btCollisionShape* collisionShape = new btBvhTriangleMeshShape(staticMesh, true);
	collisionShape->setMargin(0.05f);

	m_vecCollisionShape.push_back(collisionShape);


	btDefaultMotionState* motionState =
		new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));

	btRigidBody::btRigidBodyConstructionInfo
		rigidBodyCI(0, motionState, collisionShape);

	btRigidBody* rigidbody = new btRigidBody(rigidBodyCI);

	m_vecRigidbody.push_back(rigidbody);

	m_dynamicsWorld->addRigidBody(rigidbody);
	m_mapRigidbody[tag] = rigidbody;

}

void MyPhysicsWorld::CreateCapsuleRigidbody(D3DXVECTOR3 pos, float radius, float height, float mass ,string tag)
{
	btCollisionShape* collisionShape = new btCapsuleShape(radius, height);

	btDefaultMotionState* motionState =
		new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(pos.x, pos.y, pos.z)));



	btVector3 fallInertia(0, 0, 0);
	collisionShape->calculateLocalInertia(mass, fallInertia);

	btRigidBody::btRigidBodyConstructionInfo
		rigidBodyCI(1, motionState, collisionShape, fallInertia);

	btRigidBody* rigidbody = new btRigidBody(rigidBodyCI);
	m_dynamicsWorld->addRigidBody(rigidbody);
	m_mapRigidbody[tag] = rigidbody;
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

	//m_dynamicsWorld->setDebugDrawer()
	//
	////튕기게하고싶으면 두 줄의 코드를 추가. mass를 높게할수록 높이 뜀. 10을 주면 엄청 뜀.
	//m_rigidbody_map->setRestitution(1.0f);
	//m_rigidbody_player->setRestitution(1.0f);
	//
}

void MyPhysicsWorld::update()
{
	m_dynamicsWorld->stepSimulation(1 / 60.0f, 10);

	//btTransform playerTransform;
	//m_rigidbody_player->getMotionState()->getWorldTransform(playerTransform);


	//Model* player = gameUtil.m_mapTag["player"];
	//player->transform.setPos(playerTransform.getOrigin().getX(),
	//	playerTransform.getOrigin().getY(),
	//	playerTransform.getOrigin().getZ());

	//btQuaternion quat = playerTransform.getRotation();
	//D3DXQUATERNION d3dxquat = D3DXQUATERNION(quat.x(), quat.y(), quat.z(), quat.w());

	//player->transform.setRot(d3dxquat);



	//if (keyMgr.IsPressed(VK_1))
	//{

	//	m_rigidbody_player->activate(true);

	//	//clear all velocity. 
	//	m_rigidbody_player->setAngularVelocity(btVector3(0, 0, 0));
	//	m_rigidbody_player->setLinearVelocity(btVector3(0, 0, 0));

	//	m_rigidbody_player->getWorldTransform().setIdentity();
	//	m_rigidbody_player->getWorldTransform().setOrigin(btVector3(0, 50, 0));
	//	
	//}

	//if (keyMgr.IsPressing(VK_2))
	//{
	//	m_rigidbody_player->activate(true);

	//	m_rigidbody_player->applyCentralImpulse(btVector3(0, 0, 1.f));
	//}
}

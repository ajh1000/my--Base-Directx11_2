#pragma once

#include <btBulletDynamicsCommon.h>
#include <string>
class MyPhysicsWorld
{
public:
	MyPhysicsWorld();
	virtual ~MyPhysicsWorld();

	void init();
	void update();

	//create and add rigidbody to dynamic world.
	void CreateSphereRigidbody(D3DXVECTOR3 pos, float radius, string tag);
	void CreateStaticRigidbody(vector<Model::MeshInfo>& meshInfo,
		vector<vertex_ptn_skinned_xm>& vertices,
		vector<UINT>& indices, string tag);
	void CreateCapsuleRigidbody(D3DXVECTOR3 pos, float radius,float height, float mass, string tag);

	vector<btTriangleMesh*> m_vecTriangleMesh;
	vector<btCollisionShape*> m_vecCollisionShape;
	vector<btRigidBody*> m_vecRigidbody;

	map<string, btRigidBody*> m_mapRigidbody;
public:
	//must use after MyPhysicsWorld::init();
	btDiscreteDynamicsWorld* m_dynamicsWorld = nullptr;

private:
	btBroadphaseInterface* m_broadphase=nullptr;
	btCollisionDispatcher* m_dispatcher = nullptr;
	btDefaultCollisionConfiguration* m_collisionConfiguration = nullptr;
	btSequentialImpulseConstraintSolver* m_solver = nullptr;
	
};


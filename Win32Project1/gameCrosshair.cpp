#include "stdafx.h"
#include "gameCrosshair.h"

gameCrosshair::gameCrosshair()
{
}


gameCrosshair::~gameCrosshair()
{
}

void gameCrosshair::init()
{
	gameObject::Init_compileShader("./cube_tex_vs.hlsl", "./cube_tex_ps.hlsl");
	gameObject::Init_polygonLayout(gameObject::EPolygonLayout::LAYOUT_PT);

	vertex_pt vertices[4] = {};
	vertices[0].pos = D3DXVECTOR3(-1, 1, 0.0f);
	vertices[0].uv = D3DXVECTOR2(0.0f, 0);

	vertices[1].pos = D3DXVECTOR3(1, 1, 0.0f);
	vertices[1].uv = D3DXVECTOR2(1.0f, 0.0f);

	vertices[2].pos = D3DXVECTOR3(-1, -1, 0.0f);
	vertices[2].uv = D3DXVECTOR2(0, 1.0f);

	vertices[3].pos = D3DXVECTOR3(1, -1, 0.0f);
	vertices[3].uv = D3DXVECTOR2(1.0f, 1.0f);

	UINT indices[6] =
	{
		0,1,2,
		2,1,3
	};

	gameObject::Init_CreateVertexBuffer(vertices, sizeof(vertex_pt), 4, D3D11_USAGE_DEFAULT,
		D3D11_BIND_VERTEX_BUFFER, 0);

	gameObject::Init_option_CreateIndexBuffer(indices, sizeof(UINT), 6, D3D11_USAGE_DEFAULT, D3D11_BIND_INDEX_BUFFER, 0);

	m_texture.load("./DATA/crosshair.png");

	m_pPlayer = ((gamePlayer*)gameUtil.m_mapTag["player"]);
	m_pCamera = ((gameCameraThirdPerson*)gameUtil.GetMainCamera());

}

void gameCrosshair::update()
{

}

void gameCrosshair::lateUpdate()
{

	D3DXVECTOR3 forward = m_pCamera->m_vForward;
	D3DXVECTOR3 pos = m_pPlayer->transform.getPos();
	pos.y += 4.5;
	
	
	btVector3 Start = btVector3(pos.x, pos.y, pos.z);
	btVector3 End = Start + btVector3(forward.x, forward.y, forward.z) * 20;
	btVector3 Normal;

	btCollisionWorld::ClosestRayResultCallback RayCallback(Start, End);


	gameUtil.m_physicsWorld->m_dynamicsWorld->rayTest(Start, End, RayCallback);

	if (RayCallback.hasHit()) {
		const btCollisionObject* object = RayCallback.m_collisionObject;
		string* type = (string*)object->getUserPointer();
		if (type != nullptr)
		{
			if (type->compare("bullet")) // bullet 타입이 아닐 경우에만 히트 포인트로 갱신.
			{
				End = RayCallback.m_hitPointWorld;
				Normal = RayCallback.m_hitNormalWorld;
				End += Normal * 2;
				transform.setPos(End.x(), End.y(), End.z());
			}
		}
		else
		{
			End = RayCallback.m_hitPointWorld;
			Normal = RayCallback.m_hitNormalWorld;
			End += Normal * 2;
			transform.setPos(End.x(), End.y(), End.z());
		}

	}
	else
	{
		transform.setPos(pos + forward * 20);
	}


	float pitch = D3DXToDegree(asin(-forward.y));
	float yaw = D3DXToDegree(atan2(forward.x, forward.z));

	transform.setRot(pitch, yaw, 0);
}

void gameCrosshair::render()
{
	m_texture.render();

	gameObject::renderIndexed();
}

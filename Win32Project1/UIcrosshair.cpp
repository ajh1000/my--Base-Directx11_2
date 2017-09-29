#include "stdafx.h"
#include "UIcrosshair.h"

UIcrosshair::UIcrosshair()
{
}


UIcrosshair::~UIcrosshair()
{
}

void UIcrosshair::init(int xpos, int ypos, int vertexWidth, int vertexHeight, int textureWidth, int textureHeight, string URL)
{
	GameUIObject::init(xpos, ypos, vertexWidth, vertexHeight, textureWidth, textureHeight, URL);


}

void UIcrosshair::update()
{/*
	static float t = 0;

	t += gameTimer.getDeltaTime();
	if (t >= 1)
	{
		m_htmlTexture.m_rect.left += 1;

		RECT rect=m_htmlTexture.m_rect;

		rect.left += rect.right / 2;
		rect.top += rect.bottom / 2;

		t = 0;
		setPos(rect.left, rect.top,
			rect.right, rect.bottom);
	}*/
	gamePlayer* pPlayer = (gamePlayer*)gameUtil.m_mapTag["player"];
	gameCameraThirdPerson* pCamera = (gameCameraThirdPerson*)gameUtil.GetMainCamera();

	D3DXVECTOR3 forward = pCamera->m_vForward;
	D3DXVECTOR3 pos = pPlayer->transform.getPos();
	pos.y += 4.5;


	btVector3 start = btVector3(pos.x, pos.y, pos.z);
	btVector3 end = start + btVector3(forward.x, forward.y, forward.z) * 1000;
	btVector3 Normal;

	btCollisionWorld::ClosestRayResultCallback RayCallback(start, end);


	gameUtil.m_physicsWorld->m_dynamicsWorld->rayTest(start, end, RayCallback);

	if (RayCallback.hasHit())
	{
		end = RayCallback.m_hitPointWorld;
		Normal = RayCallback.m_hitNormalWorld;

		const btCollisionObject* object = RayCallback.m_collisionObject;
		string* type = (string*)object->getUserPointer();
		if (type != nullptr)
		{
			if (type->compare("bullet")) // bullet 타입이 아닐 경우에만 히트 포인트로 갱신.
			{
				end = RayCallback.m_hitPointWorld;
				Normal = RayCallback.m_hitNormalWorld;

				D3DXVECTOR3 screenPoint = screenToWorld(D3DXVECTOR3(end.x(), end.y(), end.z()));

				setPos(screenPoint.x, screenPoint.y);

			}
		}

	}
	else
	{
		D3DXVECTOR3 screenPoint = screenToWorld(D3DXVECTOR3(end.x(), end.y(), end.z()));
		setPos(screenPoint.x, screenPoint.y);
		
	}
}

void UIcrosshair::render()
{
	GameUIObject::render();
}

D3DXVECTOR3 UIcrosshair::screenToWorld(D3DXVECTOR3 vIn)
{
	D3DXMATRIX proj;
	D3DXMATRIX view;
	D3DXMATRIX world;

	proj = gameUtil.GetMainCamera()->GetProjMat();
	view = gameUtil.GetMainCamera()->GetViewMat();
	D3DXMatrixIdentity(&world);
	D3DXMatrixTranslation(&world, vIn.x, vIn.y, vIn.z);


	
	XMFLOAT4X4 xmproj, xmview, xmworld;
	memcpy(xmproj.m, proj.m, sizeof(float) * 16);
	memcpy(xmview.m, view.m, sizeof(float) * 16);
	memcpy(xmworld.m, world.m, sizeof(float) * 16);
	

	
	FXMMATRIX  testProj = XMLoadFloat4x4(&xmproj);
	XMMATRIX testView= XMLoadFloat4x4(&xmview);
	XMMATRIX testWorld= XMLoadFloat4x4(&xmworld);


	const float HalfViewportWidth = 1600 * 0.5f;
	const float HalfViewportHeight = 900 * 0.5f;

	XMVECTOR Scale = XMVectorSet(HalfViewportWidth, -HalfViewportHeight, 1 - 0, 0.0f);
	XMVECTOR Offset = XMVectorSet(0 + HalfViewportWidth, 0 + HalfViewportHeight, 0, 0.0f);

	XMMATRIX Transform = XMMatrixMultiply(testWorld, testView);
	Transform = XMMatrixMultiply(Transform, testProj);

	XMVECTOR Result = XMVector3TransformCoord(XMVectorZero(), Transform);

	Result = XMVectorMultiplyAdd(Result, Scale, Offset);

	/*
	projection 부분에서 에러난다. FXMMATRIX으로 받는게 원인인 것 같다.
	XMVECTOR vOut = myXMVector3Project(XMVectorZero(),
		0, 0, 1600, 900, 0, 1,
		testProj, testView, testWorld);
	  */
	XMFLOAT3 screenPoint;
	XMStoreFloat3(&screenPoint, Result);
	
	return D3DXVECTOR3(screenPoint.x, screenPoint.y, screenPoint.z);
	
}

#include "stdafx.h"
#include "gameCrosshair.h"

gameCrosshair::gameCrosshair()
{
}


gameCrosshair::~gameCrosshair()
{
	SAFE_RELEASE(m_depthStencilState);
	SAFE_RELEASE(m_depthDisabledStencilState);
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
				//End += Normal * 2;
				transform.setPos(End.x(), End.y(), End.z());
			}
		}
		else
		{
			End = RayCallback.m_hitPointWorld;
			Normal = RayCallback.m_hitNormalWorld;
			//End += Normal * 2;
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
	TurnZBufferOff();
	m_texture.render();

	gameObject::renderIndexed();
	TurnZBufferOn();
}



void gameCrosshair::initDepthStencilState()
{
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state.
	HRESULT result = gameUtil.getDevice()->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);

	D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;

	// Clear the second depth stencil state before setting the parameters.
	ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));

	// Now create a second depth stencil state which turns off the Z buffer for 2D rendering.  The only difference is 
	// that DepthEnable is set to false, all other parameters are the same as the other depth stencil state.
	depthDisabledStencilDesc.DepthEnable = false;
	depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthDisabledStencilDesc.StencilEnable = true;
	depthDisabledStencilDesc.StencilReadMask = 0xFF;
	depthDisabledStencilDesc.StencilWriteMask = 0xFF;
	depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the state using the device.
	result = gameUtil.getDevice()->CreateDepthStencilState(&depthDisabledStencilDesc, &m_depthDisabledStencilState);

}

void gameCrosshair::TurnZBufferOn()
{
	gameUtil.getDeviceContext()->OMSetDepthStencilState(m_depthStencilState, 1);
}

void gameCrosshair::TurnZBufferOff()
{
	gameUtil.getDeviceContext()->OMSetDepthStencilState(m_depthDisabledStencilState, 1);
}
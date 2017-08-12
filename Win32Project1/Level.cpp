#include "stdafx.h"
#include "Level.h"
#include "MyPathfinder.h"
#include "MyPhysicsWorld.h"

Level::Level()
{
}


Level::~Level()
{
	std::vector<gameObject*>::iterator it;
	for (it = gameUtil.m_vecGameObjects.begin(); it != gameUtil.m_vecGameObjects.end(); ++it)
	{
		SAFE_DELETE(*it);
	}

	std::vector<Model*>::iterator it2;
	for (it2 = gameUtil.m_vecEnemys.begin(); it2 != gameUtil.m_vecEnemys.end(); ++it2)
	{
		SAFE_DELETE(*it2);
	}
}

void Level::init()
{
	/*
		UI
	*/
	m_uiScene.init();
	gameUtil.m_gameUIScene = &m_uiScene;



	/*
		PLAYER
	*/
	Model* player = new Model();
	player->init("./DATA/ArmyPilot/", "ArmyPilot.x");
	player->transform.setPos(0, 50, 0);
	player->transform.setScale(0.02, 0.02, 0.02);
	player->transform.rotate(0, 180, 0);
	player->m_AnimName = "Idle";
	player->m_lightProperties.lightDirection = D3DXVECTOR3(0, -1, 1);
	gameUtil.m_vecGameObjects.push_back(player);
	gameUtil.m_mapTag["player"] = player;

	vector<XMFLOAT3> arr;
	arr.resize(player->m_vertex_skinned_xm.size());
	for (int i = 0; i < arr.size(); ++i)
	{
		arr[i] = player->m_vertex_skinned_xm[i].pos;
	}

	BoundingBox::CreateFromPoints(player->m_box,
		arr.size(),
		&arr[0],
		sizeof(XMFLOAT3));

	XMFLOAT4X4 mat;
	player->transform.buildMatrixWVP();
	D3DXMATRIX d3dxmat = player->transform.getWVP().world;
	memcpy(&mat._11, &d3dxmat._11, sizeof(float) * 16);
	player->m_box.Transform(player->m_box, XMLoadFloat4x4(&mat));

	//map
	Model* pmap = new Model();
	pmap->init("./DATA/map/", "map.obj");
	pmap->transform.setPos(0, 0, 0);
	pmap->transform.setRot(0, 0, 0);
	gameUtil.m_vecGameObjects.push_back(pmap);
	gameUtil.m_mapTag["map"] = pmap;

	//map AABB
	Model* pmapAABB = new Model();
	pmapAABB->init("./DATA/map/", "aabb.obj");
	pmapAABB->transform.setPos(0, 0, 0);
	pmapAABB->transform.setRot(0, 0, 0);
	gameUtil.m_vecGameObjects.push_back(pmapAABB);


	arr.clear();
	arr.resize(pmapAABB->m_vertex_skinned_xm.size());
	for (int i = 0; i < arr.size(); ++i)
	{
		arr[i] = pmapAABB->m_vertex_skinned_xm[i].pos;
	}

	BoundingBox::CreateFromPoints(pmapAABB->m_box,
		arr.size(),
		&arr[0],
		sizeof(XMFLOAT3));

	ZeroMemory(&mat._11, 4 * 16);

	pmapAABB->transform.buildMatrixWVP();
	d3dxmat = pmapAABB->transform.getWVP().world;
	memcpy(&mat._11, &d3dxmat._11, sizeof(float) * 16);

	pmapAABB->m_box.Transform(pmapAABB->m_box, XMLoadFloat4x4(&mat));

	std::vector<gameObject*>::iterator it;
	for (it = gameUtil.m_vecGameObjects.begin(); it != gameUtil.m_vecGameObjects.end(); ++it)
	{
		//(*it)->init();
	}


	phyWorld.init();

	myPath.init("all_tiles_navmesh.bin");
	gameUtil.myPath = &myPath;
}

void Level::update()
{
	Model* player = (Model*)gameUtil.m_vecGameObjects[0];

	myPath.update();

	//if (keyMgr.IsPressed(VK_LBUTTON)) // CREATE ENEMY
	//{
	//	gameUtil.GetMainCamera()->pick(gameUtil.m_mouseX, gameUtil.m_mouseY);

	//	myPath.addAgent(gameUtil.GetMainCamera()->pickedPos[0],
	//		gameUtil.GetMainCamera()->pickedPos[1],
	//		gameUtil.GetMainCamera()->pickedPos[2]);

	//	Model* enemy = new Model();
	//	enemy->init("./DATA/ArmyPilot/", "ArmyPilot.x");
	//	enemy->transform.setPos(gameUtil.GetMainCamera()->pickedPos[0],
	//		gameUtil.GetMainCamera()->pickedPos[1],
	//		gameUtil.GetMainCamera()->pickedPos[2]);
	//	enemy->transform.setScale(0.02, 0.02, 0.02);
	//	enemy->m_AnimName = "Idle";
	//	enemy->m_lightProperties.lightDirection = D3DXVECTOR3(0, -1, 1);
	//	gameUtil.m_vecEnemys.push_back(enemy);

	//	m_agentOldPos.push_back(D3DXVECTOR3(myPath.crowd->getAgent(m_agentOldPos.size())->npos[0],
	//		myPath.crowd->getAgent(m_agentOldPos.size())->npos[1],
	//		myPath.crowd->getAgent(m_agentOldPos.size())->npos[2]));
	//}

	//if (keyMgr.IsPressed(VK_RBUTTON)) //SET PLAYER POS
	//{
	//	gameUtil.GetMainCamera()->pick(gameUtil.m_mouseX, gameUtil.m_mouseY);


	//	player->transform.setPos(gameUtil.GetMainCamera()->pickedPos[0],
	//		gameUtil.GetMainCamera()->pickedPos[1],
	//		gameUtil.GetMainCamera()->pickedPos[2]);

	//}
	//if (keyMgr.IsPressed(VK_RETURN))
	//{
	//	gameUtil.GetMainCamera()->GameStart = true;


	//	for (int i = 0; i < gameUtil.m_vecEnemys.size(); ++i)
	//	{
	//		gameUtil.m_vecEnemys[i]->m_AnimName = "Run_Forwards";
	//	}
	//}


	//if (gameUtil.GetMainCamera()->GameStart == true)
	//{
	//	dtNavMeshQuery* navquery = gameUtil.myPath->navQuery;

	//	dtCrowd* pcrowd = gameUtil.myPath->crowd;
	//	const dtQueryFilter* filter = pcrowd->getFilter(0);
	//	const float* ext = pcrowd->getQueryExtents();

	//	D3DXVECTOR3 playerPos = gameUtil.m_vecGameObjects[0]->transform.getPos();

	//	float p[3] = {
	//		playerPos.x,
	//		playerPos.y,
	//		playerPos.z };

	//	navquery->findNearestPoly(p, ext, filter, &gameUtil.myPath->m_targetRef, gameUtil.myPath->m_targetPos);

	//	for (int i = 0; i < gameUtil.m_vecEnemys.size(); ++i)
	//	{
	//		D3DXVECTOR3 agentPos = {
	//			myPath.crowd->getAgent(i)->npos[0],
	//			myPath.crowd->getAgent(i)->npos[1],
	//			myPath.crowd->getAgent(i)->npos[2]
	//		};

	//		D3DXVECTOR3 playerPos = player->transform.getPos();
	//		float dist = D3DXVec3Length(&(playerPos - agentPos));
	//		if (dist > 4)
	//		{
	//			gameUtil.m_vecEnemys[i]->m_AnimName = "Run_Forwards";

	//			gameUtil.myPath->crowd->requestMoveTarget(i, gameUtil.myPath->m_targetRef, gameUtil.myPath->m_targetPos);

	//			D3DXVECTOR3 agentOldPos = m_agentOldPos[i];

	//			D3DXVECTOR3 agentNewPos = {
	//				myPath.crowd->getAgent(i)->npos[0],
	//				myPath.crowd->getAgent(i)->npos[1],
	//				myPath.crowd->getAgent(i)->npos[2]
	//			};

	//			D3DXVECTOR3 agentDir = agentNewPos - agentOldPos;
	//			D3DXVec3Normalize(&agentDir, &agentDir);

	//			D3DXVECTOR3 forward = D3DXVECTOR3(0, 0, 1);

	//			float angle = D3DXVec3Dot(&forward, &agentDir);
	//			angle = acos(angle)* (180.0f / D3DX_PI);
	//			if (agentDir.x >= 0)
	//			{
	//				gameUtil.m_vecEnemys[i]->transform.setRot(0, angle, 0);
	//			}
	//			else
	//			{
	//				gameUtil.m_vecEnemys[i]->transform.setRot(0, -angle, 0);
	//			}

	//			m_agentOldPos[i] = agentNewPos;
	//		}
	//		else
	//		{
	//			gameUtil.m_vecEnemys[i]->m_AnimName = "Idle_Firing";
	//			static float elapsedTime = 0;
	//			elapsedTime += gameTimer.getDeltaTime();
	//			if (elapsedTime > 6.f)
	//			{
	//				Dead = true;

	//				gameUtil.m_vecEnemys[i]->m_AnimName = "Idle";
	//			}
	//		}

	//	}

	//}
	//for (int i = 0; i < gameUtil.m_vecEnemys.size(); ++i)
	//{


	//}

/*
	for (int i = 0; i < gameUtil.m_vecEnemys.size(); ++i)
	{
		gameUtil.m_vecEnemys[i]->update();
	}*/


	std::vector<gameObject*>::iterator it;
	for (it = gameUtil.m_vecGameObjects.begin(); it != gameUtil.m_vecGameObjects.end(); ++it)
	{
		(*it)->update();
	}

	lateUpdate();
}

void Level::lateUpdate()
{
	// must be first
	gameUtil.GetMainCamera()->update();

	phyWorld.update();
	m_uiScene.update();
}

void Level::render()
{

	std::vector<gameObject*>::iterator it;
	for (it = gameUtil.m_vecGameObjects.begin(); it != gameUtil.m_vecGameObjects.end(); ++it)
	{
		if (it == gameUtil.m_vecGameObjects.begin())
		{
			if (Dead == true)
			{
				continue;
			}
		}
		(*it)->render();
	}
	/*
	for (int i = 0; i < gameUtil.m_vecEnemys.size(); ++i)
	{
		gameUtil.m_vecEnemys[i]->render();
	}*/

	m_uiScene.render();
}


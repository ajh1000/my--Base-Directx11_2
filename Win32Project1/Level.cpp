#include "stdafx.h"
#include "Level.h"
#include "MyPathfinder.h"
#include "MyPhysicsWorld.h"
#include "gamePlayer.h"
#include "gameMap.h"
#include "gameCameraThirdPerson.h"
#include "BulletManager.h"
#include "gameCrosshair.h"

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

	BulletManager::GetInstance().shutdown();
}

void Level::init()
{
	gameUtil.m_isPaused = false;

	/*
		UI
	*/
	m_uiScene.init();
	gameUtil.m_gameUIScene = &m_uiScene;

	/*
		PathFinder
	*/
	myPath.init("all_tiles_navmesh.bin");
	gameUtil.myPath = &myPath;

	/*
		PhysicsWorld
	*/
	phyWorld.init();
	gameUtil.m_physicsWorld = &phyWorld;

	/*
		Bullet Pooling Manager
	*/
	BulletManager::GetInstance().init(10);



	/*
		SPIDER
	*/
	D3DXVECTOR3 lightDir = D3DXVECTOR3(1, -1, -1);
	D3DXVec3Normalize(&lightDir, &lightDir);

	Model* spider = new Model();
	spider->transform.setPos(0, 0, 0);
	spider->transform.setScale(0.02, 0.02, 0.02);
	spider->transform.rotate(0, 0, 0);
	spider->init("./DATA/spider/", "Spider_3.fbx");
	spider->m_AnimName = "Spider_Armature|Attack";
	spider->m_lightProperties.lightDirection = lightDir;
	gameUtil.m_vecGameObjects.push_back(spider);


	/*
		PLAYER
	*/
	gamePlayer* player = new gamePlayer();
	player->transform.setPos(0, 3, 0);
	player->transform.setScale(0.02, 0.02, 0.02);
	player->transform.rotate(0, 0, 0);
	player->init("./DATA/ArmyPilot/", "ArmyPilot.x");
	player->m_AnimName = "Idle_Aim";
	player->m_lightProperties.lightDirection = lightDir;
	gameUtil.m_vecGameObjects.push_back(player);
	gameUtil.m_mapTag["player"] = player;

	((gameCameraThirdPerson*)gameUtil.GetMainCamera())->setTargetPos(player);

	/*
		MAP
	*/
	gameMap* pmap = new gameMap();
	pmap->init("./DATA/map2/", "map2.obj");
	pmap->transform.setPos(0, 0, 0);
	pmap->transform.setRot(0, 0, 0);
	player->m_lightProperties.lightDirection = lightDir;
	gameUtil.m_vecGameObjects.push_back(pmap);
	gameUtil.m_mapTag["map"] = pmap;

	/*
	crosshair
	*/
	gameCrosshair* crosshair = new gameCrosshair();
	crosshair->init();
	crosshair->transform.setPos(0, 0, 0);
	crosshair->transform.setScale(2, 2, 2);
	crosshair->transform.rotate(0, 0, 0);
	gameUtil.m_vecGameObjects.push_back(crosshair);
	gameUtil.m_mapTag["crosshair"] = crosshair;
	//std::vector<gameObject*>::iterator it;
	//for (it = gameUtil.m_vecGameObjects.begin(); it != gameUtil.m_vecGameObjects.end(); ++it)
	//{
	//	//(*it)->init();
	//}


}

void Level::update()
{
	
	if (keyMgr.IsPressed(VK_ESCAPE))
	{
		if (gameUtil.m_isPaused == false)
			gameUtil.m_isPaused = true;
		else
			gameUtil.m_isPaused = false;
	}

	if (gameUtil.m_isPaused == true)
	{
		static HCURSOR hcursor = LoadCursor(0, IDC_ARROW);

		SetCursor(hcursor);


	}
	else
	{
		POINT pt;
		RECT rc;
		GetClientRect(gameUtil.GetHWND(), &rc);
		pt.x = (rc.right - rc.left) / 2;
		pt.y = (rc.bottom - rc.top) / 2;
		ClientToScreen(gameUtil.GetHWND(), &pt);
		SetCursorPos(pt.x, pt.y);

		SetCursor(NULL);
	}


	if (gameUtil.m_isPaused == false)
		gameUtil.GetMainCamera()->update();

	//Model* player = (Model*)gameUtil.m_vecGameObjects[0];


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


	for (int i = 0; i < gameUtil.m_vecGameObjects.size(); ++i)
	{
		gameUtil.m_vecGameObjects[i]->update();
	}

	lateUpdate();
}

void Level::lateUpdate()
{
	
	if (gameUtil.m_isPaused == false)
	{
		gameUtil.GetMainCamera()->lateUpdate();
		((gamePlayer*)gameUtil.m_mapTag["player"])->lateUpdate();
		((gameCrosshair*)gameUtil.m_mapTag["crosshair"])->lateUpdate();
		BulletManager::GetInstance().update();
		myPath.update();
		phyWorld.update();
	}

	m_uiScene.update();
}

void Level::render()
{

	for (int i = 0; i < gameUtil.m_vecGameObjects.size(); ++i)
	{
		gameUtil.m_vecGameObjects[i]->render();
	}

	BulletManager::GetInstance().render();
	m_uiScene.render();
}


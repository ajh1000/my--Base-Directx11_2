#include "stdafx.h"
#include "Level.h"
#include "MyPathfinder.h"
#include "MyPhysicsWorld.h"
#include "gamePlayer.h"
#include "gameMap.h"
#include "gameCameraThirdPerson.h"
#include "BulletManager.h"
#include "gameCrosshair.h"
#include "gameEnemyManager.h"
#include "UIhealthBar.h"
#include "gameCubemap.h"
#include "gameCapsule.h"

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

	
		//Cubemap
	
	gameCubemap* cubemap = new gameCubemap();
	cubemap->init();
	
	gameUtil.m_vecGameObjects.push_back(cubemap);
	gameUtil.m_mapTag["cubemap"] = cubemap;
	
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
	BulletManager::GetInstance().init(100);
	
	/*
		Enemy Pooling Manager
	*/
	gameEnemyManager::GetInstance().init(20);


	//global light direction.
	D3DXVECTOR3 lightDir = D3DXVECTOR3(1, -1, -1);
	D3DXVec3Normalize(&lightDir, &lightDir);


	/*
		SPIDER
	
	

	Model* spider = new Model();
	spider->transform.setPos(0, 0, 0);
	spider->transform.setScale(0.02, 0.02, 0.02);
	spider->transform.rotate(0, 0, 0);
	spider->init("./DATA/spider/", "Spider_3.fbx");
	spider->m_AnimName = "Spider_Armature|Attack";
	spider->m_lightProperties.lightDirection = lightDir;
	gameUtil.m_vecGameObjects.push_back(spider);*/


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
	//pmap->transform.setScale(10, 10, 10);
	player->m_lightProperties.lightDirection = lightDir;
	gameUtil.m_vecGameObjects.push_back(pmap);
	gameUtil.m_mapTag["map"] = pmap;

	/*
		CAPSULE
	*/
	gameCapsule* capsule = new gameCapsule();
	capsule->init("./data/capsule/", "capsule.obj");
	capsule->transform.setPos(10, 0, -14);
	capsule->transform.setScale(0.5, 0.5, 0.5);
	gameUtil.m_vecGameObjects.push_back(capsule);
	
	/*
		//¾Æ±º
	*/
	Model* aliance = new Model();
	aliance->init("./DATA/spider/", "Spider_3.fbx");
	aliance->transform.setPos(0,250, 0);
	aliance->transform.setRot(0, 90, 0);
	aliance->transform.setScale(0.02, 0.02, 0.02);
	aliance->setAnimation("Idle_Aim");
	gameUtil.m_vecGameObjects.push_back(aliance);
	
	Model* aliance2 = new Model();
	aliance2->init("./DATA/ArmyPilot/", "ArmyPilot.x");
	aliance2->transform.setPos(-13, 0, 2);
	aliance2->transform.setRot(0, 90, 0);
	aliance2->setAnimation("Idle_Aim");
	aliance2->transform.setScale(0.02, 0.02, 0.02);
	gameUtil.m_vecGameObjects.push_back(aliance2);


	Model* aliance3 = new Model();
	aliance3->init("./DATA/ArmyPilot/", "ArmyPilot.x");
	aliance3->transform.setPos(-16, 0, 4);
	aliance3->transform.setRot(0, 90, 0);
	aliance3->setAnimation("Idle_Aim");
	aliance3->transform.setScale(0.02, 0.02, 0.02);
	gameUtil.m_vecGameObjects.push_back(aliance3);
	

	Model* aliance4 = new Model();
	aliance4->init("./DATA/ArmyPilot/", "ArmyPilot.x");
	aliance4->transform.setPos(-18, 0, -2);
	aliance4->transform.setRot(0, 90, 0);
	aliance4->setAnimation("Idle_Aim");
	aliance4->transform.setScale(0.02, 0.02, 0.02);
	gameUtil.m_vecGameObjects.push_back(aliance4);


	Model* aliance5 = new Model();
	aliance5->init("./DATA/ArmyPilot/", "ArmyPilot.x");
	aliance5->transform.setPos(-17, 0, -4);
	aliance5->transform.setRot(0, 90, 0);
	aliance5->setAnimation("Idle_Aim");
	aliance5->transform.setScale(0.02, 0.02, 0.02);
	gameUtil.m_vecGameObjects.push_back(aliance5);
	

	/*
	crosshair
	
	gameCrosshair* crosshair = new gameCrosshair();
	crosshair->init();
	crosshair->transform.setPos(0, 0, 0);
	crosshair->transform.setScale(2, 2, 2);
	crosshair->transform.rotate(0, 0, 0);
	gameUtil.m_vecGameObjects.push_back(crosshair);
	gameUtil.m_mapTag["crosshair"] = crosshair;*/


	((gameCameraThirdPerson*)gameUtil.GetMainCamera())->m_vEye = D3DXVECTOR3(200, 200, -200);
	int a = 0;
}

void Level::update()
{
	gameCameraThirdPerson* camera = (gameCameraThirdPerson*)gameUtil.GetMainCamera();

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

	if (keyMgr.IsPressed(VK_RETURN))
	{
		camera->m_camMode = camera->TPS_MODE;

	}

	if (keyMgr.IsPressed(VK_1))
	{
		camera->m_camMode = camera->FPS_MODE;

		camera->m_vEye = D3DXVECTOR3(200, 200, -200);
		camera->m_vLookat = D3DXVECTOR3(0, 0, 0);

		((UIhealthBar*)gameUtil.m_gameUIScene->m_mapUI["healthBar"])->reset();

		gameEnemyManager::GetInstance().reset();
		BulletManager::GetInstance().reset();

		((gamePlayer*)gameUtil.m_mapTag["player"])->transform.setPos(0, 3, 0);

		((gamePlayer*)gameUtil.m_mapTag["player"])->transform.setRot(0, 3, 0);
		((gamePlayer*)gameUtil.m_mapTag["player"])->m_isDead = false;
		//clear all velocity. 
		((gamePlayer*)gameUtil.m_mapTag["player"])->m_rigidbody->setAngularVelocity(btVector3(0, 0, 0));
		((gamePlayer*)gameUtil.m_mapTag["player"])->m_rigidbody->setLinearVelocity(btVector3(0, 0, 0));

		((gamePlayer*)gameUtil.m_mapTag["player"])->m_rigidbody->getWorldTransform().setIdentity();
		((gamePlayer*)gameUtil.m_mapTag["player"])->m_rigidbody->getWorldTransform().setOrigin(
			btVector3(0, 3, 0));

		for (int i = 0; i < gameUtil.myPath->crowd->getAgentCount(); ++i)
		{
			gameUtil.myPath->crowd->removeAgent(i);

		}
	}

	for (int i = 0; i < gameUtil.m_vecGameObjects.size(); ++i)
	{
		gameUtil.m_vecGameObjects[i]->update();
	}

	lateUpdate();
}

void Level::lateUpdate()
{
	gameCameraThirdPerson* camera = (gameCameraThirdPerson*)gameUtil.GetMainCamera();

	if (gameUtil.m_isPaused == true
			&& camera->m_camMode==camera->FPS_MODE)
	{
		myPath.update();

	}


	if (gameUtil.m_isPaused == false)
	{
		gameUtil.GetMainCamera()->lateUpdate();
		((gamePlayer*)gameUtil.m_mapTag["player"])->lateUpdate();
		BulletManager::GetInstance().update();

		gameEnemyManager::GetInstance().update();
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
	gameEnemyManager::GetInstance().render();
	m_uiScene.render();
}


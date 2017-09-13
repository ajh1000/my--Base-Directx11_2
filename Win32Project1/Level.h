#pragma once

#include "cube.h"

#include "cube_tex.h"
#include "cube_normal.h"

#include "Model.h"
#include "Terrain.h"
#include "MyPhysicsWorld.h"
#include "MyPathfinder.h"
#include "GameUIScene.h"

class Level
{
public:
	Level();
	~Level();

	void init();
	void update();
	void lateUpdate();
	void render();

private:
	GameUIScene m_uiScene;

	MyPhysicsWorld phyWorld;

	MyPathfinder myPath;
	vector<D3DXVECTOR3> m_agentOldPos;
	
};


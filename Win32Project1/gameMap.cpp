#include "stdafx.h"
#include "gameMap.h"


gameMap::gameMap()
{
}


gameMap::~gameMap()
{
}

void gameMap::init(char* dir, char* FileName)
{
	Model::init(dir, FileName);
	gameUtil.m_physicsWorld->CreateStaticRigidbody(m_vecMesh, m_vertex_skinned_xm, m_indices,"map");
}

void gameMap::update()
{
}
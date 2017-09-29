#pragma once
#include "gameEnemy.h"
class gameEnemyManager
{
public:
	gameEnemyManager();
	virtual ~gameEnemyManager();


	void init(int cnt);
	void update();
	void render();

	void shutdown();
	void instantiate(D3DXVECTOR3 pos);
	void reset();
	singleton_class(gameEnemyManager)
public:
	float m_duration = 5.f;
	vector<gameEnemy*>  m_vecEnemys;
	int m_enableIdx = 0;
	int m_capacity = 0;
};


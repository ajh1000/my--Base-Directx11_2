#pragma once
#include "gameBullet.h"

class BulletManager
{
public:
	BulletManager();
	virtual ~BulletManager();

	void init(int cnt);
	void update();
	void render();

	void shutdown();
	void instantiate(D3DXVECTOR3 pos, D3DXVECTOR3 dir);
	singleton_class(BulletManager)
public:
	float m_duration = 5.f;
	vector<gameBullet*>  m_vecBullets;
	int m_enableIdx = 0;
	int m_capacity = 0;
};


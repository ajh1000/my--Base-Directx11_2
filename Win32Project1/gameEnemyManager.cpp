#include "stdafx.h"
#include "gameEnemyManager.h"


gameEnemyManager::gameEnemyManager()
{
}


gameEnemyManager::~gameEnemyManager()
{
}

void gameEnemyManager::shutdown()
{
	SAFE_RELEASE(m_vecEnemys[0]->m_vs);
	SAFE_RELEASE(m_vecEnemys[0]->m_ps);
	SAFE_RELEASE(m_vecEnemys[0]->m_vsblob);
	SAFE_RELEASE(m_vecEnemys[0]->m_psblob);
	SAFE_DELETE(m_vecEnemys[0]);
	for (int i = 1; i < m_vecEnemys.size(); ++i)
	{
		m_vecEnemys[i]->m_vs = nullptr;
		m_vecEnemys[i]->m_ps = nullptr;
		m_vecEnemys[i]->m_vsblob = nullptr;
		m_vecEnemys[i]->m_psblob = nullptr;

		SAFE_DELETE(m_vecEnemys[i]);
	}


}
void gameEnemyManager::init(int cnt)
{
	m_capacity = cnt;
	m_vecEnemys.resize(cnt);

	m_vecEnemys[0] = new gameEnemy();
	m_vecEnemys[0]->Init_compileShader("./cube_normal_vs.hlsl", "./cube_normal_ps.hlsl");
	m_vecEnemys[0]->m_tag = "enemy";
	m_vecEnemys[0]->m_tag.append(std::to_string(0));
	m_vecEnemys[0]->init("./DATA/spider/", "Spider_3.fbx");

	for (int i = 1; i < cnt; ++i)
	{
		m_vecEnemys[i] = new gameEnemy();
		m_vecEnemys[i]->m_vs = m_vecEnemys[0]->m_vs;
		m_vecEnemys[i]->m_ps = m_vecEnemys[0]->m_ps;
		m_vecEnemys[i]->m_vsblob = m_vecEnemys[0]->m_vsblob;
		m_vecEnemys[i]->m_psblob = m_vecEnemys[0]->m_psblob;
		m_vecEnemys[i]->m_tag = "enemy";
		m_vecEnemys[i]->m_tag.append(std::to_string(i));
		m_vecEnemys[i]->init("./DATA/spider/", "Spider_3.fbx");


	}
}

void gameEnemyManager::update()
{
	for (int i = 0; i < m_vecEnemys.size(); ++i)
	{
		if (m_vecEnemys[i]->m_isEnabled == true)
			m_vecEnemys[i]->update();
	}
}

void gameEnemyManager::render()
{
	for (int i = 0; i < m_vecEnemys.size(); ++i)
	{
		if (m_vecEnemys[i]->m_isEnabled == true)
			m_vecEnemys[i]->render();
	}
}


void gameEnemyManager::instantiate(D3DXVECTOR3 pos)
{
	if (m_enableIdx >= m_capacity)
	{
		m_enableIdx = 0;
	}


	m_vecEnemys[m_enableIdx]->setEnable(pos);


	m_enableIdx++;
}

void gameEnemyManager::reset()
{
	for (int i = 0; i < m_vecEnemys.size(); ++i)
	{
		m_vecEnemys[i]->m_isEnabled = false;

	}
}

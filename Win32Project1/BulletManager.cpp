#include "stdafx.h"
#include "BulletManager.h"


BulletManager::BulletManager()
{
}


BulletManager::~BulletManager()
{
}

void BulletManager::shutdown()
{
	SAFE_RELEASE(m_vecBullets[0]->m_vs);
	SAFE_RELEASE(m_vecBullets[0]->m_ps);
	SAFE_RELEASE(m_vecBullets[0]->m_vsblob);
	SAFE_RELEASE(m_vecBullets[0]->m_psblob);
	SAFE_DELETE(m_vecBullets[0]);
	for (int i = 1; i < m_vecBullets.size(); ++i)
	{
		m_vecBullets[i]->m_vs = nullptr;
		m_vecBullets[i]->m_ps = nullptr;
		m_vecBullets[i]->m_vsblob = nullptr;
		m_vecBullets[i]->m_psblob = nullptr;

		SAFE_DELETE(m_vecBullets[i]);
	}
}

void BulletManager::init(int cnt)
{



	m_capacity = cnt;
	m_vecBullets.resize(cnt);

	m_vecBullets[0] = new gameBullet();
	m_vecBullets[0]->Init_compileShader("./cube_normal_vs.hlsl", "./cube_normal_ps.hlsl");
	m_vecBullets[0]->m_tag = "bullet";
	m_vecBullets[0]->m_tag.append(std::to_string(0));
	m_vecBullets[0]->init();

	for (int i = 1; i < cnt; ++i)
	{
		m_vecBullets[i] = new gameBullet();
		m_vecBullets[i]->m_vs = m_vecBullets[0]->m_vs;
		m_vecBullets[i]->m_ps= m_vecBullets[0]->m_ps;
		m_vecBullets[i]->m_vsblob = m_vecBullets[0]->m_vsblob;
		m_vecBullets[i]->m_psblob = m_vecBullets[0]->m_psblob;
		m_vecBullets[i]->m_tag = "bullet";
		m_vecBullets[i]->m_tag.append(std::to_string(i));
		m_vecBullets[i]->init();

		
	}
}

void BulletManager::update()
{
	for (int i = 0; i < m_vecBullets.size(); ++i)
	{
		if(m_vecBullets[i]->m_isEnabled==true)
			m_vecBullets[i]->update();
	}
}

void BulletManager::render()
{
	for (int i = 0; i < m_vecBullets.size(); ++i)
	{
		if (m_vecBullets[i]->m_isEnabled == true)
			m_vecBullets[i]->render();
	}
}

void BulletManager::instantiate(D3DXVECTOR3 pos, D3DXVECTOR3 dir)
{
	if (m_enableIdx >= m_capacity)
	{
		m_enableIdx = 0;
	}
	
	
	m_vecBullets[m_enableIdx]->setEnable(pos, dir);


	m_enableIdx++;
}

void BulletManager::reset()
{
	for (int i = 0; i < m_vecBullets.size(); ++i)
	{
		m_vecBullets[i]->m_isEnabled = false;

	}
}

#pragma once
#include "gamePlayer.h"
class gameBullet : public Model
{
public:
	gameBullet();
	virtual ~gameBullet();

	void init();
	virtual void update();
	virtual void render();
	void setEnable(D3DXVECTOR3 pos,D3DXVECTOR3 dir);
public:
	gamePlayer* m_player=nullptr;
	btRigidBody* m_rigidbody = nullptr;
	bool m_isEnabled = false;
	float m_power = 50;
	float m_elapsedTime = 0;
	string m_tag;
	string m_strType;

};


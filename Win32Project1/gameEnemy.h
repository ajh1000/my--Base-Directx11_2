#pragma once
#include "Model.h"
class gameEnemy : public Model
{
public:
	gameEnemy();
	virtual ~gameEnemy();

	virtual void init(char* dir, char* FileName);
	virtual void update();
	virtual void lateUpdate();
	virtual void render();

	void setEnable(D3DXVECTOR3 pos);
	void setPos(D3DXVECTOR3 pos);

public:
	string m_tag;
	string m_strType;
	bool m_isEnabled=false;
	btRigidBody* m_rigidbody = nullptr;
	float m_elapsedTime = 0;
	bool m_isDead = false;
	float m_fireRate = 4;
	float m_fireElapsedTime = 4;
	float m_attackAnimationTime = 0;
};


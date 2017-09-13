#pragma once
#include "Model.h"
class gameMap :
	public Model
{
public:
	gameMap();
	virtual ~gameMap();

	void init(char* dir, char* FileName);
	virtual void update();
	string m_strType;
};


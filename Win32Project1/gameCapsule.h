#pragma once
#include "Model.h"
class gameCapsule :
	public Model
{
public:
	gameCapsule();
	~gameCapsule();

	void init(char* dir, char* FileName);
	void update();
	void render();
};


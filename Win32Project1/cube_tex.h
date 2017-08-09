#pragma once
#include "gameObject.h"

class cube_tex :public gameObject
{
public:
	cube_tex();
	virtual ~cube_tex();
	
	void init();
	void update();
	virtual void render() override;

private:
	textureclass texture;
};


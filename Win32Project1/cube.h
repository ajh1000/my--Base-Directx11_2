#pragma once
#include "gameObject.h"

class cube : public gameObject
{
public:
	cube();
	virtual ~cube();

	void init();
	void update();
	virtual void render() override;

};


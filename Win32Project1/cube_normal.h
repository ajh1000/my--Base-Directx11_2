#pragma once
#include "gameObject.h"

class cube_normal : public gameObject
{
public:
	cube_normal();
	cube_normal(float x,float y, float z);
	virtual ~cube_normal();

	virtual void init() override;
	virtual void update() override;
	virtual void render() override;

private:
	textureclass texture;

	ID3D11Buffer* m_LightBuffer = 0;
	LightBuffer m_lightProperties = {};

};



#pragma once

#include "Camera.h"

class TestCamera : public Camera
{
public:
	TestCamera();
	virtual ~TestCamera();

	virtual void update();
};


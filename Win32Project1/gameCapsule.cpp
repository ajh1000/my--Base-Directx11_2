#include "stdafx.h"
#include "gameCapsule.h"


gameCapsule::gameCapsule()
{
}


gameCapsule::~gameCapsule()
{
}

void gameCapsule::init(char* dir, char* FileName)
{
	Model::init(dir, FileName);
}

void gameCapsule::update()
{
	Model::update();
}

void gameCapsule::render()
{
	Model::render();
}

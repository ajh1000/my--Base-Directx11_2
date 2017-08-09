#include "stdafx.h"
#include "MyPathfinder.h"
#include "Model.h"

MyPathfinder::MyPathfinder()
{
}


MyPathfinder::~MyPathfinder()
{
	if (navQuery)
		dtFreeNavMeshQuery(navQuery);
	if (crowd)
		dtFreeCrowd(crowd);
	if (navMesh)
		dtFreeNavMesh(navMesh);
}

dtNavMesh* MyPathfinder::loadAll(const char* path)
{
	FILE* fp = fopen(path, "rb");
	if (!fp) return 0;

	// Read header.
	NavMeshSetHeader header;
	size_t readLen = fread(&header, sizeof(NavMeshSetHeader), 1, fp);
	if (readLen != 1)
	{
		fclose(fp);
		return 0;
	}
	if (header.magic != NAVMESHSET_MAGIC)
	{
		fclose(fp);
		return 0;
	}
	if (header.version != NAVMESHSET_VERSION)
	{
		fclose(fp);
		return 0;
	}

	dtNavMesh* mesh = dtAllocNavMesh();
	if (!mesh)
	{
		fclose(fp);
		return 0;
	}
	dtStatus status = mesh->init(&header.params);
	if (dtStatusFailed(status))
	{
		fclose(fp);
		return 0;
	}

	// Read tiles.
	for (int i = 0; i < header.numTiles; ++i)
	{
		NavMeshTileHeader tileHeader;
		readLen = fread(&tileHeader, sizeof(tileHeader), 1, fp);
		if (readLen != 1)
		{
			fclose(fp);
			return 0;
		}

		if (!tileHeader.tileRef || !tileHeader.dataSize)
			break;

		unsigned char* data = (unsigned char*)dtAlloc(tileHeader.dataSize, DT_ALLOC_PERM);
		if (!data) break;
		memset(data, 0, tileHeader.dataSize);
		readLen = fread(data, tileHeader.dataSize, 1, fp);
		if (readLen != 1)
		{
			fclose(fp);
			return 0;
		}

		mesh->addTile(data, tileHeader.dataSize, DT_TILE_FREE_DATA, tileHeader.tileRef, 0);
	}

	fclose(fp);

	return mesh;
}

void MyPathfinder::init(string dir)
{
	loadAll(dir.c_str());

	navMesh = loadAll(dir.c_str());


	navQuery = dtAllocNavMeshQuery();
	navQuery->init(navMesh, 2048);
	
	crowd = dtAllocCrowd();
	crowd->init(1000, 10, navMesh);

}

void MyPathfinder::addAgent(float x, float y, float z)
{
	dtCrowdAgent agent;
	dtCrowdAgentParams params = {};
	params.radius = 0.6;
	params.height = 2.0;
	params.maxAcceleration = 8.0f;
	params.maxSpeed = 3.5f;
	params.collisionQueryRange = params.radius * 12.0f;
	params.pathOptimizationRange = params.radius * 30.0f;
	params.updateFlags =  DT_CROWD_SEPARATION;
	params.obstacleAvoidanceType = 3.0;
	params.separationWeight = 3.0f;

	float pos[] = { x,y,z };
	int idx = crowd->addAgent(pos, &params);
	
}

void MyPathfinder::update()
{
	Model* player = (Model*)gameUtil.m_vecGameObjects[0];

	if (gameUtil.m_vecEnemys.size() > 0)
	{

		dtCrowdAgentDebugInfo info;
		crowd->update(gameTimer.getDeltaTime(), &info);
		
		/*
			player->transform.setPos(crowd.getAgent(0)->npos[0],
				crowd.getAgent(0)->npos[1],
				crowd.getAgent(0)->npos[2]);
		*/
	}

	for (int i = 0; i < gameUtil.m_vecEnemys.size(); ++i)
	{
		D3DXVECTOR3 agentPos = {
			crowd->getAgent(i)->npos[0],
			crowd->getAgent(i)->npos[1],
			crowd->getAgent(i)->npos[2]
		};

		D3DXVECTOR3 playerPos = player->transform.getPos();
		float dist = D3DXVec3Length(&(playerPos - agentPos));
		if (dist> 4)
		{
			gameUtil.m_vecEnemys[i]->transform.setPos(agentPos);
		}
		else
		{

		}
	}
}

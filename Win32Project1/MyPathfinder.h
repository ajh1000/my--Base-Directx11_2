#pragma once


#include <DetourNavMesh.h>
#include <DetourNavMeshBuilder.h>
#include <DetourCrowd.h>

class MyPathfinder
{
public:
	MyPathfinder();
	virtual ~MyPathfinder();

	dtNavMesh* loadAll(const char* path);

	void init(string dir);
	void addAgent(float x, float y, float z);

	void update();

	dtNavMesh* navMesh = nullptr;
	dtNavMeshQuery* navQuery = nullptr;
	dtCrowd* crowd = nullptr;
	dtPolyRef m_targetRef = 0;
	float m_targetPos[3] = {};
private:


	struct NavMeshSetHeader
	{
		int magic;
		int version;
		int numTiles;
		dtNavMeshParams params;
	};


	struct NavMeshTileHeader
	{
		dtTileRef tileRef;
		int dataSize;
	};



	static const int NAVMESHSET_MAGIC = 'M' << 24 | 'S' << 16 | 'E' << 8 | 'T'; //'MSET';
	static const int NAVMESHSET_VERSION = 1;
};


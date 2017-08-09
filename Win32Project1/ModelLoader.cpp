#include "stdafx.h"
#include "ModelLoader.h"
#include <assimp/Importer.hpp>     // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include <assimp/Exporter.hpp>

ModelLoader::ModelLoader()
{
}


ModelLoader::~ModelLoader()
{
}

void ModelLoader::init()
{  // Create an instance of the Importer class
	Assimp::Importer importer;
	// And have it read the given file with some example postprocessing
	// Usually - if speed is not the most important aspect for you - you'll 
	// propably to request more postprocessing than we do in this example.
	const aiScene* scene = importer.ReadFile("DATA/Elf-Final.blend",
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);

	Assimp::Exporter exporter;
}

#pragma once
#include "fbxsdk.h"
#include <vector>
#include <string.h>

#ifdef IOS_REF
#undef  IOS_REF
#define IOS_REF (*(pManager->GetIOSettings()))
#endif

class fbxLoader
{
public:

	fbxLoader(FbxString _name);
	std::vector<std::vector<float>> getWeights();
	std::vector<int> getOrder();
private:
	std::vector<int> controlPoint;
	std::vector<std::vector<float>> weights;

	bool InitializeSdkObjects(FbxManager*& pManager, FbxScene*& pScene);
	bool LoadScene(FbxManager* pManager, FbxDocument* pScene, const char* pFilename);
	void recursive(FbxNode *pNode);
	void skinWeights(FbxGeometry* pGeometry);
	void fbxLoader::savePolygons(FbxMesh* pMesh);
};


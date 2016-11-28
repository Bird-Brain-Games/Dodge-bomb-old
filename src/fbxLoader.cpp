#include "fbxLoader.h"
fbxLoader::fbxLoader(FbxString _name)
{
	std::vector<float> temp;
	for (int i = 0; i < 613; i++)
	{
		weights.push_back(temp);
	}
	FbxManager* lSdkManager = NULL;
	FbxScene* lScene = NULL;
	bool lResult;


	// Prepare the FBX SDK.
	InitializeSdkObjects(lSdkManager, lScene);
	// Load the scene.
	// The example can take a FBX file as an argument.
	FbxString lFilePath(_name);

	if (lFilePath.IsEmpty())
	{
		lResult = false;
		FBXSDK_printf("\n\nUsage: ImportScene <FBX file name>\n\n");
	}
	else
	{
		FBXSDK_printf("\n\nFile: %s\n\n", lFilePath.Buffer());
		lResult = LoadScene(lSdkManager, lScene, lFilePath.Buffer());
	}

	if (lResult == false)
	{
		FBXSDK_printf("\n\nAn error occurred while loading the scene...");
	}
	FbxNode* lNode = lScene->GetRootNode();


	if (lNode)
	{
		for (int i = 0; i < lNode->GetChildCount(); i++)
		{
			recursive(lNode->GetChild(i));
		}
	}

	std::vector<float> stop;

}


void fbxLoader::recursive(FbxNode *pNode)
{

	FbxNodeAttribute::EType lAttributeType;
	lAttributeType = pNode->GetNodeAttribute()->GetAttributeType();
	switch (lAttributeType)
	{
	default:
		break;
	case FbxNodeAttribute::eMesh:
		FbxMesh* lMesh = (FbxMesh*)pNode->GetNodeAttribute();
		savePolygons(lMesh);
		skinWeights(lMesh);
		break;
	}

	for (int i = 0; i < pNode->GetChildCount(); i++)
	{
		recursive(pNode->GetChild(i));
	}
}


void fbxLoader::savePolygons(FbxMesh* pMesh)
{
	int i, j, l;
	int	lPolygonCount = pMesh->GetPolygonCount();
	FbxVector4* lControlPoints = pMesh->GetControlPoints();
	int vertexId = 0;
	for (i = 0; i < lPolygonCount; i++)
	{
		int lPolygonSize = pMesh->GetPolygonSize(i);

		for (j = 0; j < lPolygonSize; j++)
		{
			int lControlPointIndex = pMesh->GetPolygonVertex(i, j);
			controlPoint.push_back(lControlPointIndex);
		}
		vertexId++;
	} // for polygonSize
}

void fbxLoader::skinWeights(FbxGeometry* pGeometry)
{
	//Display cluster now

	//int i, lLinkCount;
	//FbxCluster* lLink;

	int i, j;
	int lSkinCount = 0;
	int lClusterCount = 0;
	FbxCluster* lCluster;

	lSkinCount = pGeometry->GetDeformerCount(FbxDeformer::eSkin);



	//lLinkCount = pGeometry->GetLinkCount();
	for (i = 0; i != lSkinCount; ++i)
	{
		lClusterCount = ((FbxSkin *)pGeometry->GetDeformer(i, FbxDeformer::eSkin))->GetClusterCount();
		for (j = 0; j != lClusterCount; ++j)
		{
			lCluster = ((FbxSkin *)pGeometry->GetDeformer(i, FbxDeformer::eSkin))->GetCluster(j);
			//lLink = pGeometry->GetLink(i);    


			FbxString lString1 = "        Link Indices: ";
			FbxString lString2 = "        Weight Values: ";

			int k, lIndexCount = lCluster->GetControlPointIndicesCount();
			int* lIndices = lCluster->GetControlPointIndices();
			double* lWeights = lCluster->GetControlPointWeights();

			for (k = 0; k < lIndexCount; k++)
			{
				printf("spot: %d \n", lIndices[k]);
				weights[lIndices[k]].push_back((float)lWeights[k]);

			}


			//base translation

			//lMatrix = lCluster->GetTransformMatrix(lMatrix);
			//Display3DVector("        Transform Translation: ", lMatrix.GetT());
			//Display3DVector("        Transform Rotation: ", lMatrix.GetR());
			//Display3DVector("        Transform Scaling: ", lMatrix.GetS());

			//lMatrix = lCluster->GetTransformLinkMatrix(lMatrix);
			//Display3DVector("        Transform Link Translation: ", lMatrix.GetT());
			//Display3DVector("        Transform Link Rotation: ", lMatrix.GetR());
			//Display3DVector("        Transform Link Scaling: ", lMatrix.GetS());

			//if (lCluster->GetAssociateModel() != NULL)
			//{
			//	lMatrix = lCluster->GetTransformAssociateModelMatrix(lMatrix);
			//	DisplayString("        Associate Model: ", (char *)lCluster->GetAssociateModel()->GetName());
			//	Display3DVector("        Associate Model Translation: ", lMatrix.GetT());
			//	Display3DVector("        Associate Model Rotation: ", lMatrix.GetR());
			//	Display3DVector("        Associate Model Scaling: ", lMatrix.GetS());
			//}

			//DisplayString("");
		}
	}
}




bool fbxLoader::InitializeSdkObjects(FbxManager*& pManager, FbxScene*& pScene)
{
	//The first thing to do is to create the FBX Manager which is the object allocator for almost all the classes in the SDK
	pManager = FbxManager::Create();

	//Create an IOSettings object. This object holds all import/export settings.
	FbxIOSettings* ios = FbxIOSettings::Create(pManager, IOSROOT);
	pManager->SetIOSettings(ios);

	//Load plugins from the executable directory (optional)
	FbxString lPath = FbxGetApplicationDirectory();
	pManager->LoadPluginsDirectory(lPath.Buffer());

	//Create an FBX scene. This object holds most objects imported/exported from/to files.
	pScene = FbxScene::Create(pManager, "My Scene");
	if (!pScene)
	{
		return false;
	}
	return true;
}


bool fbxLoader::LoadScene(FbxManager* pManager, FbxDocument* pScene, const char* pFilename)
{
	int lFileMajor, lFileMinor, lFileRevision;
	int lSDKMajor, lSDKMinor, lSDKRevision;
	//int lFileFormat = -1;
	int i, lAnimStackCount;
	bool lStatus;
	char lPassword[1024];

	// Get the file version number generate by the FBX SDK.
	FbxManager::GetFileFormatVersion(lSDKMajor, lSDKMinor, lSDKRevision);

	// Create an importer.
	FbxImporter* lImporter = FbxImporter::Create(pManager, "");

	// Initialize the importer by providing a filename.
	const bool lImportStatus = lImporter->Initialize(pFilename, -1, pManager->GetIOSettings());
	lImporter->GetFileVersion(lFileMajor, lFileMinor, lFileRevision);

	if (!lImportStatus)
	{
		return false;
	}

	
	if (lImporter->IsFBX())
	{
		// Set the import states. By default, the import states are always set to 
		// true. The code below shows how to change these states.
		IOS_REF.SetBoolProp(IMP_FBX_MATERIAL, true);
		IOS_REF.SetBoolProp(IMP_FBX_TEXTURE, true);
		IOS_REF.SetBoolProp(IMP_FBX_LINK, true);
		IOS_REF.SetBoolProp(IMP_FBX_SHAPE, true);
		IOS_REF.SetBoolProp(IMP_FBX_GOBO, true);
		IOS_REF.SetBoolProp(IMP_FBX_ANIMATION, true);
		IOS_REF.SetBoolProp(IMP_FBX_GLOBAL_SETTINGS, true);
	}

	// Import the scene.
	lStatus = lImporter->Import(pScene);


	// Destroy the importer.
	lImporter->Destroy();

	return lStatus;
}
std::vector<std::vector<float>> fbxLoader::getWeights() { return weights; };
std::vector<int> fbxLoader::getOrder() { return controlPoint; };
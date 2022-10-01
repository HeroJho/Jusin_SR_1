#include "stdafx.h"
#include "..\Public\DataManager.h"

#include "BinModel.h"
#include "HierarchyNode.h"

IMPLEMENT_SINGLETON(CDataManager)

CDataManager::CDataManager()
{
}

HRESULT CDataManager::Init()
{


	return S_OK;
}



HRESULT CDataManager::ReadSceneData(const char * pFileName, DATA_HEROSCENE* ReadScene)
{

	ZeroMemory(ReadScene, sizeof(DATA_HEROSCENE));
	std::ifstream ifs("Test.txt", ios::in | ios::binary);

	if (!ifs)
		return E_FAIL;

	// Node
	int iNodeCount = 0;
	ifs.read((char*)&iNodeCount, sizeof(int));
	ReadScene->iNodeCount = iNodeCount;
	ReadScene->pHeroNodes = new DATA_HERONODE[iNodeCount];
	for (int i = 0; i < iNodeCount; ++i)
	{
		DATA_HERONODE* pHeroNode = &ReadScene->pHeroNodes[i];
		ifs.read((char*)pHeroNode, sizeof(DATA_HERONODE));
	}

	// Material
	int iMaterialCount = 0;
	ifs.read((char*)&iMaterialCount, sizeof(int));
	ReadScene->iMaterialCount = iMaterialCount;
	ReadScene->pHeroMaterial = new DATA_HEROMATERIAL[iMaterialCount];
	for (int i = 0; i < iMaterialCount; ++i)
	{
		DATA_HEROMATERIAL* pHeroMarterial = &ReadScene->pHeroMaterial[i];
		ifs.read((char*)pHeroMarterial, sizeof(DATA_HEROMATERIAL));
	}

	// MashNode
	int iMashCount = 0;
	ifs.read((char*)&iMashCount, sizeof(int));
	ReadScene->iMeshCount = iMashCount;
	ReadScene->pHeroMesh = new DATA_HEROMETH[iMashCount];
	for (int i = 0; i < iMashCount; ++i)
	{
		DATA_HEROMETH* pHeroMash = &ReadScene->pHeroMesh[i];

		ifs.read((char*)&pHeroMash->cName, sizeof(char)*MAX_PATH);
		ifs.read((char*)&pHeroMash->iMaterialIndex, sizeof(int));

		ifs.read((char*)&pHeroMash->NumVertices, sizeof(int));

		int iIsAnim = 0;
		ifs.read((char*)&iIsAnim, sizeof(int));
		if (iIsAnim)
		{
			pHeroMash->pAnimVertices = new VTXANIMMODEL[pHeroMash->NumVertices];
			for (int j = 0; j < pHeroMash->NumVertices; ++j)
			{
				VTXANIMMODEL* VtxAniModel = &pHeroMash->pAnimVertices[j];
				ifs.read((char*)VtxAniModel, sizeof(VTXANIMMODEL));
			}
		}
		else
		{
			pHeroMash->pNonAnimVertices = new VTXMODEL[pHeroMash->NumVertices];
			for (int j = 0; j < pHeroMash->NumVertices; ++j)
			{
				VTXMODEL* VtxNonAniModel = &pHeroMash->pNonAnimVertices[j];
				ifs.read((char*)VtxNonAniModel, sizeof(VTXMODEL));
			}
		}

		pHeroMash->iNumPrimitives = 0;
		ifs.read((char*)&pHeroMash->iNumPrimitives, sizeof(int));
		pHeroMash->pIndices = new FACEINDICES32[pHeroMash->iNumPrimitives];
		for (int j = 0; j < pHeroMash->iNumPrimitives; ++j)
		{
			FACEINDICES32* Indices32 = &pHeroMash->pIndices[j];
			ifs.read((char*)Indices32, sizeof(FACEINDICES32));
		}


		pHeroMash->iNumBones = 0;
		ifs.read((char*)&pHeroMash->iNumBones, sizeof(int));
		pHeroMash->pBones = new DATA_HEROBONE[pHeroMash->iNumBones];
		for (int j = 0; j < pHeroMash->iNumBones; ++j)
		{
			DATA_HEROBONE* bon = &pHeroMash->pBones[j];
			ifs.read((char*)bon, sizeof(DATA_HEROBONE));
		}
	}


	// Animation
	int iNumAnimations = 0;
	ifs.read((char*)&iNumAnimations, sizeof(int));
	ReadScene->iNumAnimations = iNumAnimations;
	ReadScene->pHeroAnim = new DATA_HEROANIM[iNumAnimations];

	for (int i = 0; i < iNumAnimations; ++i)
	{
		ifs.read((char*)&ReadScene->pHeroAnim[i].iNumChannels, sizeof(int));
		ifs.read((char*)&ReadScene->pHeroAnim[i].fDuration, sizeof(float));
		ifs.read((char*)&ReadScene->pHeroAnim[i].fTickPerSecond, sizeof(float));

		ReadScene->pHeroAnim[i].pHeroChannel = new DATA_HEROCHANNEL[ReadScene->pHeroAnim[i].iNumChannels];
		for (int j = 0; j < ReadScene->pHeroAnim[i].iNumChannels; ++j)
		{
			DATA_HEROCHANNEL* pChannel = &ReadScene->pHeroAnim[i].pHeroChannel[j];
			ifs.read((char*)&pChannel->szName, sizeof(char)*MAX_PATH);
			ifs.read((char*)&pChannel->iNumKeyFrames, sizeof(int));

			pChannel->pKeyFrames = new KEYFRAME[pChannel->iNumKeyFrames];
			for (int k = 0; k < pChannel->iNumKeyFrames; ++k)
			{
				KEYFRAME* pKeyFrame = &pChannel->pKeyFrames[k];
				ifs.read((char*)pKeyFrame, sizeof(KEYFRAME));
			}
		}
	}


	ifs.close();

}





void CDataManager::Free()
{

}

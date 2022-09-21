#include "..\Public\Model.h"
#include "MeshContainer.h"
#include "Texture.h"
#include "HierarchyNode.h"

CModel::CModel(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent(pDevice, pContext)
{
}

CModel::CModel(const CModel & rhs)
	: CComponent(rhs)
	, m_iNumMeshes(rhs.m_iNumMeshes)
	, m_iNumMaterials(rhs.m_iNumMaterials)
	, m_Meshes(rhs.m_Meshes)
	, m_Materials(rhs.m_Materials)
{
	for (auto& pMeshContainer : m_Meshes)
		Safe_AddRef(pMeshContainer);


	for (auto& Material : m_Materials)
	{
		for (_uint i = 0; i < AI_TEXTURE_TYPE_MAX; ++i)
			Safe_AddRef(Material.pTexture[i]);
	}
}

HRESULT CModel::Initialize_Prototype(TYPE eType, const char * pModelFilePath, const char * pModelFileName, _fmatrix PivotMatrix)
{
	XMStoreFloat4x4(&m_PivotMatrix, PivotMatrix);

	char		szFullPath[MAX_PATH] = "";

	strcpy_s(szFullPath, pModelFilePath);
	strcat_s(szFullPath, pModelFileName);

	_uint		iFlag = 0;

	if (TYPE_NONANIM == eType)
		iFlag |= aiProcess_PreTransformVertices | aiProcess_ConvertToLeftHanded | aiProcess_CalcTangentSpace;
	else
		iFlag |= aiProcess_ConvertToLeftHanded | aiProcess_CalcTangentSpace;

	m_pAIScene = m_Importer.ReadFile(szFullPath, iFlag);

	if (nullptr == m_pAIScene)
		return E_FAIL;

	/* 모델을 구성하는 메시들을 만든다. */
	if (FAILED(Ready_MeshContainers(PivotMatrix)))
		return E_FAIL;

	if (FAILED(Ready_Materials(pModelFilePath)))
		return E_FAIL;

	Ready_HierarchyNodes(m_pAIScene->mRootNode, 0);

	sort(m_HierarchyNodes.begin(), m_HierarchyNodes.end(), [](CHierarchyNode* pSour, CHierarchyNode* pDest) 
	{	
		return pSour->Get_Depth() < pDest->Get_Depth();
	});



	return S_OK;
}

HRESULT CModel::Initialize(void * pArg)
{
	return S_OK;
}

HRESULT CModel::SetUp_OnShader(CShader * pShader, _uint iMaterialIndex, aiTextureType eTextureType, const char * pConstantName)
{
	if (iMaterialIndex >= m_iNumMaterials)
		return E_FAIL;

	return m_Materials[iMaterialIndex].pTexture[eTextureType]->Set_SRV(pShader, pConstantName);	
}

HRESULT CModel::Render(_uint iMeshIndex)
{

		//_uint		iMaterialIndex = pMeshContainer->Get_MaterialIndex();

		//m_Materials[iMaterialIndex].pTexture[aiTextureType_DIFFUSE]->Set_SRV()

	m_Meshes[iMeshIndex]->Render();


	return S_OK;
}

HRESULT CModel::Ready_MeshContainers(_fmatrix PivotMatrix)
{
	m_iNumMeshes = m_pAIScene->mNumMeshes;

	for (_uint i = 0; i < m_iNumMeshes; ++i)
	{
		CMeshContainer*		pMeshContainer = CMeshContainer::Create(m_pDevice, m_pContext, m_pAIScene->mMeshes[i], PivotMatrix);
		if (nullptr == pMeshContainer)
			return E_FAIL;

		m_Meshes.push_back(pMeshContainer);
	}

	return S_OK;
}

HRESULT CModel::Ready_Materials(const char* pModelFilePath)
{
	if (nullptr == m_pAIScene)
		return E_FAIL;

	m_iNumMaterials = m_pAIScene->mNumMaterials;

	for (_uint i = 0; i < m_iNumMaterials; ++i)
	{
		MATERIALDESC		MaterialDesc;
		ZeroMemory(&MaterialDesc, sizeof(MATERIALDESC));

		aiMaterial*			pAIMaterial = m_pAIScene->mMaterials[i];

		for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; ++j)
		{			
			aiString		strPath;			

			if (FAILED(pAIMaterial->GetTexture(aiTextureType(j), 0, &strPath)))
				continue;		

			char			szFullPath[MAX_PATH] = "";
			char			szFileName[MAX_PATH] = "";
			char			szExt[MAX_PATH] = "";

			_splitpath_s(strPath.data, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);

			strcpy_s(szFullPath, pModelFilePath);
			strcat_s(szFullPath, szFileName);
			strcat_s(szFullPath, szExt);

			_tchar			szWideFullPath[MAX_PATH] = TEXT("");

			MultiByteToWideChar(CP_ACP, 0, szFullPath, strlen(szFullPath), szWideFullPath, MAX_PATH);


			MaterialDesc.pTexture[j] = CTexture::Create(m_pDevice, m_pContext, szWideFullPath);
			if (nullptr == MaterialDesc.pTexture[j])
				return E_FAIL;			
		}	

		m_Materials.push_back(MaterialDesc);
	}

	return S_OK;
}

HRESULT CModel::Ready_HierarchyNodes(aiNode* pNode, _uint iDepth)
{
	CHierarchyNode*		pHierarchyNode = CHierarchyNode::Create(pNode, iDepth++);

	if (nullptr == pHierarchyNode)
		return E_FAIL;

	m_HierarchyNodes.push_back(pHierarchyNode);

	for (_uint i = 0; i < pNode->mNumChildren; ++i)
	{
		Ready_HierarchyNodes(pNode->mChildren[i], iDepth);
	}	

	return S_OK;
}

HRESULT CModel::Ready_Animations()
{
	m_iNumAnimations = m_pAIScene->mNumAnimations;

	for (_uint i = 0; i < m_pAIScene->mNumAnimations; ++i)
	{
		aiAnimation*		pAIAnimation = m_pAIScene->mAnimations[i];
	}

	return S_OK;
}

CModel * CModel::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, TYPE eType, const char * pModelFilePath, const char * pModelFileName, _fmatrix PivotMatrix)
{
	CModel*			pInstance = new CModel(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eType, pModelFilePath, pModelFileName, PivotMatrix)))
	{
		MSG_BOX(TEXT("Failed To Created : CTexture"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CModel::Clone(void * pArg)
{
	CModel*			pInstance = new CModel(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CModel"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CModel::Free()
{
	__super::Free();

	for (auto& Material : m_Materials)
	{
		for (_uint i = 0; i < AI_TEXTURE_TYPE_MAX; ++i)
			Safe_Release(Material.pTexture[i]);
	}
	m_Materials.clear();

	for (auto& pMeshContainer : m_Meshes)
		Safe_Release(pMeshContainer);

	m_Meshes.clear();

	m_Importer.FreeScene();
}

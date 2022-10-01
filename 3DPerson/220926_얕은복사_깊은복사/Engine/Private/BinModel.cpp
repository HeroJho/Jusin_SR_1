#include "..\Public\BinModel.h"
#include "BinMeshContainer.h"
#include "Texture.h"
#include "BInHierarchyNode.h"
#include "BinAnimation.h"
#include "Shader.h"


CBinModel::CBinModel(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent(pDevice, pContext)
{
}

CBinModel::CBinModel(const CBinModel & rhs)
	: CComponent(rhs)
	, m_pAIScene(rhs.m_pAIScene)
	, m_iNumMeshes(rhs.m_iNumMeshes)
	, m_iNumMaterials(rhs.m_iNumMaterials)
	, m_Meshes(rhs.m_Meshes)
	, m_Materials(rhs.m_Materials)
	, m_eModelType(rhs.m_eModelType)
	/*, m_HierarchyNodes(rhs.m_HierarchyNodes)*/
	, m_Animations(rhs.m_Animations)
	, m_iCurrentAnimIndex(rhs.m_iCurrentAnimIndex)
	, m_PivotMatrix(rhs.m_PivotMatrix)
	, m_iNumAnimations(rhs.m_iNumAnimations)

{
	for (auto& pMeshContainer : m_Meshes)
		Safe_AddRef(pMeshContainer);


	for (auto& Material : m_Materials)
	{
		for (_uint i = 0; i < AI_TEXTURE_TYPE_MAX; ++i)
			Safe_AddRef(Material.pTexture[i]);
	}

	for (auto& pAnimation : m_Animations)
		Safe_AddRef(pAnimation);

}

CBinHierarchyNode * CBinModel::Get_HierarchyNode(const char * pNodeName)
{
	auto	iter = find_if(m_HierarchyNodes.begin(), m_HierarchyNodes.end(), [&](CBinHierarchyNode* pNode)
	{
		return !strcmp(pNodeName, pNode->Get_Name());
	});

	if (iter == m_HierarchyNodes.end())
		return nullptr;

	return *iter;
}

_uint CBinModel::Get_MaterialIndex(_uint iMeshIndex)
{
	return m_Meshes[iMeshIndex]->Get_MaterialIndex();
}

HRESULT CBinModel::Initialize_Prototype(DATA_HEROSCENE* pScene, TYPE eType, const char* pModelFilePath, const char * pModelFileName, _fmatrix PivotMatrix)
{
	XMStoreFloat4x4(&m_PivotMatrix, PivotMatrix);
	m_eModelType = eType;

	m_pAIScene = pScene;

	if (nullptr == m_pAIScene)
		return E_FAIL;



	/* 모델을 구성하는 메시들을 만든다. */
	if (FAILED(Ready_MeshContainers(PivotMatrix)))
		return E_FAIL;

	if (FAILED(Ready_Materials(pModelFilePath)))
		return E_FAIL;

	if (FAILED(Ready_Animations()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBinModel::Initialize(void * pArg)
{
	Ready_HierarchyNodes();

	sort(m_HierarchyNodes.begin(), m_HierarchyNodes.end(), [](CBinHierarchyNode* pSour, CBinHierarchyNode* pDest)
	{
		return pSour->Get_Depth() < pDest->Get_Depth();
	});

	for (int i = 0; i < m_pAIScene->iNodeCount; ++i)
	{
		for (int j = 0; j < m_pAIScene->iNodeCount; ++j)
		{
			if (nullptr != m_HierarchyNodes[i]->Get_Parent())
				break;

			m_HierarchyNodes[i]->Set_FindParent(m_HierarchyNodes[j]);
		}
	}


	if (TYPE_ANIM == m_eModelType)
	{
		_uint		iNumMeshes = 0;

		vector<CBinMeshContainer*>		MeshContainers;

		for (auto& pPrototype : m_Meshes)
		{
			CBinMeshContainer*		pMeshContainer = (CBinMeshContainer*)pPrototype->Clone();
			if (nullptr == pMeshContainer)
				return E_FAIL;

			MeshContainers.push_back(pMeshContainer);

			Safe_Release(pPrototype);
		}

		m_Meshes.clear();

		m_Meshes = MeshContainers;

		for (auto& pMeshContainer : m_Meshes)
		{
			if (nullptr != pMeshContainer)
				pMeshContainer->SetUp_HierarchyNodes(this, &m_pAIScene->pHeroMesh[iNumMeshes++]);
		}
	}

	vector<CBinAnimation*>		Animations;

	for (auto& pPrototype : m_Animations)
	{
		CBinAnimation*		pAnimation = pPrototype->Clone(this);
		if (nullptr == pAnimation)
			return E_FAIL;

		Animations.push_back(pAnimation);

		Safe_Release(pPrototype);
	}

	m_Animations.clear();

	m_Animations = Animations;

	return S_OK;
}

HRESULT CBinModel::SetUp_OnShader(CShader * pShader, _uint iMaterialIndex, aiTextureType eTextureType, const char * pConstantName)
{
	if (iMaterialIndex >= m_iNumMaterials)
		return E_FAIL;

	return m_Materials[iMaterialIndex].pTexture[eTextureType]->Set_SRV(pShader, pConstantName);
}

HRESULT CBinModel::Play_Animation(_float fTimeDelta)
{
	if (m_iCurrentAnimIndex >= m_iNumAnimations)
		return E_FAIL;

	/* 현재 재생하고자하는 애니메이션이 제어해야할 뼈들의 지역행렬을 갱신해낸다. */
	m_Animations[m_iCurrentAnimIndex]->Play_Animation(fTimeDelta);

	/* 지역행렬을 순차적으로(부모에서 자식으로) 누적하여 m_CombinedTransformation를 만든다.  */
	for (auto& pHierarchyNode : m_HierarchyNodes)
	{
		pHierarchyNode->Set_CombinedTransformation();
	}

	return S_OK;
}

HRESULT CBinModel::Render(CShader* pShader, _uint iMeshIndex)
{
	_float4x4		BoneMatrices[256];

	if (TYPE_ANIM == m_eModelType)
	{
		m_Meshes[iMeshIndex]->SetUp_BoneMatrices(BoneMatrices, XMLoadFloat4x4(&m_PivotMatrix));

		if (FAILED(pShader->Set_RawValue("g_BoneMatrices", BoneMatrices, sizeof(_float4x4) * 256)))
			return E_FAIL;
	}

	pShader->Begin(0);

	m_Meshes[iMeshIndex]->Render();

	return S_OK;
}

HRESULT CBinModel::Ready_MeshContainers(_fmatrix PivotMatrix)
{
	m_iNumMeshes = m_pAIScene->iMeshCount;

	for (_uint i = 0; i < m_iNumMeshes; ++i)
	{
		CBinMeshContainer*		pMeshContainer = CBinMeshContainer::Create(m_pDevice, m_pContext, m_eModelType, &m_pAIScene->pHeroMesh[i], this, PivotMatrix);
		if (nullptr == pMeshContainer)
			return E_FAIL;

		m_Meshes.push_back(pMeshContainer);
	}

	return S_OK;
}

HRESULT CBinModel::Ready_Materials(const char* pModelFilePath)
{
	if (nullptr == m_pAIScene)
		return E_FAIL;

	m_iNumMaterials = m_pAIScene->iMaterialCount;

	for (_uint i = 0; i < m_iNumMaterials; ++i)
	{
		MATERIALDESC		MaterialDesc;
		ZeroMemory(&MaterialDesc, sizeof(MATERIALDESC));

		DATA_HEROMATERIAL			pAIMaterial = m_pAIScene->pHeroMaterial[i];

		for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; ++j)
		{
			if (!strcmp(pAIMaterial.cNames[j], ""))
				continue;


			char			szFullPath[MAX_PATH] = "";
			char			szExt[MAX_PATH] = ".dds";

			strcpy_s(szFullPath, pModelFilePath);
			strcat_s(szFullPath, pAIMaterial.cNames[j]);
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

HRESULT CBinModel::Ready_HierarchyNodes()
{
	for (int i = 0; i < m_pAIScene->iNodeCount; ++i)
	{
		DATA_HERONODE Node = m_pAIScene->pHeroNodes[i];

		CBinHierarchyNode*		pHierarchyNode = CBinHierarchyNode::Create(&Node);
		if (nullptr == pHierarchyNode)
			return E_FAIL;

		m_HierarchyNodes.push_back(pHierarchyNode);
	}

	return S_OK;
}

HRESULT CBinModel::Ready_Animations()
{
	m_iNumAnimations = m_pAIScene->iNumAnimations;

	for (_uint i = 0; i < m_pAIScene->iNumAnimations; ++i)
	{
		DATA_HEROANIM*		pAIAnimation = &m_pAIScene->pHeroAnim[i];

		CBinAnimation*			pAnimation = CBinAnimation::Create(pAIAnimation);
		if (nullptr == pAnimation)
			return E_FAIL;

		m_Animations.push_back(pAnimation);
	}
	return S_OK;
}

CBinModel * CBinModel::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, DATA_HEROSCENE* pScene, TYPE eType, const char * pModelFilePath, const char * pModelFileName, _fmatrix PivotMatrix)
{
	CBinModel*			pInstance = new CBinModel(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pScene, eType, pModelFilePath, pModelFileName, PivotMatrix)))
	{
		MSG_BOX(TEXT("Failed To Created : CTexture"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CBinModel::Clone(void * pArg)
{
	CBinModel*			pInstance = new CBinModel(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CBinModel"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBinModel::Free()
{
	__super::Free();

	for (auto& pHierarchyNode : m_HierarchyNodes)
		Safe_Release(pHierarchyNode);

	m_HierarchyNodes.clear();

	for (auto& Material : m_Materials)
	{
		for (_uint i = 0; i < AI_TEXTURE_TYPE_MAX; ++i)
			Safe_Release(Material.pTexture[i]);
	}
	m_Materials.clear();

	for (auto& pMeshContainer : m_Meshes)
		Safe_Release(pMeshContainer);

	m_Meshes.clear();

	for (auto& pAnimation : m_Animations)
		Safe_Release(pAnimation);

	m_Animations.clear();

	m_Importer.FreeScene();
}

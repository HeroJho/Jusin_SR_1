#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CBinModel final : public CComponent
{
public:
	enum TYPE { TYPE_NONANIM, TYPE_ANIM, TYPE_END };
private:
	CBinModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBinModel(const CBinModel& rhs);
	virtual ~CBinModel() = default;

public:
	class CBinHierarchyNode* Get_HierarchyNode(const char* pNodeName);

	_uint Get_NumMeshes() const {
		return m_iNumMeshes;
	}

	_uint Get_MaterialIndex(_uint iMeshIndex);

	void Set_AnimIndex(_uint iAnimIndex) {
		m_iCurrentAnimIndex = iAnimIndex;
	}


public:
	virtual HRESULT Initialize_Prototype(DATA_HEROSCENE* pScene, TYPE eType, const char* pModelFilePath, const char* pModelFileName, _fmatrix PivotMatrix);
	virtual HRESULT Initialize(void* pArg);


public:
	HRESULT SetUp_OnShader(class CShader* pShader, _uint iMaterialIndex, aiTextureType eTextureType, const char* pConstantName);
	HRESULT Play_Animation(_float fTimeDelta);
	HRESULT Render(class CShader* pShader, _uint iMeshIndex);

private:
	DATA_HEROSCENE*				m_pAIScene = nullptr;
	Assimp::Importer			m_Importer;

	_float4x4					m_PivotMatrix;
	TYPE						m_eModelType = TYPE_END;

private:
	_uint									m_iNumMeshes = 0;
	vector<class CBinMeshContainer*>			m_Meshes;
	typedef vector<class CBinMeshContainer*>	MESHES;

private:
	_uint									m_iNumMaterials = 0;
	vector<MATERIALDESC>					m_Materials;

private:
	vector<class CBinHierarchyNode*>			m_HierarchyNodes;

private:
	_uint								m_iCurrentAnimIndex = 0;
	_uint								m_iNumAnimations = 0;
	vector<class CBinAnimation*>			m_Animations;


private:
	HRESULT Ready_MeshContainers(_fmatrix PivotMatrix);
	HRESULT Ready_Materials(const char* pModelFilePath);
	HRESULT Ready_HierarchyNodes();
	HRESULT Ready_Animations();

private:



public:
	static CBinModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, DATA_HEROSCENE* pScene, TYPE eType, const char* pModelFilePath, const char* pModelFileName, _fmatrix PivotMatrix = XMMatrixIdentity());
	virtual CComponent* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END
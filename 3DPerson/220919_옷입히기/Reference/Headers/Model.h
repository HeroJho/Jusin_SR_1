#pragma once

#include "Component.h"
#include "MeshContainer.h"

BEGIN(Engine)

class ENGINE_DLL CModel final : public CComponent
{
public:
	enum TYPE { TYPE_NONANIM, TYPE_ANIM, TYPE_END };
private:
	CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CModel(const CModel& rhs);
	virtual ~CModel() = default;

public:
	_uint Get_NumMeshes() const {
		return m_iNumMeshes;
	}

	_uint Get_MaterialIndex(_uint iMeshIndex) {
		return m_Meshes[iMeshIndex]->Get_MaterialIndex();
	}
public:
	virtual HRESULT Initialize_Prototype(TYPE eType, const char* pModelFilePath, const char* pModelFileName, _fmatrix PivotMatrix);
	virtual HRESULT Initialize(void* pArg);

public:
	HRESULT SetUp_OnShader(class CShader* pShader, _uint iMaterialIndex, aiTextureType eTextureType, const char* pConstantName);
	HRESULT Render(_uint iMeshIndex);

private:
	const aiScene*				m_pAIScene = nullptr;
	Assimp::Importer			m_Importer;

	_float4x4					m_PivotMatrix;

private:
	_uint									m_iNumMeshes = 0;
	vector<class CMeshContainer*>			m_Meshes;
	typedef vector<class CMeshContainer*>	MESHES;

private:
	_uint									m_iNumMaterials = 0;
	vector<MATERIALDESC>					m_Materials;

private:
	HRESULT Ready_MeshContainers(_fmatrix PivotMatrix);
	HRESULT Ready_Materials(const char* pModelFilePath);

public:
	static CModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, TYPE eType, const char* pModelFilePath, const char* pModelFileName, _fmatrix PivotMatrix = XMMatrixIdentity());
	virtual CComponent* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END
#pragma once


#include "VIBuffer.h"
#include "BinModel.h"


/* ���� �����ϴ� �ϳ��� �޽�. */
/* �� �޽ø� �����ϴ� ����, �ε��� ���۸� �����Ѵ�. */

BEGIN(Engine)

class CBinMeshContainer final : public CVIBuffer
{
private:
	CBinMeshContainer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBinMeshContainer(const CBinMeshContainer& rhs);
	virtual ~CBinMeshContainer() = default;

public:
	_uint Get_MaterialIndex() const {
		return m_iMaterialIndex;
	}

public:
	virtual HRESULT Initialize_Prototype(CBinModel::TYPE eModelType, DATA_HEROMETH* pAIMesh, class CBinModel* pModel, _fmatrix PivotMatrix);
	virtual HRESULT Initialize(void* pArg);

public:
	HRESULT SetUp_HierarchyNodes(class CBinModel* pModel, DATA_HEROMETH* pAIMesh);
	void SetUp_BoneMatrices(_float4x4* pBoneMatrices, _fmatrix PivotMatrix);


private:
	char				m_szName[MAX_PATH] = "";
	_uint				m_iMaterialIndex = 0;
private:
	/* �� �޽ÿ� ���⤷�� �ִ� ���� ����. */
	_uint							m_iNumBones = 0;

	/* �� �޽ÿ� ������ �ִ� ������ ��Ƴ�����. */
	/* why ? : �޽� ���� �������� �� �� �޽ÿ� ���⤷�� �ִ� ������ ����� ��Ƽ� ��{���𷯤� ���ä�������. */
	vector<class CBinHierarchyNode*>	m_Bones;

private:
	HRESULT Ready_Vertices(DATA_HEROMETH* pAIMesh, _fmatrix PivotMatrix);
	HRESULT Ready_AnimVertices(DATA_HEROMETH* pAIMesh, CBinModel* pModel);

public:
	static CBinMeshContainer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CBinModel::TYPE eModelType, DATA_HEROMETH* pAIMesh, class CBinModel* pModel, _fmatrix PivotMatrix);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END
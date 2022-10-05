#include "..\Public\AABB.h"
#include "PipeLine.h"

CAABB::CAABB(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CCollider(pDevice, pContext)
{

}

CAABB::CAABB(const CAABB & rhs)
	: CCollider(rhs)
	, m_pAABB(rhs.m_pAABB)
{

}

HRESULT CAABB::Initialize_Prototype(CCollider::TYPE eColliderType)
{
	if (FAILED(__super::Initialize_Prototype(eColliderType)))
		return E_FAIL;

	return S_OK;
}

HRESULT CAABB::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	/* �����ɶ� �����ϰ����ϴ� ���·� �����Ѵ�. */
	m_pOriginal_AABB = new BoundingBox(m_ColliderDesc.vCenter, _float3(m_ColliderDesc.vSize.x * 0.5f,
		m_ColliderDesc.vSize.y * 0.5f, 
		m_ColliderDesc.vSize.z * 0.5f));

	m_pAABB = new BoundingBox(*m_pOriginal_AABB);

	return S_OK;
}

void CAABB::Update(_fmatrix TransformMatrix)
{
	m_pOriginal_AABB->Transform(*m_pAABB, Remove_Rotation(TransformMatrix));
}

#ifdef _DEBUG

HRESULT CAABB::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	m_pBatch->Begin();

	DX::Draw(m_pBatch, *m_pAABB);

	m_pBatch->End();

	return S_OK;
}

#endif // _DEBUG

CAABB * CAABB::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, CCollider::TYPE eColliderType)
{
	CAABB*			pInstance = new CAABB(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eColliderType)))
	{
		MSG_BOX(TEXT("Failed To Created : CAABB"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CAABB::Clone(void * pArg)
{
	CAABB*			pInstance = new CAABB(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CAABB"));
		Safe_Release(pInstance);
	}
	return pInstance;
}


void CAABB::Free()
{
	__super::Free();

	Safe_Delete(m_pOriginal_AABB);
	Safe_Delete(m_pAABB);
}

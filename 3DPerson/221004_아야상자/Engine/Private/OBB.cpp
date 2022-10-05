#include "..\Public\OBB.h"

COBB::COBB(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CCollider(pDevice, pContext)
{

}

COBB::COBB(const COBB & rhs)
	: CCollider(rhs)
{

}

HRESULT COBB::Initialize_Prototype(CCollider::TYPE eColliderType)
{
	if (FAILED(__super::Initialize_Prototype(eColliderType)))
		return E_FAIL;

	return S_OK;
}

HRESULT COBB::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void COBB::Update(_fmatrix TransformMatrix)
{
}

HRESULT COBB::Render()
{
	return S_OK;
}

COBB * COBB::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, CCollider::TYPE eColliderType)
{
	COBB*			pInstance = new COBB(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eColliderType)))
	{
		MSG_BOX(TEXT("Failed To Created : COBB"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * COBB::Clone(void * pArg)
{
	COBB*			pInstance = new COBB(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : COBB"));
		Safe_Release(pInstance);
	}
	return pInstance;
}


void COBB::Free()
{
	__super::Free();

}

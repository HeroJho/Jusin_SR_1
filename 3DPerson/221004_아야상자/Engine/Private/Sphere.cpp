#include "..\Public\Sphere.h"

CSphere::CSphere(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CCollider(pDevice, pContext)
{

}

CSphere::CSphere(const CSphere & rhs)
	: CCollider(rhs)
{

}

HRESULT CSphere::Initialize_Prototype(CCollider::TYPE eColliderType)
{
	if (FAILED(__super::Initialize_Prototype(eColliderType)))
		return E_FAIL;

	return S_OK;
}

HRESULT CSphere::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CSphere::Update(_fmatrix TransformMatrix)
{
}

HRESULT CSphere::Render()
{
	return S_OK;
}

CSphere * CSphere::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, CCollider::TYPE eColliderType)
{
	CSphere*			pInstance = new CSphere(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eColliderType)))
	{
		MSG_BOX(TEXT("Failed To Created : CSphere"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CSphere::Clone(void * pArg)
{
	CSphere*			pInstance = new CSphere(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CSphere"));
		Safe_Release(pInstance);
	}
	return pInstance;
}


void CSphere::Free()
{
	__super::Free();

}

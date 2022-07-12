#include "..\Public\Transform.h"

CTransform::CTransform(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{

}

CTransform::CTransform(const CTransform & rhs)
	: CComponent(rhs)
{

}

HRESULT CTransform::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTransform::Initialize(void * pArg)
{
	D3DXMatrixIdentity(&m_WorldMatrix);

	if (nullptr != pArg)
		memcpy(&m_TransformDesc, pArg, sizeof(TRANSFORMDESC));

	return S_OK;
}

HRESULT CTransform::Bind_WorldMatrix()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_WorldMatrix);

	return S_OK;
}

void CTransform::Go_Straight(_float fTimeDelta)
{
	_float3		vPosition = Get_State(STATE_POSITION);
	_float3		vLook = Get_State(STATE_LOOK);

	vPosition += *D3DXVec3Normalize(&vLook, &vLook) * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	Set_State(CTransform::STATE_POSITION, vPosition);
}

void CTransform::Go_Backward(_float fTimeDelta)
{
	_float3		vPosition = Get_State(STATE_POSITION);
	_float3		vLook = Get_State(STATE_LOOK);

	vPosition -= *D3DXVec3Normalize(&vLook, &vLook) * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	Set_State(CTransform::STATE_POSITION, vPosition);
}

CTransform * CTransform::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTransform*			pInstance = new CTransform(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CTransform"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CTransform::Clone(void * pArg)
{
	CTransform*			pInstance = new CTransform(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CTransform"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTransform::Free()
{
	__super::Free();

}

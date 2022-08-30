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
	vPosition.y = 1.f;

	Set_State(CTransform::STATE_POSITION, vPosition);
}

void CTransform::Go_Backward(_float fTimeDelta)
{
	_float3		vPosition = Get_State(STATE_POSITION);
	_float3		vLook = Get_State(STATE_LOOK);

	vPosition -= *D3DXVec3Normalize(&vLook, &vLook) * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	Set_State(CTransform::STATE_POSITION, vPosition);
}

void CTransform::Rot_Right(_float fTimeDelta)
{
	// 1. Look과 Right을 가져온다.
	_float3		vLook = Get_State(STATE_LOOK);
	_float3		vRight = Get_State(STATE_RIGHT);


	// 2. Y축 회전을 Look과 Right에 적용한다.
	_float4x4	mRot;
	_float fRad = m_TransformDesc.fRotationPerSec * fTimeDelta * 1;
	D3DXMatrixRotationY(&mRot, fRad);

	D3DXVec3TransformCoord(&vLook, &vLook, &mRot);
	D3DXVec3TransformCoord(&vRight, &vRight, &mRot);


	// 3. 월드 행렬에 다시 넣는다.
	Set_State(CTransform::STATE_LOOK, vLook);
	Set_State(CTransform::STATE_RIGHT, vRight);

}

void CTransform::Rot_Left(_float fTimeDelta)
{
	// 1. Look과 Right을 가져온다.
	_float3		vLook = Get_State(STATE_LOOK);
	_float3		vRight = Get_State(STATE_RIGHT);


	// 2. Y축 회전을 Look과 Right에 적용한다.
	_float4x4	mRot;
	_float fRad = m_TransformDesc.fRotationPerSec * fTimeDelta * -1;

	D3DXMatrixRotationY(&mRot, fRad);

	D3DXVec3TransformCoord(&vLook, &vLook, &mRot);
	D3DXVec3TransformCoord(&vRight, &vRight, &mRot);


	// 3. 월드 행렬에 다시 넣는다.
	Set_State(CTransform::STATE_LOOK, vLook);
	Set_State(CTransform::STATE_RIGHT, vRight);

}

void CTransform::Go_Pos(_float fTimeDelta, _float3 vTargetPos)
{
	// 1. Look을 vTargetPos로 바라보게한다.
	_float3	vPos = Get_State(STATE_POSITION);
	_float3 vTargetVector = vTargetPos - vPos;
	D3DXVec3Normalize(&vTargetVector , &vTargetVector);

	_float3 vLook = vTargetVector, vRight, vUp = {0.f, 1.f, 0.f};
	D3DXVec3Cross(&vRight, &vUp, &vLook);
	D3DXVec3Cross(&vUp, &vLook, &vRight);

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);

	// 2. 전진한다.
	Go_Straight(fTimeDelta);

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

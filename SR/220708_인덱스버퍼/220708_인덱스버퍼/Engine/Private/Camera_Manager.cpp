#include "..\Public\Camera_Manager.h"
#include "Graphic_Device.h"
#include "GameObject.h"

IMPLEMENT_SINGLETON(CCamera_Manager)

CCamera_Manager::CCamera_Manager()
{
	ZeroMemory(m_pPos, sizeof(_float3));
}

HRESULT CCamera_Manager::InitCam(_uint iWinSizeX, _uint iWinSizeY)
{
	m_pGraphic_Device = CGraphic_Device::Get_Instance()->Get_Device();
	Safe_AddRef(m_pGraphic_Device);

	m_iWinSizeX = iWinSizeX;
	m_iWinSizeY = iWinSizeY;

	return S_OK;
}

void CCamera_Manager::Tick(_float fTimeDelta)
{

	//if (GetKeyState(VK_LEFT) & 0x8000)
	//{
	//	m_fX -= 0.1f;
	//}
	//if (GetKeyState(VK_RIGHT) & 0x8000)
	//{
	//	m_fX += 0.1f;
	//}
	//if (GetKeyState(VK_UP) & 0x8000)
	//{
	//	m_fZ += 0.1f;
	//}
	//if (GetKeyState(VK_DOWN) & 0x8000)
	//{
	//	m_fZ -= 0.1f;
	//}

	if (GetKeyState('W') & 0x8000)
	{
		m_fA += 0.1f;
	}
	if (GetKeyState('S') & 0x8000)
	{
		m_fA -= 0.1f;
	}

	if (GetKeyState('Q') & 0x8000)
	{
		m_bCamMode = false;
	}
	if (GetKeyState('E') & 0x8000)
	{
		m_bCamMode = true;
	}

}

void CCamera_Manager::LateTick(_float fTimeDelta)
{
	
}

void CCamera_Manager::SetCamMatrix()
{
	if (nullptr == m_pTarget)
		return;

	m_pPos = m_pTarget->GetPos() - m_pTarget->GetDir() * 3.f;
	_float3 vLook = m_pTarget->GetPos() + m_pTarget->GetDir() * 2.f;

	_float4x4	ViewMatrix, ProjMatrix;
	if(m_bCamMode)
		D3DXMatrixLookAtLH(&ViewMatrix, &_float3(m_pTarget->GetPos().x, 1.f, m_pTarget->GetPos().z), &_float3(vLook.x, 1.f, vLook.z), &_float3(0.f, 1.f, 0.f));
	else
		D3DXMatrixLookAtLH(&ViewMatrix, &_float3(m_pPos.x, 2.f, m_pPos.z), &vLook, &_float3(0.f, 1.f, 0.f));
	
	D3DXMatrixPerspectiveFovLH(&ProjMatrix, D3DXToRadian(m_fA), m_iWinSizeX / (_float)m_iWinSizeY, 0.2f, 300.f);

	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &ViewMatrix);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &ProjMatrix);
}

void CCamera_Manager::SetTarget(CGameObject * pTarget)
{
	if (nullptr == pTarget)
		return;

	m_pTarget = pTarget;
	Safe_AddRef(m_pTarget);
}

void CCamera_Manager::Free()
{
	Safe_Release(m_pTarget);
	Safe_Release(m_pGraphic_Device);
}

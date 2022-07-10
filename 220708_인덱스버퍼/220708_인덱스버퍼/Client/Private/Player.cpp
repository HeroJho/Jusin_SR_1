#include "stdafx.h"
#include "..\Public\Player.h"

#include "GameInstance.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CPlayer::CPlayer(const CPlayer & rhs)
	: CGameObject(rhs)
{
}

HRESULT CPlayer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlayer::Initialize(void * pArg)
{
	if (nullptr != pArg)
	{
		// memcpy(&m_BackDesc, pArg, sizeof(BACKDESC));
		m_BackDesc = *(BACKDESC*)pArg;
	}

	/* 내일 이거 수정해라. */

	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	m_pRendererCom = (CRenderer*)pGameInstance->Clone_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"));
	if (nullptr == m_pRendererCom)
		return E_FAIL;

	m_pVIBufferCom = (CVIBuffer_Rect*)pGameInstance->Clone_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"));
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	ZeroMemory(m_vPos, sizeof(_float3));
	m_vDir = { 0.f, 0.f, 1.f };

	Safe_Release(pGameInstance);

	return S_OK;
}

void CPlayer::Tick(_float fTimeDelta)
{
	if (GetKeyState('J') & 0x8000)
	{
		m_fDegree -= 0.1f;
	}
	if (GetKeyState('L') & 0x8000)
	{
		m_fDegree += 0.1f;
	}
	if (GetKeyState('I') & 0x8000)
	{
		m_vPos += 0.01f * m_vDir;
	}
	if (GetKeyState('K') & 0x8000)
	{
		m_vPos -= 0.01f * m_vDir;
	}

}

void CPlayer::LateTick(_float fTimeDelta)
{
	

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CPlayer::Render()
{
	m_vDir = { 0.f, 0.f, 1.f };

	_float4x4	matTotal, matScale, matRotY, matTrans;
	D3DXMatrixIdentity(&matTotal);
	D3DXMatrixIdentity(&matScale);
	D3DXMatrixIdentity(&matRotY);
	D3DXMatrixIdentity(&matTrans);

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
	D3DXMatrixRotationY(&matRotY, D3DXToRadian(m_fDegree));
	D3DXMatrixTranslation(&matTrans, m_vPos.x, m_vPos.y, m_vPos.z);

	matTotal = matScale * matRotY * matTrans;

	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &matTotal);



	D3DXVec3TransformNormal(&m_vDir, &m_vDir, &matTotal);

	// m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	m_pVIBufferCom->Render();

	return S_OK;
}

CPlayer * CPlayer::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPlayer*		pInstance = new CPlayer(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CPlayer"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CPlayer::Clone(void* pArg)
{
	CPlayer*		pInstance = new CPlayer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Created : CPlayer"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}


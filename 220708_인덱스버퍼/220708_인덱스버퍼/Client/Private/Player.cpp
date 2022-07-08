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

	Safe_Release(pGameInstance);

	return S_OK;
}

void CPlayer::Tick(_float fTimeDelta)
{
	int a = 10;
}

void CPlayer::LateTick(_float fTimeDelta)
{
	

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CPlayer::Render()
{
	_float4x4	Matrix;
	D3DXMatrixIdentity(&Matrix);


	m_pGraphic_Device->SetTransform(D3DTS_WORLD, D3DXMatrixTranslation(&Matrix, 5.f, 0.f, 10.f));

	_float4x4	ViewMatrix, ProjMatrix;
	D3DXMatrixLookAtLH(&ViewMatrix, &_float3(0.f, 10.f, -15.0f), &_float3(0.f, 0.f, 0.f), &_float3(0.f, 1.f, 0.f));
	D3DXMatrixPerspectiveFovLH(&ProjMatrix, D3DXToRadian(60.0f), g_iWinSizeX / (_float)g_iWinSizeY, 0.2f, 300.f);

	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &ViewMatrix);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &ProjMatrix);

	

	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

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


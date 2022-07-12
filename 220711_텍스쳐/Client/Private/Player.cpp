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

	m_pTextureCom = (CTexture*)pGameInstance->Clone_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Player"));
	if (nullptr == m_pTextureCom)
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


	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &Matrix);

	_float4x4	ViewMatrix, ProjMatrix;
	D3DXMatrixLookAtLH(&ViewMatrix, &_float3(0.f, 0.5f, -1.0f), &_float3(0.f, 0.f, 0.f), &_float3(0.f, 1.f, 0.f));
	D3DXMatrixPerspectiveFovLH(&ProjMatrix, D3DXToRadian(60.0f), g_iWinSizeX / (_float)g_iWinSizeY, 0.2f, 300.f);

	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &ViewMatrix);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &ProjMatrix);

	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;

	if (FAILED(Set_RenderState()))
		return E_FAIL;

	m_pVIBufferCom->Render();

	if (FAILED(Reset_RenderState()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer::Set_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);	
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 254);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CPlayer::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

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

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}


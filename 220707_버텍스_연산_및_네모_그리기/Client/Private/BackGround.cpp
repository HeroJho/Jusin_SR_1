#include "stdafx.h"
#include "..\Public\BackGround.h"

#include "GameInstance.h"

CBackGround::CBackGround(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CBackGround::CBackGround(const CBackGround & rhs)
	: CGameObject(rhs)
{
}

HRESULT CBackGround::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBackGround::Initialize(void * pArg)
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

void CBackGround::Tick(_float fTimeDelta)
{
	int a = 10;
}

void CBackGround::LateTick(_float fTimeDelta)
{
	

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_PRIORITY, this);
}

HRESULT CBackGround::Render()
{
	_float4x4	Matrix;
	D3DXMatrixIdentity(&Matrix);


	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &Matrix);
	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &Matrix);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &Matrix);

	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	m_pVIBufferCom->Render();

	return S_OK;
}

CBackGround * CBackGround::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBackGround*		pInstance = new CBackGround(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CBackGround"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CBackGround::Clone(void* pArg)
{
	CBackGround*		pInstance = new CBackGround(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Created : CBackGround"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBackGround::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}


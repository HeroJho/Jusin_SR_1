#include "stdafx.h"
#include "..\Public\Terrain.h"

#include "GameInstance.h"

CTerrain::CTerrain(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CTerrain::CTerrain(const CTerrain & rhs)
	: CGameObject(rhs)
{
}

HRESULT CTerrain::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTerrain::Initialize(void * pArg)
{
	/* 내일 이거 수정해라. */
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	m_pRendererCom = (CRenderer*)pGameInstance->Clone_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"));
	if (nullptr == m_pRendererCom)
		return E_FAIL;



	//m_Components.emplace(TEXT("Com_REnderer"), m_pRendererCom);

	m_pVIBufferCom = (CVIBuffer_AssignTerrain*)pGameInstance->Clone_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Terrain"));
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;


	m_pTextureCom = (CTexture*)pGameInstance->Clone_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Terrain"));
	if (nullptr == m_pTextureCom)
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

void CTerrain::Tick(_float fTimeDelta)
{

}

void CTerrain::LateTick(_float fTimeDelta)
{	

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CTerrain::Render()
{
	_float4x4	Matrix;
	D3DXMatrixIdentity(&Matrix);

	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &Matrix);

	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;

	// m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	m_pVIBufferCom->Render();

	return S_OK;
}

CTerrain * CTerrain::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTerrain*		pInstance = new CTerrain(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CTerrain"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTerrain::Clone(void* pArg)
{
	CTerrain*		pInstance = new CTerrain(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Created : CTerrain"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTerrain::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}


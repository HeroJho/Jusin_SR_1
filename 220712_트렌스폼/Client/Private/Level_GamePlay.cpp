#include "stdafx.h"
#include "..\Public\Level_GamePlay.h"

#include "GameInstance.h"


CLevel_GamePlay::CLevel_GamePlay(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel(pGraphic_Device)
{
}

HRESULT CLevel_GamePlay::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	return S_OK;
}

void CLevel_GamePlay::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

HRESULT CLevel_GamePlay::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;


	SetWindowText(g_hWnd, TEXT("게임플레이레벨임"));

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Player(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Player"), LEVEL_GAMEPLAY, pLayerTag)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Terrain"), LEVEL_GAMEPLAY, pLayerTag)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

CLevel_GamePlay * CLevel_GamePlay::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_GamePlay*		pInstance = new CLevel_GamePlay(pGraphic_Device);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed To Created : CLevel_GamePlay"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_GamePlay::Free()
{
	__super::Free();

}



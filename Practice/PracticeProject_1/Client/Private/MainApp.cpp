#include "stdafx.h"
#include "MainApp.h"
#include "Graphic_Device.h"


HRESULT CMainApp::Initiallize()
{
	if (FAILED(CGraphic_Device::Get_Instance()->InitDevice(g_hWnd, g_iWinSizeX, g_iWinSizeY, CGraphic_Device::MODE_WIN)))
		return E_FAIL;

	return S_OK;
}

void CMainApp::Tick(_float fTimeDelta)
{
}

HRESULT CMainApp::Render()
{
	CGraphic_Device::Get_Instance()->Render_Begin();



	CGraphic_Device::Get_Instance()->Render_End();

	return S_OK;
}

CMainApp * CMainApp::Create()
{
	CMainApp*	pInstance = new CMainApp();

	if (FAILED(pInstance->Initiallize()))
	{

	}

	return pInstance;
}

void CMainApp::Free()
{

}
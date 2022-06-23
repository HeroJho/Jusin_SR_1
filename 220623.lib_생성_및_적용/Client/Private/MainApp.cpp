#include "stdafx.h"
#include "..\Public\MainApp.h"



CMainApp::CMainApp()
	: CBase()
{
}

HRESULT CMainApp::Initialize()
{
	return S_OK;
}

CMainApp * CMainApp::Create()
{	
	CMainApp*		pInstance = new CMainApp();

	if (FAILED(pInstance->Initialize()))
	{

	}

	return pInstance;
}

void CMainApp::Free()
{

}

#include "..\Public\Boxel.h"

#include "Cube.h"

CBoxel::CBoxel(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{
}

CBoxel::CBoxel(const CBoxel & rhs)
	: CComponent(rhs)
{
}

HRESULT CBoxel::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBoxel::Initialize(void * pArg)
{
	return S_OK;
}

HRESULT CBoxel::Render()
{
	for (auto& pCube : m_CubeObjects)
		pCube->Render();

	return S_OK;
}

CBoxel * CBoxel::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBoxel*			pInstance = new CBoxel(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CBoxel"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CBoxel::Clone(void * pArg)
{
	CBoxel*			pInstance = new CBoxel(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CBoxel"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBoxel::Free()
{
	for (auto& pCube : m_CubeObjects)
		Safe_Release(pCube);

	m_CubeObjects.clear();
}

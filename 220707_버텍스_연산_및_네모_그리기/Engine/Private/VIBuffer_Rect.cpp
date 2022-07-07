#include "..\Public\VIBuffer_Rect.h"

CVIBuffer_Rect::CVIBuffer_Rect(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer(pGraphic_Device)
{
}

CVIBuffer_Rect::CVIBuffer_Rect(const CVIBuffer_Rect & rhs)
	: CVIBuffer(rhs)
{
}

HRESULT CVIBuffer_Rect::Initialize_Prototype()
{
	m_iStride = sizeof(VTXTEX);
	m_iNumVertices = 6;
	m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX1 /*| D3DFVF_TEXCOORDSIZE2(0) | D3DFVF_TEXCOORDSIZE3(1)*/;
	m_ePrimitiveType = D3DPT_TRIANGLELIST;
	m_iNumPrimitive = 2;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	VTXTEX*		pVertices = nullptr;

	//void*		pData = new int;
	//void*		pData1 = new VTXTEX;

	m_pVB->Lock(0, /*m_iStride * m_iNumVertices*/0, (void**)&pVertices, 0);

	pVertices[0].vPosition = _float3(-0.5f, 0.5f, 0.f);
	pVertices[0].vTexture = _float2(0.f, 0.f);

	pVertices[1].vPosition = _float3(0.5f, 0.5f, 0.f);
	pVertices[1].vTexture = _float2(1.f, 0.f);

	pVertices[2].vPosition = _float3(0.5f, -0.5f, 0.f);
	pVertices[2].vTexture = _float2(1.f, 1.f);

	pVertices[3].vPosition = _float3(-0.5f, 0.5f, 0.f);
	pVertices[3].vTexture = _float2(0.f, 0.f);

	pVertices[4].vPosition = _float3(0.5f, -0.5f, 0.f);
	pVertices[4].vTexture = _float2(1.f, 1.f);

	pVertices[5].vPosition = _float3(-0.5f, -0.5f, 0.f);
	pVertices[5].vTexture = _float2(0.f, 1.f);

	m_pVB->Unlock();
	
	return S_OK;
}

HRESULT CVIBuffer_Rect::Initialize(void * pArg)
{
	return S_OK;
}

CVIBuffer_Rect * CVIBuffer_Rect::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CVIBuffer_Rect*			pInstance = new CVIBuffer_Rect(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CVIBuffer_Rect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CVIBuffer_Rect::Clone(void * pArg)
{
	CVIBuffer_Rect*			pInstance = new CVIBuffer_Rect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CVIBuffer_Rect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Rect::Free()
{
	__super::Free();
}

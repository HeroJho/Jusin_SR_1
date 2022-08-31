#include "..\Public\VIBuffer.h"


CVIBuffer::CVIBuffer(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent(pDevice, pContext)
{
}

CVIBuffer::CVIBuffer(const CVIBuffer & rhs)
	: CComponent(rhs)
{
}

HRESULT CVIBuffer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CVIBuffer::Initialize(void * pArg)
{
	return S_OK;
}

HRESULT CVIBuffer::Create_VertexBuffer()
{
	if (nullptr == m_pDevice)
		return E_FAIL;

	if (FAILED(m_pDevice->CreateBuffer(&m_BufferDesc, &m_SubResourceData, &m_pVB)))
		return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer::Create_IndexBuffer()
{
	if (nullptr == m_pDevice)
		return E_FAIL;

	if (FAILED(m_pDevice->CreateBuffer(&m_BufferDesc, &m_SubResourceData, &m_pIB)))
		return E_FAIL;

	return S_OK;
}

void CVIBuffer::Free()
{
	__super::Free();

}

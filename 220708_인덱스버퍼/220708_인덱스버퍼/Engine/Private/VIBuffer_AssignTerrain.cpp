#include "..\Public\VIBuffer_AssignTerrain.h"

CVIBuffer_AssignTerrain::CVIBuffer_AssignTerrain(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer(pGraphic_Device)
{
}

CVIBuffer_AssignTerrain::CVIBuffer_AssignTerrain(const CVIBuffer_AssignTerrain & rhs)
	: CVIBuffer(rhs)
{
}


int GetRendNum(int iLeft, int iRight)
{
	// �õ尪�� ��� ���� random_device ����.
	std::random_device rd;

	// random_device �� ���� ���� ���� ������ �ʱ�ȭ �Ѵ�.
	std::mt19937 gen(rd());

	// 0 ���� 99 ���� �յ��ϰ� ��Ÿ���� �������� �����ϱ� ���� �յ� ���� ����.
	std::uniform_int_distribution<int> dis(iLeft, iRight);

	return dis(gen);
}

HRESULT CVIBuffer_AssignTerrain::Initialize_Prototype()
{
	m_iSizeTerrain = 100;
	m_iStride = sizeof(VTXTEX);
	m_iNumVertices = m_iSizeTerrain * m_iSizeTerrain;
	m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX1 /*| D3DFVF_TEXCOORDSIZE2(0) | D3DFVF_TEXCOORDSIZE3(1)*/;
	m_ePrimitiveType = D3DPT_TRIANGLELIST;
	int iRectCount = (m_iSizeTerrain - 1) * (m_iSizeTerrain - 1);
	m_iNumPrimitive = iRectCount * 2;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	VTXTEX*		pVertices = nullptr;

	
	m_pVB->Lock(0, /*m_iStride * m_iNumVertices*/0, (void**)&pVertices, 0);

	for (_int i = 0; i < m_iSizeTerrain; ++i)
	{
		for (_int j = 0; j < m_iSizeTerrain; ++j)
		{
			int iIndex = i * m_iSizeTerrain + j;
			pVertices[iIndex].vPosition = _float3(_float(j), _float(GetRendNum(0, 0)), _float(i));
			pVertices[iIndex].vTexture = _float2(j / _float(m_iSizeTerrain - 1), i / _float(m_iSizeTerrain - 1));
		}
	}

	m_pVB->Unlock();

	m_iIndexSizeofPrimitive = sizeof(FACEINDICES32);
	m_eIndexFormat = D3DFMT_INDEX32;

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	FACEINDICES32*		pIndices = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);


	for (_int i = 0; i < iRectCount - 1; ++i)
	{
		if (m_iNumVertices - m_iSizeTerrain == i)
			break;

		if ((i + 1) % m_iSizeTerrain == 0)
			continue;
		
		pIndices[i + i]._0 = i;
		pIndices[i + i]._1 = m_iSizeTerrain + i;
		pIndices[i + i]._2 = 1 + i;
		
		pIndices[i + i + 1]._0 = (1+i);
		pIndices[i + i + 1]._1 = m_iSizeTerrain + i;
		pIndices[i + i + 1]._2 = m_iSizeTerrain + 1 + i;

	}


	m_pIB->Unlock();
	
	return S_OK;
}

HRESULT CVIBuffer_AssignTerrain::Initialize(void * pArg)
{
	return S_OK;
}

CVIBuffer_AssignTerrain * CVIBuffer_AssignTerrain::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CVIBuffer_AssignTerrain*			pInstance = new CVIBuffer_AssignTerrain(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CVIBuffer_Terrain"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CVIBuffer_AssignTerrain::Clone(void * pArg)
{
	CVIBuffer_AssignTerrain*			pInstance = new CVIBuffer_AssignTerrain(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CVIBuffer_Terrain"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_AssignTerrain::Free()
{
	__super::Free();
}


#include "..\Public\QuadTree.h"
#include "Frustum.h"

CQuadTree::CQuadTree()
{
}

HRESULT CQuadTree::Initialize(_uint iLT, _uint iRT, _uint iRB, _uint iLB)
{
	// 4 코너
	m_iCorners[CORNER_LT] = iLT;
	m_iCorners[CORNER_RT] = iRT;
	m_iCorners[CORNER_RB] = iRB;
	m_iCorners[CORNER_LB] = iLB;

	// 더 이상 쪼개질 수 없을 때(변의 길이가 1일 때)
	if (iRT - iLT == 1)
		return S_OK;

	// 중점
	m_iCenterIndex = (iLT + iRB) >> 1;

	// 변의 센터
	_uint		iLC, iTC, iRC, iBC;
	iLC = (iLT + iLB) >> 1;
	iTC = (iLT + iRT) >> 1;
	iRC = (iRT + iRB) >> 1;
	iBC = (iLB + iRB) >> 1;	

	// 재귀적으로 호출하면서 더 이상 쪼개질 수 없을 때까지 만든다.
	m_pChild[CHILD_LT] = CQuadTree::Create(iLT, iTC, m_iCenterIndex, iLC);
	m_pChild[CHILD_RT] = CQuadTree::Create(iTC, iRT, iRC, m_iCenterIndex);
	m_pChild[CHILD_RB] = CQuadTree::Create(m_iCenterIndex, iRC, iRB, iBC);
	m_pChild[CHILD_LB] = CQuadTree::Create(iLC, m_iCenterIndex, iBC, iLB);

	return S_OK;
}

void CQuadTree::Culling(const _float3* pVerticesPos, CFrustum* pFrustum, FACEINDICES32* pFaceIndices, _uint* pNumFaces)
{
	// 계속 쪼갯는데, 자식이 없을 때
	if (nullptr == m_pChild[CHILD_LB])
	{
		_uint		iIndices[] = {
			m_iCorners[CORNER_LT], 
			m_iCorners[CORNER_RT],
			m_iCorners[CORNER_RB],
			m_iCorners[CORNER_LB],
		};

		_bool		isIn[] = {
			pFrustum->isIn_LocalSpace(XMLoadFloat3(&pVerticesPos[iIndices[0]])),
			pFrustum->isIn_LocalSpace(XMLoadFloat3(&pVerticesPos[iIndices[1]])),
			pFrustum->isIn_LocalSpace(XMLoadFloat3(&pVerticesPos[iIndices[2]])),
			pFrustum->isIn_LocalSpace(XMLoadFloat3(&pVerticesPos[iIndices[3]])),
		};

		/* 오른쪽 위 삼각형을 그려야한다. */
		if (true == isIn[0] &&
			true == isIn[1] &&
			true == isIn[2])
		{
			pFaceIndices[*pNumFaces]._0 = iIndices[0];
			pFaceIndices[*pNumFaces]._1 = iIndices[1];
			pFaceIndices[*pNumFaces]._2 = iIndices[2];
			++*pNumFaces;
		}

		/* 왼쪽 하단 삼각형을 그려야한다. */
		if (true == isIn[0] &&
			true == isIn[2] &&
			true == isIn[3])
		{
			pFaceIndices[*pNumFaces]._0 = iIndices[0];
			pFaceIndices[*pNumFaces]._1 = iIndices[2];
			pFaceIndices[*pNumFaces]._2 = iIndices[3];
			++*pNumFaces;
		}

		return;
	}	


	// 절두체와 충돌할 원의 반지름을 구한다.
	_float		fRadius = XMVectorGetX(XMVector3Length(XMLoadFloat3(&pVerticesPos[m_iCorners[CORNER_LT]]) - XMLoadFloat3(&pVerticesPos[m_iCenterIndex])));
	// 충돌했다면 쪼갠다 -> 자식의 Culling 호출
	if (true == pFrustum->isIn_LocalSpace(XMLoadFloat3(&pVerticesPos[m_iCenterIndex]), fRadius))
	{
		for (_uint i = 0; i < CHILD_END; ++i)
		{
			if (nullptr != m_pChild[i])
				m_pChild[i]->Culling(pVerticesPos, pFrustum, pFaceIndices,pNumFaces);
		}
	}

	return;
}



CQuadTree * CQuadTree::Create(_uint iLT, _uint iRT, _uint iRB, _uint iLB)
{
	CQuadTree*		pInstance = new CQuadTree();

	if (FAILED(pInstance->Initialize(iLT, iRT, iRB, iLB)))
	{
		MSG_BOX(TEXT("Failed To Created : CQuadTree"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CQuadTree::Free()
{
	for (auto& pChild : m_pChild)
		Safe_Release(pChild);
}

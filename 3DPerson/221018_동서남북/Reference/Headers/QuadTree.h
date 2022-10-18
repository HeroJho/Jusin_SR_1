#pragma once

#include "Base.h"

BEGIN(Engine)

class CQuadTree final : public CBase
{
public:
	enum CHILD { CHILD_LT, CHILD_RT, CHILD_RB, CHILD_LB, CHILD_END };
	enum CORNER { CORNER_LT, CORNER_RT, CORNER_RB, CORNER_LB, CORNER_END };
private:
	CQuadTree();
	virtual ~CQuadTree() = default;

public:
	HRESULT Initialize(_uint iLT, _uint iRT, _uint iRB, _uint iLB);
	void Culling(const _float3* pVerticesPos, class CFrustum* pFrustum, FACEINDICES32* pFaceIndices, _uint* pNumFaces);

private:
	CQuadTree*				m_pChild[CHILD_END] = { nullptr };
	_uint					m_iCenterIndex = 0;
	_uint					m_iCorners[CORNER_END];

public:
	static CQuadTree* Create(_uint iLT, _uint iRT, _uint iRB, _uint iLB);
	virtual void Free() override;
};

END
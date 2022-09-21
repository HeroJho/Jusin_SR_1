#pragma once

#include "Base.h"

/* 계층구조(뼈끼리의 상속관계)를 표현하기위한 aiNode이다. */

BEGIN(Engine)

class CHierarchyNode final : public CBase
{
private:
	CHierarchyNode();
	virtual ~CHierarchyNode() = default;

public:
	_uint Get_Depth() const {
		return m_iDepth;
	}

public:
	HRESULT Initialize(aiNode* pAINode, _uint iDepth);

private:
	char				m_szName[MAX_PATH] = "";	// 해당 뼈의 이름
	_float4x4			m_Transformation;			// 해당 뼈의 행렬
	_float4x4			m_CombinedTransformation;	// 부모 행렬이 적용된 행렬
	CHierarchyNode*		m_pParent = nullptr;		// 부모
	_uint				m_iDepth = 0;				// 벡터 정렬을 위한 깊이 값

public:
	static CHierarchyNode* Create(aiNode* pAINode, _uint iDepth);
	virtual void Free();
};

END
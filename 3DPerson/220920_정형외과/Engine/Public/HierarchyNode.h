#pragma once

#include "Base.h"

/* ��������(�������� ��Ӱ���)�� ǥ���ϱ����� aiNode�̴�. */

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
	char				m_szName[MAX_PATH] = "";	// �ش� ���� �̸�
	_float4x4			m_Transformation;			// �ش� ���� ���
	_float4x4			m_CombinedTransformation;	// �θ� ����� ����� ���
	CHierarchyNode*		m_pParent = nullptr;		// �θ�
	_uint				m_iDepth = 0;				// ���� ������ ���� ���� ��

public:
	static CHierarchyNode* Create(aiNode* pAINode, _uint iDepth);
	virtual void Free();
};

END
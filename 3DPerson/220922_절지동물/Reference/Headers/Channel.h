#pragma once

#include "Base.h"

BEGIN(Engine)

class CChannel final : public CBase
{
public:
	CChannel();
	virtual ~CChannel() = default;

public:
	HRESULT Initialize(aiNodeAnim*	pAIChannel, CModel* pModel);
	void Update_Transformation(_float fPlayTime);

private:
	char							m_szName[MAX_PATH] = "";

	_uint							m_iNumKeyFrames = 0;
	vector<KEYFRAME>				m_KeyFrames;

	class CHierarchyNode*			m_pHierarchyNode = nullptr;

	/* ���� ����ǰ� �ִ� Ű������. */
	_uint							m_iCurrentKeyFrame = 0;

public:
	static CChannel* Create(aiNodeAnim*	pAIChannel, CModel* pModel);
	virtual void Free() override;
};

END
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
	// 해당 Channel의 뼈 이름
	char							m_szName[MAX_PATH] = "";

	// 크 자 이 중에서 최대로 많은 키 프레임 갯수
	_uint							m_iNumKeyFrames = 0;

	vector<KEYFRAME>				m_KeyFrames;

	// 해당 Channel이 영향을 주고 있는 계층 뼈
	class CHierarchyNode*			m_pHierarchyNode = nullptr;

	/* 현재 재생되고 있던 키프레임. */
	_uint							m_iCurrentKeyFrame = 0;

public:
	static CChannel* Create(aiNodeAnim*	pAIChannel, CModel* pModel);
	virtual void Free() override;
};

END
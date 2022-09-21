#pragma once

#include "Base.h"

BEGIN(Engine)

class CChannel final : public CBase
{
public:
	CChannel();
	virtual ~CChannel() = default;

public:
	HRESULT Initialize(aiNodeAnim*	pAIChannel);


private:
	_uint							m_iNumKeyFrames = 0;
	vector<KEYFRAME>				m_KeyFrames;

public:
	static CChannel* Create(aiNodeAnim*	pAIChannel);
	virtual void Free() override;
};

END
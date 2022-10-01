#pragma once

#include "Base.h"

BEGIN(Engine)

class CBinChannel final : public CBase
{
public:
	CBinChannel();
	virtual ~CBinChannel() = default;

public:
	const char* Get_Name() const {
		return m_szName;
	}

public:
	HRESULT Initialize(DATA_HEROCHANNEL*	pAIChannel);
	_uint Update_Transformation(_float fPlayTime, _uint iCurrentKeyFrame, class CBinHierarchyNode* pNode);


private:
	char							m_szName[MAX_PATH] = "";

	_uint							m_iNumKeyFrames = 0;
	vector<KEYFRAME>				m_KeyFrames;


public:
	static CBinChannel* Create(DATA_HEROCHANNEL*	pAIChannel);
	virtual void Free() override;
};

END
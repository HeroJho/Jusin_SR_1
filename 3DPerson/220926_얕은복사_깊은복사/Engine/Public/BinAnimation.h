#pragma once

#include "Base.h"

BEGIN(Engine)

class CBinAnimation final : public CBase
{
private:
	CBinAnimation();
	CBinAnimation(const CBinAnimation& rhs);
	virtual ~CBinAnimation() = default;

public:
	HRESULT Initialize_Prototype(DATA_HEROANIM* pAIAnimation);
	HRESULT Initialize(class CBinModel* pModel);
	HRESULT Play_Animation(_float fTimeDelta);

private:
	/* 이 애니메이션을 구동하기위해 사용되는 뼈의 갯수. */
	_uint						m_iNumChannels = 0;
	vector<class CBinChannel*>		m_Channels;

	/* 애니메이션 재생하는데 걸리는 전체시간. */
	_float						m_fDuration = 0.f;

	/* 애니메이션의 초당 재생 속도. */
	_float						m_fTickPerSecond = 0.f;

	_float						m_fPlayTime = 0.f;

private: /* 복제된 애니메이션 마다 따로 가진다. */
	vector<class CBinHierarchyNode*>	m_HierarchyNodes;
	vector<_uint>					m_ChannelKeyFrames;

public:
	static CBinAnimation* Create(DATA_HEROANIM* pAIAnimation);
	CBinAnimation* Clone(class CBinModel* pModel);
	virtual void Free() override;
};

END
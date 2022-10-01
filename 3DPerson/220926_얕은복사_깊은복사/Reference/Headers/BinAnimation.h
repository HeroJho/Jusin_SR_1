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
	/* �� �ִϸ��̼��� �����ϱ����� ���Ǵ� ���� ����. */
	_uint						m_iNumChannels = 0;
	vector<class CBinChannel*>		m_Channels;

	/* �ִϸ��̼� ����ϴµ� �ɸ��� ��ü�ð�. */
	_float						m_fDuration = 0.f;

	/* �ִϸ��̼��� �ʴ� ��� �ӵ�. */
	_float						m_fTickPerSecond = 0.f;

	_float						m_fPlayTime = 0.f;

private: /* ������ �ִϸ��̼� ���� ���� ������. */
	vector<class CBinHierarchyNode*>	m_HierarchyNodes;
	vector<_uint>					m_ChannelKeyFrames;

public:
	static CBinAnimation* Create(DATA_HEROANIM* pAIAnimation);
	CBinAnimation* Clone(class CBinModel* pModel);
	virtual void Free() override;
};

END
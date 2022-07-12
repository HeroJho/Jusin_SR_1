#pragma once

#include "Component.h"

/* 1.��ü�� ������¸� �����Ѵ�.(���) */
/* 2.���û����� ������ ������·� ��ȯ���ֱ����ѱ��  */

BEGIN(Engine)

class ENGINE_DLL CTransform final : public CComponent
{
public:
	enum STATETYPE { STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_POSITION, STATE_END };

public:
	typedef struct tagTransformDesc
	{
		_float			fSpeedPerSec;
		_float			fRotationPerSec;
	}TRANSFORMDESC;
public:
	CTransform(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTransform(const CTransform& rhs);
	virtual ~CTransform() = default;

public: 
	_float3 Get_State(STATETYPE eState) const {
		return *(_float3*)&m_WorldMatrix.m[eState][0];
	}

	void Set_State(STATETYPE eState, const _float3& State) {
		memcpy(&m_WorldMatrix.m[eState][0], &State, sizeof(_float3));

		m_WorldMatrix.m[eState][0] = *State; 
	}


public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	HRESULT Bind_WorldMatrix();

public:
	void Go_Straight(_float fTimeDelta);
	void Go_Backward(_float fTimeDelta);


private:
	_float4x4			m_WorldMatrix;
	TRANSFORMDESC		m_TransformDesc;

public:
	static CTransform* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END
#pragma once

#include "Component.h"

/* 1.객체의 월드상태를 보관한다.(행렬) */
/* 2.로컬상태의 정보를 월드상태로 변환해주기위한기능  */

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
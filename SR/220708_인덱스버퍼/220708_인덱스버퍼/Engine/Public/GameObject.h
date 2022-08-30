#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CGameObject abstract : public CBase
{
protected:
	CGameObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGameObject(const CGameObject& rhs);
	virtual ~CGameObject() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	_float3 GetPos() { return m_vPos; }
	_float3 GetDir() { return m_vDir; }

protected:
	LPDIRECT3DDEVICE9			m_pGraphic_Device = nullptr;

	_float3					m_vPos;
	_float3					m_vDir;
	_float					m_fDegree = 0.f;

protected:
	//map<const _tchar*, class CComponent*>			m_Components;
	//typedef map<const _tchar*, class CComponent*>	COMPONENTS;

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END
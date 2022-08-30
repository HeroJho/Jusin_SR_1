#pragma once

#include "Base.h"

BEGIN(Engine)
class CGameObject; 
END

BEGIN(Engine)

class CCamera_Manager final : public CBase
{
	DECLARE_SINGLETON(CCamera_Manager)
private:
	CCamera_Manager();
	virtual ~CCamera_Manager() = default;

public:
	HRESULT InitCam(_uint iWinSizeX, _uint iWinSizeY);
	void Tick(_float fTimeDelta);
	void LateTick(_float fTimeDelta);
	void SetCamMatrix();
	void SetTarget(CGameObject* pTarget);

private:
	LPDIRECT3DDEVICE9	m_pGraphic_Device = nullptr;
	_int m_iWinSizeX = 0;
	_int m_iWinSizeY = 0;
	
	CGameObject*		m_pTarget = nullptr;
	
	_float3		m_pPos;
	_float		m_fA = 60.f;
	_bool		m_bCamMode = false;

public:
	virtual void Free() override;

};

END

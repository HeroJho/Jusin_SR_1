#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CBoxel final : public CComponent
{

private:
	CBoxel(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBoxel(const CBoxel& rhs);
	virtual ~CBoxel() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Render();

private:
	list<class CCube*>				m_CubeObjects;
	typedef list<class CCube*>		RENDEROBJECTS;


public:
	static CBoxel* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END


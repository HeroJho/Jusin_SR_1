#pragma once

#include "Graphic_Device.h"
#include "Level_Manager.h"

BEGIN(Engine)

class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance)
private:
	CGameInstance();
	virtual ~CGameInstance() = default;

public:
	HRESULT Initialize_Engine(const GRAPHICDESC& GraphicDesc, IDirect3DDevice9** ppOut);
	void Tick_Engine(_float fTimeDelta);

public:
	void Render_Begin(void);
	void Render_End(HWND hWnd = 0);

public:
	HRESULT Open_Level(class CLevel* pNewLevel);
	HRESULT Render_Level();

private:
	CGraphic_Device*			m_pGraphic_Device = nullptr;
	CLevel_Manager*				m_pLevel_Manager = nullptr;

public:
	static void Release_Engine();
	virtual void Free() override;
};

END
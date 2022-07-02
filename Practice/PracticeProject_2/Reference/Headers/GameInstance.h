#pragma once

#include "Graphic_Device.h"
#include "Level_Manager.h"
#include "Object_Manager.h"

BEGIN(Engine)

class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance)

public:
	CGameInstance();
	virtual ~CGameInstance() = default;

public:
	HRESULT Initialize_Engine(_uint iNumLevels, const GRAPHICDESC& GraphicDesc, IDirect3DDevice9** ppOut);
	void Tick_Engine(_float fTimeDelta);

public:
	void Render_Begin(void);
	void Render_End(HWND hWnd = 0);

public:
	HRESULT Open_Level(class CLevel* pNewLevel);
	HRESULT Render_Level();

public:
	HRESULT Add_Prototype(const _tchar* pPrototypeTag, class CGameObject* pPrototype);
	HRESULT Add_GameObjectToLayer(const _tchar* pPrototypeTag, _uint iLevelIndex, const _tchar* pLayerTag, void* pArg = nullptr);

private:
	CGraphic_Device*	m_pGraphic_Device = nullptr;
	CLevel_Manager*		m_pLevel_Manager = nullptr;
	CObject_Manager*	m_pObject_Manager = nullptr;

public:
	static void Release_Engine();
	virtual void Free() override;

};

END
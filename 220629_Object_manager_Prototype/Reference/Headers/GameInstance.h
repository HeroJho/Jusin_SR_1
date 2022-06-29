#pragma once

#include "Graphic_Device.h"
#include "Level_Manager.h"
#include "Object_Manager.h"

/* 클라이언트로 보여주기위한 가장 대표적인 클래스이다. */
/* 각종 매니져클래스들의 주요함수를 클라로 보여준다.  */
/* 엔진초기화. */
/* 엔진정리. */

BEGIN(Engine)

class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance)

private:
	CGameInstance();
	virtual ~CGameInstance() = default;

public: /* For.Engine */
	HRESULT Initialize_Engine(_uint iNumLevel, const GRAPHICDESC& GraphicDesc, IDirect3DDevice9** pOut);
	void Tick_Engine(_float fTimeDelta);
	

public: /* For.Graphic_Device */
	void Render_Begin(void);
	void Render_End(HWND hWnd = 0);

public: /* For.Level_Manager */				
	HRESULT Open_Level(class CLevel* pNewLevel);
	HRESULT Render_Level();


private:
	CGraphic_Device*				m_pGraphic_Device = nullptr;
	CLevel_Manager*					m_pLevel_Manager = nullptr;
	CObject_Manager*				m_pObject_Manager = nullptr;


public:
	static void Release_Engine();
	virtual void Free() override;
};

END
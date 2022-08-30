#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)

class CMainApp final : public CBase
{
private:
	CMainApp();
	virtual ~CMainApp() = default;

public:
	HRESULT Initialize();
	void Tick(_float fTimeDelta);
	HRESULT Render();	

public:
	static CMainApp* Create();
	virtual void Free();
};


END
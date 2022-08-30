#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)

class CMainApp final : public CBase
{
protected:
	CMainApp() = default;
	virtual ~CMainApp() = default;

public:
	HRESULT Initiallize();
	void Tick(_float fTimeDelta);
	HRESULT Render();
	
public:
	static CMainApp* Create();
	void Free();

};

END

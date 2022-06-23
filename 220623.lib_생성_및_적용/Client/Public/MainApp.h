#pragma once

#include "Base.h"

class CMainApp final : public CBase
{
private:
	CMainApp();
	virtual ~CMainApp() = default;

public:
	HRESULT Initialize();

public:
	static CMainApp* Create();
	virtual void Free();
};


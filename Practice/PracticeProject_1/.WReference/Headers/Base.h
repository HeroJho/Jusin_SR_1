#pragma once

#include "Engine_Defines.h"

class ENGINE_DLL CBase abstract
{
protected:
	CBase() = default;
	virtual ~CBase() = default;

public:
	unsigned long AddRef();
	unsigned long Release();

protected:
	unsigned long m_dwRefCnt = 0;

public:
	virtual void Free() = 0;

};


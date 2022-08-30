#pragma once

#include "Engine_Defines.h"

/* 모든 클래스들의 부모가 되는 클래스이다. */
/* 레퍼런스 카운트를 관리한다. 증가, 감소 or 삭제 */

class _declspec(dllimport) CBase abstract
{
protected:
	CBase() = default;
	virtual ~CBase() = default;

public:
	unsigned long AddRef();
	unsigned long Release();

protected:
	unsigned long			m_dwRefCnt = 0;

public:
	virtual void Free() = 0;

};


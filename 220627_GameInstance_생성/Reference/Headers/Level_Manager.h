#pragma once

#include "Base.h"

/* 현재 보여줘야할 레벨의 퐁니터를 가지고 있는다. */
/* 레벨의 틱, 렌더를 무한호출한다. */
/* 레[벨교체시에 기존레벨 삭제,. 새로운로 교체한다. */

BEGIN(Engine)

class CLevel_Manager final : public CBase
{
	DECLARE_SINGLETON(CLevel_Manager)
private:
	CLevel_Manager();
	virtual ~CLevel_Manager() = default;

public:
	/* 기존레벨 삭제. 새로운레벨 셋팅. */
	HRESULT Open_Level(class CLevel* pNewLevel);


private:
	class CLevel*				m_pCurrentLevel = nullptr;

public:
	virtual void Free() override;
};

END
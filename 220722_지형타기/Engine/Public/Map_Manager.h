#pragma once

/* 특정 레벨에 사용되는 객체들응ㄹ 모아서 보관해두낟. */
/* 보관하고 잇는 개체들의 Tick를 호출해준다. */

#include "Base.h"

BEGIN(Engine)

class CMap_Manager final : public CBase
{
	DECLARE_SINGLETON(CMap_Manager)

public:
	typedef struct tagCubeData
	{
		_float3	vPos;
		const _tchar* pTextureTag;
	}CUBEDATA;

private:
	CMap_Manager();
	virtual ~CMap_Manager() = default;

public:
	HRESULT LoadMapData();
	list<CUBEDATA>* ReadMap(const _tchar* pMapTag);


private:
	list<CUBEDATA>* Find_Map(const _tchar* pMapTag);


private: /* 원형 Map들을 보관할 커넽이너ㅣ */
	map<const _tchar*, list<CUBEDATA>>			m_MapPrototypes;
	typedef map<const _tchar*, list<CUBEDATA>>	MAPDATA;


public:
	virtual void Free() override;

};

END
#pragma once
#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)

class CModel;

END

BEGIN(Client)

class CDataManager final : public CBase
{
	DECLARE_SINGLETON(CDataManager)
public:
	CDataManager();
	virtual ~CDataManager() = default;


	// For. Static
public:
	HRESULT Init();

	HRESULT ReadSceneData(const char* pFileName, DATA_HEROSCENE* pScene);

public: // For. Map


private:


private:




public:
	virtual void Free() override;

};

END


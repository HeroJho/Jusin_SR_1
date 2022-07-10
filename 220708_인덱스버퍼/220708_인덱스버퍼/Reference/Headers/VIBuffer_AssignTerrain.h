#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_AssignTerrain final : public CVIBuffer
{
private:
	CVIBuffer_AssignTerrain(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer_AssignTerrain(const CVIBuffer_AssignTerrain& rhs);
	virtual ~CVIBuffer_AssignTerrain() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	static CVIBuffer_AssignTerrain* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;

private:
	int m_iSizeTerrain = 0;

};

END
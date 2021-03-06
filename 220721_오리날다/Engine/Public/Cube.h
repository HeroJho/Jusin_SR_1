#pragma once

#include "Engine_Defines.h"
#include "GameObject.h"


class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Cube;

BEGIN(Engine)

class CCube final : public CGameObject
{
public:
	typedef struct tagBlockDesc
	{
		_uint		iSizeX;
		_uint		iSizeY;

		_float3		fPos;

	}BLOCKDESC;
private:
	CCube(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCube(const CCube& rhs);
	virtual ~CCube() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CTexture*				m_pTextureCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CVIBuffer_Cube*			m_pVIBufferCom = nullptr;

private:
	_float3			m_vTargetPos = _float3(0.f, 0.f, 0.f);

private:
	HRESULT Set_RenderState();
	HRESULT Reset_RenderState();
private:
	HRESULT SetUp_Components();

public:
	static CCube* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END
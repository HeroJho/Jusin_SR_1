#pragma once

#include "Client_Defines.h"
#include "LandObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CEffect final : public CLandObject
{
public:
	typedef struct tagBackGroundDesc
	{
		_uint		iSizeX;
		_uint		iSizeY;
	}BACKDESC;
private:
	CEffect(LPDIRECT3DDEVICE9 pGraphic_Device);
	CEffect(const CEffect& rhs);
	virtual ~CEffect() = default;

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
	CVIBuffer_Rect*			m_pVIBufferCom = nullptr;

private:
	_float					m_fFrame = 0.f;

private:
	HRESULT Set_RenderState();
	HRESULT Reset_RenderState();
private:
	HRESULT SetUp_Components();

public:
	static CEffect* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END
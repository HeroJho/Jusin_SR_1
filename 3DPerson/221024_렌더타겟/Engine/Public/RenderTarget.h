#pragma once

#include "Base.h"

BEGIN(Engine)

class CRenderTarget final : public CBase
{
private:
	CRenderTarget(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CRenderTarget() = default;

public:
	ID3D11RenderTargetView* Get_RTV() const {
		return m_pRTV;
	}

	ID3D11Texture2D* Get_Texture2D() const {
		return m_pTexture2D;
	}

public:
	HRESULT Initialize(_uint iSizeX, _uint iSizeY, DXGI_FORMAT eFormat, const _float4* pClearColor);

private:
	ID3D11Device*				m_pDevice = nullptr;
	ID3D11DeviceContext*		m_pContext = nullptr;
	
	ID3D11Texture2D*			m_pTexture2D = nullptr;
	ID3D11RenderTargetView*		m_pRTV = nullptr;

	_float4						m_vClearColor = _float4(0.f, 0.f, 0.f, 1.f);	

public:
	static CRenderTarget* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uint iSizeX, _uint iSizeY, DXGI_FORMAT eFormat, const _float4* pClearColor);
	virtual void Free() override;
};

END
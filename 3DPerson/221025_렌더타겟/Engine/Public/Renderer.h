#pragma once

#include "Component.h"

/* ȭ�鿡 �׷������� ��ü���� �׷������� ������� ��Ƽ� �����Ѵ�.  */
/* �����ϰ� �ִ� ���b���� �����Լ��� ȣ�����ֳ�. */

BEGIN(Engine)

class ENGINE_DLL CRenderer final : public CComponent
{
public: /* �׷����� ������ ���� ���� */
	enum RENDERGROUP { RENDER_PRIORITY, RENDER_NONALPHABLEND, RENDER_NONLIGHT, RENDER_ALPHABLEND, RENDER_UI, RENDER_END };


private:
	CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CRenderer(const CRenderer& rhs);
	virtual ~CRenderer() = default;



public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
public:
	HRESULT Add_RenderGroup(RENDERGROUP eRenderGroup, class CGameObject* pGameObject);
	HRESULT Draw();

private:
	list<class CGameObject*>				m_RenderObjects[RENDER_END];
	typedef list<class CGameObject*>		RENDEROBJECTS;

private:
	class CTarget_Manager*					m_pTarget_Manager = nullptr;

private:
	HRESULT Render_Priority();
	HRESULT Render_NonAlphaBlend();
	HRESULT Render_NonLight();
	HRESULT Render_AlphaBlend();
	HRESULT Render_UI();

private: /* For.Deferred Lighting */ 
	


public:
	static CRenderer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END
#pragma once

#include "Collider.h"

BEGIN(Engine)

class ENGINE_DLL COBB final : public CCollider
{
protected:
	COBB(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	COBB(const COBB& rhs);
	virtual ~COBB() = default;

public:
	virtual HRESULT Initialize_Prototype(CCollider::TYPE eColliderType);
	virtual HRESULT Initialize(void* pArg);
	virtual void Update(_fmatrix TransformMatrix);

#ifdef _DEBUG
public:
	virtual HRESULT Render();
#endif // _DEBUG

private:
	BoundingOrientedBox*	m_pOBB = nullptr;

public:
	static COBB* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CCollider::TYPE eColliderType);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;


};

END
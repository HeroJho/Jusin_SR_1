#pragma once

/* V : Vertex */
/* I : Index */

/* 실제 모델들의 부모가 되는 클래스다. */
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer abstract : public CComponent
{
protected:
	CVIBuffer(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer(const CVIBuffer& rhs);
	virtual ~CVIBuffer() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Render();




protected:
	LPDIRECT3DVERTEXBUFFER9	m_pVB = nullptr;		// 정점 버퍼의 컴객체
	_uint					m_iStride = 0;			// 정점하나의 byte크기
	_uint					m_iNumVertices = 0;		// 버텍스 수
	_ulong					m_dwFVF = 0;			// 버텍스 구조체의 정보
	D3DPRIMITIVETYPE		m_ePrimitiveType;		// 그릴 도형의 타입
	_uint					m_iNumPrimitive = 0;    // 그릴 도형의 갯수


protected:
	HRESULT Create_VertexBuffer();					// 자식의 정보를 가지고 버퍼를 만든다.

public:
	virtual CComponent* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END
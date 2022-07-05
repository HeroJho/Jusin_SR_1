#include "stdafx.h"
#include "..\Public\BackGround.h"

#include "GameInstance.h"


CBackGround::CBackGround(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CBackGround::CBackGround(const CBackGround & rhs)
	: CGameObject(rhs)
{
}

HRESULT CBackGround::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CBackGround::Initialize(void * pArg)
{
	if (nullptr != pArg)
	{
		// memcpy(&m_BackDesc, pArg, sizeof(BACKDESC));
		m_BackDesc = *(BACKDESC*)pArg;
	}

	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	m_pRendererCom = (CRenderer*)pGameInstance->Clone_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"));
	if (nullptr == m_pRendererCom)
		return E_FAIL;

	Safe_Release(pGameInstance);



	m_pGraphic_Device->CreateVertexBuffer(
		3 * sizeof(VERTEX),
		D3DUSAGE_WRITEONLY,
		D3DFVF_XYZ,
		D3DPOOL_MANAGED,
		&m_VB,
		0
	);

	m_pGraphic_Device->CreateIndexBuffer(
		36 * sizeof(WORD),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_IB,
		0
	);

	VERTEX* vertices;
	m_VB->Lock(0, 0, (void**)&vertices, 0);

	// vertices of a unit cube
	vertices[0] = VERTEX(-1.0f, -1.0f, -1.0f);
	vertices[1] = VERTEX(-1.0f, 1.0f, -1.0f);
	vertices[2] = VERTEX(1.0f, 1.0f, -1.0f);
	vertices[3] = VERTEX(1.0f, -1.0f, -1.0f);
	vertices[4] = VERTEX(-1.0f, -1.0f, 1.0f);
	vertices[5] = VERTEX(-1.0f, 1.0f, 1.0f);
	vertices[6] = VERTEX(1.0f, 1.0f, 1.0f);
	vertices[7] = VERTEX(1.0f, -1.0f, 1.0f);

	m_VB->Unlock();


	WORD* indices = 0;
	m_IB->Lock(0, 0, (void**)&indices, 0);

	// front side
	indices[0] = 0; indices[1] = 1; indices[2] = 2;
	indices[3] = 0; indices[4] = 2; indices[5] = 3;

	// back side
	indices[6] = 4; indices[7] = 6; indices[8] = 5;
	indices[9] = 4; indices[10] = 7; indices[11] = 6;

	// left side
	indices[12] = 4; indices[13] = 5; indices[14] = 1;
	indices[15] = 4; indices[16] = 1; indices[17] = 0;

	// right side
	indices[18] = 3; indices[19] = 2; indices[20] = 6;
	indices[21] = 3; indices[22] = 6; indices[23] = 7;

	// top
	indices[24] = 1; indices[25] = 5; indices[26] = 6;
	indices[27] = 1; indices[28] = 6; indices[29] = 2;

	// bottom
	indices[30] = 4; indices[31] = 0; indices[32] = 3;
	indices[33] = 4; indices[34] = 3; indices[35] = 7;

	m_IB->Unlock();



	D3DXVECTOR3 position(0.0f, 0.0f, -5.0f);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMATRIX V;
	D3DXMatrixLookAtLH(&V, &position, &target, &up);

	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &V);

	//
	// Set the projection matrix.
	//

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
		&proj,
		D3DX_PI * 0.5f, // 90 - degree
		(float)1280 / (float)720,
		1.0f,
		1000.0f);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &proj);

	//
	// Switch to wireframe mode.
	//

	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);





	return S_OK;
}

void CBackGround::Tick(_float fTimeDelta)
{
	int a = 10;
}

void CBackGround::LateTick(_float fTimeDelta)
{
	

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_PRIORITY, this);
}

HRESULT CBackGround::Render()
{

	DrawPaint();

	return S_OK;
}

CBackGround * CBackGround::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBackGround*		pInstance = new CBackGround(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CBackGround"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CBackGround::Clone(void* pArg)
{
	CBackGround*		pInstance = new CBackGround(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Created : CBackGround"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBackGround::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
}

void CBackGround::DrawPaint()
{
	D3DXMATRIX Rx, Rpos, w;
	D3DXMatrixIdentity(&Rpos);
	D3DXMatrixIdentity(&Rx);
	D3DXMatrixIdentity(&w);

	D3DXMatrixTranslation(&Rpos, 0.f, 0.f, 0.f);
	D3DXMatrixRotationX(&Rx, 3.14f / 4.0f);

	w = Rx * Rpos;

	//static float y = 0.0f;
	//D3DXMatrixRotationY(&Ry, y);
	//y += ;

	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &w);
	m_pGraphic_Device->SetStreamSource(0, m_VB, 0, sizeof(VERTEX));
	m_pGraphic_Device->SetIndices(m_IB);
	m_pGraphic_Device->SetFVF(D3DFVF_XYZ);
	m_pGraphic_Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);

}


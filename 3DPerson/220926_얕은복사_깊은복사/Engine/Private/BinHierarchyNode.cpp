#include "..\Public\BinHierarchyNode.h"

CBinHierarchyNode::CBinHierarchyNode()
{

}

HRESULT CBinHierarchyNode::Initialize(DATA_HERONODE* pNode)
{
	strcpy_s(m_szName, pNode->cName);
	strcpy_s(m_szParentName, pNode->cParent);

	XMStoreFloat4x4(&m_OffsetMatrix, XMMatrixIdentity());

	memcpy(&m_Transformation, &pNode->mTransform, sizeof(_float4x4));
	// XMStoreFloat4x4(&m_Transformation, XMMatrixTranspose(XMLoadFloat4x4(&m_Transformation)));

	m_iDepth = pNode->iDepth;

	XMStoreFloat4x4(&m_CombinedTransformation, XMMatrixIdentity());

	return S_OK;
}

void CBinHierarchyNode::Set_CombinedTransformation()
{
	if (nullptr != m_pParent)
		XMStoreFloat4x4(&m_CombinedTransformation, XMLoadFloat4x4(&m_Transformation) * XMLoadFloat4x4(&m_pParent->m_CombinedTransformation));
	else
		m_CombinedTransformation = m_Transformation;
}

void CBinHierarchyNode::Set_OffsetMatrix(_fmatrix OffsetMatrix)
{
	XMStoreFloat4x4(&m_OffsetMatrix, OffsetMatrix);
}

void CBinHierarchyNode::Set_FindParent(CBinHierarchyNode* pNode)
{
	const char* pName =  pNode->Get_Name();
	if (!strcmp(m_szParentName, pName))
	{
		m_pParent = pNode;
		Safe_AddRef(m_pParent);
		return;
	}
	m_pParent = nullptr;
}

CBinHierarchyNode * CBinHierarchyNode::Create(DATA_HERONODE* pNode)
{
	CBinHierarchyNode*			pInstance = new CBinHierarchyNode();

	if (FAILED(pInstance->Initialize(pNode)))
	{
		MSG_BOX(TEXT("Failed To Created : CBinHierarchyNode"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBinHierarchyNode::Free()
{
	Safe_Release(m_pParent);
}

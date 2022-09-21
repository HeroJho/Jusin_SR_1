#include "..\Public\Channel.h"
#include "Model.h"
#include "HierarchyNode.h"



CChannel::CChannel()
{
}

HRESULT CChannel::Initialize(aiNodeAnim * pAIChannel, CModel* pModel)
{
	/* Ư�� �ִϸ��̼ǿ��� ���Ǵ� ���� �����̴�. */
	/* �� �̸��� ���� ������ �ִ� HierarchyNodes�� ���� �� �ѳ�� �̸��� ���� ���̴�. */
	strcpy_s(m_szName, pAIChannel->mNodeName.data);

	m_pHierarchyNode = pModel->Get_HierarchyNode(m_szName);
	if (nullptr == m_pHierarchyNode)
		return E_FAIL;

	Safe_AddRef(m_pHierarchyNode);


	/* Ű������ �����鸦 �ε��Ѵ�. */
	/* Ű������ : ��ü�ִϸ��̼� ���� ��, Ư�� �ð��뿡 �̻��� ǥ���ؾ��� ������ ���� ��������̴�. */
	m_iNumKeyFrames = max(pAIChannel->mNumScalingKeys, pAIChannel->mNumRotationKeys);
	m_iNumKeyFrames = max(m_iNumKeyFrames, pAIChannel->mNumPositionKeys);

	_float3			vScale;
	_float4			vRotation;
	_float3			vPosition;

	// �ִ� Ű ������ �� ��ŭ ��ȸ
	for (_uint i = 0; i < m_iNumKeyFrames; ++i)
	{
		KEYFRAME			KeyFrame;
		ZeroMemory(&KeyFrame, sizeof(KEYFRAME));

		if(i < pAIChannel->mNumScalingKeys)
		{
			memcpy(&vScale, &pAIChannel->mScalingKeys[i], sizeof(_float3));
			KeyFrame.fTime = pAIChannel->mScalingKeys[i].mTime;
		}
		if (i < pAIChannel->mNumRotationKeys)
		{ // ���ʹϾ� ���� ��� mValue�� ���� ������ wxyz�̱� ������ ������ ����
			vRotation.x = pAIChannel->mRotationKeys[i].mValue.x;
			vRotation.y = pAIChannel->mRotationKeys[i].mValue.y;
			vRotation.z = pAIChannel->mRotationKeys[i].mValue.z;
			vRotation.w = pAIChannel->mRotationKeys[i].mValue.w;
			KeyFrame.fTime = pAIChannel->mRotationKeys[i].mTime;
		}
		if (i < pAIChannel->mNumPositionKeys)
		{
			memcpy(&vPosition, &pAIChannel->mPositionKeys[i], sizeof(_float3));
			KeyFrame.fTime = pAIChannel->mPositionKeys[i].mTime;
		}

		KeyFrame.vScale = vScale;
		KeyFrame.vRotation = vRotation;
		KeyFrame.vPosition = vPosition;

		m_KeyFrames.push_back(KeyFrame);
	}	

	return S_OK;
}

void CChannel::Update_Transformation(_float fPlayTime)
{
	_float3			vScale;
	_float4			vRotation;
	_float3			vPosition;

	// ������ KeyFrame Time�� �Ѿ��� ��� ������ ���� ����
	if (fPlayTime >= m_KeyFrames.back().fTime)
	{
		vScale = m_KeyFrames.back().vScale;
		vRotation = m_KeyFrames.back().vRotation;
		vPosition = m_KeyFrames.back().vPosition;		
	}
	// ������ KeyFrame Time�� ���� ���� ��� ��������
	else
	{
		// ������ ����� ���ϰ� �Ͼ ��� fPlayTime�� �ް��ϰ� ������ �� ����. while�� �ذ�
		// ���� FrameTime�� �Ѿ��� ��� CurrentKeyFrame ����
		while (fPlayTime >= m_KeyFrames[m_iCurrentKeyFrame + 1].fTime)
			++m_iCurrentKeyFrame;

		// ���������� ���� ���� ���ϱ�
		_float		fRatio = (fPlayTime - m_KeyFrames[m_iCurrentKeyFrame].fTime) / 
			(m_KeyFrames[m_iCurrentKeyFrame + 1].fTime - m_KeyFrames[m_iCurrentKeyFrame].fTime);

		// ��������
		_float3		vSourScale, vDestScale;
		_float4		vSourRotation, vDestRotation;
		_float3		vSourPosition, vDestPosition;

		vSourScale = m_KeyFrames[m_iCurrentKeyFrame].vScale;
		vDestScale = m_KeyFrames[m_iCurrentKeyFrame + 1].vScale;

		vSourRotation = m_KeyFrames[m_iCurrentKeyFrame].vRotation;
		vDestRotation = m_KeyFrames[m_iCurrentKeyFrame + 1].vRotation;

		vSourPosition = m_KeyFrames[m_iCurrentKeyFrame].vPosition;
		vDestPosition = m_KeyFrames[m_iCurrentKeyFrame + 1].vPosition;

		XMStoreFloat3(&vScale, XMVectorLerp(XMLoadFloat3(&vSourScale), XMLoadFloat3(&vDestScale), fRatio));
		XMStoreFloat4(&vRotation, XMQuaternionSlerp(XMLoadFloat4(&vSourRotation), XMLoadFloat4(&vDestRotation), fRatio));
		XMStoreFloat3(&vPosition, XMVectorLerp(XMLoadFloat3(&vSourPosition), XMLoadFloat3(&vDestPosition), fRatio));
	}

	// ���������� ũ���̷� ��ĸ����
	_matrix		TransformationMatrix = XMMatrixAffineTransformation(XMLoadFloat3(&vScale), XMVectorSet(0.f, 0.f, 0.f, 1.f), XMLoadFloat4(&vRotation), XMVectorSetW(XMLoadFloat3(&vPosition), 1.f));	

	// ���� ��忡 �Ѱ��ֱ�
	if (nullptr != m_pHierarchyNode)
		m_pHierarchyNode->Set_Transformation(TransformationMatrix);
}

CChannel * CChannel::Create(aiNodeAnim * pAIChannel, CModel* pModel)
{
	CChannel*			pInstance = new CChannel();

	if (FAILED(pInstance->Initialize(pAIChannel, pModel)))
	{
		MSG_BOX(TEXT("Failed To Created : CChannel"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CChannel::Free()
{
	Safe_Release(m_pHierarchyNode);
}


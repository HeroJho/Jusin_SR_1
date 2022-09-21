#include "..\Public\Channel.h"
#include "Model.h"
#include "HierarchyNode.h"



CChannel::CChannel()
{
}

HRESULT CChannel::Initialize(aiNodeAnim * pAIChannel, CModel* pModel)
{
	/* 특정 애니메이션에서 사용되는 뼈의 정보이다. */
	/* 이 이름은 모델이 가지고 있는 HierarchyNodes의 뼈대 중 한놈과 이름이 같을 것이다. */
	strcpy_s(m_szName, pAIChannel->mNodeName.data);

	m_pHierarchyNode = pModel->Get_HierarchyNode(m_szName);
	if (nullptr == m_pHierarchyNode)
		return E_FAIL;

	Safe_AddRef(m_pHierarchyNode);


	/* 키프레임 정보들를 로드한다. */
	/* 키프레임 : 전체애니메이션 동작 중, 특정 시간대에 이뼈가 표현해야할 동작의 상태 행렬정보이다. */
	m_iNumKeyFrames = max(pAIChannel->mNumScalingKeys, pAIChannel->mNumRotationKeys);
	m_iNumKeyFrames = max(m_iNumKeyFrames, pAIChannel->mNumPositionKeys);

	_float3			vScale;
	_float4			vRotation;
	_float3			vPosition;

	// 최대 키 프레임 수 만큼 순회
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
		{ // 쿼터니언 같은 경우 mValue의 저장 순서가 wxyz이기 때문에 일일이 저장
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

	// 마지막 KeyFrame Time을 넘었을 경우 마지막 상태 유지
	if (fPlayTime >= m_KeyFrames.back().fTime)
	{
		vScale = m_KeyFrames.back().vScale;
		vRotation = m_KeyFrames.back().vRotation;
		vPosition = m_KeyFrames.back().vPosition;		
	}
	// 마지막 KeyFrame Time을 넘지 않을 경우 선형보간
	else
	{
		// 프레임 드랍이 심하게 일어날 경우 fPlayTime이 급격하게 증가할 수 있음. while로 해결
		// 다음 FrameTime이 넘었을 경우 CurrentKeyFrame 증가
		while (fPlayTime >= m_KeyFrames[m_iCurrentKeyFrame + 1].fTime)
			++m_iCurrentKeyFrame;

		// 선형보간을 위해 비율 구하기
		_float		fRatio = (fPlayTime - m_KeyFrames[m_iCurrentKeyFrame].fTime) / 
			(m_KeyFrames[m_iCurrentKeyFrame + 1].fTime - m_KeyFrames[m_iCurrentKeyFrame].fTime);

		// 선형보간
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

	// 선형보간된 크자이로 행렬만들기
	_matrix		TransformationMatrix = XMMatrixAffineTransformation(XMLoadFloat3(&vScale), XMVectorSet(0.f, 0.f, 0.f, 1.f), XMLoadFloat4(&vRotation), XMVectorSetW(XMLoadFloat3(&vPosition), 1.f));	

	// 계층 노드에 넘겨주기
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


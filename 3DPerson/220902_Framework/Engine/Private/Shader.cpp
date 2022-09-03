#include "..\Public\Shader.h"

CShader::CShader(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent(pDevice, pContext)
{
}

CShader::CShader(const CShader & rhs)
	: CComponent(rhs)
	, m_pEffect(rhs.m_pEffect)
	, m_Passes(rhs.m_Passes)
{
	for (auto& PassDesc : m_Passes)
	{
		Safe_AddRef(PassDesc.pInputLayout);
		Safe_AddRef(PassDesc.pPass);
	}

		
	Safe_AddRef(m_pEffect);
}

HRESULT CShader::Initialize_Prototype(const _tchar * pShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElements)
{
	_uint		iFlag = 0;

#ifdef _DEBUG
	iFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else 
	iFlag = D3DCOMPILE_OPTIMIZATION_LEVEL1;
#endif	

	// 이펙트 컴객체를 만든다.
	if (FAILED(D3DX11CompileEffectFromFile(pShaderFilePath, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, iFlag, 0, m_pDevice, &m_pEffect, nullptr)))
		return E_FAIL;


	// 해당 이펙트 컴객체의 테크니커 컴객체를 얻어온다.(패스를 얻어오기 위해서)
	// 이때 테크니커는 정의된 순서대로 인덱스가 매겨진다. (대부분 하나의 테크니커만 쓰기 때문에 상수 0을 박아둔다.)
	ID3DX11EffectTechnique*	pTechnique = m_pEffect->GetTechniqueByIndex(0);
	if (nullptr == pTechnique)
		return E_FAIL;


	// 테크니커의 정보를 얻어온다.
	// 이 정보안에 테크니커가 가지고있는 패스 컴객체와 패스의 갯수가 있다.
	D3DX11_TECHNIQUE_DESC		TechniqueDesc;
	ZeroMemory(&TechniqueDesc, sizeof(D3DX11_TECHNIQUE_DESC));

	if (FAILED(pTechnique->GetDesc(&TechniqueDesc)))
		return E_FAIL;


	// 패스의 갯수만큼 반복문을 돈다.
	for (_uint i = 0; i < TechniqueDesc.Passes; ++i)
	{
		// 테크니커의 i번째 패스 컴객체를 얻어온다.
		PASSDESC				PassDesc;
		ZeroMemory(&PassDesc, sizeof(PASSDESC));
				
		PassDesc.pPass = pTechnique->GetPassByIndex(i);

		// i번째 패스의 정보를 읽어온다.
		D3DX11_PASS_DESC		PassInfo;

		PassDesc.pPass->GetDesc(&PassInfo);

		//  i번째 패스의 인풋레이아웃을 만들어 준다
		//		1인자 : 쉐이더로 그리기 위해 쉐이더에 전달할 정점 정보(배열로 넘어온다)
		//		2인자 : 정점 멤버 갯수(데이터)
		//		3인자 : 자기가 인자로 받을 정점 정보를 포함한 셰이더를 컴파일해서 메모리에 올라간 그 주소를 넘긴다. (패스 정보에서 얻어온다)
		
		//		셰이더를 받아왔으니 내가 1인자로 넘겨준 정점 멤버와 셰이더 인자로 받는 정점 멤버를 비교할 수 있다!
		//		즉, 멤버가 일치해야 Input레이아웃을 만들 수 있고, 인풋레이아웃이 있어야 그릴 수 있따. 
		//		완전히 멤버가 같아야만 가능! >> 만약 멤버를 다르면 CreatInpuLayout에서 FAIL을 리턴한다!(검증 과정이라고도 할 수 있따)
		//		그래서 9과 비교해서 유연성이 사라졌따.
		//		9에서는 패스별로 다른 정점을 구성해도 쓰는대는 상관없다.
		//		하지만 지금 구조에서 패스만큼 폴문을 돌면서 레이아웃 생성 함수에 버텍스 각각 다른 정보를 던져주는게 힘들다
		if (FAILED(m_pDevice->CreateInputLayout(pElements, iNumElements, PassInfo.pIAInputSignature, PassInfo.IAInputSignatureSize, &PassDesc.pInputLayout)))
			return E_FAIL;


		m_Passes.push_back(PassDesc);
	}

	return S_OK;
}

HRESULT CShader::Initialize(void * pArg)
{
	return S_OK;
}

CShader * CShader::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const _tchar * pShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElements)
{
	CShader*			pInstance = new CShader(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pShaderFilePath, pElements, iNumElements)))
	{
		MSG_BOX(TEXT("Failed To Created : CShader"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CShader::Clone(void * pArg)
{
	CShader*			pInstance = new CShader(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CShader"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShader::Free()
{
	__super::Free();

	for (auto& PassDesc : m_Passes)
	{
		Safe_Release(PassDesc.pInputLayout);
	}

	m_Passes.clear();

	Safe_Release(m_pEffect);
}

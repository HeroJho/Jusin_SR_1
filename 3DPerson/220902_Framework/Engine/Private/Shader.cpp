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

	// ����Ʈ �İ�ü�� �����.
	if (FAILED(D3DX11CompileEffectFromFile(pShaderFilePath, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, iFlag, 0, m_pDevice, &m_pEffect, nullptr)))
		return E_FAIL;


	// �ش� ����Ʈ �İ�ü�� ��ũ��Ŀ �İ�ü�� ���´�.(�н��� ������ ���ؼ�)
	// �̶� ��ũ��Ŀ�� ���ǵ� ������� �ε����� �Ű�����. (��κ� �ϳ��� ��ũ��Ŀ�� ���� ������ ��� 0�� �ھƵд�.)
	ID3DX11EffectTechnique*	pTechnique = m_pEffect->GetTechniqueByIndex(0);
	if (nullptr == pTechnique)
		return E_FAIL;


	// ��ũ��Ŀ�� ������ ���´�.
	// �� �����ȿ� ��ũ��Ŀ�� �������ִ� �н� �İ�ü�� �н��� ������ �ִ�.
	D3DX11_TECHNIQUE_DESC		TechniqueDesc;
	ZeroMemory(&TechniqueDesc, sizeof(D3DX11_TECHNIQUE_DESC));

	if (FAILED(pTechnique->GetDesc(&TechniqueDesc)))
		return E_FAIL;


	// �н��� ������ŭ �ݺ����� ����.
	for (_uint i = 0; i < TechniqueDesc.Passes; ++i)
	{
		// ��ũ��Ŀ�� i��° �н� �İ�ü�� ���´�.
		PASSDESC				PassDesc;
		ZeroMemory(&PassDesc, sizeof(PASSDESC));
				
		PassDesc.pPass = pTechnique->GetPassByIndex(i);

		// i��° �н��� ������ �о�´�.
		D3DX11_PASS_DESC		PassInfo;

		PassDesc.pPass->GetDesc(&PassInfo);

		//  i��° �н��� ��ǲ���̾ƿ��� ����� �ش�
		//		1���� : ���̴��� �׸��� ���� ���̴��� ������ ���� ����(�迭�� �Ѿ�´�)
		//		2���� : ���� ��� ����(������)
		//		3���� : �ڱⰡ ���ڷ� ���� ���� ������ ������ ���̴��� �������ؼ� �޸𸮿� �ö� �� �ּҸ� �ѱ��. (�н� �������� ���´�)
		
		//		���̴��� �޾ƿ����� ���� 1���ڷ� �Ѱ��� ���� ����� ���̴� ���ڷ� �޴� ���� ����� ���� �� �ִ�!
		//		��, ����� ��ġ�ؾ� Input���̾ƿ��� ���� �� �ְ�, ��ǲ���̾ƿ��� �־�� �׸� �� �ֵ�. 
		//		������ ����� ���ƾ߸� ����! >> ���� ����� �ٸ��� CreatInpuLayout���� FAIL�� �����Ѵ�!(���� �����̶�� �� �� �ֵ�)
		//		�׷��� 9�� ���ؼ� �������� �������.
		//		9������ �н����� �ٸ� ������ �����ص� ���´�� �������.
		//		������ ���� �������� �н���ŭ ������ ���鼭 ���̾ƿ� ���� �Լ��� ���ؽ� ���� �ٸ� ������ �����ִ°� �����
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

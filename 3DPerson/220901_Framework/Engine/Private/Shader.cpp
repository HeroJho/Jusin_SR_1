#include "..\Public\Shader.h"

CShader::CShader(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent(pDevice, pContext)
{
}

CShader::CShader(const CShader & rhs)
	: CComponent(rhs)
	, m_pEffect(rhs.m_pEffect)
{
	Safe_AddRef(m_pEffect);
}

HRESULT CShader::Initialize_Prototype(const _tchar * pShaderFilePath)
{
	_uint		iFlag = 0;

#ifdef _DEBUG
	iFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else 
	iFlag = D3DCOMPILE_OPTIMIZATION_LEVEL1;
#endif	

	if (FAILED(D3DX11CompileEffectFromFile(pShaderFilePath, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, iFlag, 0, m_pDevice, &m_pEffect, nullptr)))
		return E_FAIL;


	return S_OK;
}

HRESULT CShader::Initialize(void * pArg)
{
	return S_OK;
}

CShader * CShader::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const _tchar * pShaderFilePath)
{
	CShader*			pInstance = new CShader(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pShaderFilePath)))
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

	Safe_Release(m_pEffect);
}

#include "Shader.h"

CShader::CShader(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent{ pGraphic_Device }
{
}

CShader::CShader(const CShader& Prototype)
	: CComponent{ Prototype }
	, m_pEffect{ Prototype.m_pEffect }
{
	Safe_AddRef(m_pEffect);
}

HRESULT CShader::Initialize_Prototype(const _tchar* pShaderFilePath)
{
	if (FAILED(D3DXCreateEffectFromFile(m_pGraphic_Device, pShaderFilePath, nullptr, nullptr, 0, nullptr, &m_pEffect, nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CShader::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CShader::Bind_Texture(D3DXHANDLE hParameter, LPDIRECT3DBASETEXTURE9 pTexture)
{

	return m_pEffect->SetTexture(hParameter, pTexture);
}

HRESULT CShader::Bind_Matrix(D3DXHANDLE hParameter, const _float4x4* pMatrix)
{
	return m_pEffect->SetMatrix(hParameter, pMatrix);

}

void CShader::Begin(_uint iPassIndex)
{
	m_pEffect->Begin(nullptr, 0);
	m_pEffect->BeginPass(iPassIndex);
}

void CShader::End()
{
	m_pEffect->EndPass();
	m_pEffect->End();
}

HRESULT CShader::Set_Float(const char* pParamName, _float fValue)
{
	{
		if (nullptr == m_pEffect || nullptr == pParamName)
			return E_FAIL;

		return m_pEffect->SetFloat(pParamName, fValue);
	}
}

HRESULT CShader::Set_Vector(const char* pParamName, const _float4* pValue)
{
	if (nullptr == m_pEffect || nullptr == pParamName || nullptr == pValue)
		return E_FAIL;

	return m_pEffect->SetVector(pParamName, pValue);
}
HRESULT CShader::Set_Vector(const _wstring& strConstantName, const D3DXVECTOR4* pVector)
{
	string strAnsi(strConstantName.begin(), strConstantName.end());
	return m_pEffect->SetVector(strAnsi.c_str(), pVector);
}

HRESULT CShader::Set_Float(const _wstring& strConstantName, _float fValue)
{
	string strAnsi(strConstantName.begin(), strConstantName.end());
	return m_pEffect->SetFloat(strAnsi.c_str(), fValue);
}

HRESULT CShader::Set_Int(const _wstring& strConstantName, _int iValue)
{
	string strAnsi(strConstantName.begin(), strConstantName.end());
	return m_pEffect->SetInt(strAnsi.c_str(), iValue);
}

CShader* CShader::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pShaderFilePath)
{
	CShader* pInstance = new CShader(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(pShaderFilePath)))
	{
		MSG_BOX(TEXT("Failed to Created : CShader"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CShader::Clone(void* pArg)
{
	CShader* pInstance = new CShader(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created : CShader"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShader::Free()
{
	__super::Free();

	Safe_Release(m_pEffect);
}

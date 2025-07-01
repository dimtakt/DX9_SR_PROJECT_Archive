#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CShader final : public CComponent
{
private:
	CShader(LPDIRECT3DDEVICE9 pGraphic_Device);
	CShader(const CShader& Prototype);
	virtual ~CShader() = default;
public:
	virtual HRESULT Initialize_Prototype(const _tchar* pShaderFilePath);
	virtual HRESULT Initialize(void* pArg) override;

	HRESULT Bind_Texture(D3DXHANDLE hParameter, LPDIRECT3DBASETEXTURE9 pTexture);
	HRESULT Bind_Matrix(D3DXHANDLE hParameter, const _float4x4* pMatrix);


public:
	void Begin(_uint iPassIndex);
	void End();


public:
	HRESULT Set_Float(const char* pParamName, _float fValue);
	HRESULT Set_Vector(const char* pParamName, const _float4* pValue);
	HRESULT Set_Vector(const _wstring& strConstantName, const D3DXVECTOR4* pVector);
	HRESULT Set_Float(const _wstring& strConstantName, _float fValue);
	HRESULT Set_Int(const _wstring& strConstantName, _int iValue);
	

private:
	LPD3DXEFFECT				m_pEffect = { nullptr };

public:
	static CShader* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pShaderFilePath);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

};


END
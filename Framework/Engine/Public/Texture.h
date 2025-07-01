#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTexture final : public CComponent
{
private:
	CTexture(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTexture(const CTexture& Prototype);
	virtual ~CTexture() = default;

public:
	HRESULT Initialize_Prototype(TEXTURE eType, const _tchar* pTextureFilePath, _uint iNumTextures);
	HRESULT Initialize(void* pArg);
	HRESULT Bind_Texture(_uint iTextureIndex = 0);
	HRESULT Bind_Texture(class CShader* pShader, D3DXHANDLE hParameters, _uint iTextureIndex = 0);
	LPDIRECT3DTEXTURE9 Get_Textures(_uint iIndex);

public:
	_uint Get_NumTextures() { return m_iNumTextures; }
	_uint Get_NumBindTexture() { return m_iNumBindTexture; }

private:
	_uint									m_iNumTextures = {};
	vector<IDirect3DBaseTexture9*>			m_Textures;
	_uint									m_iNumBindTexture = {};

public:
	static CTexture* Create(LPDIRECT3DDEVICE9 pGraphic_Device, TEXTURE eType, const _tchar* pTextureFilePath, _uint iNumTextures);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;


};

END
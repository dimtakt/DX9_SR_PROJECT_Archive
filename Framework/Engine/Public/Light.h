#pragma once

#include "Base.h"

BEGIN(Engine)

class CLight : public CBase
{
private:
	CLight(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLight() = default;

public:
	HRESULT	Ready_Light(const D3DLIGHT9* pLightInfo, const _uint& iIndex);

private:
	LPDIRECT3DDEVICE9		m_pGraphic_Device;

	D3DLIGHT9				m_LightDesc;
	_uint					m_iIndex;

public:
	static CLight* Create(LPDIRECT3DDEVICE9 pGraphic_Device,
		const D3DLIGHT9* pLightInfo,
		const _uint& iIndex);
	virtual void Free();
};

END
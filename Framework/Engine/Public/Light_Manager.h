#pragma once

#include "Base.h"

BEGIN(Engine)

class CLight_Manager final : public CBase
{
private:
	CLight_Manager(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLight_Manager() = default;

public:
	HRESULT	Ready_Light(const D3DLIGHT9* pLightInfo,
						const _uint& iIndex);

private:
	LPDIRECT3DDEVICE9		m_pGraphic_Device;
	list<class CLight*>		m_LightList;

public:
	static CLight_Manager* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void			Free();

};

END
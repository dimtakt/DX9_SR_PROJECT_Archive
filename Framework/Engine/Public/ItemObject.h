#pragma once
#include "GameObject.h"
BEGIN(Engine)
class ENGINE_DLL CItemObject abstract : public CGameObject
{
protected:
							CItemObject(LPDIRECT3DDEVICE9 pGraphic_Device);
							CItemObject(const CItemObject& Prototype);
	virtual					~CItemObject() = default;

public:
	virtual HRESULT			Initialize(void* pArg = nullptr);
	virtual HRESULT			Initialize_Prototype();
	virtual void			Priority_Update(_float fTimeDelta);
	virtual void			Update(_float fTimeDelta);
	virtual void			Late_Update(_float fTimeDelta);
	virtual HRESULT			Render();

protected:
	_uint					m_iItemID = {};
	_uint					m_iItemType = {};
	_uint					m_iRarity = {};
	_wstring				m_szName = {};
	_wstring				m_szDescription = {};
	
public:
	virtual CGameObject*	Clone(void* pArg) = 0;
	virtual void			Free();
};
END

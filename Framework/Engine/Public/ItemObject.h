#pragma once
#include "UIObject.h"
BEGIN(Engine)
class ENGINE_DLL CItemObject abstract : public CUIObject
{
public:
		typedef struct tagItemObjectDesc
	{
		_uint					iItemID;
		_uint					iItemType;
		_uint					iRarity;
		_wstring				szName;
		_wstring				szDescription;
		_int					iItemTextureID;
		_int					iiValue;
	}ITEMOBJECT_DESC;

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
	
	class CVIBuffer_Rect*	m_pVIBufferCom = { nullptr };
	class CTexture*			m_pTextureCom = { nullptr };

public:
	virtual CGameObject*	Clone(void* pArg) = 0;
	virtual void			Free();
};
END

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
		_int					iItemValue;		//0이면 등급 없음
		_float					fAngle;
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

public:
	tagItemObjectDesc* Item_Info();

protected:
	_uint					m_iItemID = {};
	_uint					m_iItemType = {};
	_uint					m_iRarity = {};
	_uint					m_iItemTextureID = {};
	_uint					m_iItemValue = {};
	_wstring				m_szName = {};
	_wstring				m_szDescription = {};

	_float					m_fAngle = {};

	class CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	class CTexture* m_pTextureCom = { nullptr };

public:
	virtual CGameObject*	Clone(void* pArg) = 0;
	virtual void			Free();
};
END

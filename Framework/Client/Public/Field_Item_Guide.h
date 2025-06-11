#pragma once
#include "Client_Defines.h"
#include "UIObject.h"
#include "Engine_Defines.h"
#include "Client_Defines_Item.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
END

BEGIN(Client)
class CField_Item_Guide final : public CUIObject
{
private:
								CField_Item_Guide(LPDIRECT3DDEVICE9 pGraphic_Device);
								CField_Item_Guide(const CField_Item_Guide& Prototype);
	virtual						~CField_Item_Guide() = default;

public:
	virtual HRESULT				Initialize_Prototype(LEVEL eLevel);
	virtual HRESULT				Initialize(void* pArg) override;
	virtual void				Priority_Update(_float fTimeDelta) override;
	virtual void				Update(_float fTimeDelta) override;
	virtual void				Late_Update(_float fTimeDelta);
	virtual HRESULT				Render() override;

public:
	void						FontRenderSwitch(_bool bRender) { m_bFontRender = bRender; }

private:
	CVIBuffer_Rect*				m_pVIBufferCom = { nullptr };
	CTexture*					m_pTextureCom = { nullptr };

	LEVEL						m_eLevel = {};

	_uint						m_iItemID = {};
	_int						m_iWinPosX = {};
	_int						m_iWinPosY = {};

	_bool						m_bFontRender = { false };

private:
	HRESULT						Ready_Components();

	HRESULT						Ready_ChildPrototype(LEVEL eLevel);
	HRESULT						Ready_Children();

	void						Target_Pos();
	void						Render_Font();
	void						Rneder_Font_NotBuy();
public:
	static CField_Item_Guide*	Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel);
	virtual CGameObject*		Clone(void* pArg) override;
	virtual void				Free() override;
};
END


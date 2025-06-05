#pragma once
#include "Client_Defines.h"
#include "UIObject.h"
#include "Inven_Slot.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
class CItemObject;
END

BEGIN(Client)
class CInventory final : public CUIObject
{
private:
	CInventory(LPDIRECT3DDEVICE9 pGraphic_Device);
	CInventory(const CInventory& Prototype);
	virtual						~CInventory() = default;
public:
	virtual HRESULT				Initialize_Prototype(LEVEL eLevel);
	virtual HRESULT				Initialize(void* pArg) override;
	virtual void				Priority_Update(_float fTimeDelta) override;
	virtual void				Update(_float fTimeDelta) override;
	virtual void				Late_Update(_float fTimeDelta) override;
	virtual HRESULT				Render() override;

public:
	void						UI_Switch();
	void						Add_Item_Inven(_uint ItemIndex);

	void						Open_UI(_float fX, _float fY);
	void						Close_UI();
	void						Push_Item_Slot(CItem_Base* pItem, _uint iCount);

private:
	CVIBuffer_Rect*				m_pVIBufferCom = { nullptr };
	CTexture*					m_pTextureCom = { nullptr };
	LEVEL						m_eLevel = {};
	_bool						m_bIsOpen = {};
	vector<CInven_Slot*>		m_vecInventory = {};

private:
	void						Set_Grade();

private:
	HRESULT						Ready_Components();

	HRESULT						Ready_ChildPrototype(LEVEL eLevel);
	HRESULT						Ready_Children();

public:
	static CInventory*			Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel);
	virtual CGameObject*		Clone(void* pArg) override;
	virtual void				Free() override;

};
END

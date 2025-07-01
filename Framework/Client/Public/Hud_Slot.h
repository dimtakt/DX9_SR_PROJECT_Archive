#pragma once
#include "UIObject.h"
#include "Client_Defines.h"
#include "Hud_Quick_Slot.h"
BEGIN(Client)
class CHud_Slot final : public CUIObject
{
private:
									CHud_Slot(LPDIRECT3DDEVICE9 pGraphic_Device);
									CHud_Slot(const CHud_Slot& Prototype);
	virtual							~CHud_Slot() = default;

public:
	virtual HRESULT					Initialize_Prototype(LEVEL eLevel);
	virtual HRESULT					Initialize(void* pArg) override;
	virtual void					Priority_Update(_float fTimeDelta) override;
	virtual void					Update(_float fTimeDelta) override;
	virtual void					Late_Update(_float fTimeDelta) override;
	virtual HRESULT					Render() override;

public:
	void							SlotToItem(CItem_Base* pItem);
	void							Subscribe_Item(CItem_Base* pItem);

private:
	LEVEL							m_eLevel = {};
	vector<CHud_Quick_Slot*>		m_vecQuickSlot;

private:
	HRESULT							Ready_Components();

	HRESULT							Ready_ChildPrototype(LEVEL eLevel);
	HRESULT							Ready_Children();

	void							Selete_Slot();
public:
	static CHud_Slot*				Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel);
	virtual CGameObject*			Clone(void* pArg) override;
	virtual void					Free() override;
};
END

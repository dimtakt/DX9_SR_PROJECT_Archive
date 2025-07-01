#pragma once
#include "Client_Defines.h"
#include "Client_Defines_Item.h"
#include "Button.h"
#include "ItemObject.h"
#include "Item_Base.h"
BEGIN(Client)
class CHud_Quick_Slot final : public CButton
{
private:
									CHud_Quick_Slot(LPDIRECT3DDEVICE9 pGraphic_Device);
									CHud_Quick_Slot(const CHud_Quick_Slot& Prototype);
	virtual							~CHud_Quick_Slot() = default;

public:
	virtual HRESULT					Initialize_Prototype(LEVEL eLevel);
	virtual HRESULT					Initialize(void* pArg) override;
	virtual void					Priority_Update(_float fTimeDelta) override;
	virtual void					Update(_float fTimeDelta) override;
	virtual void					Late_Update(_float fTimeDelta) override;
	virtual HRESULT					Render() override;
	
public:
	CItem_Base*						Get_Itme() {return m_pSlotItem;}
	void							Push_Item(CItem_Base* pItem) { m_pSlotItem = pItem; }
	void							Selete_Slot(_int iKey);
	void							Release_Item() { m_pSlotItem = nullptr; }

private:
	LEVEL							m_eLevel = {};
	_int							m_iIndex = {};
	_tchar							m_szVkKey = {};
	_bool							m_bSelete = { false };

	CItem_Base*						m_pSlotItem = { nullptr };

	_bool							m_bEating = { false };
	_bool							m_bEatPotion = { false };
	_float							m_fEatSpeed = {};

	_float							m_fItemCulCool = {};
	_float							m_fItemCooltime = {};

	_float							m_fPotionCulCool = {};
	_float							m_fPotionCoolTime = {};
	_int							m_iItemCount = {};

	EVENTDATA						pEvent{};
private:
	HRESULT							Ready_Components();

	HRESULT							Ready_ChildPrototype(LEVEL eLevel);
	HRESULT							Ready_Children();

	void							Render_Font();
	void							ItemCount();
	_bool							Use_Potion();
public:
	static CHud_Quick_Slot*			Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel);
	virtual CGameObject*			Clone(void* pArg) override;
	virtual void					Free() override;
};

END
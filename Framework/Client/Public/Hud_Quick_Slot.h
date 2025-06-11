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

private:
	LEVEL							m_eLevel = {};
	_int							m_iIndex = {};

	_bool							m_bIsPick = {};
	_uint							m_iSlotIndex = {};
	_int							m_iSlotItem_Tex = {};

	CItem_Base*						m_pSlotItem = { nullptr };
	CItem_Base*						m_pOldSlotItem = { nullptr };
	
private:
	HRESULT							Ready_Components();

	HRESULT							Ready_ChildPrototype(LEVEL eLevel);
	HRESULT							Ready_Children();
public:
	static CHud_Quick_Slot*			Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel);
	virtual CGameObject*			Clone(void* pArg) override;
	virtual void					Free() override;
};

END
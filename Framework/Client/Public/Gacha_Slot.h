#pragma once
#include "Client_Defines.h"
#include "Client_Defines_Item.h"
#include "Button.h"
#include "ItemObject.h"
#include "Item_Base.h"
BEGIN(Client)
class CGacha_Slot final : public CButton
{
private:
									CGacha_Slot(LPDIRECT3DDEVICE9 pGraphic_Device);
									CGacha_Slot(const CGacha_Slot& Prototype);
	virtual							~CGacha_Slot() = default;

public:
	virtual HRESULT					Initialize_Prototype(LEVEL eLevel);
	virtual HRESULT					Initialize(void* pArg) override;
	virtual void					Priority_Update(_float fTimeDelta) override;
	virtual void					Update(_float fTimeDelta) override;
	virtual void					Late_Update(_float fTimeDelta) override;
	virtual HRESULT					Render() override;

public:
	void							Push_Item(CItem_Base* pItem) { m_pSlotItem = pItem; }
private:
	LEVEL							m_eLevel = {};
	_bool							m_bIsPick = {};
	_uint							m_iSlotIndex = {};
	_int							m_iSlotItem_Tex = {};

	CItem_Base*						m_pSlotItem = { nullptr };

private:
	void							Item_Selete();

private:
	HRESULT							Ready_Components();

	HRESULT							Ready_ChildPrototype(LEVEL eLevel);
	HRESULT							Ready_Children();
public:
	static CGacha_Slot*				Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel);
	virtual CGameObject*			Clone(void* pArg) override;
	virtual void					Free() override;
};

END
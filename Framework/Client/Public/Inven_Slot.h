#pragma once
#include "Client_Defines.h"
#include "Client_Defines_Item.h"
#include "Button.h"
#include "ItemObject.h"
#include "Item_Base.h"
BEGIN(Client)
class CInven_Slot final : public CButton
{
public:
	typedef struct tagInvenSlotDesc : public UIOBJECT_DESC
	{
		_uint		iSlotInedx;

	}INVEN_SLOT_DESC;
private:
									CInven_Slot(LPDIRECT3DDEVICE9 pGraphic_Device);
									CInven_Slot(const CInven_Slot& Prototype);
	virtual							~CInven_Slot() = default;

public:
	virtual HRESULT					Initialize_Prototype(LEVEL eLevel);
	virtual HRESULT					Initialize(void* pArg) override;
	virtual void					Priority_Update(_float fTimeDelta) override;
	virtual void					Update(_float fTimeDelta) override;
	virtual void					Late_Update(_float fTimeDelta) override;
	virtual HRESULT					Render() override;

public:
	void							Add_Item(CItem_Base* pItem);
	void							Release_Item() { m_pSlotItem = nullptr; }

	const _int						Get_SlotGrade() { return m_iSlotGradeCount; }
	virtual void					Push_Item(CItemObject* pItem) override;
	virtual void					Push_Item_Count(_uint iItemCount) override { m_iItemCount = iItemCount; }
	virtual void					IsPick_off() override { m_bIsPick = false; }
	
	void							Reset_GradeCount() { m_iSlotGradeCount= 0; }

	CItem_Base*						Pop_Item();
	_uint							Pop_Item_Count() { return m_iItemCount; }
	_int							Slot_Info(ITEM_INFO eInfo);
	void							Add_GradeCount(_int iValue);
private:
	LEVEL							m_eLevel = {};
	_bool							m_bIsOver = {};
	_bool							m_bIsPick = {};
	_uint							m_iSlotIndex = {};
	CItem_Base*						m_pOldSlotItem = { nullptr };
	CItem_Base*						m_pSlotItem = { nullptr };

	_uint							m_iSlotItem_Tex = {};		//아이템 텍스처
	_int							m_iSlotItem_MaxGrade = {};	//아이템 최대 강화 수치
	_int							m_iSlotGradeCount = {};		//현재 슬롯에 아이템 강화 수치

	ITEM_TYPE						m_eItemType = {};			//현재 슬롯 아이템 타입
	_int							m_iItemValue = {};			//현재 슬롯 아이템이 가진 값

	_float							m_fItem_Angle = {};
	_uint							m_iItemCount = {};

	
private:
	void							Setting_Item();
	void							Item_Selete();
	

private:
	HRESULT							Ready_Components();

	HRESULT							Ready_ChildPrototype(LEVEL eLevel);
	HRESULT							Ready_Children();

	void							Render_Font();
public:
	static CInven_Slot* Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void					Free() override;
};

END
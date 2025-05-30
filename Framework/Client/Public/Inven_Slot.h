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
	void							Add_Item(CItem_Base* pItem) { m_pSlotItem = pItem, m_iItemCount += 1; }
	void							Release_Item() { m_pSlotItem = nullptr; }

	_bool							IsKey_Down_Check();
	_bool							IsKey_Up_Check();

	CItem_Base*						Pop_Item();
	void							Push_Item(CItem_Base* pItem);
	_uint							Pop_Item_Count() { return m_iItemCount; }
	void							Push_Item_Count(_uint iItemCount) { m_iItemCount = iItemCount; }
	void							ItemRender();
	_int							Slot_Info(ITEM_INFO eInfo);
	void							Add_GradeCount(_int iValue);

private:
	LEVEL							m_eLevel = {};
	_bool							m_bIsOver = {};

	_uint							m_iSlotIndex = {};
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

private:
	HRESULT							Ready_Components();
	void							SetUp_RenderState();
	void							Reset_RenderState();

	HRESULT							Ready_ChildPrototype(LEVEL eLevel);
	HRESULT							Ready_Children();

	void							Render_Font();
public:
	static CInven_Slot* Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void					Free() override;
};

END
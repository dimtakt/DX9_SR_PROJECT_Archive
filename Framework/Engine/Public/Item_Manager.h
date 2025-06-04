#pragma once
#include "Base.h"

BEGIN(Engine)
class CItem_Manager final : public CBase
{
private:
									CItem_Manager(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual							~CItem_Manager() = default;

public:
	HRESULT							Initialize();
	
	class CItemObject*				Pop_Item();
	class CButton*					Pop_Slot();
	_uint							Pop_Item_Count();
	void							Pick_ItemSlot(class CItemObject* pPickItem, class CButton* pSlot, _uint iItemCount);
	void							Pick_Reset();

public:
	class CItemObject*				Get_ItemObject(_uint iIndex);	
	CBase*							find_ItemObject(_uint iIndex);
	
public:
	HRESULT							Setting_Item(void* pArg, _uint iMaxItemIndex, _uint iLevelIndex, const _wstring& strItemBaseTag);

private:
	LPDIRECT3DDEVICE9				m_pGraphic_Device = { nullptr };
	class CGameInstance*			m_pGameInstance = { nullptr };

	_uint							m_iLevelIndex = {};
	_wstring						m_strItemBaseTag;
	vector<class CItemObject*>		m_ItemObjects;
	
	_uint							m_iItemCount = {};
	class CItemObject*				m_pPickItem = { nullptr };
	class CButton*					m_pPickSlot = { nullptr };
public:
	static CItem_Manager*			Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void					Free() override;
};
END

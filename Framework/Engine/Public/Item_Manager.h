#pragma once
#include "Base.h"

BEGIN(Engine)
class CItem_Manager final : public CBase
{
private:
								CItem_Manager(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual						~CItem_Manager() = default;

public:
	HRESULT						Initialize();

	class CItemObject*			Get_ItemObject(_uint iIndex);	
	CBase*						find_ItemObject(_uint iIndex);
	
public:
	HRESULT						Setting_Item(void* pArg, _uint iMaxItemIndex, _uint iLevelIndex, const _wstring& strItemBaseTag);

private:
	LPDIRECT3DDEVICE9			m_pGraphic_Device = { nullptr };
	class CGameInstance*		m_pGameInstance = { nullptr };

	vector<class CItemObject*>	m_ItemObjects;
public:
	static CItem_Manager*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void				Free() override;
};
END

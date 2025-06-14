#include "Item_Manager.h"
#include "GameInstance.h"

#include "ItemObject.h"
#include "Button.h"
CItem_Manager::CItem_Manager(LPDIRECT3DDEVICE9 pGraphic_Device) : m_pGraphic_Device(pGraphic_Device), m_pGameInstance{ CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pGraphic_Device);
	Safe_AddRef(m_pGameInstance);
}

HRESULT CItem_Manager::Initialize()
{
	m_mapItemMaxCool.emplace(TEXT("Snow Hamer"), 0.f);
	m_mapItemMaxCool.emplace(TEXT("Yellow Planet"), 0.f);
	m_mapItemMaxCool.emplace(TEXT("Red Planet"), 0.f);
	m_mapItemMaxCool.emplace(TEXT("Bule Planet"), 0.f);
	m_mapItemMaxCool.emplace(TEXT("Ice Bolt"), 0.f);
	m_mapItemMaxCool.emplace(TEXT("Lightning Bolt"), 0.f);
	m_mapItemMaxCool.emplace(TEXT("Projection Sword"), 0.f);

	m_mapItemCulCool.emplace(TEXT("Snow Hamer"), 0.f);
	m_mapItemCulCool.emplace(TEXT("Yellow Planet"), 0.f);
	m_mapItemCulCool.emplace(TEXT("Red Planet"), 0.f);
	m_mapItemCulCool.emplace(TEXT("Bule Planet"), 0.f);
	m_mapItemCulCool.emplace(TEXT("Ice Bolt"), 0.f);
	m_mapItemCulCool.emplace(TEXT("Lightning Bolt"), 0.f);
	m_mapItemCulCool.emplace(TEXT("Projection Sword"), 0.f);

	return S_OK;
}

void CItem_Manager::Update()
{
	if (m_pGameInstance->IsKeyUp(VK_LBUTTON) && m_pPickSlot != nullptr)
	{
		m_pPickSlot->IsPick_off();
		Pick_Reset();
	}
}

CItemObject* CItem_Manager::Pop_Item()
{
	return m_pPickItem;
}

CButton* CItem_Manager::Pop_Slot()
{
	return m_pPickSlot;
}

_uint CItem_Manager::Pop_Item_Count()
{
	return m_iItemCount;
}

const _uint CItem_Manager::Pop_ISlot_Type()
{
	return m_iSlotType;
}

void CItem_Manager::Pick_ItemSlot(CItemObject* pPickItem, CButton* pSlot, _uint iItemCount, _uint iSlottype)
{
	m_pPickItem = pPickItem;
	m_pPickSlot = pSlot;
	m_iItemCount = iItemCount;
	m_iSlotType = iSlottype;
}

void CItem_Manager::Pick_Reset()
{
	m_pPickItem = nullptr;
	m_pPickSlot = nullptr;
	m_iItemCount = 0;
	m_iSlotType = 0;
}

void CItem_Manager::Item_CulCool(_wstring szEffectTag, _float fCulcool)
{
	m_mapItemCulCool.find(szEffectTag)->second = fCulcool;
}

void CItem_Manager::Item_MaxCool(_wstring szEffectTag, _float fMaxcool)
{
	m_mapItemMaxCool.find(szEffectTag)->second = fMaxcool;
}

const _float CItem_Manager::Get_ItemCool(bool bMaxCool, _wstring szEffectTag)
{
	if(bMaxCool)
		return m_mapItemMaxCool.find(szEffectTag)->second;
	else
		return m_mapItemCulCool.find(szEffectTag)->second;
}

CItemObject* CItem_Manager::Get_ItemObject(_uint iIndex, _bool isInven)
{
	if (isInven)
		if (m_ItemObjects[iIndex]->Item_Info()->iItemType != 2)
			m_vecAcquiredItems.push_back(m_ItemObjects[iIndex]->Item_Info()->iItemID);

	CItemObject::ITEMOBJECT_DESC pDesc = *m_ItemObjects[iIndex]->Item_Info();
	
	CItemObject* pItemObject = dynamic_cast<CItemObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, m_iLevelIndex, m_strItemBaseTag, &pDesc));
	return pItemObject;
}

CBase* CItem_Manager::find_ItemObject(_uint iIndex)
{
	return m_ItemObjects[iIndex];
}

HRESULT CItem_Manager::Setting_Item(void* pArg, _uint MaxItemIndex, _uint iLevelIndex, const _wstring& strItemBaseTag)
{
	if (pArg == nullptr)
	{
		MSG_BOX(TEXT("Failed to Set : CItem_Manager"));
		return E_FAIL;
	}

	CItemObject* pItemObject = nullptr;
	CItemObject::ITEMOBJECT_DESC* pDesc = static_cast<CItemObject::ITEMOBJECT_DESC*>(pArg);

	
	//CItemObject::ITEMOBJECT_DESC* pDesc = static_cast<CItemObject::ITEMOBJECT_DESC*>(pArg);
	
	for (_int i = 0; i < MaxItemIndex; ++i)
	{
		pItemObject = dynamic_cast<CItemObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, iLevelIndex, strItemBaseTag, &pDesc[i]));
		if (nullptr == pItemObject)
		{
			MSG_BOX(TEXT("Failed to Set : CItem_Manager"));
			return E_FAIL;
		}
		m_ItemObjects.push_back(pItemObject);
	}

	m_iLevelIndex = iLevelIndex;
	m_strItemBaseTag = strItemBaseTag;

	return S_OK;
}

CItem_Manager* CItem_Manager::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CItem_Manager* pInstance = new CItem_Manager(pGraphic_Device);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CItem_Manager"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CItem_Manager::Free()
{
	__super::Free();

	m_mapItemMaxCool.clear();
	m_mapItemCulCool.clear();

	for (auto& pItemObject : m_ItemObjects)
		Safe_Release(pItemObject);
	m_ItemObjects.clear();

	m_pPickItem = nullptr;
	Safe_Release(m_pGraphic_Device);
	//Safe_Release(m_pGameInstance);

}

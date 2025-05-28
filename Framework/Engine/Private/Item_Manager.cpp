#include "Item_Manager.h"
#include "GameInstance.h"

#include "ItemObject.h"

CItem_Manager::CItem_Manager(LPDIRECT3DDEVICE9 pGraphic_Device) : m_pGraphic_Device(pGraphic_Device), m_pGameInstance{ CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pGraphic_Device);
	Safe_AddRef(m_pGameInstance);
}

HRESULT CItem_Manager::Initialize()
{
	return S_OK;
}

CItemObject* CItem_Manager::Get_ItemObject(_uint iIndex)
{
	return m_ItemObjects[iIndex];
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

	for (auto& pItemObject : m_ItemObjects)
		Safe_Release(pItemObject);
	m_ItemObjects.clear();

	Safe_Release(m_pGraphic_Device);
	Safe_Release(m_pGameInstance);
}

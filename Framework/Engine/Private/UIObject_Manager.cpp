#include "UIObject_Manager.h"
#include "GameInstance.h"
#include "UIObject.h"
CUIObject_Manager::CUIObject_Manager() : m_pGameInstance{ CGameInstance::GetInstance() }
{
}

HRESULT CUIObject_Manager::Initialize(_uint iNumLevels)
{
    m_pUIObjects = new map<const _wstring, class CUIObject*>[iNumLevels];

    m_iNumLevels = iNumLevels;

    return S_OK;
}

HRESULT CUIObject_Manager::Add_UIObject(_uint iLevelIndex, const _wstring& strUITag, CUIObject* pUIObj)
{
    if (pUIObj == nullptr)
        return E_FAIL;

    auto iter = m_pUIObjects[iLevelIndex].find(strUITag);

    if (iter != m_pUIObjects[iLevelIndex].end())
        return E_FAIL;

    m_pUIObjects[iLevelIndex].emplace(strUITag, pUIObj);
    Safe_AddRef(pUIObj);
    return S_OK;
}

void CUIObject_Manager::Update_On(_uint iLevelIndex, const _wstring& strUITag)
{
    auto iter = m_pUIObjects[iLevelIndex].find(strUITag);

    if (iter == m_pUIObjects[iLevelIndex].end())
        return;

    iter->second->Update_On();
}

void CUIObject_Manager::Update_Off(_uint iLevelIndex, const _wstring& strUITag)
{
    auto iter = m_pUIObjects[iLevelIndex].find(strUITag);

    if (iter == m_pUIObjects[iLevelIndex].end())
        return;

    iter->second->Update_Off();
}

void CUIObject_Manager::All_Update_On()
{
    for (_int i = 0; i < m_iNumLevels; ++i)
    {
        for (auto& Pair : m_pUIObjects[i])
            Pair.second->Update_On();       
    }
}

void CUIObject_Manager::All_Update_Off()
{
    for (_int i = 0; i < m_iNumLevels; ++i)
    {
        for (auto& Pair : m_pUIObjects[i])
            Pair.second->Update_Off();
    }
}

void CUIObject_Manager::Clear_UiObj(_uint iLevelIndex)
{
    if (iLevelIndex >= m_iNumLevels)
        return;

    for (auto& Pair : m_pUIObjects[iLevelIndex])
    {
        Safe_Release(Pair.second);
    }
    m_pUIObjects[iLevelIndex].clear();

}

CUIObject* CUIObject_Manager::Find_UIObj(_uint iLevelIndex, const _wstring& strUITag)
{
    auto iter = m_pUIObjects[iLevelIndex].find(strUITag);

    if(iter == m_pUIObjects[iLevelIndex].end())
        return nullptr;

    return iter->second;
}


CUIObject_Manager* CUIObject_Manager::Create(_uint iNumLevels)
{
    CUIObject_Manager* pInstance = new CUIObject_Manager();

    if (FAILED(pInstance->Initialize(iNumLevels)))
    {
        MSG_BOX(TEXT("Failed to Created : CUIObject_Manager"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CUIObject_Manager::Free()
{
    __super::Free();

    for (_int i = 0; i < m_iNumLevels; ++i)
    {
        for (auto& Pair : m_pUIObjects[i])
            Safe_Release(Pair.second);

        m_pUIObjects[i].clear();
    }

    Safe_Release(m_pGameInstance);
    Safe_Delete_Array(m_pUIObjects);
}

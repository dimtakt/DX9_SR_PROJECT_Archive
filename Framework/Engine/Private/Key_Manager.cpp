#include "Key_Manager.h"

#include "GameInstance.h"

CKey_Manager::CKey_Manager()
{
}

HRESULT CKey_Manager::Initialize()
{
	m_mPrevState.clear();
	m_mCurrState.clear();
	m_mKeyHoldTime.clear();
    m_vTrackingKeys.clear();

	return S_OK;
}

void CKey_Manager::Update(float fDeltaTime)
{
    for (int vk : m_vTrackingKeys)
    {
        bool bCurr = (GetAsyncKeyState(vk) & 0x8000) != 0;

        m_mPrevState[vk] = m_mCurrState[vk];
        m_mCurrState[vk] = bCurr;

        if (bCurr)
            m_mKeyHoldTime[vk] += fDeltaTime;
        else
            m_mKeyHoldTime[vk] = 0.0f;
    }
}

void CKey_Manager::AddTrackingKey(int iKey)
{
    // 중복 방지
    if (std::find(m_vTrackingKeys.begin(), m_vTrackingKeys.end(), iKey) == m_vTrackingKeys.end())
        m_vTrackingKeys.push_back(iKey);
}

bool CKey_Manager::IsKeyDown(int iKey) const
{
    auto itPrev = m_mPrevState.find(iKey);
    auto itCurr = m_mCurrState.find(iKey);
    return itCurr != m_mCurrState.end() && itCurr->second && (!itPrev->second);
}

bool CKey_Manager::IsKeyUp(int iKey) const
{
    auto itPrev = m_mPrevState.find(iKey);
    auto itCurr = m_mCurrState.find(iKey);
    return itCurr != m_mCurrState.end() && !itCurr->second && itPrev->second;
}

bool CKey_Manager::IsKeyHold(int iKey) const
{
    auto itCurr = m_mCurrState.find(iKey);
    return itCurr != m_mCurrState.end() && itCurr->second;
}

float CKey_Manager::GetKeyHoldTime(int iKey) const
{
    auto it = m_mKeyHoldTime.find(iKey);
    if (it != m_mKeyHoldTime.end())
        return it->second;
    return 0.0f;
}

CKey_Manager* CKey_Manager::Create()
{
	CKey_Manager* pInstance = new CKey_Manager();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CKey_Manager"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CKey_Manager::Free()
{
	__super::Free();

}

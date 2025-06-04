#include "Stat_Manager.h"

IMPLEMENT_SINGLETON(CStat_Manager)

CStat_Manager::CStat_Manager()
{
}

HRESULT CStat_Manager::Initialize()
{
    m_fOriginStats[static_cast<int>(STAT_INFO::MAXHP)] = 200.f; // 최대 생명력
    m_fOriginStats[static_cast<int>(STAT_INFO::CULHP)] = 200.f; // 현재 생명력
    m_fOriginStats[static_cast<int>(STAT_INFO::MAXMP)] = 200.f; // 최대 마나
    m_fOriginStats[static_cast<int>(STAT_INFO::CULMP)] = 200.f; // 현재 마나
    m_fOriginStats[static_cast<int>(STAT_INFO::CULDEF)] = 10.f; // 현재 방어력
    m_fOriginStats[static_cast<int>(STAT_INFO::CULDAMAGE)] = 20.f; // 현재 물리 피해
    m_fOriginStats[static_cast<int>(STAT_INFO::CULCRITICAL)] = 5.f; // 현재 치명타 확률
    m_fOriginStats[static_cast<int>(STAT_INFO::CRITICALDAMAGE)] = 10.f; // 현재 치명타 피해
    m_fOriginStats[static_cast<int>(STAT_INFO::MAXDASH)] = 3.f; // 최대 대시 횟수
    m_fOriginStats[static_cast<int>(STAT_INFO::CULDASH)] = 3.f; // 현재 대시 횟수
    m_fOriginStats[static_cast<int>(STAT_INFO::REGENDASH)] = 1.2f; // 대시 회복 속도
    m_fOriginStats[static_cast<int>(STAT_INFO::EXP)] = 0.f; // 현재 경험치
    m_fOriginStats[static_cast<int>(STAT_INFO::MAXSTATPOINT)] = 0.f; // 획득한 재능 포인트
    m_fOriginStats[static_cast<int>(STAT_INFO::CULSTATPOINT)] = 0.f; // 현재 재능 포인트
    m_fOriginStats[static_cast<int>(STAT_INFO::LEVEL)] = 1.f; // 레벨
    m_fOriginStats[static_cast<int>(STAT_INFO::GOLD)] = 0.f; // 보유 골드
    m_fOriginStats[static_cast<int>(STAT_INFO::DICE)] = 3.f; // 주사위

    m_fCurStats[static_cast<int>(STAT_INFO::MAXHP)] = 200.f; // 최대 생명력
    m_fCurStats[static_cast<int>(STAT_INFO::CULHP)] = 200.f; // 현재 생명력
    m_fCurStats[static_cast<int>(STAT_INFO::MAXMP)] = 200.f; // 최대 마나
    m_fCurStats[static_cast<int>(STAT_INFO::CULMP)] = 200.f; // 현재 마나
    m_fCurStats[static_cast<int>(STAT_INFO::CULDEF)] = 10.f; // 현재 방어력
    m_fCurStats[static_cast<int>(STAT_INFO::CULDAMAGE)] = 20.f; // 현재 물리 피해
    m_fCurStats[static_cast<int>(STAT_INFO::CULCRITICAL)] = 5.f; // 현재 치명타 확률
    m_fCurStats[static_cast<int>(STAT_INFO::CRITICALDAMAGE)] = 10.f; // 현재 치명타 피해
    m_fCurStats[static_cast<int>(STAT_INFO::MAXDASH)] = 3.f; // 최대 대시 횟수
    m_fCurStats[static_cast<int>(STAT_INFO::CULDASH)] = 3.f; // 현재 대시 횟수
    m_fCurStats[static_cast<int>(STAT_INFO::REGENDASH)] = 1.2f; // 대시 회복 속도
    m_fCurStats[static_cast<int>(STAT_INFO::EXP)] = 0.f; // 현재 경험치
    m_fCurStats[static_cast<int>(STAT_INFO::MAXSTATPOINT)] = 20.f; // 획득한 재능 포인트
    m_fCurStats[static_cast<int>(STAT_INFO::CULSTATPOINT)] = 20.f; // 현재 재능 포인트
    m_fCurStats[static_cast<int>(STAT_INFO::LEVEL)] = 1.f; // 레벨
    m_fCurStats[static_cast<int>(STAT_INFO::GOLD)] = 0.f; // 보유 골드
    m_fCurStats[static_cast<int>(STAT_INFO::DICE)] = 3.f; // 주사위

    return S_OK;
}

void CStat_Manager::Update(_float fTimeDelta)
{
    if (m_fCurStats[static_cast<int>(STAT_INFO::CULMP)] < m_fCurStats[static_cast<int>(STAT_INFO::MAXMP)])
    {
        m_fCurStats[static_cast<int>(STAT_INFO::CULMP)] += fTimeDelta;
        if (m_fCurStats[static_cast<int>(STAT_INFO::CULMP)] >= m_fCurStats[static_cast<int>(STAT_INFO::MAXMP)])
            m_fCurStats[static_cast<int>(STAT_INFO::CULMP)] == m_fCurStats[static_cast<int>(STAT_INFO::MAXMP)];
    }

    if (m_fCurStats[static_cast<int>(STAT_INFO::CULDASH)] < m_fCurStats[static_cast<int>(STAT_INFO::MAXDASH)])
    {
        m_fCurStats[static_cast<int>(STAT_INFO::CULDASH)] += fTimeDelta * m_fCurStats[static_cast<int>(STAT_INFO::REGENDASH)];
        if (m_fCurStats[static_cast<int>(STAT_INFO::CULDASH)] >= m_fCurStats[static_cast<int>(STAT_INFO::MAXDASH)])
            m_fCurStats[static_cast<int>(STAT_INFO::CULDASH)] == m_fCurStats[static_cast<int>(STAT_INFO::MAXDASH)];
    }
    
}

void CStat_Manager::Cal_Stats(STAT_INFO eStat, float fValue)
{
    m_fCurStats[static_cast<int>(eStat)] += fValue;

    if (eStat == STAT_INFO::EXP)
    {
        if (m_fCurStats[static_cast<int>(eStat)] >= 100.f)
        {
            m_fCurStats[static_cast<int>(eStat)] -= 100.f;
            m_fCurStats[static_cast<int>(STAT_INFO::LEVEL)] += 1.f;
        }

    }
    
}

void CStat_Manager::Interaction_Obj_Stat(GAMEOBJ_TYPE eType)
{
    if (eType == GAMEOBJ_TYPE::EXP)
    {
        m_fCurStats[static_cast<int>(STAT_INFO::LEVEL)] += 1.f;
        m_fCurStats[static_cast<int>(STAT_INFO::EXP)] = 0.f;
    }
    else if (eType == GAMEOBJ_TYPE::HP)
    {
        m_fCurStats[static_cast<int>(STAT_INFO::MAXHP)] += 15.f;
        m_fCurStats[static_cast<int>(STAT_INFO::CULHP)] += 15.f;
    }
}

void CStat_Manager::Reset_CurStats()
{
    for (size_t i = 0; i < static_cast<int>(STAT_INFO::STAT_END); i++)
    {
        m_fCurStats[i] = m_fOriginStats[i];
    }
}

void CStat_Manager::Free()
{
    DestroyInstance();
}

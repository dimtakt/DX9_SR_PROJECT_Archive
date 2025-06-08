#include "Stat_Manager.h"
#include "GameInstance.h"

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
    m_fOriginStats[static_cast<int>(STAT_INFO::EXP)] = 0.f; // 현재 경험치6
    m_fOriginStats[static_cast<int>(STAT_INFO::MAXSTATPOINT)] = 0.f; // 획득한 재능 포인트
    m_fOriginStats[static_cast<int>(STAT_INFO::CULSTATPOINT)] = 0.f; // 현재 재능 포인트
    m_fOriginStats[static_cast<int>(STAT_INFO::LEVEL)] = 1.f; // 레벨
    m_fOriginStats[static_cast<int>(STAT_INFO::GOLD)] = 0.f; // 보유 골드
    m_fOriginStats[static_cast<int>(STAT_INFO::DICE)] = 3.f; // 주사위
    m_fOriginStats[static_cast<int>(STAT_INFO::LEVELUPPOINT)] = 0.f; // 주사위

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
    m_fCurStats[static_cast<int>(STAT_INFO::LEVELUPPOINT)] = 0.f; // 주사위

    m_pGameInstance = CGameInstance::GetInstance();

    Safe_AddRef(m_pGameInstance);

    m_mapHasItem.emplace(TEXT("Snow Hamer"), false);
    m_mapHasItem.emplace(TEXT("Yellow Planet"), false);
    m_mapHasItem.emplace(TEXT("Red Planet"), false);
    m_mapHasItem.emplace(TEXT("Bule Planet"), false);
    m_mapHasItem.emplace(TEXT("Ice Bolt"), false);
    m_mapHasItem.emplace(TEXT("Lightning Bolt"), false);

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
    

    if (m_fCurStats[static_cast<int>(eStat)] <= 0)
        m_fCurStats[static_cast<int>(eStat)] = 0;

    if (eStat == STAT_INFO::EXP)
    {
        if (m_fCurStats[static_cast<int>(eStat)] >= 100.f)
        {
            m_fCurStats[static_cast<int>(eStat)] -= 100.f;
            m_fCurStats[static_cast<int>(STAT_INFO::LEVEL)] += 1.f;
            m_fCurStats[static_cast<int>(STAT_INFO::LEVELUPPOINT)] += 1.f;
        }
    }
}

void CStat_Manager::Set_Stats(STAT_INFO eStat, float fValue)
{
    m_fCurStats[static_cast<int>(eStat)] = fValue;
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

_float CStat_Manager::Get_Player_Damage(DAMAGE eDamage)
{
    _float fDamage = 0.f;
    if (eDamage == DAMAGE::NORMAL)
    {
        fDamage = m_fCurStats[static_cast<int>(STAT_INFO::CULDAMAGE)];
    }
    else if (eDamage == DAMAGE::SPECIAL)
    {
        fDamage = m_fCurStats[static_cast<int>(STAT_INFO::CULDAMAGE)] + 10;
    }
    else if (eDamage == DAMAGE::DASH)
    {
        fDamage = m_fCurStats[static_cast<int>(STAT_INFO::CULDAMAGE)] + 5;
    }
    
    _float fRand = m_pGameInstance->Compute_Random(0, 99);

    if (fRand < m_fCurStats[static_cast<int>(STAT_INFO::CULCRITICAL)])
    {
        fDamage = fDamage * (1.0f + m_fCurStats[static_cast<int>(STAT_INFO::CRITICALDAMAGE)] / 100.0f);;
    }

    return fDamage;
    //return 1000.f;
}

_float CStat_Manager::Get_Monster_Damage(_float fDamage)
{
    return ((100.f - m_fCurStats[static_cast<int>(STAT_INFO::CULDEF)]) / 100.f) * fDamage;;
}


const _bool CStat_Manager::Get_HasItem(_wstring szEffectTag)
{
    auto iter = m_mapHasItem.find(szEffectTag);
    if (iter == m_mapHasItem.end())
        return false;

    return iter->second;
}

void CStat_Manager::HasItem(_wstring szEffectTag, _bool bHasItme)
{
    m_mapHasItem.find(szEffectTag)->second = bHasItme;
}

void CStat_Manager::Free()
{
    Safe_Release(m_pGameInstance);
    DestroyInstance();
}

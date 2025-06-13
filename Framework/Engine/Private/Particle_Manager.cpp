#include "Particle_Manager.h"
#include "GameInstance.h"
#include "Particle.h"

CParticle_Manager::CParticle_Manager()
	: m_pGameInstance{ CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CParticle_Manager::Create_Particle_Fast(_uint iParticleType, _uint iLayerLevelIndex, const _wstring& strLayerTag, _int iOffSetType, _float3 vScale, _bool bUseOrtho)
{
  
    for (_int i = 0; i < 200; i++)
    {
        CParticle::PARTICLE_DESC pDesc = {};

        pDesc.iType = iParticleType;
        pDesc.vPos = { 12580.f, 12580.f, 12580.f }; // 임시 아무위치 생성 후 옮겨줄 예정
        pDesc.vScale = vScale;
        pDesc.bUseOrtho = bUseOrtho;
        pDesc.fLifeTime = m_pGameInstance->Compute_Random(1.f, 2.f);
        pDesc.fSpeed = m_pGameInstance->Compute_Random(15.f, 20.f);
        pDesc.fSpwanTimer = m_pGameInstance->Compute_Random(0.f, 2.f);                                  
        

        CParticle* pPaticle = dynamic_cast<CParticle*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, iLayerLevelIndex, strLayerTag, &pDesc));

        m_mParticle[iParticleType].push_back(pPaticle); 
    }

    if (iOffSetType == 1)
        Create_OffSet_20();     // 값하나 더 받아서 20으로 설정할건지 30으롷 할건지 조건으로 바꿔주면 될듯?
    else if (iOffSetType == 2)
        Create_OffSet_30();

    return S_OK;
}

HRESULT CParticle_Manager::Create_Particle_Low(_uint iParticleType, _uint iLayerLevelIndex, const _wstring& strLayerTag, _int iOffSetType, _bool bUseOrtho)
{

    for (_int i = 0; i < 200; i++)
    {
        CParticle::PARTICLE_DESC pDesc = {};

        pDesc.iType = iParticleType;
        pDesc.vPos = { 12580.f, 12580.f, 12580.f }; // 임시 아무위치 생성 후 옮겨줄 예정
        _float fScale = m_pGameInstance->Compute_Random(0.1f, 0.3f);
        pDesc.vScale = _float3(fScale, fScale, fScale);
        _int Texture = _int(m_pGameInstance->Compute_Random(0.f, 2.f));
        pDesc.iTexture = Texture;
        pDesc.bUseOrtho = bUseOrtho;
        pDesc.fLifeTime = m_pGameInstance->Compute_Random(3.f, 8.f);
        pDesc.fSpeed = m_pGameInstance->Compute_Random(0.5f, 2.5f);
        pDesc.fSpwanTimer = m_pGameInstance->Compute_Random(3.f, 6.f);


        CParticle* pPaticle = dynamic_cast<CParticle*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, iLayerLevelIndex, strLayerTag, &pDesc));

        m_mParticle[iParticleType].push_back(pPaticle);
    }

    if(iOffSetType == 1)
      Create_OffSet_20();     //일반룸 크기
    else if (iOffSetType == 2)
      Create_OffSet_30();   // 보스룸 크기


    return S_OK;
}

HRESULT CParticle_Manager::Play(_uint iParticleType, _float3 vPos)
{
    size_t i = 0;

    auto& ParticleList = m_mParticle[iParticleType];
    size_t iOffSetSize = m_vOffSet.size();

    for (auto& pParticle : ParticleList)
    {
        _float3 vOffset = m_vOffSet[i % iOffSetSize];

        pParticle->Set_StartPos(vPos + vOffset);
        pParticle->Set_Active(true);
        ++i;
    }

 
    return S_OK;
}

void CParticle_Manager::Create_OffSet_20()
{
    for (int i = 0; i < 200; ++i)
    {
        float x = ((i % 10) - 5) * 2.0f;  
        float z = ((i / 10) - 5) * 2.0f;  
        m_vOffSet.push_back(_float3{ x, 0.f, z });
    }
}

void CParticle_Manager::Create_OffSet_30()
{

    for (int i = 0; i < 200; ++i)
    {
        float x = ((i % 20) - 10) * 1.5f;  
        float z = ((i / 20) - 5) * 1.5f;   

        m_vOffSet.push_back(_float3(x, 0.f, z));
    }
}

void CParticle_Manager::Clear()
{
    for (auto& pair : m_mParticle)
    {
        for (auto& p : pair.second)
            Safe_Release(p);
        pair.second.clear();
    }
    m_mParticle.clear();

    m_vOffSet.clear();
}

void CParticle_Manager::Priority_Update(_float fTimeDelta)
{
}

void CParticle_Manager::Update(_float fTimeDelta, _uint iParticleType)
{
    auto& ParticleList = m_mParticle[iParticleType];

    for (auto& pParticle : ParticleList)
    {
        if (pParticle->Get_Active())
            pParticle->Update(fTimeDelta);
    }
}

void CParticle_Manager::Late_Update(_float fTimeDelta, _uint iParticleType)
{
    auto& ParticleList = m_mParticle[iParticleType];

    for (auto& pParticle : ParticleList)
    {
        if (pParticle->Get_Active())
            pParticle->Late_Update(fTimeDelta);
    }
}


CParticle_Manager* CParticle_Manager::Create()
{
    CParticle_Manager* pInstance = new CParticle_Manager();

    return pInstance;
}

void CParticle_Manager::Free()
{
    Safe_Release(m_pGameInstance);

    for (auto& pair : m_mParticle)
    {
        for (auto& p : pair.second)
            Safe_Release(p);
        pair.second.clear();
    }

    m_mParticle.clear();

    m_vOffSet.clear();

    __super::Free();
}

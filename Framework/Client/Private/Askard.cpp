#include "Askard.h"
#include "Effect_Factory.h"

#include "Room_Manager.h"
//#include "Askard_Tentacle.h"
//#include "Askard_Eye.h"
#include "Event_ClashPattern.h"
#include "Camera_Follow.h"


CAskard::CAskard(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CMonster{ pGraphic_Device }
{
}

CAskard::CAskard(const CAskard& Prototype)
    : CMonster{ Prototype }
{
}

HRESULT CAskard::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CAskard::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(this->Ready_Components(pArg)))
        return E_FAIL;

    MONSTERDESC* desc = static_cast<MONSTERDESC*>(pArg);

    CTransform* pTerrainTransform = dynamic_cast<CTransform*>(desc->pTerrainBox->Find_Component(TEXT("Com_Transform_TerrainBox")));
    m_pTerrainTransformCom = pTerrainTransform;
    _float3 fTerrainPos = pTerrainTransform->Get_State(STATE::POSITION);
    _float3 fTerrainScale = pTerrainTransform->Get_Scaled();

    // 위치 정해지는 부분. from __super
    m_pTransformCom->Set_State(STATE::POSITION, _float3(
        fTerrainPos.x,
        0.f,
        fTerrainPos.z + fTerrainScale.z / 2 - 15.f));

    m_pTransformCom->Scaling(3.f, 3.f, 3.f);

    m_isSummoned = true;
    Ready_Object();

    m_iMaxHp = 200;
    m_iCulHp = 200;

    m_eMonsterType = MONSTER_TYPE::ASKARD; // ksta
    m_ePattern = PATTERN_ASKARD::PT_IDLE;

    return S_OK;
}

void CAskard::Priority_Update(_float fTimeDelta)
{
    //__super::Priority_Update(fTimeDelta);
    if (m_bIsHit)
    {
        m_dwHitTime += 1.f;
    }

    if (m_dwHitTime >= 10.f)
    {
        m_dwHitTime = 0.f;
        m_bIsHit = false;
    }



    if (m_pHpBar != nullptr &&
        m_isSummoned)
        m_pHpBar->Render_HP_Progress(m_pTransformCom, m_iCulHp, m_iMaxHp);



    // 페이즈 및 사망 관리

    if (m_iCulHp <= 0 &&
        !m_isPhaseChanging &&
        m_iPhase == 0)
    {
        //m_iPhase++;
        m_iCulHp = m_iMaxHp;
        m_isPhaseChanging = true;
        // ksta4 : 연출용 페이즈 재생.. 그 후
        // m_iPhase++ 하여 다음 패턴 재생

    }
    else if (m_iCulHp <= 0 && 
        m_isPhaseChanging &&
        m_iPhase == 0)
    {
        if (m_iPhase == 1)  // 페이즈 전환이 끝나면 페이즈가 1이 되도록 함.
            m_iCulHp = m_iMaxHp;
    }
    else if (m_iCulHp <= 0 &&
        m_iPhase == 1)
    {
        m_isPhaseChanging = true;
    }


    if (m_iPhase >= 2)   // 2페이즈에서 체력 다 닳을 시 비로소 사망
    {
        m_iCulHp = 0;
        m_bDead = true;
    }
}

void CAskard::Update(_float fTimeDelta)
{
    // 최초 진입시.
    if (!m_bStart)
    {
        // 아래 if 조건문에 시작 조건 삽입
        if (m_pGameInstance->IsKeyDown('P'))
        {
            m_bStart = true;
            m_pAnimatorCom->Change_State(L"P1_Idle");
        }

        m_pTerrainBox->SetUp_OnTerrainBox(m_pTransformCom, _float3(0.05f, 0.8f, 0.05f));
        return;
    }
    else if (m_isPhaseChanging && m_iPhase == 0)
    {
        // ksta : 임시 (1페이즈와 2페이즈 아스카드의 이미지 크기가 달라 크기 및 위치를 키움)
        if (m_pAnimatorCom->Change_State(L"P1_SummonStaff"))
            CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::NORMAL_EFFECT, L"Prototype_Component_Boss_Askard_Phase1_PhaseChange_FX",
                m_pTransformCom->Get_State(STATE::POSITION) + _float3{0, -0.5f, 0}, {0, 0, 0, 1}, {3, 3, 3});

        if (m_pAnimatorCom->Get_CurStateTag() == L"P1_SummonStaff")
            if(m_pAnimatorCom->Change_State(L"P1_PhaseChange"))
                CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::NORMAL_EFFECT, L"Prototype_Component_Boss_Askard_Phase1_PhaseChange_FX",
                    m_pTransformCom->Get_State(STATE::POSITION) + _float3{ 0, -0.5f, 0 }, { 0, 0, 0, 1 }, { 3, 3, 3 });


        // Prototype_Component_Boss_Askard_Phase1_PhaseChange_FX

        m_pTerrainBox->SetUp_OnTerrainBox(m_pTransformCom, _float3(0.05f, 1.6f, 0.05f));
        m_pTransformCom->Scaling(6.f, 6.f, 6.f);

        if (m_pAnimatorCom->Get_CurStateTag() == L"P1_PhaseChange" &&
            m_pAnimatorCom->Get_IsLastFrame())
        {
            m_pAnimatorCom->Change_State(L"P2_Idle", true, 0.0f, true);
            m_ePattern = PATTERN_ASKARD::PT_IDLE;
            m_isPhaseChanging = false;
            m_iElapsedFrame_Update = 0;     // 패턴 진행 프레임 초기화
            m_iElapsedFrame_Pattern = 0;
            m_iPhase++;
        }


        return;
    }
    else if (m_isPhaseChanging && m_iPhase == 1)
    {
        // ksta : 임시 (1페이즈와 2페이즈 아스카드의 이미지 크기가 달라 크기 및 위치를 키움)
        m_pAnimatorCom->Change_State(L"Askard_Die");
        m_pTerrainBox->SetUp_OnTerrainBox(m_pTransformCom, _float3(0.05f, 3.2f, 0.05f));
        m_pTransformCom->Scaling(6.f, 12.f, 12.f);

        if (m_pAnimatorCom->Get_IsLastFrame())
            m_iPhase++;

        return;
    }


    // ksta3 : 패턴 구현..




    // 동작


    // 1페이즈 패턴
    if (m_iPhase == 0)
    {
        _int iPatternLoopCycle = 2000;   // 주기 프레임
        _int iStandardPatternFrame = m_iElapsedFrame_Update % iPatternLoopCycle;

        switch (iStandardPatternFrame)
        {
            // Play_Corner_Laser_ADV    // 350
            // Play_Dark_Tentacle       // 1300
            // Play_Spark               // 150
            // Play_Corner_Laser        // 230
            // Play_Spawn_Line          // 200
            // Play_Spawn_Cross         // 255
            // Play_Spawn_Width         // 300 
        case 30:    m_ePattern = PATTERN_ASKARD::PT_SPAWN_WIDTH;        break;
        case 340:   m_ePattern = PATTERN_ASKARD::PT_CORNER_LASER;       break;
        case 605:   m_ePattern = PATTERN_ASKARD::PT_SPAWN_CROSS;        break;
        case 870:   m_ePattern = PATTERN_ASKARD::PT_SPAWN_LINE;         break;
        case 1030:  m_ePattern = PATTERN_ASKARD::PT_SPARK;              break;
        case 1240:  m_ePattern = PATTERN_ASKARD::PT_SPAWN_LINE;         break;
        case 1505:  m_ePattern = PATTERN_ASKARD::PT_SPAWN_CROSS;        break;
        case 1770:  m_ePattern = PATTERN_ASKARD::PT_CORNER_LASER;       break;  // 1770+230 = 2000


        default:
            break;
        }
    }
    else if (m_iPhase == 1)
    {
        _int iPatternLoopCycle = 3550;   // 주기 프레임
        _int iStandardPatternFrame = m_iElapsedFrame_Update % iPatternLoopCycle;

        switch (iStandardPatternFrame)
        {
        case 30:    m_ePattern = PATTERN_ASKARD::PT_DARK_TENTACLE;      break;
            // Play_Corner_Laser_ADV    // 350
            // Play_Dark_Tentacle       // 1300
            // Play_Spark               // 150
            // Play_Corner_Laser        // 230
            // Play_Spawn_Line          // 200
            // Play_Spawn_Cross         // 255
            // Play_Spawn_Width         // 300 
        case 1340:  m_ePattern = PATTERN_ASKARD::PT_SPAWN_WIDTH;        break;
        case 1650:  m_ePattern = PATTERN_ASKARD::PT_CORNER_LASER_ADV;   break;
        case 2035:  m_ePattern = PATTERN_ASKARD::PT_SPAWN_CROSS;        break;
        case 2300:  m_ePattern = PATTERN_ASKARD::PT_SPAWN_LINE;         break;
        case 2460:  m_ePattern = PATTERN_ASKARD::PT_SPARK;              break;
        case 2670:  m_ePattern = PATTERN_ASKARD::PT_SPAWN_LINE;         break;
        case 2925:  m_ePattern = PATTERN_ASKARD::PT_SPAWN_CROSS;        break;
        case 3190:  m_ePattern = PATTERN_ASKARD::PT_CORNER_LASER_ADV;   break;  // 3190+230 = 3420



        default:
            break;
        }
    }



    // 격돌 진행중이라 멈춰있으면..
    if (m_isAllStop)
    {
        // 이동안 계속 이벤트 진행관련 겟함수 가져오면 될듯
        _uint iCurLevel = m_pGameInstance->Get_CurrentLevel();
        CEvent_ClashPattern::CLASH_RESULT eResult = static_cast<CEvent_ClashPattern*>(m_pGameInstance->Find_UIObj(iCurLevel, L"EVENT_Clash"))->Get_Event_Result();

        switch (eResult)
        {
        case Client::CEvent_ClashPattern::CLASH_RESULT::CLASH_PLAYING:
            return;
            break;
        default:
            m_isAllStop = false;
            break;
        }
    }















    // 패턴 별 행동

    switch (m_ePattern)
    {

    case Client::CAskard::PATTERN_ASKARD::PT_IDLE:
        break;

        /* ***** Phase 1 ***** */
    case Client::CAskard::PATTERN_ASKARD::PT_SPAWN_WIDTH:
        Play_Spawn_Width(fTimeDelta);
        break;
    //case Client::CAskard::PATTERN_ASKARD::PT_FOLLOWING_EYES:
        //Play_Following_Eyes(fTimeDelta);
        //break;
    case Client::CAskard::PATTERN_ASKARD::PT_CORNER_LASER:
        Play_Corner_Laser(fTimeDelta);
        break;
    case Client::CAskard::PATTERN_ASKARD::PT_SPAWN_CROSS:
        Play_Spawn_Cross(fTimeDelta);
        break;
    case Client::CAskard::PATTERN_ASKARD::PT_SPAWN_LINE:
        Play_Spawn_Line(fTimeDelta);
        break;
    case Client::CAskard::PATTERN_ASKARD::PT_SPARK:
        Play_Spark(fTimeDelta);
        break;

        /* ***** Phase 2 ***** */
    case Client::CAskard::PATTERN_ASKARD::PT_DARK_TENTACLE:
        Play_Dark_Tentacle(fTimeDelta);
        break;
    //case Client::CAskard::PATTERN_ASKARD::PT_SPAWN_WIDTH_ADV:
        //Play_Spawn_Width_ADV(fTimeDelta);
        //break;
    //case Client::CAskard::PATTERN_ASKARD::PT_FOLLOWING_EYES_ADV:
        //Play_Following_Eyes_ADV(fTimeDelta);
        //break;
    case Client::CAskard::PATTERN_ASKARD::PT_CORNER_LASER_ADV: // 필요
        Play_Corner_Laser_ADV(fTimeDelta);
        break;
    //case Client::CAskard::PATTERN_ASKARD::PT_SPAWN_CROSS_ADV:
        //Play_Spawn_Cross_ADV(fTimeDelta);
        //break;
    case Client::CAskard::PATTERN_ASKARD::PT_SPAWN_LINE_ADV: // 얼마 안걸리면 ㄱ
        //Play_Spawn_Line_ADV(fTimeDelta);
        break;
    case Client::CAskard::PATTERN_ASKARD::PT_SPARK_ADV: // 필요
        //Play_Spark_ADV(fTimeDelta);
        break;
    default:
        break;
    }





    // Tentacle Test
    //if (m_pAnimatorCom->Get_CurStackedFrame() == 10)
    //{
    //    Summon_Tentacle({ vMonsterPos.x - 5, vMonsterPos.y, vMonsterPos.z - 5 }, CAskard_Tentacle::TYPE_TENTACLE::TYPE_NORMAL_1);
    //    Summon_Tentacle({ vMonsterPos.x, vMonsterPos.y, vMonsterPos.z - 5 }, CAskard_Tentacle::TYPE_TENTACLE::TYPE_NORMAL_2);
    //    Summon_Tentacle({ vMonsterPos.x + 5, vMonsterPos.y, vMonsterPos.z - 5 }, CAskard_Tentacle::TYPE_TENTACLE::TYPE_NORMAL_3);
    //    Summon_Tentacle({ vMonsterPos.x, vMonsterPos.y, vMonsterPos.z - 10 }, CAskard_Tentacle::TYPE_TENTACLE::TYPE_ODD_1);
    //}

    _wstring strCurStateTag = m_pAnimatorCom->Get_CurStateTag();

    m_iElapsedFrame_Update++;

    if (m_pTerrainBox != nullptr &&
        !(strCurStateTag == L"P1_Wave" ||
            strCurStateTag == L"P2_Wave")) {
        // ksta : 임시 (1페이즈와 2페이즈 아스카드의 이미지 크기가 달라 위치를 수정함)
        if (m_iPhase == 0)
            m_pTerrainBox->SetUp_OnTerrainBox(m_pTransformCom, _float3(0.05f, 0.8f, 0.05f));
        else if (m_iPhase == 1)
            m_pTerrainBox->SetUp_OnTerrainBox(m_pTransformCom, _float3(0.05f, 1.6f, 0.05f));
    }
}

void CAskard::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);

    if (m_bStart)
        m_pBossHp->Render_Hpbar(m_iCulHp, m_iMaxHp, fTimeDelta);

    //_float3 vPos = m_pTransformCom->Get_State(STATE::POSITION);
    //std::cout << "Askard Pos : " << vPos.x << ", " << vPos.y << ", " << vPos.z << std::endl;
}


HRESULT CAskard::Render()
{
    if (m_pAnimatorCom->Get_CurState()->pTextureCom == nullptr)
        return S_OK;

    if (!m_pTransformCom)
        return S_OK;

    //Adjust_Scale();

    _uint iCurLevel = m_pGameInstance->GetInstance()->Get_CurrentLevel();
    _float3 vTargetPos = {}, vMonsterPos = {};    // 플레이어 좌표
    CTransform* pTargetTransform = dynamic_cast<CTransform*>(m_pGameInstance->GetInstance()->Get_Component(iCurLevel, TEXT("Layer_Player"), TEXT("Com_Transform")));
    vTargetPos = pTargetTransform->Get_State(STATE::POSITION);
    vMonsterPos = m_pTransformCom->Get_State(STATE::POSITION);

    _wstring strCurStateTag = m_pAnimatorCom->Get_CurStateTag();
    if (strCurStateTag == L"P1_Idle" ||
        strCurStateTag == L"P2_Idle")
    {
        // 좌우반전
        if (vTargetPos.x < vMonsterPos.x && !m_isFlippedX)   // 좌측
        {
            m_pVIBufferCom->ChangeUV_FlipX(true);
            m_isFlippedX = true;
            //std::cout << "[CPlayer::Update] FlippedX Changed to True." << std::endl;
        }
        else if (vTargetPos.x > vMonsterPos.x && m_isFlippedX)    // 우측
        {
            m_pVIBufferCom->ChangeUV_FlipX(false);
            m_isFlippedX = false;
            //std::cout << "[CPlayer::Update] FlippedX Changed to False." << std::endl;
        }
    }


    SetUp_RenderState();

    m_pTransformCom->Bind_Matrix();


    m_pAnimatorCom->Update_State(); // Bind_Texture

    m_pVIBufferCom->Bind_Buffers();


    m_pVIBufferCom->Render();

    m_pTerrainBox->Render();


    Reset_RenderState();

    if (m_isFlippedX)
    {
        m_pVIBufferCom->ResetUV_FlipX();
        m_isFlippedX = false;
    }

    return S_OK;
}

HRESULT CAskard::Ready_Components(void* pArg)
{
    MONSTERDESC* desc = static_cast<MONSTERDESC*>(pArg);

    // 컴포넌트 추가
    /* For.Com_Texture */
#pragma region Phase 1
    // Attack (5)
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Boss_Askard_Phase1_Attack"),
        TEXT("Com_Texture_P1_Attack"), reinterpret_cast<CComponent**>(&m_pTextureCom_P1_Attack))))
        return E_FAIL;
    // Attack_End (1)
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Boss_Askard_Phase1_Attack_End"),
        TEXT("Com_Texture_P1_Attack_End"), reinterpret_cast<CComponent**>(&m_pTextureCom_P1_Attack_End))))
        return E_FAIL;
    // Attack_Ready (8)
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Boss_Askard_Phase1_Attack_Ready"),
        TEXT("Com_Texture_P1_Attack_Ready"), reinterpret_cast<CComponent**>(&m_pTextureCom_P1_Attack_Ready))))
        return E_FAIL;
    // Die (36, P1)
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Boss_Askard_Phase1_Die"),
        TEXT("Com_Texture_P1_Attack_Die"), reinterpret_cast<CComponent**>(&m_pTextureCom_P1_Die))))
        return E_FAIL;
    // GroundIdle (6)
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Boss_Askard_Phase1_GroundIdle"),
        TEXT("Com_Texture_P1_GroundIdle"), reinterpret_cast<CComponent**>(&m_pTextureCom_P1_GroundIdle))))
        return E_FAIL;
    // Idle (12)
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Boss_Askard_Phase1_Idle"),
        TEXT("Com_Texture_P1_Idle"), reinterpret_cast<CComponent**>(&m_pTextureCom_P1_Idle))))
        return E_FAIL;
    // Laser (14)
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Boss_Askard_Phase1_Laser"),
        TEXT("Com_Texture_P1_Laser"), reinterpret_cast<CComponent**>(&m_pTextureCom_P1_Laser))))
        return E_FAIL;
    // PhaseChange (31)
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Boss_Askard_Phase1_PhaseChange"),
        TEXT("Com_Texture_P1_PhaseChange"), reinterpret_cast<CComponent**>(&m_pTextureCom_P1_PhaseChange))))
        return E_FAIL;
    // PhaseStart (8)
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Boss_Askard_Phase1_PhaseStart"),
        TEXT("Com_Texture_P1_PhaseStart"), reinterpret_cast<CComponent**>(&m_pTextureCom_P1_PhaseStart))))
        return E_FAIL;
    // RangeAttack (13)
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Boss_Askard_Phase1_RangeAttack"),
        TEXT("Com_Texture_P1_RangeAttack"), reinterpret_cast<CComponent**>(&m_pTextureCom_P1_RangeAttack))))
        return E_FAIL;
    // StaffIdle (6)
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Boss_Askard_Phase1_StaffIdle"),
        TEXT("Com_Texture_P1_StaffIdle"), reinterpret_cast<CComponent**>(&m_pTextureCom_P1_StaffIdle))))
        return E_FAIL;
    // SummonStaff (17)
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Boss_Askard_Phase1_SummonStaff"),
        TEXT("Com_Texture_P1_SummonStaff"), reinterpret_cast<CComponent**>(&m_pTextureCom_P1_SummonStaff))))
        return E_FAIL;
    // Wave (21)
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Boss_Askard_Phase1_Wave"),
        TEXT("Com_Texture_P1_Wave"), reinterpret_cast<CComponent**>(&m_pTextureCom_P1_Wave))))
        return E_FAIL;
#pragma endregion

#pragma region Phase 2
    // Attack (5)
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Boss_Askard_Phase2_Attack"),
        TEXT("Com_Texture_P2_Attack"), reinterpret_cast<CComponent**>(&m_pTextureCom_P2_Attack))))
        return E_FAIL;
    // Attack_End (1)
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Boss_Askard_Phase2_Attack_End"),
        TEXT("Com_Texture_P2_Attack_End"), reinterpret_cast<CComponent**>(&m_pTextureCom_P2_Attack_End))))
        return E_FAIL;
    // Attack_Ready (8)
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Boss_Askard_Phase2_Attack_Ready"),
        TEXT("Com_Texture_P2_Attack_Ready"), reinterpret_cast<CComponent**>(&m_pTextureCom_P2_Attack_Ready))))
        return E_FAIL;
    // Idle (12)
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Boss_Askard_Phase2_Idle"),
        TEXT("Com_Texture_P2_Idle"), reinterpret_cast<CComponent**>(&m_pTextureCom_P2_Idle))))
        return E_FAIL;
    // Laser (14)
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Boss_Askard_Phase2_Laser"),
        TEXT("Com_Texture_P2_Laser"), reinterpret_cast<CComponent**>(&m_pTextureCom_P2_Laser))))
        return E_FAIL;
    // RangeAttack (13)
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Boss_Askard_Phase2_RangeAttack"),
        TEXT("Com_Texture_P2_RangeAttack"), reinterpret_cast<CComponent**>(&m_pTextureCom_P2_RangeAttack))))
        return E_FAIL;
    // Tentacle (16)
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Boss_Askard_Phase2_Tentacle"),
        TEXT("Com_Texture_P2_Tentacle"), reinterpret_cast<CComponent**>(&m_pTextureCom_P2_Tentacle))))
        return E_FAIL;
    // Tentacle_End (7)
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Boss_Askard_Phase2_Tentacle_End"),
        TEXT("Com_Texture_P2_Tentacle_End"), reinterpret_cast<CComponent**>(&m_pTextureCom_P2_Tentacle_End))))
        return E_FAIL;
    // Tentacle_Unlit (20)
    //if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Boss_Askard_Phase2_Tentacle_Unlit"),
    //    TEXT("Com_Texture_P2_Tentacle_Unlit"), reinterpret_cast<CComponent**>(&m_pTextureCom_P2_Tentacle_Unlit))))
    //    return E_FAIL;
    // Wave (21)
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Boss_Askard_Phase2_Wave"),
        TEXT("Com_Texture_P2_Wave"), reinterpret_cast<CComponent**>(&m_pTextureCom_P2_Wave))))
        return E_FAIL;
    
    // Askard_Die (47)
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Boss_Askard_Die"),
        TEXT("Com_Texture_Askard_Die"), reinterpret_cast<CComponent**>(&m_pTextureCom_Askard_Die))))
        return E_FAIL;
    // Hidden (1)
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Boss_Askard_Hidden"),
        TEXT("Com_Texture_Askard_Hidden"), reinterpret_cast<CComponent**>(&m_pTextureCom_Askard_Hidden))))
        return E_FAIL;

    
#pragma endregion



    //...


    /* For Com_Animator */
    CAnimator::ANIMSTATE_DESC StartAnimStateDesc{};
    StartAnimStateDesc.strTimerTag = L"Animator_Boss_Askard";   // 해당 애니메이터가 타이머에서 사용할 태그 key값
    StartAnimStateDesc.pParentTransform = nullptr;
    StartAnimStateDesc.pChildTransform = nullptr;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Animator"),
        TEXT("Com_Animator"), reinterpret_cast<CComponent**>(&m_pAnimatorCom), &StartAnimStateDesc)))
        return E_FAIL;

    // State 삽입

#pragma region States...

    // Phase 1
    m_pAnimatorCom->Add_State(L"Standby",           { m_pTextureCom_P1_Idle         , 4, true });
    m_pAnimatorCom->Add_State(L"P1_Idle",           { m_pTextureCom_P1_Idle         , 4, true });

    m_pAnimatorCom->Add_State(L"P1_Attack",         { m_pTextureCom_P1_Attack		, 4, false });
    m_pAnimatorCom->Add_State(L"P1_Attack_End",     { m_pTextureCom_P1_Attack_End	, 4, false });
    m_pAnimatorCom->Add_State(L"P1_Attack_Ready",   { m_pTextureCom_P1_Attack_Ready	, 8, false });
    m_pAnimatorCom->Add_State(L"P1_Die",            { m_pTextureCom_P1_Die			, 4, true });
    m_pAnimatorCom->Add_State(L"P1_GroundIdle",     { m_pTextureCom_P1_GroundIdle	, 4, true });
    m_pAnimatorCom->Add_State(L"P1_Laser",          { m_pTextureCom_P1_Laser		, 5, false });
    m_pAnimatorCom->Add_State(L"P1_PhaseChange",    { m_pTextureCom_P1_PhaseChange	, 4, false });
    m_pAnimatorCom->Add_State(L"P1_PhaseStart",     { m_pTextureCom_P1_PhaseStart	, 4, true });
    m_pAnimatorCom->Add_State(L"P1_RangeAttack",    { m_pTextureCom_P1_RangeAttack	, 4, false });
    m_pAnimatorCom->Add_State(L"P1_StaffIdle",      { m_pTextureCom_P1_StaffIdle	, 4, true });
    m_pAnimatorCom->Add_State(L"P1_SummonStaff",    { m_pTextureCom_P1_SummonStaff	, 4, false });
    m_pAnimatorCom->Add_State(L"P1_Wave",           { m_pTextureCom_P1_Wave			, 4, false });

    // Phase 2
    m_pAnimatorCom->Add_State(L"P2_Attack",         { m_pTextureCom_P2_Attack		, 4, false });
    m_pAnimatorCom->Add_State(L"P2_Attack_End",     { m_pTextureCom_P2_Attack_End	, 4, false });
    m_pAnimatorCom->Add_State(L"P2_Attack_Ready",   { m_pTextureCom_P2_Attack_Ready	, 4, false });
    m_pAnimatorCom->Add_State(L"P2_Idle",           { m_pTextureCom_P2_Idle			, 4, true });
    m_pAnimatorCom->Add_State(L"P2_Laser",          { m_pTextureCom_P2_Laser		, 4, false });
    m_pAnimatorCom->Add_State(L"P2_RangeAttack",    { m_pTextureCom_P2_RangeAttack	, 4, false });
    m_pAnimatorCom->Add_State(L"P2_Tentacle",       { m_pTextureCom_P2_Tentacle		, 5, false });
    m_pAnimatorCom->Add_State(L"P2_Tentacle_End",   { m_pTextureCom_P2_Tentacle_End	, 4, false });
    //m_pAnimatorCom->Add_State(L"P2_Tentacle_Unlit", { m_pTextureCom_P2_Tentacle_Unlit, 4, true });
    m_pAnimatorCom->Add_State(L"P2_Wave",           { m_pTextureCom_P2_Wave			, 4, false });

    m_pAnimatorCom->Add_State(L"Askard_Die",        { m_pTextureCom_Askard_Die		, 4, false });
    m_pAnimatorCom->Add_State(L"Hidden",            { m_pTextureCom_Askard_Hidden	, 4, true });

    

#pragma endregion


    return S_OK;
}

HRESULT CAskard::Ready_Object()
{
    //m_pHpBar = dynamic_cast<CField_Hp*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_UI_Field_Hp")));

    if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_BOSS2), TEXT("Layer_HPBar"), ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_UI_BossHp_Askard"))))
        return E_FAIL;

    m_pBossHp = static_cast<CBossHp_Askard*>(m_pGameInstance->Find_UIObj(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("BossHp_Askard")));

    return S_OK;
}

void CAskard::Summon_Tentacle(_float3 vPosition, CAskard_Tentacle::TYPE_TENTACLE eType)
{
    CAskard_Tentacle::TENTACLE_DESC pDesc = {};
    pDesc.vPosition = vPosition;
    pDesc.eType = eType;

    pDesc.pTerrainBox = CRoom_Manager::GetInstance()->Get_CurrentRoom()->Get_TerrainBox();


    m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STATIC), L"Layer_Effect",
        ENUM_CLASS(LEVEL::LEVEL_BOSS2), L"Prototype_GameObject_Boss_Askard_Tentacle", &pDesc);
}

void CAskard::Summon_Dark_Tentacle(_float3 vPosition, CAskard_Dark_Tentacle::DARKTENTACLE_DIR eDir)
{
    CAskard_Dark_Tentacle::DARKTENTACLE_DESC pDesc = {};
    pDesc.vPosition = vPosition;
    pDesc.eDir = eDir;

    pDesc.pTerrainBox = CRoom_Manager::GetInstance()->Get_CurrentRoom()->Get_TerrainBox();

    m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STATIC), L"Layer_Effect",
        ENUM_CLASS(LEVEL::LEVEL_BOSS2), L"Prototype_GameObject_Boss_Askard_Dark_Tentacle", &pDesc);
}


//void CAskard::Summon_FollowingEye(_float3 vPosition)
//{
//    CAskard_Eye::EYE_DESC pDesc = {};
//    pDesc.vPosition = vPosition;
//
//    pDesc.pTerrainBox = CRoom_Manager::GetInstance()->Get_CurrentRoom()->Get_TerrainBox();
//
//    m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STATIC), L"Layer_Effect",
//        ENUM_CLASS(LEVEL::LEVEL_BOSS2), L"Prototype_GameObject_Boss_Askard_Eye", &pDesc);
//}

//void CAskard::Adjust_Scale()    // 이미지 리소스를 수정하지 못한 상태에서, 크기를 맞추기 위한 임시 변환 용도. 삭제해도 괜찮습니다.
//{
//    _wstring strCurStateTag = m_pAnimatorCom->Get_CurStateTag();
//
//    if (strCurStateTag == L"P1_Laser")
//        m_pTransformCom->Scaling(3.f * 61.f / 84.f, 3.f * 53.f / 73.f, 3.f);
//    else
//        m_pTransformCom->Scaling(3.f, 3.f, 3.f);
//}

void CAskard::OnCollision(CGameObject* pGameObject)
{
    if (pGameObject == nullptr)
        return;

    __super::OnCollision(pGameObject);

    switch (pGameObject->Get_ObjType())
    {
    case GAMEOBJ_TYPE::PLAYER_EFFECT:
        break;
    }
}

CAskard* CAskard::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CAskard* pInstance = new CAskard(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Create : CAskard"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CAskard::Clone(void* pArg)
{
    CAskard* pInstance = new CAskard(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Clone : CAskard"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CAskard::Free()
{
    m_pGameInstance->Remove_Collider_ByOwner(this);
    Safe_Release(m_pTextureCom_P1_Attack);
    Safe_Release(m_pTextureCom_P1_Attack_End);
    Safe_Release(m_pTextureCom_P1_Attack_Ready);
    Safe_Release(m_pTextureCom_P1_Die);
    Safe_Release(m_pTextureCom_P1_GroundIdle);
    Safe_Release(m_pTextureCom_P1_Idle);
    Safe_Release(m_pTextureCom_P1_Laser);
    Safe_Release(m_pTextureCom_P1_PhaseChange);
    Safe_Release(m_pTextureCom_P1_PhaseStart);
    Safe_Release(m_pTextureCom_P1_RangeAttack);
    Safe_Release(m_pTextureCom_P1_StaffIdle);
    Safe_Release(m_pTextureCom_P1_SummonStaff);
    Safe_Release(m_pTextureCom_P1_Wave);

    Safe_Release(m_pTextureCom_P2_Attack);
    Safe_Release(m_pTextureCom_P2_Attack_End);
    Safe_Release(m_pTextureCom_P2_Attack_Ready);
    Safe_Release(m_pTextureCom_P2_Idle);
    Safe_Release(m_pTextureCom_P2_Laser);
    Safe_Release(m_pTextureCom_P2_RangeAttack);
    Safe_Release(m_pTextureCom_P2_Tentacle);
    Safe_Release(m_pTextureCom_P2_Tentacle_End);
    //Safe_Release(m_pTextureCom_P2_Tentacle_Unlit);
    Safe_Release(m_pTextureCom_P2_Wave);

    Safe_Release(m_pTextureCom_Askard_Die);
    Safe_Release(m_pTextureCom_Askard_Hidden);


    Safe_Release(m_pAnimatorCom);

    __super::Free();
}





// *------------------------------*
// * 이하 패턴 함수들
// *------------------------------*


// Phase 1

void CAskard::Play_Spawn_Width(_float fTimeDelta)
{
    // 아래에서 사용할 변수들
#pragma region Variables Setting

    _uint iCurLevel = CGameInstance::GetInstance()->Get_CurrentLevel();

    CTransform* pTargetTransform = dynamic_cast<CTransform*>(m_pGameInstance->GetInstance()->Get_Component(iCurLevel, TEXT("Layer_Player"), TEXT("Com_Transform")));

    _float3 vMonsterPos = m_pTransformCom->Get_State(STATE::POSITION);
    _float3 vTargetPos = pTargetTransform->Get_State(STATE::POSITION);

    _float3 vDiff = -vMonsterPos + vTargetPos;
    _float fDistance = D3DXVec3Length(&vDiff);


    // ********* matMonster 구하기
    _float4x4 matMonsterWorld = *m_pTransformCom->Get_WorldMatrix();

    _wstring strCurStateTag = m_pAnimatorCom->Get_CurStateTag();


    _float3 vTerrainPos = m_pTerrainTransformCom->Get_State(STATE::POSITION);
    _float3 vTerrainScale = m_pTerrainTransformCom->Get_Scaled();

#pragma endregion

    int iMaxFrame_Pattern = 300;      // 이 패턴은 몇프레임동안 플레이될 것인가


#pragma region memo

    // - 생성 조건?
    // 1.2초 (72프레임) 주기로 3회, 번갈아서 일렬로 4줄 가량의 촉수를 쫙 깔음.
    // 촉수의 모양은 1~3번 중 랜덤히 설정
    // 2번째 소환 시 변종촉수 1기 랜덤히 소환

    // vTerrainScale 은 각각 방향으로 30. (x, z 기준)
    // vTerrainPos.z 기준 +-3.75, +-11.25 만큼 차이나는 선분 기준으로 깔아야 함
    // x축 선분상의 소환 갯수는 원작 기준 11개. 이는 보고 조절
    // 대신 z축 상의 랜덤 오프셋이 필요해보임. 한 +-0.5정도?

    // 우선 줄만큼 나오도록 만들어보고
    // 여기에 랜덤성 (타입, 위치 오프셋) 추가
    // 그 후 변종 발동조건 추가

#pragma endregion

    // 시간에 따른 패턴 실행

    if (m_iElapsedFrame_Pattern == 10)
    {
        if      (m_iPhase == 0)     m_pAnimatorCom->Change_State(L"P1_Attack_Ready");
        else if (m_iPhase == 1)     m_pAnimatorCom->Change_State(L"P2_Attack_Ready");
    }
    else if (m_iElapsedFrame_Pattern == 15)
    {
        CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::NORMAL_EFFECT, L"Prototype_Component_Boss_Askard_Attack_1_FX",
            vMonsterPos + _float3{ 0.5f, 0, 0 }, { 0, 0, 0, 1 }, { 3, 3, 3 });
    }
    else if
        (m_iElapsedFrame_Pattern == 40 ||
            m_iElapsedFrame_Pattern == 120 ||
            m_iElapsedFrame_Pattern == 200)
    {
        _bool isOddLine = (m_iElapsedFrame_Pattern == 120) ? true : false;
        _float fZRandOffset = 1.5f;

        _int iNumX = 15;        // 가로 방향 촉수 개수
        _int iNumZ = 5;         // 세로 방향의 촉수 줄



        _float fUnitZ = vTerrainScale.z / pow(iNumZ, 2);        // 줄 계산용
        _float fStartZ = vTerrainPos.z - vTerrainScale.z / 2.f; // Z축의 아랫쪽 끝을 의미

        vector<_int> iZLine = {};           // 몇번째 선에 생성될 것인가

        if (!isOddLine)
            for (int i = 0; i < iNumZ; i++)
                iZLine.push_back(iNumZ - (iNumZ - 1) + i * iNumZ);
        else
            for (int i = 0; i < iNumZ; i++)
                iZLine.push_back(iNumZ - (iNumZ - 1) + i * iNumZ + 2);


        _int iOddTentacleIndex = static_cast<_int>(m_pGameInstance->Compute_Random(0, iNumX * iNumZ));  // 별종 넣을 인덱스 선정

        for (int z = 0; z < iNumZ; z++)
        {
            _float fPosZ = fStartZ + fUnitZ * iZLine[z];

            for (int j = 0; j < iNumX; j++)
            {
                _float fPosX = vTerrainPos.x - vTerrainScale.x / 2.f + (vTerrainScale.x / (iNumX - 1)) * j;

                _float fRand = m_pGameInstance->Compute_Random(-fZRandOffset / 2, fZRandOffset / 2);    // Z축 랜덤값
                _int iRandType = static_cast<_int>(m_pGameInstance->Compute_Random(0.f, 3.f));

                if ((iOddTentacleIndex == ((z + 1) * (j + 1) + j - 1)) && m_iElapsedFrame_Pattern == 120)
                    iRandType = 3; // 해당 인덱스에 별종 삽입

                Summon_Tentacle(_float3{ fPosX, vMonsterPos.y, fPosZ + fRand }, static_cast<CAskard_Tentacle::TYPE_TENTACLE>(iRandType));
            }
        }
    }


    // 이전과 이어지는 동작 관리

    if (strCurStateTag == L"P1_Attack_Ready")
        m_pAnimatorCom->Change_State(L"P1_Attack");
    else if (strCurStateTag == L"P1_Attack")
        m_pAnimatorCom->Change_State(L"P1_Attack_End");

    if (strCurStateTag == L"P2_Attack_Ready")
        m_pAnimatorCom->Change_State(L"P2_Attack");
    else if (strCurStateTag == L"P2_Attack")
        m_pAnimatorCom->Change_State(L"P2_Attack_End");




    // *******




    m_iElapsedFrame_Pattern++;

    // m_iElapsedFrame_Pattern 으로, 언제 어떤 패턴이 실행될 지를 제어
    // 각 패턴 종료시간 도달 시 
    // m_iElapsedFrame_Pattern 변수를 0으로 만들기

    if (m_iElapsedFrame_Pattern >= iMaxFrame_Pattern)
    {
        m_iElapsedFrame_Pattern = 0;
        m_ePattern = PATTERN_ASKARD::PT_IDLE;
        if      (m_iPhase == 0)     m_pAnimatorCom->Change_State(L"P1_Idle");
        else if (m_iPhase == 1)     m_pAnimatorCom->Change_State(L"P2_Idle");
    }

}

void CAskard::Play_Spawn_Cross(_float fTimeDelta)
{
    // 아래에서 사용할 변수들
#pragma region Variables Setting

    _uint iCurLevel = CGameInstance::GetInstance()->Get_CurrentLevel();

    CTransform* pTargetTransform = dynamic_cast<CTransform*>(m_pGameInstance->GetInstance()->Get_Component(iCurLevel, TEXT("Layer_Player"), TEXT("Com_Transform")));

    const _float3 vMonsterPos = m_pTransformCom->Get_State(STATE::POSITION);
    _float3 vTargetPos = pTargetTransform->Get_State(STATE::POSITION);

    _float3 vDiff = -vMonsterPos + vTargetPos;
    _float fDistance = D3DXVec3Length(&vDiff);


    // ********* matMonster 구하기
    _float4x4 matMonsterWorld = *m_pTransformCom->Get_WorldMatrix();

    _wstring strCurStateTag = m_pAnimatorCom->Get_CurStateTag();


    _float3 vTerrainPos = m_pTerrainTransformCom->Get_State(STATE::POSITION);
    _float3 vTerrainScale = m_pTerrainTransformCom->Get_Scaled();

#pragma endregion

    _int iPatternCycleFrame = 85;
    _int iCalcedCycleFrame = m_iElapsedFrame_Pattern % iPatternCycleFrame;

    _int iMaxFrame_Pattern = iPatternCycleFrame * 3;      // 이 패턴은 몇프레임동안 플레이될 것인가

    //std::cout << iCalcedCycleFrame << std::endl;

    // 플레이어에게 다가와서 4방향 촉수 소환    

    // 1. 플레이어의 방향을 구함
    // 2. 플레이어 기준으로 vTargettedPos 4종을 정한 뒤, 행렬 등 연산을 통해 90도씩 전환한 방향을 구함
    // 3. 해당 방향으로 1프레임 당 각 방향마다 1개씩 촉수 생성
    // 4. 단, 현재 Terrain 좌표를 벗어난다면 생성 X
    // 옵션1. 2번째에서 생성 시에 랜덤하게 변종 하나 섞어야함
    // 옵션2. 전체적인 촉수 방향에 랜덤값을 약간 줌, 개별 촉수 생성위치 역시 랜덤값을 약간 줌  

    _float fMoveSpeed = 1.5f;       // 아스카드 도약 이속
    _float fLimitRange = 2.f;       // 아스카드가 다가갈 때 얼마나 거리둘건지
    _float fTentacleSpace = 2.f;    // 촉수 소환 간격
    _float fRandOffsetRange = 1.f;  // 촉수 개별마다의 소환위치 랜덤성 강도



    _float3 vToTargetDir = m_vTargettedPos - vMonsterPos;
    vToTargetDir.y = 0;
    D3DXVec3Normalize(&vToTargetDir, &vToTargetDir);

    _float4x4 matRotY90;                   // Y축 기준 90도 회전하는 행렬
    D3DXMatrixIdentity(&matRotY90);
    D3DXMatrixRotationY(&matRotY90, D3DXToRadian(90));


    if (iCalcedCycleFrame == 5)
    {
        if      (m_iPhase == 0)     m_pAnimatorCom->Change_State(L"P1_Attack_Ready");
        else if (m_iPhase == 1)     m_pAnimatorCom->Change_State(L"P2_Attack_Ready");
    }
    else if (iCalcedCycleFrame == 15)
    {
        if (m_iElapsedFrame_Pattern == iCalcedCycleFrame)
            CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::NORMAL_EFFECT, L"Prototype_Component_Boss_Askard_Attack_1_FX",
                vMonsterPos + _float3{ 0.5f, 0, 0 }, { 0, 0, 0, 1 }, { 3, 3, 3 });
        else
            CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::NORMAL_EFFECT, L"Prototype_Component_Boss_Askard_Attack_1_FX_Cycle",
                vMonsterPos + _float3{ 0.5f, 0, 0 }, { 0, 0, 0, 1 }, { 3, 3, 3 });
    }
    else if (IS_BETWEEN(iCalcedCycleFrame, 30, 40))
    {
        if (iCalcedCycleFrame == 30)
            m_vTargettedPos = vTargetPos;
        _float3 vDiff = vTargetPos - vMonsterPos;
        _float fDiff = D3DXVec3Length(&vDiff);
        m_pTransformCom->Move_To(m_vTargettedPos, fTimeDelta * fDiff * fMoveSpeed, fLimitRange);
    }
    else if (IS_BETWEEN(iCalcedCycleFrame, 40, 55))
    {
        if (iCalcedCycleFrame == 40)
            m_vTargettedPos = vTargetPos;   // 이전 위치 저장

        _int iOddTentacleIndex = -1;
        if (iPatternCycleFrame + 45 == m_iElapsedFrame_Pattern) // 2번째 공격이라면.. 변종 1개 섞음
            iOddTentacleIndex = m_pGameInstance->Compute_Random(0.f, 4.f);

        _int iFrame = iCalcedCycleFrame - 40;

        vToTargetDir = m_vTargettedPos - vMonsterPos;
        vToTargetDir.y = 0;
        D3DXVec3Normalize(&vToTargetDir, &vToTargetDir);

        for (int i = 0; i < 4; i++)
        {
            _int iRandType = static_cast<_int>(m_pGameInstance->Compute_Random(0.f, 3.f));
            _float fRandOffsetX = m_pGameInstance->Compute_Random(-fRandOffsetRange / 2, +fRandOffsetRange / 2);
            _float fRandOffsetZ = m_pGameInstance->Compute_Random(-fRandOffsetRange / 2, +fRandOffsetRange / 2);

            _float3 vSummonPos = vMonsterPos + vToTargetDir * fTentacleSpace * (iFrame + 1) +
                _float3{ fRandOffsetX, 0, fRandOffsetZ }; // 소환위치 랜덤성 부여

            if (iOddTentacleIndex != -1 && iOddTentacleIndex == i)  // 변종 소환
                iRandType = 3;

            // 소환할 좌표가 터레인 외부면 소환하지 않음
            if ((IS_BETWEEN(vSummonPos.x, vTerrainPos.x - vTerrainScale.x / 2, vTerrainPos.x + vTerrainScale.x / 2)) &&
                (IS_BETWEEN(vSummonPos.z, vTerrainPos.z - vTerrainScale.z / 2, vTerrainPos.z + vTerrainScale.z / 2)))
                Summon_Tentacle(vSummonPos, static_cast<CAskard_Tentacle::TYPE_TENTACLE>(iRandType));  // 각도 맞춰 위치반영 소환

            D3DXVec3TransformNormal(&vToTargetDir, &vToTargetDir, &matRotY90);
        }
    }

    // 이전과 이어지는 동작 관리

    if (strCurStateTag == L"P1_Attack_Ready")
        m_pAnimatorCom->Change_State(L"P1_Attack");
    else if (strCurStateTag == L"P1_Attack")
        m_pAnimatorCom->Change_State(L"P1_Attack_End");

    if (strCurStateTag == L"P2_Attack_Ready")
        m_pAnimatorCom->Change_State(L"P2_Attack");
    else if (strCurStateTag == L"P2_Attack")
        m_pAnimatorCom->Change_State(L"P2_Attack_End");

    m_iElapsedFrame_Pattern++;

    if (m_iElapsedFrame_Pattern >= iMaxFrame_Pattern)
    {
        m_iElapsedFrame_Pattern = 0;
        m_ePattern = PATTERN_ASKARD::PT_IDLE;
        if      (m_iPhase == 0)     m_pAnimatorCom->Change_State(L"P1_Idle");
        else if (m_iPhase == 1)     m_pAnimatorCom->Change_State(L"P2_Idle");
    }
}

void CAskard::Play_Spawn_Line(_float fTimeDelta)
{
    // ksta : 나중에 Laser 상태 이미지 좀 늘릴 필요 있을 듯


    // 아래에서 사용할 변수들
#pragma region Variables Setting

    _uint iCurLevel = CGameInstance::GetInstance()->Get_CurrentLevel();

    CTransform* pTargetTransform = dynamic_cast<CTransform*>(m_pGameInstance->GetInstance()->Get_Component(iCurLevel, TEXT("Layer_Player"), TEXT("Com_Transform")));

    const _float3 vMonsterPos = m_pTransformCom->Get_State(STATE::POSITION);
    _float3 vTargetPos = pTargetTransform->Get_State(STATE::POSITION);

    _float3 vDiff = -vMonsterPos + vTargetPos;
    _float fDistance = D3DXVec3Length(&vDiff);


    // ********* matMonster 구하기
    _float4x4 matMonsterWorld = *m_pTransformCom->Get_WorldMatrix();

    _wstring strCurStateTag = m_pAnimatorCom->Get_CurStateTag();


    _float3 vTerrainPos = m_pTerrainTransformCom->Get_State(STATE::POSITION);
    _float3 vTerrainScale = m_pTerrainTransformCom->Get_Scaled();

#pragma endregion

    _int iPatternCycleFrame = 200;
    _int iCalcedCycleFrame = m_iElapsedFrame_Pattern % iPatternCycleFrame;

    _int iMaxFrame_Pattern = iPatternCycleFrame;      // 이 패턴은 몇프레임동안 플레이될 것인가


    // 플레이어에게서 떨어져서 촉수 두 줄 소환
    // 촉수는 직선형이 아닌, 플레이어가 맞게끔 약간의 곡선형으로 감
    // 플레이어 거리까지는 직선형으로, 이후 곡선형으로 보내도 될 듯



    _float fMoveSpeed = 1.5f;       // 아스카드 도약 이속

    _float fTentacleSpace = 2.f;    // 촉수 소환 간격
    _float fRandOffsetRange = 1.f;  // 촉수 개별마다의 소환위치 랜덤성 강도



    _float3 vToTargetDir = m_vTargettedPos - vMonsterPos;
    vToTargetDir.y = 0;
    D3DXVec3Normalize(&vToTargetDir, &vToTargetDir);

    _float4x4 matRotY1, matRotYM1;                   // Y축 기준 5도 회전하는 행렬
    D3DXMatrixIdentity(&matRotY1);
    D3DXMatrixIdentity(&matRotYM1);
    D3DXMatrixRotationY(&matRotY1, D3DXToRadian(1));
    D3DXMatrixRotationY(&matRotYM1, D3DXToRadian(-1));




    if (iCalcedCycleFrame == 5)
    {
        if      (m_iPhase == 0)     m_pAnimatorCom->Change_State(L"P1_Attack_Ready");
        else if (m_iPhase == 1)     m_pAnimatorCom->Change_State(L"P2_Attack_Ready");
    }
    else if (iCalcedCycleFrame == 15)
    {
        if (m_iElapsedFrame_Pattern == iCalcedCycleFrame)
            CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::NORMAL_EFFECT, L"Prototype_Component_Boss_Askard_Attack_1_FX",
                vMonsterPos + _float3{ 0.5f, 0, 0 }, { 0, 0, 0, 1 }, { 3, 3, 3 });
        else
            CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::NORMAL_EFFECT, L"Prototype_Component_Boss_Askard_Attack_1_FX_Cycle",
                vMonsterPos + _float3{ 0.5f, 0, 0 }, { 0, 0, 0, 1 }, { 3, 3, 3 });
    }
    else if (IS_BETWEEN(iCalcedCycleFrame, 30, 40))
    {
        if (iCalcedCycleFrame == 30)
        {
            m_vTargettedPos = vTargetPos;

            // 저장했던 플레이어의 위치를 기준으로, 해당 사분면을 제외하고 랜덤한 사분면으로 이동

            _float3 vMovePos{};
            _float fPosOffset = 12.f;

            _float3 vMovePosList[4] = {};
            vMovePosList[0] = vTerrainPos + _float3{ -fPosOffset, 0, fPosOffset };
            vMovePosList[1] = vTerrainPos + _float3{ fPosOffset, 0, fPosOffset };
            vMovePosList[2] = vTerrainPos + _float3{ fPosOffset, 0, -fPosOffset };
            vMovePosList[3] = vTerrainPos + _float3{ -fPosOffset, 0, -fPosOffset };

            _bool isPlayerPosX_Positive = (vTerrainPos.x < m_vTargettedPos.x) ? true : false;
            _bool isPlayerPosZ_Positive = (vTerrainPos.z < m_vTargettedPos.z) ? true : false;

            _int iNotMovePosIndex;
            if (!isPlayerPosX_Positive && isPlayerPosZ_Positive)       iNotMovePosIndex = 0;
            else if (isPlayerPosX_Positive && isPlayerPosZ_Positive)        iNotMovePosIndex = 1;
            else if (isPlayerPosX_Positive && !isPlayerPosZ_Positive)       iNotMovePosIndex = 2;
            else if (!isPlayerPosX_Positive && !isPlayerPosZ_Positive)      iNotMovePosIndex = 3;

            _int iRandPosIndex = static_cast<_int>(m_pGameInstance->Compute_Random(0.f, 4.f));
            if (iNotMovePosIndex == iRandPosIndex)
                iRandPosIndex = (iRandPosIndex + 2) % 4;

            m_vMovePos = vMovePosList[iRandPosIndex]; // 최종 정해진 이동 위치
        }

        _float3 vDiff = vTargetPos - vMonsterPos;
        _float fDiff = D3DXVec3Length(&vDiff);

        // 실제 이동

        m_pTransformCom->Move_To(m_vMovePos, fTimeDelta * fDiff * fMoveSpeed, 0.5f);

    }
    else if (IS_BETWEEN(iCalcedCycleFrame, 40, 55) ||
        IS_BETWEEN(iCalcedCycleFrame, 90, 105) ||
        IS_BETWEEN(iCalcedCycleFrame, 140, 155))
    {
        if (iCalcedCycleFrame == 40 ||
            iCalcedCycleFrame == 90 ||
            iCalcedCycleFrame == 140)
            m_vTargettedPos = vTargetPos;   // 이전 위치 저장

        _int iOddTentacleIndex = -1;
        _int iOddTentacleLineIndex = -1;
        //if (iPatternCycleFrame == 90) // 2번째 공격이라면.. 변종 1개 섞음
        //{
        //    iOddTentacleIndex       = static_cast<_int>(m_pGameInstance->Compute_Random(2.f, 7.f));
        //    iOddTentacleLineIndex   = static_cast<_int>(m_pGameInstance->Compute_Random(0.f, 2.f));
        //}

        _int iFrame = (iCalcedCycleFrame) % 50 - 40;
        iFrame = (iFrame < 0) ? iFrame + 50 : iFrame;


        vToTargetDir = m_vTargettedPos - vMonsterPos;
        vToTargetDir.y = 0;
        D3DXVec3Normalize(&vToTargetDir, &vToTargetDir);

        for (int i = 0; i < iFrame + 1; i++)
            D3DXVec3TransformNormal(&vToTargetDir, &vToTargetDir, &matRotY1);

        for (int i = 0; i < 2; i++)
        {
            _int iRandType = static_cast<_int>(m_pGameInstance->Compute_Random(0.f, 3.f));
            if (iCalcedCycleFrame == 95)
                iRandType = 3;

            _float fRandOffsetX = m_pGameInstance->Compute_Random(-fRandOffsetRange / 2, +fRandOffsetRange / 2);
            _float fRandOffsetZ = m_pGameInstance->Compute_Random(-fRandOffsetRange / 2, +fRandOffsetRange / 2);

            _float3 vSummonPos = vMonsterPos + vToTargetDir * fTentacleSpace * (iFrame + 1) +
                _float3{ fRandOffsetX, 0, fRandOffsetZ }; // 소환위치 랜덤성 부여

            //if (iOddTentacleIndex != -1 &&
            //    iOddTentacleLineIndex == i &&
            //    iOddTentacleIndex == (iCalcedCycleFrame - 90))  // 변종 소환
            //    iRandType = 3;

            for (int i = 0; i < 8 + 1; i++)
                D3DXVec3TransformNormal(&vToTargetDir, &vToTargetDir, &matRotYM1);

            // 소환할 좌표가 터레인 외부면 소환하지 않음
            if ((IS_BETWEEN(vSummonPos.x, vTerrainPos.x - vTerrainScale.x / 2, vTerrainPos.x + vTerrainScale.x / 2)) &&
                (IS_BETWEEN(vSummonPos.z, vTerrainPos.z - vTerrainScale.z / 2, vTerrainPos.z + vTerrainScale.z / 2)))
                Summon_Tentacle(vSummonPos, static_cast<CAskard_Tentacle::TYPE_TENTACLE>(iRandType));  // 각도 맞춰 위치반영 소환
        }

    }


    if (strCurStateTag == L"P1_Attack_Ready")
        m_pAnimatorCom->Change_State(L"P1_Attack");
    else if (strCurStateTag == L"P1_Attack")
        m_pAnimatorCom->Change_State(L"P1_Attack_End");

    if (strCurStateTag == L"P2_Attack_Ready")
        m_pAnimatorCom->Change_State(L"P2_Attack");
    else if (strCurStateTag == L"P2_Attack")
        m_pAnimatorCom->Change_State(L"P2_Attack_End");


    m_iElapsedFrame_Pattern++;

    if (m_iElapsedFrame_Pattern >= iMaxFrame_Pattern)
    {
        m_iElapsedFrame_Pattern = 0;
        m_ePattern = PATTERN_ASKARD::PT_IDLE;
        if      (m_iPhase == 0)     m_pAnimatorCom->Change_State(L"P1_Idle");
        else if (m_iPhase == 1)     m_pAnimatorCom->Change_State(L"P2_Idle");
    }

}

void CAskard::Play_Corner_Laser(_float fTimeDelta)
{
    // 아래에서 사용할 변수들
#pragma region Variables Setting

    _uint iCurLevel = CGameInstance::GetInstance()->Get_CurrentLevel();

    CTransform* pTargetTransform = dynamic_cast<CTransform*>(m_pGameInstance->GetInstance()->Get_Component(iCurLevel, TEXT("Layer_Player"), TEXT("Com_Transform")));

    const _float3 vMonsterPos = m_pTransformCom->Get_State(STATE::POSITION);
    _float3 vTargetPos = pTargetTransform->Get_State(STATE::POSITION);

    _float3 vDiff = -vMonsterPos + vTargetPos;
    _float fDistance = D3DXVec3Length(&vDiff);


    // ********* matMonster 구하기
    _float4x4 matMonsterWorld = *m_pTransformCom->Get_WorldMatrix();

    _wstring strCurStateTag = m_pAnimatorCom->Get_CurStateTag();


    _float3 vTerrainPos = m_pTerrainTransformCom->Get_State(STATE::POSITION);
    _float3 vTerrainScale = m_pTerrainTransformCom->Get_Scaled();

#pragma endregion

    // 이펙트 크기조절용 초기설정
#pragma region Effect Setting

    // 이펙트용
    // 1. 원점으로 이동
    _float4x4 matTransToOrigin = {};
    D3DXMatrixIdentity(&matTransToOrigin);
    D3DXMatrixTranslation(&matTransToOrigin, -matMonsterWorld._41, -matMonsterWorld._42, -matMonsterWorld._43);

    // 2. 크기
    _float4x4 matScale = {};
    D3DXMatrixIdentity(&matScale);
    D3DXMatrixScaling(&matScale, -1.f, 1.f, 1.f);

    // 3. 자전
    _float4x4 matRotateChild = {};
    D3DXMatrixIdentity(&matRotateChild);
    //D3DXMatrixRotationX(&matRotateChild, D3DXToRadian(-90)); // 안되면 -90도도 해보기

    _float4x4 matRotateChildtoPlayer = {};
    D3DXMatrixIdentity(&matRotateChildtoPlayer);
    _float fAngle = atan2f(vTargetPos.x - vMonsterPos.x, vTargetPos.z - vMonsterPos.z);
    _float fDegree = D3DXToDegree(fAngle) + 180;
    //D3DXMatrixRotationY(&matRotateChildtoPlayer, D3DXToRadian(fDegree));  // 플레이어 방향을 보게 하고 싶다면 이쪽 주석을 해제

    // 4. 원래 위치(몬스터)로 재이동
    _float4x4 matTransReturn = {};
    D3DXMatrixIdentity(&matTransReturn);
    D3DXMatrixTranslation(&matTransReturn, matMonsterWorld._41, matMonsterWorld._42, matMonsterWorld._43);

    // 5. 거기에 추가 이동 (y축 오프셋 후 플레이어 방향)
    _float4x4 matTransOffset = {};
    D3DXMatrixIdentity(&matTransOffset);
    //D3DXMatrixTranslation(&matTransOffset, 0, -0.12f, 0);

    _float4x4 matTransAddition = {};
    D3DXMatrixIdentity(&matTransAddition);
    //_float3 vDiff = -vPlayerPos + vMonsterPos;       // 플레이어 위치에서 마우스 교차좌표로 가는 벡터 
    vDiff = -vMonsterPos + vTargetPos;
    D3DXVec3Normalize(&vDiff, &vDiff);              // 를 단위벡터화, 안되면 vDiff 순서 바꿔보기
    _float fDistanceOffset = 1.0f;                   // ** ksta : 중점으로부터 떨어져 있을 거리 **
    vDiff *= fDistanceOffset;
    //D3DXMatrixTranslation(&matTransAddition, vDiff.x, 0, vDiff.z);

    matMonsterWorld = matTransToOrigin * matScale * matRotateChild * matRotateChildtoPlayer * matTransReturn * matTransOffset * matTransAddition;

#pragma endregion

    _int iMaxFrame_Pattern = 230;      // 이 패턴은 몇프레임동안 플레이될 것인가


    // 완전한 가장자리로 가서 레이저 공격 후 플레이어에게 돌아옴


    _float fMoveSpeed = 1.5f;       // 아스카드 도약 이속

    if (IS_BETWEEN(m_iElapsedFrame_Pattern, 10, 25))
    {
        if (m_iElapsedFrame_Pattern == 10)
        {
            m_vTargettedPos = vTargetPos;

            // 저장했던 플레이어의 위치를 기준으로, 해당 사분면을 제외하고 랜덤한 사분면으로 이동

            _float3 vMovePos{};
            _float fPosOffset = 14.9f;

            _float3 vMovePosList[4] = {};
            vMovePosList[0] = vTerrainPos + _float3{ -fPosOffset, 0, fPosOffset };
            vMovePosList[1] = vTerrainPos + _float3{ fPosOffset, 0, fPosOffset };
            vMovePosList[2] = vTerrainPos + _float3{ fPosOffset, 0, -fPosOffset };
            vMovePosList[3] = vTerrainPos + _float3{ -fPosOffset, 0, -fPosOffset };

            _bool isPlayerPosX_Positive = (vTerrainPos.x < m_vTargettedPos.x) ? true : false;
            _bool isPlayerPosZ_Positive = (vTerrainPos.z < m_vTargettedPos.z) ? true : false;

            _int iNotMovePosIndex;
            if (!isPlayerPosX_Positive && isPlayerPosZ_Positive)           iNotMovePosIndex = 0;
            else if (isPlayerPosX_Positive && isPlayerPosZ_Positive)            iNotMovePosIndex = 1;
            else if (isPlayerPosX_Positive && !isPlayerPosZ_Positive)           iNotMovePosIndex = 2;
            else if (!isPlayerPosX_Positive && !isPlayerPosZ_Positive)          iNotMovePosIndex = 3;

            _int iRandPosIndex = static_cast<_int>(m_pGameInstance->Compute_Random(0.f, 4.f));
            if (iNotMovePosIndex == iRandPosIndex)
                iRandPosIndex = (iRandPosIndex + 2) % 4;

            m_vMovePos = vMovePosList[iRandPosIndex]; // 최종 정해진 이동 위치
        }

        _float3 vDiff = vTargetPos - vMonsterPos;
        _float fDiff = D3DXVec3Length(&vDiff);

        // 실제 이동

        m_pTransformCom->Move_To(m_vMovePos, fTimeDelta * (25.f - fDiff) * fMoveSpeed, 0.5f);
    }
    else if (m_iElapsedFrame_Pattern == 25)
    {
        // 레이저 준비 (상태변화)
        CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::NORMAL_EFFECT, L"Prototype_Component_Boss_Askard_Laser_FX",
            vMonsterPos + _float3{ 0, 0, 0 }, { 0, 0, 0, 1 }, { 3, 3, 3 });
        if      (m_iPhase == 0)     m_pAnimatorCom->Change_State(L"P1_Laser");
        else if (m_iPhase == 1)     m_pAnimatorCom->Change_State(L"P2_Laser");
    }
    else if (m_iElapsedFrame_Pattern == 85)
    {
        // 레이저 가이드
        
        _float fGuideOffset = 0.8f; // 4방향 표현을 위해 중점으로부터 얼만큼 이동시킬건지
        _float fYPosOffset = 0.2f;

        _float fGuideDeg = 0.f;
        _float3 vGuidePosOffset = {};
        if      (vMonsterPos.x < vTerrainPos.x && vMonsterPos.z < vTerrainPos.z){ // 제4 사분면
            vGuidePosOffset = { -fGuideOffset, 0, -fGuideOffset };
            // 각도 조건 넣어서 1143번째줄에 넣어줘야함
            fGuideDeg = 0.f;
        }
        else if (vMonsterPos.x > vTerrainPos.x && vMonsterPos.z < vTerrainPos.z){ // 제3 사분면
            vGuidePosOffset = { fGuideOffset, 0, -fGuideOffset };
            fGuideDeg = 270.f;
        }
        else if (vMonsterPos.x > vTerrainPos.x && vMonsterPos.z > vTerrainPos.z){ // 제2 사분면
            vGuidePosOffset = { fGuideOffset, 0, fGuideOffset };
            fGuideDeg = 180.f;
        }
        else if (vMonsterPos.x < vTerrainPos.x && vMonsterPos.z > vTerrainPos.z){ // 제1 사분면
            vGuidePosOffset = { -fGuideOffset, 0, fGuideOffset };
            fGuideDeg = 90.f;
        }

#pragma region Effect Setting Change
        _float4x4 matTargetWorld = *pTargetTransform->Get_WorldMatrix();

        // 1. 원점으로 이동
        _float4x4 matTransToOriginP = {};
        D3DXMatrixIdentity(&matTransToOriginP);
        D3DXMatrixTranslation(&matTransToOriginP, -matTargetWorld._41, -matTargetWorld._42, -matTargetWorld._43);

        // 2. 크기
        _float4x4 matScaleP = {};
        D3DXMatrixIdentity(&matScaleP);
        //D3DXMatrixScaling(&matScaleP, 0.5f, 0.5f, 0.5f);

        // 3. 자전
        _float4x4 matRotateChildP = {};
        D3DXMatrixIdentity(&matRotateChildP);
        D3DXMatrixRotationX(&matRotateChildP, D3DXToRadian(-90)); // 안되면 -90도도 해보기
        _float4x4 matRotateChildP2 = {};
        D3DXMatrixIdentity(&matRotateChildP2);
        D3DXMatrixRotationY(&matRotateChildP2, D3DXToRadian(fGuideDeg)); // 안되면 -90도도 해보기

        // 4. 원래 위치(플레이어)로 재이동
        _float4x4 matTransReturnP = {};
        D3DXMatrixIdentity(&matTransReturnP);
        D3DXMatrixTranslation(&matTransReturnP, matTargetWorld._41, matTargetWorld._42, matTargetWorld._43);

        // 5. 거기에 추가 이동
        _float4x4 matTransOffsetP = {};
        D3DXMatrixIdentity(&matTransOffsetP);
        D3DXMatrixTranslation(&matTransOffsetP, vGuidePosOffset.x, vGuidePosOffset.y + fYPosOffset, vGuidePosOffset.z);

        matTargetWorld = matTransToOriginP * matScaleP * matRotateChildP * matRotateChildP2 * matTransReturnP * matTransOffsetP;
#pragma endregion
        _float4x4 matEmpty;
        D3DXMatrixIdentity(&matEmpty);

        CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::NORMAL_EFFECT, L"Prototype_Component_Boss_Askard_Laser_Alert",
            *pTargetTransform->Get_WorldMatrix(), matTargetWorld, pTargetTransform, 0.f);
    }
    else if (m_iElapsedFrame_Pattern == 119)
    {
#pragma region Laser Setting Change

        // 사분면에 따라 + 랜덤값에 따라 최초 락온 지점이 다르게끔, 
        // fLaserRotateDeg 값을 이용 바깥쪽 스타트 - 안쪽으로 돌게끔 조건 부여
        _float fRotateSpeedDeg = 120.f;

        _float fLaserRotateDeg = {};
        _int iRand = static_cast<_int>(m_pGameInstance->Compute_Random(0, 2));// 레이저의 시작 각도, 도는 방향을 랜덤히 지정함
        if (iRand == 0)
        {
            if (vMonsterPos.x < vTerrainPos.x) {
                m_vLockedOnPos = { vTerrainPos.x - (vTerrainScale.x / 2), 0, vTerrainPos.z };
                fLaserRotateDeg = (vMonsterPos.z < vTerrainPos.z) ? fRotateSpeedDeg : -fRotateSpeedDeg;
            }
            else {
                m_vLockedOnPos = { vTerrainPos.x + (vTerrainScale.x / 2), 0, vTerrainPos.z };
                fLaserRotateDeg = (vMonsterPos.z < vTerrainPos.z) ? -fRotateSpeedDeg : fRotateSpeedDeg;
            }
        }
        else
        {
            if (vMonsterPos.z < vTerrainPos.z) {
                m_vLockedOnPos = { vTerrainPos.x, 0, vTerrainPos.z - (vTerrainScale.z / 2) };
                fLaserRotateDeg = (vMonsterPos.x < vTerrainPos.x) ? -fRotateSpeedDeg : fRotateSpeedDeg;
            }
            else {
                m_vLockedOnPos = { vTerrainPos.x, 0, vTerrainPos.z + (vTerrainScale.z / 2) };
                fLaserRotateDeg = (vMonsterPos.x < vTerrainPos.x) ? fRotateSpeedDeg : -fRotateSpeedDeg;
            }
        }

        // -----------

        // 크기 조절
        D3DXMatrixScaling(&matScale, -4.f, 12.f, 12.f);

        // 눕히기
        matRotateChild = {};
        D3DXMatrixIdentity(&matRotateChild);
        D3DXMatrixRotationX(&matRotateChild, D3DXToRadian(-90));

        matRotateChildtoPlayer = {};
        D3DXMatrixIdentity(&matRotateChildtoPlayer);
        fAngle = atan2f(m_vLockedOnPos.x - vMonsterPos.x, m_vLockedOnPos.z - vMonsterPos.z);
        fDegree = D3DXToDegree(fAngle) + 180;
        D3DXMatrixRotationY(&matRotateChildtoPlayer, D3DXToRadian(fDegree));  // m_vLockedOnPos 방향을 보도록 함.

        D3DXMatrixIdentity(&matTransOffset);

        matTransAddition = {};
        D3DXMatrixIdentity(&matTransAddition);
        //_float3 vDiff = -vPlayerPos + vMonsterPos;       // 플레이어 위치에서 마우스 교차좌표로 가는 벡터 
        vDiff = -vMonsterPos+m_vLockedOnPos;
        D3DXVec3Normalize(&vDiff, &vDiff);              // 를 단위벡터화, 안되면 vDiff 순서 바꿔보기
        _float fDistanceOffset = 15.f;                   // ** ksta : 중점으로부터 떨어져 있을 거리 **
        vDiff *= fDistanceOffset;
        D3DXMatrixTranslation(&matTransAddition, vDiff.x, -0.3f, vDiff.z);

        matMonsterWorld = matTransToOrigin * matScale * matRotateChild * matRotateChildtoPlayer * matTransReturn * matTransOffset * matTransAddition;

#pragma endregion

        // m_vLockedOnPos 를 기준으로 레이저 시작점 지정 (LaserGhost 처럼)
        // 300번째 줄 확인하여 돌아갈 방향 지정
        CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::MONSTER_EFFECT, L"Prototype_Component_Boss_Askard_Laser",
            *m_pTransformCom->Get_WorldMatrix(), matMonsterWorld, { 0, 0, 0 }, 0.f, 0.f, D3DXToRadian(fLaserRotateDeg));
#pragma region Setting Reset
        D3DXMatrixIdentity(&matScale);
        D3DXMatrixIdentity(&matRotateChildtoPlayer);
        D3DXMatrixIdentity(&matTransAddition);

        matMonsterWorld = matTransToOrigin * matScale * matRotateChild * matRotateChildtoPlayer * matTransReturn * matTransOffset * matTransAddition;
#pragma endregion
    }
    else if (m_iElapsedFrame_Pattern == 124)
    {
        CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::NORMAL_EFFECT, L"Prototype_Component_Boss_Askard_Laser_FX_Cycle",
            vMonsterPos + _float3{ 0, 0, 0 }, { 0, 0, 0, 1 }, { 3, 3, 3 }, 0.9f);
    }
    else if (IS_BETWEEN(m_iElapsedFrame_Pattern, 178, 188))
    {
        _float3 vDiff = vTargetPos - vMonsterPos;
        _float fDiff = D3DXVec3Length(&vDiff);
        m_pTransformCom->Move_To(m_vTargettedPos, fTimeDelta* fDiff* fMoveSpeed, 2.f);
    }



    if (strCurStateTag == L"P1_Attack_Ready")
        m_pAnimatorCom->Change_State(L"P1_Attack");
    else if (strCurStateTag == L"P1_Attack")
        m_pAnimatorCom->Change_State(L"P1_Attack_End");

    if (strCurStateTag == L"P2_Attack_Ready")
        m_pAnimatorCom->Change_State(L"P2_Attack");
    else if (strCurStateTag == L"P2_Attack")
        m_pAnimatorCom->Change_State(L"P2_Attack_End");

    if (strCurStateTag == L"P1_Laser")
        m_pAnimatorCom->Change_State(L"P1_Attack_End");

    if (strCurStateTag == L"P2_Laser")
        m_pAnimatorCom->Change_State(L"P2_Attack_End");

    m_iElapsedFrame_Pattern++;

    if (m_iElapsedFrame_Pattern >= iMaxFrame_Pattern)
    {
        m_iElapsedFrame_Pattern = 0;
        m_ePattern = PATTERN_ASKARD::PT_IDLE;
        if      (m_iPhase == 0)     m_pAnimatorCom->Change_State(L"P1_Idle");
        else if (m_iPhase == 1)     m_pAnimatorCom->Change_State(L"P2_Idle");
    }
}

//void CAskard::Play_Following_Eyes(_float fTimeDelta) {}

void CAskard::Play_Spark(_float fTimeDelta)
{

    // 아래에서 사용할 변수들
#pragma region Variables Setting

    _uint iCurLevel = CGameInstance::GetInstance()->Get_CurrentLevel();

    CTransform* pTargetTransform = dynamic_cast<CTransform*>(m_pGameInstance->GetInstance()->Get_Component(iCurLevel, TEXT("Layer_Player"), TEXT("Com_Transform")));

    const _float3 vMonsterPos = m_pTransformCom->Get_State(STATE::POSITION);
    _float3 vTargetPos = pTargetTransform->Get_State(STATE::POSITION);

    _float3 vDiff = -vMonsterPos + vTargetPos;
    _float fDistance = D3DXVec3Length(&vDiff);


    // ********* matMonster 구하기
    _float4x4 matMonsterWorld = *m_pTransformCom->Get_WorldMatrix();

    _wstring strCurStateTag = m_pAnimatorCom->Get_CurStateTag();


    _float3 vTerrainPos = m_pTerrainTransformCom->Get_State(STATE::POSITION);
    _float3 vTerrainScale = m_pTerrainTransformCom->Get_Scaled();

#pragma endregion

    // 이펙트 크기조절용 초기설정
#pragma region Effect Setting

    // 이펙트용
    // 1. 원점으로 이동
    _float4x4 matTransToOrigin = {};
    D3DXMatrixIdentity(&matTransToOrigin);
    D3DXMatrixTranslation(&matTransToOrigin, -matMonsterWorld._41, -matMonsterWorld._42, -matMonsterWorld._43);

    // 2. 크기
    _float4x4 matScale = {};
    D3DXMatrixIdentity(&matScale);
    D3DXMatrixScaling(&matScale, -1.f, 1.f, 1.f);

    // 3. 자전
    _float4x4 matRotateChild = {};
    D3DXMatrixIdentity(&matRotateChild);
    //D3DXMatrixRotationX(&matRotateChild, D3DXToRadian(-90)); // 안되면 -90도도 해보기

    _float4x4 matRotateChildtoPlayer = {};
    D3DXMatrixIdentity(&matRotateChildtoPlayer);
    _float fAngle = atan2f(vTargetPos.x - vMonsterPos.x, vTargetPos.z - vMonsterPos.z);
    _float fDegree = D3DXToDegree(fAngle) + 180;
    //D3DXMatrixRotationY(&matRotateChildtoPlayer, D3DXToRadian(fDegree));  // 플레이어 방향을 보게 하고 싶다면 이쪽 주석을 해제

    // 4. 원래 위치(몬스터)로 재이동
    _float4x4 matTransReturn = {};
    D3DXMatrixIdentity(&matTransReturn);
    D3DXMatrixTranslation(&matTransReturn, matMonsterWorld._41, matMonsterWorld._42, matMonsterWorld._43);

    // 5. 거기에 추가 이동 (y축 오프셋 후 플레이어 방향)
    _float4x4 matTransOffset = {};
    D3DXMatrixIdentity(&matTransOffset);
    //D3DXMatrixTranslation(&matTransOffset, 0, -0.12f, 0);

    _float4x4 matTransAddition = {};
    D3DXMatrixIdentity(&matTransAddition);
    //_float3 vDiff = -vPlayerPos + vMonsterPos;       // 플레이어 위치에서 마우스 교차좌표로 가는 벡터 
    vDiff = -vMonsterPos + vTargetPos;
    D3DXVec3Normalize(&vDiff, &vDiff);              // 를 단위벡터화, 안되면 vDiff 순서 바꿔보기
    _float fDistanceOffset = 1.0f;                   // ** ksta : 중점으로부터 떨어져 있을 거리 **
    vDiff *= fDistanceOffset;
    //D3DXMatrixTranslation(&matTransAddition, vDiff.x, 0, vDiff.z);

    matMonsterWorld = matTransToOrigin * matScale * matRotateChild * matRotateChildtoPlayer * matTransReturn * matTransOffset * matTransAddition;

#pragma endregion

    _int iMaxFrame_Pattern = 150;      // 이 패턴은 몇프레임동안 플레이될 것인가
    _float fMoveSpeed = 2.f;



    if (IS_BETWEEN(m_iElapsedFrame_Pattern, 10, 20))
    {
        // 플레이어 근처로 이동
        _float3 vDiff = vTargetPos - vMonsterPos;
        _float fDiff = D3DXVec3Length(&vDiff);
        m_pTransformCom->Move_To(m_vTargettedPos, fTimeDelta * fDiff * fMoveSpeed, 2.f);
    }
    else if (IS_BETWEEN(m_iElapsedFrame_Pattern, 20, 80))
    {
        if (m_iElapsedFrame_Pattern == 20)
        {
            if      (m_iPhase == 0)     m_pAnimatorCom->Change_State(L"P1_Wave");
            else if (m_iPhase == 1)     m_pAnimatorCom->Change_State(L"P2_Wave");
        }

        _float4x4 matDummy = {}; D3DXMatrixIdentity(&matDummy);
        if (m_iElapsedFrame_Pattern == 24)
            CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::NORMAL_EFFECT, L"Prototype_Component_Boss_Askard_Phase1_Wave_Back",
                *m_pTransformCom->Get_WorldMatrix(), matDummy, m_pTransformCom, 0.92f);



        _int iFrame = m_iElapsedFrame_Pattern - 20;

        // 프레임마다의 위치를 정의 (공중에 떠있는)
        _float fPosY = (iFrame <= 51) ? 2 : -pow((0.2f * (iFrame - 52.92893f)), 2.f) + 2.f;
        m_pTerrainBox->SetUp_OnTerrainBox(m_pTransformCom, _float3(0.05f, 0.8f + fPosY, 0.05f));
    }
    else if (m_iElapsedFrame_Pattern == 80)
    {
        if      (m_iPhase == 0)     m_pAnimatorCom->Change_State(L"P1_Idle", true);
        else if (m_iPhase == 1)     m_pAnimatorCom->Change_State(L"P2_Idle", true);
        m_pTerrainBox->SetUp_OnTerrainBox(m_pTransformCom, _float3(0.05f, 0.8f, 0.05f));

        CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::MONSTER_EFFECT, L"Prototype_Component_Boss_Askard_Phase1_Wave_Burst",
            *m_pTransformCom->Get_WorldMatrix());

        D3DXQUATERNION qRot = { 0, 0, 0, 1 };
        _float3 vAxis = { 1, 0, 0 };
        _float3 vScale = { 5, 5, 5 };

        D3DXQuaternionRotationAxis(&qRot, &vAxis, 90);
        CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::MONSTER_EFFECT, L"Prototype_Component_Boss_Askard_Phase1_Wave_Burst_Ground",
            vMonsterPos - _float3{0.f, -0.3f, 0.f}, qRot, vScale);

        
        // 격돌
        m_isAllStop = true;

        _uint iLevel = m_pGameInstance->Get_CurrentLevel();
        static_cast<CEvent_ClashPattern*>(m_pGameInstance->Find_UIObj(iLevel, L"EVENT_Clash"))->Start_Event();



    }
    else if (m_iElapsedFrame_Pattern == 81)
    {
        // 성공이면 패턴 넘김, 아니면 스파크 소환
         
        _uint iCurLevel = m_pGameInstance->Get_CurrentLevel();
        CEvent_ClashPattern::CLASH_RESULT eResult = static_cast<CEvent_ClashPattern*>(m_pGameInstance->Find_UIObj(iCurLevel, L"EVENT_Clash"))->Get_Event_Result();

        _bool isSuccess = false;


        switch (eResult)
        {
        case Client::CEvent_ClashPattern::CLASH_RESULT::CLASH_CLEAR:
            isSuccess = true;
            break;
        case Client::CEvent_ClashPattern::CLASH_RESULT::CLASH_FAIL:
            isSuccess = false;
            break;
        case Client::CEvent_ClashPattern::CLASH_RESULT::CLASH_NONPLAYING:
            MSG_BOX(L"격돌 플레이중 아님");
            isSuccess = false;
            break;
        default:
            isSuccess = false;
            break;
        }
         


        // 성공 시, 패턴 스킵
        if (!isSuccess)
        {
            // 찌릿이 소환
            for (int angle = 0; angle < 360; angle += 15)
            {
                // angle 도 만큼 회전하는 행렬 생성 및 적용으로 둥글게 퍼지는 이펙트 제작
                _float fThrownPower = 6.f;
                _float fThrownAtkLifeTime = 2.f;

                _float3 vThrownDir = { 1, 0, 0 };
                _float4x4 matRotY = {};
                D3DXMatrixIdentity(&matRotY);
                D3DXMatrixRotationY(&matRotY, angle);
                D3DXVec3TransformNormal(&vThrownDir, &vThrownDir, &matRotY);

#pragma region Effect Setting
                D3DXMatrixIdentity(&matScale);
                D3DXMatrixScaling(&matScale, -0.5f, 0.5f, 0.5f);

                D3DXMatrixIdentity(&matTransOffset);
                D3DXMatrixTranslation(&matTransOffset, 0.f, -0.3f, 0.f);    // 이펙트 y축 오프셋 조절

                matMonsterWorld = matTransToOrigin * matScale * matRotateChild * matRotateChildtoPlayer * matTransReturn * matTransOffset * matTransAddition;
#pragma endregion
                CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::MONSTER_EFFECT, L"Prototype_Component_Boss_Askard_ShockWaveFX",
                    *m_pTransformCom->Get_WorldMatrix(), matMonsterWorld, vThrownDir, fThrownPower, fThrownAtkLifeTime, 0.f);
#pragma region Reset Effect
                D3DXMatrixScaling(&matScale, -1.f, 1.f, 1.f);
                D3DXMatrixIdentity(&matTransOffset);

                matMonsterWorld = matTransToOrigin * matScale * matRotateChild * matRotateChildtoPlayer * matTransReturn * matTransOffset * matTransAddition;
#pragma endregion

            }
        }


    }




    if (strCurStateTag == L"P1_Wave")
        m_pAnimatorCom->Change_State(L"P1_Idle");

    if (strCurStateTag == L"P2_Wave")
        m_pAnimatorCom->Change_State(L"P2_Idle");

    m_iElapsedFrame_Pattern++;

    if (m_iElapsedFrame_Pattern >= iMaxFrame_Pattern)
    {
        m_iElapsedFrame_Pattern = 0;
        m_ePattern = PATTERN_ASKARD::PT_IDLE;

        if      (m_iPhase == 0)     m_pAnimatorCom->Change_State(L"P1_Idle");
        else if (m_iPhase == 1)     m_pAnimatorCom->Change_State(L"P2_Idle");
    }
}


// Phase 2

void CAskard::Play_Dark_Tentacle(_float fTimeDelta)
{

    // 아래에서 사용할 변수들
#pragma region Variables Setting

    _uint iCurLevel = CGameInstance::GetInstance()->Get_CurrentLevel();

    CTransform* pTargetTransform = dynamic_cast<CTransform*>(m_pGameInstance->GetInstance()->Get_Component(iCurLevel, TEXT("Layer_Player"), TEXT("Com_Transform")));

    const _float3 vMonsterPos = m_pTransformCom->Get_State(STATE::POSITION);
    _float3 vTargetPos = pTargetTransform->Get_State(STATE::POSITION);

    _float3 vDiff = -vMonsterPos + vTargetPos;
    _float fDistance = D3DXVec3Length(&vDiff);


    // ********* matMonster 구하기
    _float4x4 matMonsterWorld = *m_pTransformCom->Get_WorldMatrix();

    _wstring strCurStateTag = m_pAnimatorCom->Get_CurStateTag();


    _float3 vTerrainPos = m_pTerrainTransformCom->Get_State(STATE::POSITION);
    _float3 vTerrainScale = m_pTerrainTransformCom->Get_Scaled();

#pragma endregion

    _int iMaxFrame_Pattern = 1300;   // 패턴 지속 시간이 꽤 김

    // 최초 80프레임 간격에서 최종 24프레임 간격까지 줄어듬
    // 14회 공격. 원작기준
    // 1회 당 2~3프레임씩 줄이면 될 듯

    // 아래는 15회, 4~5프레임씩 줄임

    switch (m_iElapsedFrame_Pattern)
    {
    case 30:
    {
#pragma region Effect Setting
        _float4x4 matTranslateUnlit = {};
        D3DXMatrixIdentity(&matTranslateUnlit);
        D3DXMatrixTranslation(&matTranslateUnlit, 0.05f, 0.f, -0.01f);;
#pragma endregion

        m_pAnimatorCom->Change_State(L"P2_Tentacle"); // Hidden 말고 점점 사라지는 이펙트 있었는데
        CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::NORMAL_EFFECT, L"Prototype_Component_Boss_Askard_Phase2_Tentacle_Unlit",
            *m_pTransformCom->Get_WorldMatrix(), matTranslateUnlit);
        dynamic_cast<CCamera_Follow*>(m_pGameInstance->Get_GameObject(m_pGameInstance->Get_CurrentLevel(), TEXT("Layer_Camera")))->Trigger_CinematicLookFromTop(18.f);
    }
        break;
    case 180:
    case 260:
    case 336:
    case 408:
    case 476:
    case 540:
    case 600:
    case 656:
    case 708:
    case 756:
    case 799:
    case 837:
    case 870:
    case 898:
    case 921:
    {
        // 방향 랜덤으로 정함
        _uint iDir = m_pGameInstance->Compute_Random(0, 4);
        CAskard_Dark_Tentacle::DARKTENTACLE_DIR eDir = static_cast<CAskard_Dark_Tentacle::DARKTENTACLE_DIR>(iDir);
        //
        // 촉수 소환
        Summon_Dark_Tentacle(vTargetPos, eDir);
    }
        break;
    case 1040:
        // 촉수 마무리 패턴 (위치고정)
    {
        _int iLanes = 7;

        for (int i = 0; i < iLanes; i++)
        {
            // 3개일 때는 1/7, 3/7, 5/7
            // 5개일 때는 1/11, 3/11, 5/11, 7/11, 9/11
            // n개일 때는 1/(2n+1), ... , (2n-1)/(2n+1)

            _float fLaneStart = vTerrainPos.z - vTerrainScale.z / 2;
            _float fLaneSpace = vTerrainScale.z / (iLanes * 2 + 1);

            _float fPosZ = fLaneStart + fLaneSpace * (2 * (i + 1) - 1);

            _float3 vResultPos = { vTargetPos.x, vTargetPos.y, fPosZ };

            if (i != iLanes / 2)
                Summon_Dark_Tentacle(vResultPos, CAskard_Dark_Tentacle::DARKTENTACLE_DIR::DIR_TO_XPOS);
        }
    }
        break;
    case 1110:
    {
        // 육각총알 발사 (위치고정)
        _int iLanes = 7;

        for (int i = iLanes / 2; i < iLanes / 2 + 1; i++)
        {
            _float fLaneStart = vTerrainPos.z - vTerrainScale.z / 2;
            _float fLaneSpace = vTerrainScale.z / (iLanes * 2 + 1);

            _float fPosZ = fLaneStart + fLaneSpace * (2 * (i + 1) - 1);
            _float fPosX = vTerrainPos.x - vTerrainScale.x / 2;

            _float3 vResultPos = { fPosX, vTargetPos.y, fPosZ };

            _float4x4 matBulletFirst = {};
            D3DXMatrixTranslation(&matBulletFirst, vResultPos.x, vResultPos.y, vResultPos.z);

            // **** 이펙트 조절용 설정
#pragma region Effect Setting

            // 이펙트용
            // 1. 원점으로 이동
            _float4x4 matTransToOrigin = {};
            D3DXMatrixIdentity(&matTransToOrigin);
            D3DXMatrixTranslation(&matTransToOrigin, -vResultPos.x, -vResultPos.y, -vResultPos.z);

            // 2. 크기
            _float4x4 matScale = {};
            D3DXMatrixIdentity(&matScale);
            D3DXMatrixScaling(&matScale, -1.f, 3.f, 3.f);

            // 3. 자전
            _float4x4 matRotateChild = {};
            D3DXMatrixIdentity(&matRotateChild);
            D3DXMatrixRotationX(&matRotateChild, D3DXToRadian(90)); // 안되면 -90도도 해보기

            _float4x4 matRotateChild2 = {};
            D3DXMatrixIdentity(&matRotateChild2);
            D3DXMatrixRotationY(&matRotateChild2, D3DXToRadian(90)); // 안되면 -90도도 해보기

            // 4. 원래 위치(몬스터)로 재이동
            _float4x4 matTransReturn = {};
            D3DXMatrixIdentity(&matTransReturn);
            D3DXMatrixTranslation(&matTransReturn, vResultPos.x, vResultPos.y, vResultPos.z);

            // 5. 거기에 추가 이동 (플레이어 방향)
            _float4x4 matTransAddition = {};
            D3DXMatrixIdentity(&matTransAddition);
            //_float3 vDiff = -vPlayerPos + vMonsterPos;       // 플레이어 위치에서 마우스 교차좌표로 가는 벡터
            D3DXVec3Normalize(&vDiff, &vDiff);              // 를 단위벡터화, 안되면 vDiff 순서 바꿔보기
            _float fDistanceOffset = 1.0f;                   // ** ksta : 중점으로부터 떨어져 있을 거리 **
            vDiff *= fDistanceOffset;
            //D3DXMatrixTranslation(&matTransAddition, vDiff.x, 0, vDiff.z);

            matMonsterWorld = matTransToOrigin * matScale * matRotateChild * matRotateChild2 * matTransReturn * matTransAddition;

#pragma endregion

            CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::MONSTER_EFFECT, L"Prototype_Component_Boss_Askard_TentacleBullet",
                matBulletFirst, matMonsterWorld, _float3{1, 0, 0}, 20.f, 1.5f, 0.f);
            //Summon_Dark_Tentacle(vResultPos, CAskard_Dark_Tentacle::DARKTENTACoLE_DIR::DIR_TO_XPOS);
            
        }
    }
        break;
    case 1160:
    {
        m_pAnimatorCom->Change_State(L"P2_Tentacle_End");
        break;
    }

    }





    if (strCurStateTag == L"P2_Tentacle")
        m_pAnimatorCom->Change_State(L"Hidden");
    else if (strCurStateTag == L"P2_Tentacle_End")
        m_pAnimatorCom->Change_State(L"P2_Idle");


    //if (strCurStateTag == L"P1_Wave")
    //{
    //    m_pAnimatorCom->Change_State(L"P1_Idle");
    //}

    m_iElapsedFrame_Pattern++;

    if (m_iElapsedFrame_Pattern >= iMaxFrame_Pattern)
    {
        m_iElapsedFrame_Pattern = 0;
        m_ePattern = PATTERN_ASKARD::PT_IDLE;
        m_pAnimatorCom->Change_State(L"P2_Idle", true);
    }
}

void CAskard::Play_Corner_Laser_ADV(_float fTimeDelta)
{
    // 아래에서 사용할 변수들
#pragma region Variables Setting

    _uint iCurLevel = CGameInstance::GetInstance()->Get_CurrentLevel();

    CTransform* pTargetTransform = dynamic_cast<CTransform*>(m_pGameInstance->GetInstance()->Get_Component(iCurLevel, TEXT("Layer_Player"), TEXT("Com_Transform")));

    const _float3 vMonsterPos = m_pTransformCom->Get_State(STATE::POSITION);
    _float3 vTargetPos = pTargetTransform->Get_State(STATE::POSITION);

    _float3 vDiff = -vMonsterPos + vTargetPos;
    _float fDistance = D3DXVec3Length(&vDiff);


    // ********* matMonster 구하기
    _float4x4 matMonsterWorld = *m_pTransformCom->Get_WorldMatrix();

    _wstring strCurStateTag = m_pAnimatorCom->Get_CurStateTag();


    _float3 vTerrainPos = m_pTerrainTransformCom->Get_State(STATE::POSITION);
    _float3 vTerrainScale = m_pTerrainTransformCom->Get_Scaled();

#pragma endregion

    // 이펙트 크기조절용 초기설정
#pragma region Effect Setting

    // 이펙트용
    // 1. 원점으로 이동
    _float4x4 matTransToOrigin = {};
    D3DXMatrixIdentity(&matTransToOrigin);
    D3DXMatrixTranslation(&matTransToOrigin, -matMonsterWorld._41, -matMonsterWorld._42, -matMonsterWorld._43);

    // 2. 크기
    _float4x4 matScale = {};
    D3DXMatrixIdentity(&matScale);
    D3DXMatrixScaling(&matScale, -1.f, 1.f, 1.f);

    // 3. 자전
    _float4x4 matRotateChild = {};
    D3DXMatrixIdentity(&matRotateChild);
    //D3DXMatrixRotationX(&matRotateChild, D3DXToRadian(-90)); // 안되면 -90도도 해보기

    _float4x4 matRotateChildtoPlayer = {};
    D3DXMatrixIdentity(&matRotateChildtoPlayer);
    _float fAngle = atan2f(vTargetPos.x - vMonsterPos.x, vTargetPos.z - vMonsterPos.z);
    _float fDegree = D3DXToDegree(fAngle) + 180;
    //D3DXMatrixRotationY(&matRotateChildtoPlayer, D3DXToRadian(fDegree));  // 플레이어 방향을 보게 하고 싶다면 이쪽 주석을 해제

    // 4. 원래 위치(몬스터)로 재이동
    _float4x4 matTransReturn = {};
    D3DXMatrixIdentity(&matTransReturn);
    D3DXMatrixTranslation(&matTransReturn, matMonsterWorld._41, matMonsterWorld._42, matMonsterWorld._43);

    // 5. 거기에 추가 이동 (y축 오프셋 후 플레이어 방향)
    _float4x4 matTransOffset = {};
    D3DXMatrixIdentity(&matTransOffset);
    //D3DXMatrixTranslation(&matTransOffset, 0, -0.12f, 0);

    _float4x4 matTransAddition = {};
    D3DXMatrixIdentity(&matTransAddition);
    //_float3 vDiff = -vPlayerPos + vMonsterPos;       // 플레이어 위치에서 마우스 교차좌표로 가는 벡터 
    vDiff = -vMonsterPos + vTargetPos;
    D3DXVec3Normalize(&vDiff, &vDiff);              // 를 단위벡터화, 안되면 vDiff 순서 바꿔보기
    _float fDistanceOffset = 1.0f;                   // ** ksta : 중점으로부터 떨어져 있을 거리 **
    vDiff *= fDistanceOffset;
    //D3DXMatrixTranslation(&matTransAddition, vDiff.x, 0, vDiff.z);

    matMonsterWorld = matTransToOrigin * matScale * matRotateChild * matRotateChildtoPlayer * matTransReturn * matTransOffset * matTransAddition;

#pragma endregion

    _int iMaxFrame_Pattern = 350;      // 이 패턴은 몇프레임동안 플레이될 것인가


    // 완전한 가장자리로 가서 레이저 공격 후 플레이어에게 돌아옴


    _float fMoveSpeed = 1.5f;       // 아스카드 도약 이속

    // 아스카드 구석으로 이동 + 분신 소환
    if (IS_BETWEEN(m_iElapsedFrame_Pattern, 10, 20))
    {

        if (m_iElapsedFrame_Pattern == 10)
        {
            m_vTargettedPos = vTargetPos;

            // 저장했던 플레이어의 위치를 기준으로, 해당 사분면을 제외하고 랜덤한 사분면으로 이동

            _float3 vMovePos{};
            _float fPosOffset = 14.9f;

            _float3 vMovePosList[4] = {};
            vMovePosList[0] = vTerrainPos + _float3{ -fPosOffset, vMonsterPos.y, fPosOffset };
            vMovePosList[1] = vTerrainPos + _float3{ fPosOffset, vMonsterPos.y, fPosOffset };
            vMovePosList[2] = vTerrainPos + _float3{ fPosOffset, vMonsterPos.y, -fPosOffset };
            vMovePosList[3] = vTerrainPos + _float3{ -fPosOffset, vMonsterPos.y, -fPosOffset };

            _bool isPlayerPosX_Positive = (vTerrainPos.x < m_vTargettedPos.x) ? true : false;
            _bool isPlayerPosZ_Positive = (vTerrainPos.z < m_vTargettedPos.z) ? true : false;

            _int iNotMovePosIndex;
            if      (!isPlayerPosX_Positive && isPlayerPosZ_Positive)           iNotMovePosIndex = 0;
            else if (isPlayerPosX_Positive && isPlayerPosZ_Positive)            iNotMovePosIndex = 1;
            else if (isPlayerPosX_Positive && !isPlayerPosZ_Positive)           iNotMovePosIndex = 2;
            else if (!isPlayerPosX_Positive && !isPlayerPosZ_Positive)          iNotMovePosIndex = 3;

            _int iRandPosIndex = static_cast<_int>(m_pGameInstance->Compute_Random(0.f, 4.f));
            if (iNotMovePosIndex == iRandPosIndex)
                iRandPosIndex = (iRandPosIndex + 2) % 4;

            m_vMovePos = vMovePosList[iRandPosIndex]; // 최종 정해진 이동 위치


            // 분신 위치 지정. (플레이어가 없고, 본체가 없는 사분면 둘)
            for (int i = 0; i < 4; i++) {
                if (i != iRandPosIndex &&
                    i != iNotMovePosIndex)
                m_vecLaserMovePos.push_back(vMovePosList[i]);
            }
        }

        _float3 vDiff = vTargetPos - vMonsterPos;
        _float fDiff = D3DXVec3Length(&vDiff);

        // 실제 이동

        m_pTransformCom->Move_To(m_vMovePos, fTimeDelta * (25.f - fDiff) * fMoveSpeed * 1.5f, 0.5f);

        // 분신 소환. 3초간.
        if (m_iElapsedFrame_Pattern == 19) {
            for (int i = 0; i < m_vecLaserMovePos.size(); i++) {
                _int iFlip = (m_vecLaserMovePos[i].x > vTerrainPos.x) ? -1 : 1;
                CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::NORMAL_EFFECT, L"Prototype_Component_Boss_Askard_Phase2_Idle",
                    m_vecLaserMovePos[i], {0, 0, 0, 1}, {3.f * iFlip, 3, 3}, 3.f);
                int a = 10;
            }
        }
    }
    // 아스카드 레이저 준비 FX이펙트 및 상태변화
    else if (m_iElapsedFrame_Pattern == 20)
    {
        _float fOffsetY = -2.f;

        // 레이저 준비 (상태변화)
        CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::NORMAL_EFFECT, L"Prototype_Component_Boss_Askard_Laser_FX",
            vMonsterPos + _float3{ 0, fOffsetY, 0 }, { 0, 0, 0, 1 }, { 3, 3, 3 });
        CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::NORMAL_EFFECT, L"Prototype_Component_Boss_Askard_Laser_FX",
            m_vecLaserMovePos[0] + _float3{ 0, fOffsetY, 0 }, { 0, 0, 0, 1 }, { 3, 3, 3 });
        CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::NORMAL_EFFECT, L"Prototype_Component_Boss_Askard_Laser_FX",
            m_vecLaserMovePos[1] + _float3{ 0, fOffsetY, 0 }, { 0, 0, 0, 1 }, { 3, 3, 3 });
        m_pAnimatorCom->Change_State(L"P2_Laser");
    }
    // 레이저 가이드 생성
    else if (m_iElapsedFrame_Pattern == 80)
    {
        // 레이저 가이드

        _float fGuideOffset = 0.8f; // 4방향 표현을 위해 중점으로부터 얼만큼 이동시킬건지
        _float fYPosOffset = 0.2f;

        _float fGuideDeg = 0.f;
        _float3 vGuidePosOffset = {};
        if (vMonsterPos.x < vTerrainPos.x && vMonsterPos.z < vTerrainPos.z) {   // 제4 사분면
            vGuidePosOffset = { -fGuideOffset, 0, -fGuideOffset };
            // 각도 조건 넣어서 1143번째줄에 넣어줘야함
            fGuideDeg = 0.f;
        }
        else if (vMonsterPos.x > vTerrainPos.x && vMonsterPos.z < vTerrainPos.z) { // 제3 사분면
            vGuidePosOffset = { fGuideOffset, 0, -fGuideOffset };
            fGuideDeg = 270.f;
        }
        else if (vMonsterPos.x > vTerrainPos.x && vMonsterPos.z > vTerrainPos.z) { // 제2 사분면
            vGuidePosOffset = { fGuideOffset, 0, fGuideOffset };
            fGuideDeg = 180.f;
        }
        else if (vMonsterPos.x < vTerrainPos.x && vMonsterPos.z > vTerrainPos.z) { // 제1 사분면
            vGuidePosOffset = { -fGuideOffset, 0, fGuideOffset };
            fGuideDeg = 90.f;
        }

#pragma region Effect Setting Change
        _float4x4 matTargetWorld = *pTargetTransform->Get_WorldMatrix();

        // 1. 원점으로 이동
        _float4x4 matTransToOriginP = {};
        D3DXMatrixIdentity(&matTransToOriginP);
        D3DXMatrixTranslation(&matTransToOriginP, -matTargetWorld._41, -matTargetWorld._42, -matTargetWorld._43);

        // 2. 크기
        _float4x4 matScaleP = {};
        D3DXMatrixIdentity(&matScaleP);
        //D3DXMatrixScaling(&matScaleP, 0.5f, 0.5f, 0.5f);

        // 3. 자전
        _float4x4 matRotateChildP = {};
        D3DXMatrixIdentity(&matRotateChildP);
        D3DXMatrixRotationX(&matRotateChildP, D3DXToRadian(-90)); // 안되면 -90도도 해보기
        _float4x4 matRotateChildP2 = {};
        D3DXMatrixIdentity(&matRotateChildP2);
        D3DXMatrixRotationY(&matRotateChildP2, D3DXToRadian(fGuideDeg)); // 안되면 -90도도 해보기

        // 4. 원래 위치(플레이어)로 재이동
        _float4x4 matTransReturnP = {};
        D3DXMatrixIdentity(&matTransReturnP);
        D3DXMatrixTranslation(&matTransReturnP, matTargetWorld._41, matTargetWorld._42, matTargetWorld._43);

        // 5. 거기에 추가 이동
        _float4x4 matTransOffsetP = {};
        D3DXMatrixIdentity(&matTransOffsetP);
        D3DXMatrixTranslation(&matTransOffsetP, vGuidePosOffset.x, vGuidePosOffset.y + fYPosOffset, vGuidePosOffset.z);

        matTargetWorld = matTransToOriginP * matScaleP * matRotateChildP * matRotateChildP2 * matTransReturnP * matTransOffsetP;
#pragma endregion
        _float4x4 matEmpty;
        D3DXMatrixIdentity(&matEmpty);

        CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::NORMAL_EFFECT, L"Prototype_Component_Boss_Askard_Laser_Alert",
            *pTargetTransform->Get_WorldMatrix(), matTargetWorld, pTargetTransform, 0.f);
    }
    // Laser1. 본체 레이저 발사각 계산 및 발사
    else if (m_iElapsedFrame_Pattern == 114)
    {
#pragma region Laser Setting Change

        // 사분면에 따라 + 랜덤값에 따라 최초 락온 지점이 다르게끔, 
        // fLaserRotateDeg 값을 이용 바깥쪽 스타트 - 안쪽으로 돌게끔 조건 부여
        _float fRotateSpeedDeg = 120.f;

        _float fLaserRotateDeg = {};
        _int iRand = static_cast<_int>(m_pGameInstance->Compute_Random(0, 2));// 레이저의 시작 각도, 도는 방향을 랜덤히 지정함
        if (iRand == 0)
        {
            if (vMonsterPos.x < vTerrainPos.x) {
                m_vLockedOnPos = { vTerrainPos.x - (vTerrainScale.x / 2), 0, vTerrainPos.z };
                fLaserRotateDeg = (vMonsterPos.z < vTerrainPos.z) ? fRotateSpeedDeg : -fRotateSpeedDeg;
            }
            else {
                m_vLockedOnPos = { vTerrainPos.x + (vTerrainScale.x / 2), 0, vTerrainPos.z };
                fLaserRotateDeg = (vMonsterPos.z < vTerrainPos.z) ? -fRotateSpeedDeg : fRotateSpeedDeg;
            }
        }
        else
        {
            if (vMonsterPos.z < vTerrainPos.z) {
                m_vLockedOnPos = { vTerrainPos.x, 0, vTerrainPos.z - (vTerrainScale.z / 2) };
                fLaserRotateDeg = (vMonsterPos.x < vTerrainPos.x) ? -fRotateSpeedDeg : fRotateSpeedDeg;
            }
            else {
                m_vLockedOnPos = { vTerrainPos.x, 0, vTerrainPos.z + (vTerrainScale.z / 2) };
                fLaserRotateDeg = (vMonsterPos.x < vTerrainPos.x) ? fRotateSpeedDeg : -fRotateSpeedDeg;
            }
        }

        // -----------

        // 크기 조절
        D3DXMatrixScaling(&matScale, -3.f, 12.f, 12.f);

        // 눕히기
        matRotateChild = {};
        D3DXMatrixIdentity(&matRotateChild);
        D3DXMatrixRotationX(&matRotateChild, D3DXToRadian(-90));

        matRotateChildtoPlayer = {};
        D3DXMatrixIdentity(&matRotateChildtoPlayer);
        fAngle = atan2f(m_vLockedOnPos.x - vMonsterPos.x, m_vLockedOnPos.z - vMonsterPos.z);
        fDegree = D3DXToDegree(fAngle) + 180;
        D3DXMatrixRotationY(&matRotateChildtoPlayer, D3DXToRadian(fDegree));  // m_vLockedOnPos 방향을 보도록 함.

        D3DXMatrixIdentity(&matTransOffset);

        matTransAddition = {};
        D3DXMatrixIdentity(&matTransAddition);
        //_float3 vDiff = -vPlayerPos + vMonsterPos;       // 플레이어 위치에서 마우스 교차좌표로 가는 벡터 
        vDiff = -vMonsterPos + m_vLockedOnPos;
        D3DXVec3Normalize(&vDiff, &vDiff);              // 를 단위벡터화, 안되면 vDiff 순서 바꿔보기
        _float fDistanceOffset = 15.f;                   // ** ksta : 중점으로부터 떨어져 있을 거리 **
        vDiff *= fDistanceOffset;
        D3DXMatrixTranslation(&matTransAddition, vDiff.x, -2.f, vDiff.z);

        matMonsterWorld = matTransToOrigin * matScale * matRotateChild * matRotateChildtoPlayer * matTransReturn * matTransOffset * matTransAddition;

#pragma endregion

        // m_vLockedOnPos 를 기준으로 레이저 시작점 지정 (LaserGhost 처럼)
        // 300번째 줄 확인하여 돌아갈 방향 지정
        CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::MONSTER_EFFECT, L"Prototype_Component_Boss_Askard_Laser",
            *m_pTransformCom->Get_WorldMatrix(), matMonsterWorld, { 0, 0, 0 }, 0.f, 0.f, D3DXToRadian(fLaserRotateDeg));
#pragma region Setting Reset
        D3DXMatrixIdentity(&matScale);
        D3DXMatrixIdentity(&matRotateChildtoPlayer);
        D3DXMatrixIdentity(&matTransAddition);

        matMonsterWorld = matTransToOrigin * matScale * matRotateChild * matRotateChildtoPlayer * matTransReturn * matTransOffset * matTransAddition;
#pragma endregion
    }
    // LaserFX. 본체 및 분신들 레이저 사이클FX 이펙트
    else if (m_iElapsedFrame_Pattern == 119)
    {
        CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::NORMAL_EFFECT, L"Prototype_Component_Boss_Askard_Laser_FX_Cycle",
            vMonsterPos + _float3{ 0, 0, 0 }, { 0, 0, 0, 1 }, { 3, 3, 3 }, 0.9f);
        CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::NORMAL_EFFECT, L"Prototype_Component_Boss_Askard_Laser_FX_Cycle",
            m_vecLaserMovePos[0] + _float3{ 0, 0, 0 }, { 0, 0, 0, 1 }, { 3, 3, 3 }, 1.9f);
        CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::NORMAL_EFFECT, L"Prototype_Component_Boss_Askard_Laser_FX_Cycle",
            m_vecLaserMovePos[1] + _float3{ 0, 0, 0 }, { 0, 0, 0, 1 }, { 3, 3, 3 }, 2.9f);
    }
    // Laser2. 분신[0] 레이저 발사각 계산 및 발사
    else if (m_iElapsedFrame_Pattern == 174)
    {
#pragma region Laser Setting Change

        // 사분면에 따라 + 랜덤값에 따라 최초 락온 지점이 다르게끔, 
        // fLaserRotateDeg 값을 이용 바깥쪽 스타트 - 안쪽으로 돌게끔 조건 부여

        // 분신 위치
        _float3 vFakeAskardPos = m_vecLaserMovePos[0];

        _float fRotateSpeedDeg = 120.f;

        _float fLaserRotateDeg = {};
        _int iRand = static_cast<_int>(m_pGameInstance->Compute_Random(0, 2));// 레이저의 시작 각도, 도는 방향을 랜덤히 지정함
        if (iRand == 0)
        {
            if (vFakeAskardPos.x < vTerrainPos.x) {
                m_vLockedOnPos = { vTerrainPos.x - (vTerrainScale.x / 2), 0, vTerrainPos.z };
                fLaserRotateDeg = (vFakeAskardPos.z < vTerrainPos.z) ? fRotateSpeedDeg : -fRotateSpeedDeg;
            }
            else {
                m_vLockedOnPos = { vTerrainPos.x + (vTerrainScale.x / 2), 0, vTerrainPos.z };
                fLaserRotateDeg = (vFakeAskardPos.z < vTerrainPos.z) ? -fRotateSpeedDeg : fRotateSpeedDeg;
            }
        }
        else
        {
            if (vFakeAskardPos.z < vTerrainPos.z) {
                m_vLockedOnPos = { vTerrainPos.x, 0, vTerrainPos.z - (vTerrainScale.z / 2) };
                fLaserRotateDeg = (vFakeAskardPos.x < vTerrainPos.x) ? -fRotateSpeedDeg : fRotateSpeedDeg;
            }
            else {
                m_vLockedOnPos = { vTerrainPos.x, 0, vTerrainPos.z + (vTerrainScale.z / 2) };
                fLaserRotateDeg = (vFakeAskardPos.x < vTerrainPos.x) ? fRotateSpeedDeg : -fRotateSpeedDeg;
            }
        }

        // -----------

        D3DXMatrixIdentity(&matTransToOrigin);
        D3DXMatrixTranslation(&matTransToOrigin, -vFakeAskardPos.x, -vFakeAskardPos.y, -vFakeAskardPos.z);

        // 크기 조절
        D3DXMatrixScaling(&matScale, -3.f, 12.f, 12.f);

        // 눕히기
        matRotateChild = {};
        D3DXMatrixIdentity(&matRotateChild);
        D3DXMatrixRotationX(&matRotateChild, D3DXToRadian(-90));

        matRotateChildtoPlayer = {};
        D3DXMatrixIdentity(&matRotateChildtoPlayer);
        fAngle = atan2f(m_vLockedOnPos.x - vFakeAskardPos.x, m_vLockedOnPos.z - vFakeAskardPos.z);
        fDegree = D3DXToDegree(fAngle) + 180;
        D3DXMatrixRotationY(&matRotateChildtoPlayer, D3DXToRadian(fDegree));  // m_vLockedOnPos 방향을 보도록 함.


        D3DXMatrixIdentity(&matTransReturn);
        D3DXMatrixTranslation(&matTransReturn, vFakeAskardPos.x, vFakeAskardPos.y, vFakeAskardPos.z);



        D3DXMatrixIdentity(&matTransOffset);

        matTransAddition = {};
        D3DXMatrixIdentity(&matTransAddition);
        //_float3 vDiff = -vPlayerPos + vMonsterPos;       // 플레이어 위치에서 마우스 교차좌표로 가는 벡터 
        vDiff = -vFakeAskardPos + m_vLockedOnPos;
        D3DXVec3Normalize(&vDiff, &vDiff);              // 를 단위벡터화, 안되면 vDiff 순서 바꿔보기
        _float fDistanceOffset = 15.f;                   // ** ksta : 중점으로부터 떨어져 있을 거리 **
        vDiff *= fDistanceOffset;
        D3DXMatrixTranslation(&matTransAddition, vDiff.x, -2.f, vDiff.z);

        matMonsterWorld = matTransToOrigin * matScale * matRotateChild * matRotateChildtoPlayer * matTransReturn * matTransOffset * matTransAddition;

#pragma endregion
        _float4x4 matFakeAskard = {};
        D3DXMatrixIdentity(&matFakeAskard);
        vFakeAskardPos; // = m_vecLaserMovePos[0];


        matFakeAskard = *m_pTransformCom->Get_WorldMatrix();
        matFakeAskard._41 = vFakeAskardPos[0];
        matFakeAskard._42 = vFakeAskardPos[1];
        matFakeAskard._43 = vFakeAskardPos[2];

        //_float3 vFakeAskardScale = m_pTransformCom->Get_Scaled();    // { 3.f, 3.f, 3.f };
        //
        //_float4x4 matFakeAskardScale = {};
        //D3DXMatrixScaling(&matFakeAskardScale, vFakeAskardScale.x, vFakeAskardScale.y, vFakeAskardScale.z);

        //_float4x4 matFakeAskardTranslate = {};
        //D3DXMatrixTranslation(&matFakeAskardTranslate, vFakeAskardPos.x, vFakeAskardPos.y, vFakeAskardPos.z);

        //matFakeAskard = matFakeAskard * matFakeAskardScale * matFakeAskardTranslate;

        CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::MONSTER_EFFECT, L"Prototype_Component_Boss_Askard_Laser",
            matFakeAskard, matMonsterWorld, {0, 0, 0}, 0.f, 0.f, D3DXToRadian(fLaserRotateDeg));

#pragma region Setting Reset

        D3DXMatrixIdentity(&matTransToOrigin);
        D3DXMatrixIdentity(&matScale);
        D3DXMatrixIdentity(&matRotateChildtoPlayer);
        D3DXMatrixIdentity(&matTransReturn);
        D3DXMatrixIdentity(&matTransAddition);

        matMonsterWorld = matTransToOrigin * matScale * matRotateChild * matRotateChildtoPlayer * matTransReturn * matTransOffset * matTransAddition;
#pragma endregion
        }
    // Laser3. 분신[1] 레이저 발사각 계산 및 발사
    else if (m_iElapsedFrame_Pattern == 234)
    {
#pragma region Laser Setting Change

        // 사분면에 따라 + 랜덤값에 따라 최초 락온 지점이 다르게끔, 
        // fLaserRotateDeg 값을 이용 바깥쪽 스타트 - 안쪽으로 돌게끔 조건 부여

        // 분신 위치
        _float3 vFakeAskardPos = m_vecLaserMovePos[1];

        _float fRotateSpeedDeg = 120.f;

        _float fLaserRotateDeg = {};
        _int iRand = static_cast<_int>(m_pGameInstance->Compute_Random(0, 2));// 레이저의 시작 각도, 도는 방향을 랜덤히 지정함
        if (iRand == 0)
        {
            if (vFakeAskardPos.x < vTerrainPos.x) {
                m_vLockedOnPos = { vTerrainPos.x - (vTerrainScale.x / 2), 0, vTerrainPos.z };
                fLaserRotateDeg = (vFakeAskardPos.z < vTerrainPos.z) ? fRotateSpeedDeg : -fRotateSpeedDeg;
            }
            else {
                m_vLockedOnPos = { vTerrainPos.x + (vTerrainScale.x / 2), 0, vTerrainPos.z };
                fLaserRotateDeg = (vFakeAskardPos.z < vTerrainPos.z) ? -fRotateSpeedDeg : fRotateSpeedDeg;
            }
        }
        else
        {
            if (vFakeAskardPos.z < vTerrainPos.z) {
                m_vLockedOnPos = { vTerrainPos.x, 0, vTerrainPos.z - (vTerrainScale.z / 2) };
                fLaserRotateDeg = (vFakeAskardPos.x < vTerrainPos.x) ? -fRotateSpeedDeg : fRotateSpeedDeg;
            }
            else {
                m_vLockedOnPos = { vTerrainPos.x, 0, vTerrainPos.z + (vTerrainScale.z / 2) };
                fLaserRotateDeg = (vFakeAskardPos.x < vTerrainPos.x) ? fRotateSpeedDeg : -fRotateSpeedDeg;
            }
        }

        // -----------

        D3DXMatrixIdentity(&matTransToOrigin);
        D3DXMatrixTranslation(&matTransToOrigin, -vFakeAskardPos.x, -vFakeAskardPos.y, -vFakeAskardPos.z);

        // 크기 조절
        D3DXMatrixScaling(&matScale, -3.f, 12.f, 12.f);

        // 눕히기
        matRotateChild = {};
        D3DXMatrixIdentity(&matRotateChild);
        D3DXMatrixRotationX(&matRotateChild, D3DXToRadian(-90));

        matRotateChildtoPlayer = {};
        D3DXMatrixIdentity(&matRotateChildtoPlayer);
        fAngle = atan2f(m_vLockedOnPos.x - vFakeAskardPos.x, m_vLockedOnPos.z - vFakeAskardPos.z);
        fDegree = D3DXToDegree(fAngle) + 180;
        D3DXMatrixRotationY(&matRotateChildtoPlayer, D3DXToRadian(fDegree));  // m_vLockedOnPos 방향을 보도록 함.


        D3DXMatrixIdentity(&matTransReturn);
        D3DXMatrixTranslation(&matTransReturn, vFakeAskardPos.x, vFakeAskardPos.y, vFakeAskardPos.z);



        D3DXMatrixIdentity(&matTransOffset);

        matTransAddition = {};
        D3DXMatrixIdentity(&matTransAddition);
        //_float3 vDiff = -vPlayerPos + vMonsterPos;       // 플레이어 위치에서 마우스 교차좌표로 가는 벡터 
        vDiff = -vFakeAskardPos + m_vLockedOnPos;
        D3DXVec3Normalize(&vDiff, &vDiff);              // 를 단위벡터화, 안되면 vDiff 순서 바꿔보기
        _float fDistanceOffset = 15.f;                   // ** ksta : 중점으로부터 떨어져 있을 거리 **
        vDiff *= fDistanceOffset;
        D3DXMatrixTranslation(&matTransAddition, vDiff.x, -2.f, vDiff.z);

        matMonsterWorld = matTransToOrigin * matScale * matRotateChild * matRotateChildtoPlayer * matTransReturn * matTransOffset * matTransAddition;

#pragma endregion
        _float4x4 matFakeAskard = {};
        D3DXMatrixIdentity(&matFakeAskard);
        vFakeAskardPos; // = m_vecLaserMovePos[0];


        matFakeAskard = *m_pTransformCom->Get_WorldMatrix();
        matFakeAskard._41 = vFakeAskardPos[0];
        matFakeAskard._42 = vFakeAskardPos[1];
        matFakeAskard._43 = vFakeAskardPos[2];

        //_float3 vFakeAskardScale = m_pTransformCom->Get_Scaled();    // { 3.f, 3.f, 3.f };
        //
        //_float4x4 matFakeAskardScale = {};
        //D3DXMatrixScaling(&matFakeAskardScale, vFakeAskardScale.x, vFakeAskardScale.y, vFakeAskardScale.z);

        //_float4x4 matFakeAskardTranslate = {};
        //D3DXMatrixTranslation(&matFakeAskardTranslate, vFakeAskardPos.x, vFakeAskardPos.y, vFakeAskardPos.z);

        //matFakeAskard = matFakeAskard * matFakeAskardScale * matFakeAskardTranslate;

        CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::MONSTER_EFFECT, L"Prototype_Component_Boss_Askard_Laser",
            matFakeAskard, matMonsterWorld, { 0, 0, 0 }, 0.f, 0.f, D3DXToRadian(fLaserRotateDeg));

#pragma region Setting Reset

        D3DXMatrixIdentity(&matTransToOrigin);
        D3DXMatrixIdentity(&matScale);
        D3DXMatrixIdentity(&matRotateChildtoPlayer);
        D3DXMatrixIdentity(&matTransReturn);
        D3DXMatrixIdentity(&matTransAddition);

        matMonsterWorld = matTransToOrigin * matScale * matRotateChild * matRotateChildtoPlayer * matTransReturn * matTransOffset * matTransAddition;
#pragma endregion
        }
    // 아스카드 이동
    else if (IS_BETWEEN(m_iElapsedFrame_Pattern, 293, 303))
    {
        _float3 vDiff = vTargetPos - vMonsterPos;
        _float fDiff = D3DXVec3Length(&vDiff);
        m_pTransformCom->Move_To(m_vTargettedPos, fTimeDelta * fDiff * fMoveSpeed, 2.f);
    }



    if (strCurStateTag == L"P2_Attack_Ready")
        m_pAnimatorCom->Change_State(L"P2_Attack");
    else if (strCurStateTag == L"P2_Attack")
        m_pAnimatorCom->Change_State(L"P2_Attack_End");

    if (strCurStateTag == L"P2_Laser")
        m_pAnimatorCom->Change_State(L"P2_Attack_End");

    if (strCurStateTag == L"P2_Attack_End")
        m_pAnimatorCom->Change_State(L"P2_Idle");

    m_iElapsedFrame_Pattern++;

    if (m_iElapsedFrame_Pattern >= iMaxFrame_Pattern)
    {
        m_vecLaserMovePos.clear();
        m_iElapsedFrame_Pattern = 0;
        m_ePattern = PATTERN_ASKARD::PT_IDLE;
        m_pAnimatorCom->Change_State(L"P2_Idle");
    }
}
#include "Askard.h"
#include "Effect_Factory.h"

//#include "Askard_Tentacle.h"
#include "Room_Manager.h"


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

    m_iMaxHp = 2000;
    m_iCulHp = 2000;

    m_eMonsterType = MONSTER_TYPE::ASKARD; // ksta
    m_ePattern = PATTERN_ASKARD::PT_IDLE;

    return S_OK;
}

void CAskard::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);

    if (m_pHpBar != nullptr &&
        m_isSummoned)
        m_pHpBar->Render_HP_Progress(m_pTransformCom, m_iCulHp, m_iMaxHp);


    if (m_iCulHp <= 0)
    {
        //m_iPhase++;
        m_iCulHp = m_iMaxHp;

        // ksta4 : 연출용 페이즈 재생.. 그 후
        // m_iPhase++ 하여 다음 패턴 재생
        
    }
    if (m_iPhase > 2)   // 2페이즈에서 체력 다 닳을 시 비로소 사망
    {
        m_iCulHp = 0;
        m_bDead = true;
    }
}

void CAskard::Update(_float fTimeDelta)
{
    // Update
    // 

    // ksta3 : 패턴 구현..
    


    // 동작

    _int iPatternLoopCycle = 350;   // 주기 프레임

    _int iStandardPatternFrame = m_iElapsedFrame_Update % iPatternLoopCycle;

    //switch (iStandardPatternFrame)
    switch (m_iElapsedFrame_Update)
    {
    case 30:
    {
        //m_ePattern = PATTERN_ASKARD::PT_SPAWN_WIDTH;
        //m_ePattern = PATTERN_ASKARD::PT_SPAWN_CROSS;
        m_ePattern = PATTERN_ASKARD::PT_SPAWN_LINE;
        
    }
        break;

    default:
        break;
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
    case Client::CAskard::PATTERN_ASKARD::PT_FOLLOWING_EYES:
        //Play_Following_Eyes(fTimeDelta);
        break;
    case Client::CAskard::PATTERN_ASKARD::PT_CORNER_LASER:
        //Play_Corner_Laser(fTimeDelta);
        break;
    case Client::CAskard::PATTERN_ASKARD::PT_SPAWN_CROSS:
        Play_Spawn_Cross(fTimeDelta);
        break;
    case Client::CAskard::PATTERN_ASKARD::PT_SPAWN_LINE:
        Play_Spawn_Line(fTimeDelta);
        break;
    case Client::CAskard::PATTERN_ASKARD::PT_SPARK:
        //Play_Spark(fTimeDelta);
        break;

        /* ***** Phase 2 ***** */
    case Client::CAskard::PATTERN_ASKARD::PT_DARK_TENTACLE:
        //Play_Dark_Tentacle(fTimeDelta);
        break;
    case Client::CAskard::PATTERN_ASKARD::PT_SPAWN_WIDTH_ADV:
        //Play_Spawn_Width_ADV(fTimeDelta);
        break;
    case Client::CAskard::PATTERN_ASKARD::PT_FOLLOWING_EYES_ADV:
        //Play_Following_Eyes_ADV(fTimeDelta);
        break;
    case Client::CAskard::PATTERN_ASKARD::PT_CORNER_LASER_ADV:
        //Play_Corner_Laser_ADV(fTimeDelta);
        break;
    case Client::CAskard::PATTERN_ASKARD::PT_SPAWN_CROSS_ADV:
        //Play_Spawn_Cross_ADV(fTimeDelta);
        break;
    case Client::CAskard::PATTERN_ASKARD::PT_SPAWN_LINE_ADV:
        //Play_Spawn_Line_ADV(fTimeDelta);
        break;
    case Client::CAskard::PATTERN_ASKARD::PT_SPARK_ADV:
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

    m_iElapsedFrame_Update++;

    if (m_pTerrainBox != nullptr) {
        m_pTerrainBox->SetUp_OnTerrainBox(m_pTransformCom, _float3(0.05f, 0.8f, 0.05f));
    }
}

void CAskard::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);

    //_float3 vPos = m_pTransformCom->Get_State(STATE::POSITION);
    //std::cout << "Askard Pos : " << vPos.x << ", " << vPos.y << ", " << vPos.z << std::endl;
}


HRESULT CAskard::Render()
{
    if (m_pAnimatorCom->Get_CurState()->pTextureCom == nullptr)
        return S_OK;

    if (!m_pTransformCom)
        return S_OK;

    SetUp_RenderState();

    m_pTransformCom->Bind_Matrix();


    m_pAnimatorCom->Update_State(); // Bind_Texture

    m_pVIBufferCom->Bind_Buffers();


    m_pVIBufferCom->Render();

    m_pTerrainBox->Render();


    Reset_RenderState();

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
    // Tentacle_Unlit (20)  // 이펙트인듯, 나중에 제거or병합할것
    //if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Boss_Askard_Phase2_Tentacle_Unlit"),
    //    TEXT("Com_Texture_P2_Tentacle_Unlit"), reinterpret_cast<CComponent**>(&m_pTextureCom_P2_Tentacle_Unlit))))
    //    return E_FAIL;
    // Wave (21)
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Boss_Askard_Phase2_Wave"),
        TEXT("Com_Texture_P2_Wave"), reinterpret_cast<CComponent**>(&m_pTextureCom_P2_Wave))))
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
    m_pAnimatorCom->Add_State(L"P1_Idle",           { m_pTextureCom_P1_Idle         , 4, true });

    m_pAnimatorCom->Add_State(L"P1_Attack",         { m_pTextureCom_P1_Attack		, 4, false });
    m_pAnimatorCom->Add_State(L"P1_Attack_End",     { m_pTextureCom_P1_Attack_End	, 4, false });
    m_pAnimatorCom->Add_State(L"P1_Attack_Ready",   { m_pTextureCom_P1_Attack_Ready	, 4, false });
    m_pAnimatorCom->Add_State(L"P1_Die",            { m_pTextureCom_P1_Die			, 4, true });
    m_pAnimatorCom->Add_State(L"P1_GroundIdle",     { m_pTextureCom_P1_GroundIdle	, 4, true });
    m_pAnimatorCom->Add_State(L"P1_Laser",          { m_pTextureCom_P1_Laser		, 4, true });
    m_pAnimatorCom->Add_State(L"P1_PhaseChange",    { m_pTextureCom_P1_PhaseChange	, 4, true });
    m_pAnimatorCom->Add_State(L"P1_PhaseStart",     { m_pTextureCom_P1_PhaseStart	, 4, true });
    m_pAnimatorCom->Add_State(L"P1_RangeAttack",    { m_pTextureCom_P1_RangeAttack	, 4, true });
    m_pAnimatorCom->Add_State(L"P1_StaffIdle",      { m_pTextureCom_P1_StaffIdle	, 4, true });
    m_pAnimatorCom->Add_State(L"P1_SummonStaff",    { m_pTextureCom_P1_SummonStaff	, 4, true });
    m_pAnimatorCom->Add_State(L"P1_Wave",           { m_pTextureCom_P1_Wave			, 4, true });

    // Phase 2
    m_pAnimatorCom->Add_State(L"P2_Attack",         { m_pTextureCom_P2_Attack		, 4, true });
    m_pAnimatorCom->Add_State(L"P2_Attack_End",     { m_pTextureCom_P2_Attack_End	, 4, true });
    m_pAnimatorCom->Add_State(L"P2_Attack_Ready",   { m_pTextureCom_P2_Attack_Ready	, 4, true });
    m_pAnimatorCom->Add_State(L"P2_Idle",           { m_pTextureCom_P2_Idle			, 4, true });
    m_pAnimatorCom->Add_State(L"P2_Laser",          { m_pTextureCom_P2_Laser		, 4, true });
    m_pAnimatorCom->Add_State(L"P2_RangeAttack",    { m_pTextureCom_P2_RangeAttack	, 4, true });
    m_pAnimatorCom->Add_State(L"P2_Tentacle",       { m_pTextureCom_P2_Tentacle		, 4, true });
    m_pAnimatorCom->Add_State(L"P2_Tentacle_End",   { m_pTextureCom_P2_Tentacle_End	, 4, true });
    //m_pAnimatorCom->Add_State(L"P2_Tentacle_Unlit", { m_pTextureCom_P2_Tentacle_Unlit, 4, true });
    m_pAnimatorCom->Add_State(L"P2_Wave",           { m_pTextureCom_P2_Wave			, 4, true });

#pragma endregion


    return S_OK;
}

HRESULT CAskard::Ready_Object()
{
    m_pHpBar = dynamic_cast<CField_Hp*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_UI_Field_Hp")));

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


void CAskard::OnCollision(CGameObject* pGameObject)
{
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
    Safe_Release(m_pTextureCom_P2_Wave);            // 어딘가에서 텍스쳐 관련 Release가 두번 더 되는듯


    Safe_Release(m_pAnimatorCom);

    __super::Free();
}





// *------------------------------*
// * 이하 패턴 함수들
// *------------------------------*


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
        m_pAnimatorCom->Change_State(L"P1_Attack_Ready");
    }
    else if (m_iElapsedFrame_Pattern == 15)
    {
        CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::NORMAL_EFFECT, L"Prototype_Component_Boss_Askard_Attack_1_FX",
            vMonsterPos + _float3{0.5f, 0, 0}, {0, 0, 0, 1}, {3, 3, 3});
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

                _float fRand = m_pGameInstance->Compute_Random(- fZRandOffset / 2, fZRandOffset / 2);    // Z축 랜덤값
                _int iRandType = static_cast<_int>(m_pGameInstance->Compute_Random(0.f, 3.f));
                
                if ((iOddTentacleIndex == ((z + 1) * (j + 1) + j - 1)) && m_iElapsedFrame_Pattern == 120)
                    iRandType = 3; // 해당 인덱스에 별종 삽입

                Summon_Tentacle(_float3{ fPosX, 0, fPosZ + fRand },static_cast<CAskard_Tentacle::TYPE_TENTACLE>(iRandType));
            }
        }
    }


    // 이전과 이어지는 동작 관리

    if (strCurStateTag == L"P1_Attack_Ready")
        m_pAnimatorCom->Change_State(L"P1_Attack");
    else if (strCurStateTag == L"P1_Attack")
        m_pAnimatorCom->Change_State(L"P1_Attack_End");



    // *******




    m_iElapsedFrame_Pattern++;

    // m_iElapsedFrame_Pattern 으로, 언제 어떤 패턴이 실행될 지를 제어
    // 각 패턴 종료시간 도달 시 
    // m_iElapsedFrame_Pattern 변수를 0으로 만들기

    if (m_iElapsedFrame_Pattern >= iMaxFrame_Pattern)
    {
        m_iElapsedFrame_Pattern = 0;
        m_ePattern = PATTERN_ASKARD::PT_IDLE;
        m_pAnimatorCom->Change_State(L"P1_Idle");
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
    _int iCalcedCycleFrame = m_iElapsedFrame_Pattern % 85;

    _int iMaxFrame_Pattern = iPatternCycleFrame * 3;      // 이 패턴은 몇프레임동안 플레이될 것인가

   
    // 플레이어에게 다가와서 4방향 촉수 소환    

    // 1. 플레이어의 방향을 구함
    // 2. 플레이어 기준으로 vTargettedPos 4종을 정한 뒤, 행렬 등 연산을 통해 90도씩 전환한 방향을 구함
    // 3. 해당 방향으로 3프레임 당 각 방향마다 1개씩 촉수 생성
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
        m_pAnimatorCom->Change_State(L"P1_Attack_Ready");
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
                                _float3{fRandOffsetX, 0, fRandOffsetZ}; // 소환위치 랜덤성 부여


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

    m_iElapsedFrame_Pattern++;

    if (m_iElapsedFrame_Pattern >= iMaxFrame_Pattern)
    {
        m_iElapsedFrame_Pattern = 0;
        m_ePattern = PATTERN_ASKARD::PT_IDLE;
        m_pAnimatorCom->Change_State(L"P1_Idle");
    }
}

void CAskard::Play_Spawn_Line(_float fTimeDelta)
{

}
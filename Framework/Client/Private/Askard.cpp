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

#pragma endregion

    // **** 이펙트 크기조절용 초기설정
#pragma region Effect Setting

    // 이펙트용
    // 1. 원점으로 이동
    _float4x4 matTransToOrigin = {};
    D3DXMatrixIdentity(&matTransToOrigin);
    D3DXMatrixTranslation(&matTransToOrigin, -matMonsterWorld._41, -matMonsterWorld._42, -matMonsterWorld._43);

    // 2. 크기
    _float4x4 matScale = {};
    D3DXMatrixIdentity(&matScale);
    D3DXMatrixScaling(&matScale, -1.5f, 1.f, 1.f);

    // 3. 자전
    _float4x4 matRotateChild = {};
    D3DXMatrixIdentity(&matRotateChild);
    //D3DXMatrixRotationX(&matRotateChild, D3DXToRadian(-90)); // 안되면 -90도도 해보기

    _float4x4 matRotateChildtoPlayer = {};
    D3DXMatrixIdentity(&matRotateChildtoPlayer);
    _float fAngle = atan2f(vTargetPos.x - vMonsterPos.x, vTargetPos.z - vMonsterPos.z);
    _float fDegree = D3DXToDegree(fAngle) + 180;
    //D3DXMatrixRotationY(&matRotateChildtoPlayer, D3DXToRadian(fDegree));

    // 4. 원래 위치(몬스터)로 재이동
    _float4x4 matTransReturn = {};
    D3DXMatrixIdentity(&matTransReturn);
    D3DXMatrixTranslation(&matTransReturn, matMonsterWorld._41, matMonsterWorld._42, matMonsterWorld._43);

    // 5. 거기에 추가 이동 (플레이어 방향)
    _float4x4 matTransAddition = {};
    D3DXMatrixIdentity(&matTransAddition);
    //_float3 vDiff = -vPlayerPos + vMonsterPos;       // 플레이어 위치에서 마우스 교차좌표로 가는 벡터
    D3DXVec3Normalize(&vDiff, &vDiff);              // 를 단위벡터화, 안되면 vDiff 순서 바꿔보기
    _float fDistanceOffset = 1.0f;                   // ** ksta : 중점으로부터 떨어져 있을 거리 **
    vDiff *= fDistanceOffset;
    //D3DXMatrixTranslation(&matTransAddition, vDiff.x, 0, vDiff.z);

    matMonsterWorld = matTransToOrigin * matScale * matRotateChild * matRotateChildtoPlayer * matTransReturn * matTransAddition;

#pragma endregion
    // ***********************


    // ksta3 : 패턴 구현..w
    // switch


    if (m_pAnimatorCom->Get_CurStackedFrame() == 10)
    {
        Summon_Tentacle({ vMonsterPos.x - 5, vMonsterPos.y, vMonsterPos.z - 5 }, CAskard_Tentacle::TYPE_TENTACLE::TYPE_NORMAL_1);
        Summon_Tentacle({ vMonsterPos.x, vMonsterPos.y, vMonsterPos.z - 5 }, CAskard_Tentacle::TYPE_TENTACLE::TYPE_NORMAL_2);
        Summon_Tentacle({ vMonsterPos.x + 5, vMonsterPos.y, vMonsterPos.z - 5 }, CAskard_Tentacle::TYPE_TENTACLE::TYPE_NORMAL_3);
        Summon_Tentacle({ vMonsterPos.x, vMonsterPos.y, vMonsterPos.z - 10 }, CAskard_Tentacle::TYPE_TENTACLE::TYPE_ODD_1);
    }









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
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Boss_Askard_Phase2_Tentacle_Unlit"),
        TEXT("Com_Texture_P2_Tentacle_Unlit"), reinterpret_cast<CComponent**>(&m_pTextureCom_P2_Tentacle_Unlit))))
        return E_FAIL;
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

    // Phase 1
    m_pAnimatorCom->Add_State(L"P1_Idle",           { m_pTextureCom_P1_Idle         , 4, true });

    m_pAnimatorCom->Add_State(L"P1_Attack",         { m_pTextureCom_P1_Attack		, 4, true });
    m_pAnimatorCom->Add_State(L"P1_Attack_End",     { m_pTextureCom_P1_Attack_End	, 4, true });
    m_pAnimatorCom->Add_State(L"P1_Attack_Ready",   { m_pTextureCom_P1_Attack_Ready	, 4, true });
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
        ENUM_CLASS(LEVEL::LEVEL_BOSS1), L"Prototype_GameObject_Boss_Askard_Tentacle", &pDesc);
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
    __super::Free();

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
    Safe_Release(m_pTextureCom_P2_Tentacle_Unlit);
    Safe_Release(m_pTextureCom_P2_Wave);


    Safe_Release(m_pAnimatorCom);

    //Safe_Release(m_pAnimatorCom);
}
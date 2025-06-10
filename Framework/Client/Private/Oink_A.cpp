#include "Oink_A.h"
#include "Effect_Factory.h"

COink_A::COink_A(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CMonster{ pGraphic_Device }
{

}

COink_A::COink_A(const COink_A& Prototype)
    : CMonster{ Prototype }
{

}

HRESULT COink_A::Initialize_Prototype()
{
    return S_OK;
}

HRESULT COink_A::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(this->Ready_Components(pArg)))
        return E_FAIL;

    Ready_Object();
    
    m_iAtkCooldownFrames = static_cast<_int>(m_pGameInstance->Compute_Random(0, 300));

    m_iMaxHp = 80;
    m_iCulHp = 80;
    m_eMonsterType = MONSTER_TYPE::OINK;
    m_pTransformCom->Scaling(1.5f, 1.5f, 1.5f);
    return S_OK;
}

void COink_A::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
    if (m_pHpBar != nullptr &&
        m_isSummoned)
        m_pHpBar->Render_HP_Progress(m_pTransformCom, m_iCulHp, m_iMaxHp);
}

void COink_A::Update(_float fTimeDelta)
{    
    if (m_bDead)
        return;

    
    _float fMinDist = 5.f;      // 원거리 공격 할 기준 거리
    _float fMaxDist = 12.f;     // 어그로가 풀리는 기준 거리
    _float fAtkDist = 2.f;     // 근접공격할 기준 거리

    _float fChargeAtkCooldown = 5.f;    // 원거리 공격 쿨타임 (공격 종료 시점부터 흐름)
    _float fThrownAtkLifeTime = 2.f;    // 투사체 생존 시간
    _float fThrownPower = 5.f;          // 투사체 속도

    m_iAtkCooldownFrames++;


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
    D3DXMatrixScaling(&matScale, -1.5f, 1.5f, 1.5f);

    // 3. 자전
    _float4x4 matRotateChild = {};
    D3DXMatrixIdentity(&matRotateChild);
    D3DXMatrixRotationX(&matRotateChild, D3DXToRadian(-90)); // 안되면 -90도도 해보기

    _float4x4 matRotateChildtoPlayer = {};
    D3DXMatrixIdentity(&matRotateChildtoPlayer);
    _float fAngle = atan2f(vTargetPos.x - vMonsterPos.x, vTargetPos.z - vMonsterPos.z);
    _float fDegree = D3DXToDegree(fAngle) + 180;
    D3DXMatrixRotationY(&matRotateChildtoPlayer, D3DXToRadian(fDegree));

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
    D3DXMatrixTranslation(&matTransAddition, vDiff.x, 0, vDiff.z);

    matMonsterWorld = matTransToOrigin * matScale * matRotateChild * matRotateChildtoPlayer * matTransReturn * matTransAddition;

#pragma endregion
    // ***********************


    // 최초 소환
#pragma region Summon States

    if (m_pAnimatorCom->Get_CurStateTag() == L"Summon_Standby")
    {
        m_pAnimatorCom->Change_State(L"Summon", true, 0.5f);
    }
    else if (m_pAnimatorCom->Get_CurStateTag() == L"Summon")
    {
        if (!m_pAnimatorCom->Change_State(L"Summon_End"))
        {
            _int iCnt = m_pAnimatorCom->Get_CurStackedFrame();
            _float fY = max(-0.005f * (float)pow(iCnt, 2) + 4.5f, 0.2f);
            m_pTerrainBox->SetUp_OnTerrainBox(m_pTransformCom, _float3(0.05f, fY, 0.05f));
        }
        else
            m_isSummoned = true;
    }
    else if (m_pAnimatorCom->Get_CurStateTag() == L"Summon_End")
    {
        m_pAnimatorCom->Change_State(L"Idle");
    }

#pragma endregion


    // 행동 분기
#pragma region Other States

    if (!(m_pAnimatorCom->Get_CurStateTag() == L"ChargeReady" ||
        m_pAnimatorCom->Get_CurStateTag() == L"ChargeReady_Cycle" ||
        m_pAnimatorCom->Get_CurStateTag() == L"ChargeEnd" ||
        m_pAnimatorCom->Get_CurStateTag() == L"Attack_Standby"))
    {
        // 거리가 아주 가깝다면 근접공격 상태
        if (fDistance <= fAtkDist)
        {
            if (m_pAnimatorCom->Change_State(L"Attack"))
                m_isTracking = false;
        }
        // 거리가 약간 떨어져있다면 원거리 공격 준비
        else if (fDistance <= fMinDist &&
            m_iAtkCooldownFrames >= 60 * fChargeAtkCooldown)
        {
            if (m_pAnimatorCom->Change_State(L"ChargeReady"))
                m_isTracking = false;
        }
        // 거리가 적당히 떨어져있다면 추적 ON
        else if (fAtkDist <= fDistance &&
            fDistance <= fMaxDist)
        {
            if (m_pAnimatorCom->Change_State(L"Move"))
                m_isTracking = true;
        }
        // 충분히 멀리 떨어지면 Idle 상태
        else if (fDistance >= fMaxDist)
        {
            if (m_pAnimatorCom->Change_State(L"Idle"))
                m_isTracking = false;
        }
    }
    


    // 각 분기점마다의 행동
    if (m_isTracking)
    {
        float fMoveSpeed = 1.2f;

        // 플레이어 추적
        _float3 vPosDiff = vTargetPos - vMonsterPos;
        D3DXVec3Normalize(&vPosDiff, &vPosDiff);
        _float3 vNewMonsterPos = vMonsterPos + vPosDiff * fTimeDelta * fMoveSpeed;
        m_pTransformCom->Set_State(STATE::POSITION, vNewMonsterPos);
    }

    if (m_pAnimatorCom->Get_CurStateTag() == L"Attack" &&
        m_pAnimatorCom->Get_CurStackedFrame() == 24)
    {
        m_pGameInstance->StopSound(ENUM_CLASS(CHANNELID::SOUND_MONSTER_EFFECT));
        m_pGameInstance->PlaySoundW(L"attackSwish03.wav", ENUM_CLASS(CHANNELID::SOUND_MONSTER_EFFECT), g_fEFFECTVolume - 0.6f);
        CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::MONSTER_EFFECT, L"Prototype_Component_Texture_Oink_A_Effect_Swing",
            *m_pTransformCom->Get_WorldMatrix(), matMonsterWorld, true);
    }
    else if (m_pAnimatorCom->Get_CurStateTag() == L"Attack" &&
        m_pAnimatorCom->Get_IsLastFrame())
    {
        if (m_pAnimatorCom->Change_State(L"Attack_Standby")) {}
    }

    if (m_pAnimatorCom->Get_CurStateTag() == L"ChargeReady")
        m_pAnimatorCom->Change_State(L"ChargeReady_Cycle", true, 3.f);
    else if (m_pAnimatorCom->Get_CurStateTag() == L"ChargeReady_Cycle")
        if (m_pAnimatorCom->Change_State(L"Charge_End"))
        {
            _float3 vThrownDir = pTargetTransform->Get_State(STATE::POSITION) - vMonsterPos;

            m_pGameInstance->StopSound(ENUM_CLASS(CHANNELID::SOUND_MONSTER_EFFECT));
            m_pGameInstance->PlaySoundW(L"attackSpin.wav", ENUM_CLASS(CHANNELID::SOUND_MONSTER_EFFECT), g_fEFFECTVolume - 0.6f);

            CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::MONSTER_EFFECT, L"Prototype_Component_Texture_Oink_A_Effect_SpinSwing",
                *m_pTransformCom->Get_WorldMatrix(), matMonsterWorld, vThrownDir, fThrownPower, fThrownAtkLifeTime, 0.f, true);
        }
        else {}
    else if (m_pAnimatorCom->Get_CurStateTag() == L"Charge_End")
    {
        m_pAnimatorCom->Change_State(L"Attack_Standby");
        m_iAtkCooldownFrames = 0;
    }




    if (m_pAnimatorCom->Get_CurStateTag() == L"Charge_Airborne")
        m_pAnimatorCom->Change_State(L"Attack_Standby");

    // 딜레이 Exit 역할
    if (m_pAnimatorCom->Get_CurStateTag() == L"Attack_Standby")
        m_pAnimatorCom->Change_State(L"Idle");

#pragma endregion



    if (m_pTerrainBox != nullptr &&
        !(m_pAnimatorCom->Get_CurStateTag() == L"Summon_Standby" ||
            m_pAnimatorCom->Get_CurStateTag() == L"Summon")) {
        m_pTerrainBox->SetUp_OnTerrainBox(m_pTransformCom, _float3(0.05f, 0.4f, 0.05f));
    }
}

void COink_A::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
}

HRESULT COink_A::Render()
{
    if (!m_bDead)
    {
        // 플레이어 위치에 따라 좌우반전 적용,
    // 단 공격 중 등의 경우에는 변경 X
        _uint iCurLevel = m_pGameInstance->GetInstance()->Get_CurrentLevel();
        _float3 vTargetPos = {}, vMonsterPos = {};    // 플레이어 좌표
        CTransform* pTargetTransform = dynamic_cast<CTransform*>(m_pGameInstance->GetInstance()->Get_Component(iCurLevel, TEXT("Layer_Player"), TEXT("Com_Transform")));
        vTargetPos = pTargetTransform->Get_State(STATE::POSITION);
        vMonsterPos = m_pTransformCom->Get_State(STATE::POSITION);

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



        m_pTransformCom->Bind_Matrix();

        m_pAnimatorCom->Update_State(); // Bind_Texture

        m_pVIBufferCom->Bind_Buffers();

        SetUp_RenderState();

        m_pVIBufferCom->Render();

        m_pTerrainBox->Render();

        if (m_isFlippedX)
        {
            m_pVIBufferCom->ResetUV_FlipX();
            m_isFlippedX = false;
        }

    }
    
    return S_OK;
}

HRESULT COink_A::Ready_Components(void* pArg)
{
    MONSTERDESC* desc = static_cast<MONSTERDESC*>(pArg);

    /* For.Com_Texture */
    // Idle
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Texture_Oink_A_Idle"),
        TEXT("Com_Texture_Idle"), reinterpret_cast<CComponent**>(&m_pTextureCom_Idle))))
        return E_FAIL;
    // Move
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Texture_Oink_A_Move"),
        TEXT("Com_Texture_Move"), reinterpret_cast<CComponent**>(&m_pTextureCom_Move))))
        return E_FAIL;
    // Attack
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Texture_Oink_A_Attack"),
        TEXT("Com_Texture_Attack"), reinterpret_cast<CComponent**>(&m_pTextureCom_Attack))))
        return E_FAIL;
    // ChargeReady
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Texture_Oink_A_ChargeReady"),
        TEXT("Com_Texture_ChargeReady"), reinterpret_cast<CComponent**>(&m_pTextureCom_ChargeReady))))
        return E_FAIL;
    // ChargeReady_Cycle
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Texture_Oink_A_ChargeReady_Cycle"),
        TEXT("Com_Texture_ChargeReady_Cycle"), reinterpret_cast<CComponent**>(&m_pTextureCom_ChargeReady_Cycle))))
        return E_FAIL;
    // Charge_Cycle
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Texture_Oink_A_Charge_Cycle"),
        TEXT("Com_Texture_Charge_Cycle"), reinterpret_cast<CComponent**>(&m_pTextureCom_Charge_Cycle))))
        return E_FAIL;
    // Charge_Airborne
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Texture_Oink_A_Charge_Airborne"),
        TEXT("Com_Texture_Charge_Airborne"), reinterpret_cast<CComponent**>(&m_pTextureCom_Charge_Airborne))))
        return E_FAIL;
    // Charge_Down
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Texture_Oink_A_Charge_Down"),
        TEXT("Com_Texture_Charge_Down"), reinterpret_cast<CComponent**>(&m_pTextureCom_Charge_Down))))
        return E_FAIL;
    // Charge_End
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Texture_Oink_A_Charge_End"),
        TEXT("Com_Texture_Charge_End"), reinterpret_cast<CComponent**>(&m_pTextureCom_Charge_End))))
		return E_FAIL;


    /* For Com_Animator */
    CAnimator::ANIMSTATE_DESC StartAnimStateDesc{};
    StartAnimStateDesc.strTimerTag = L"Animator_Monster_Oink_A_Main";   // 해당 애니메이터가 타이머에서 사용할 태그 key값
    StartAnimStateDesc.pParentTransform = nullptr;
    StartAnimStateDesc.pChildTransform = nullptr;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Animator"),
        TEXT("Com_Animator"), reinterpret_cast<CComponent**>(&m_pAnimatorCom), &StartAnimStateDesc)))
        return E_FAIL;

    // State 삽입
    m_pAnimatorCom->Add_State(L"Summon_Standby",        { nullptr, 90 /* 나중에 랜덤값 삽입 */, false });  // 소환 딜레이용
    m_pAnimatorCom->Add_State(L"Summon",                { m_pTextureCom_Charge_Down, 4, false });      // 소환
    m_pAnimatorCom->Add_State(L"Summon_End",            { m_pTextureCom_Charge_Down, 4, false });      // 6

    m_pAnimatorCom->Add_State(L"Idle",                  { m_pTextureCom_Idle, 4, true });
	m_pAnimatorCom->Add_State(L"Move",                  { m_pTextureCom_Move, 4, true });
	m_pAnimatorCom->Add_State(L"Attack",                { m_pTextureCom_Attack, 6, false });
	m_pAnimatorCom->Add_State(L"ChargeReady",           { m_pTextureCom_ChargeReady, 6, false });
	m_pAnimatorCom->Add_State(L"ChargeReady_Cycle",     { m_pTextureCom_ChargeReady_Cycle, 6, true });
	m_pAnimatorCom->Add_State(L"Charge_Cycle",          { m_pTextureCom_Charge_Cycle, 4, true });
	m_pAnimatorCom->Add_State(L"Charge_End",            { m_pTextureCom_Charge_End, 6, false });
	m_pAnimatorCom->Add_State(L"Charge_Down",           { m_pTextureCom_Charge_Down, 4, false });
	m_pAnimatorCom->Add_State(L"Charge_Airborne",       { m_pTextureCom_Charge_Airborne, 4, false });
    m_pAnimatorCom->Add_State(L"Attack_Standby",        { m_pTextureCom_Idle, 6, false });
    

    return S_OK;
}

HRESULT COink_A::Ready_Object()
{
    m_pHpBar = dynamic_cast<CField_Hp*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_UI_Field_Hp")));

    return S_OK;
}

void COink_A::OnCollision(CGameObject* pGameObject)
{
	__super::OnCollision(pGameObject);


    switch (pGameObject->Get_ObjType())
    {
    case GAMEOBJ_TYPE::PLAYER_EFFECT:
        if (!m_bIsHit) {
            wstring strStateTag = {};
            _float fPointY = 0.f;       // 교차 평면의 기준이 될 Y값
            _float3 vRayPoint = {};     // fPointY 값 기준 마우스 Ray와 교차하는 좌표
            m_pGameInstance->Get_IntersectAtY(fPointY, vRayPoint);

            _float3 vThisPos = {};    // 플레이어 좌표
            vThisPos = m_pTransformCom->Get_State(STATE::POSITION);

            //strStateTag = (vRayPoint.z > vPlayerPos.z)?     L"Idle_Upper":
                                                            //L"Idle_Lower";

            //m_pAnimatorCom->Change_State(strStateTag, true, 2);
            m_pAnimatorCom->Change_State(L"Charge_Airborne", false, 0.5, true);


            CTransform* pEnemyTransform = dynamic_cast<CTransform*>(pGameObject->Find_Component(L"Com_Transform"));
            _float3 vEnemyPos = pEnemyTransform->Get_State(STATE::POSITION);
            _float3 vStunDir = vThisPos - vEnemyPos;
            D3DXVec3Normalize(&vStunDir, &vStunDir);

            _float3 vResult = vThisPos + vStunDir * 0.5f;    // 밀려날 정도 테스트
            m_pTransformCom->Set_State(STATE::POSITION, vResult);

            //m_bIsHit = true;
        }
    }


	//m_isTracking = true;
}

COink_A* COink_A::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    COink_A* pInstance = new COink_A(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Create : COink_A"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* COink_A::Clone(void* pArg)
{
    COink_A* pInstance = new COink_A(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Clone : COink_A"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void COink_A::Free()
{
    __super::Free();

    Safe_Release(m_pTextureCom_Idle);
    Safe_Release(m_pTextureCom_Move);
    Safe_Release(m_pTextureCom_Attack);
    Safe_Release(m_pTextureCom_ChargeReady);
    Safe_Release(m_pTextureCom_ChargeReady_Cycle);
    Safe_Release(m_pTextureCom_Charge_Cycle);
    Safe_Release(m_pTextureCom_Charge_Airborne);
    Safe_Release(m_pTextureCom_Charge_Down);
    Safe_Release(m_pTextureCom_Charge_End);

    Safe_Release(m_pAnimatorCom);
}
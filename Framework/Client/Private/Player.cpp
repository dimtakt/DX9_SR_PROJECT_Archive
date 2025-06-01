#include "Player.h"
#include "TerrainBox.h"
#include "GameInstance.h"
#include "Collider_OBB.h"
#include "Event_Manager.h"
#include "Effect_Factory.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{

}

CPlayer::CPlayer(const CPlayer& Prototype)
	:	CGameObject { Prototype }
{
}

HRESULT CPlayer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlayer::Initialize(void* pArg)
{
 	if (FAILED(Ready_Components()))
		return E_FAIL;

    m_eObjType = GAMEOBJ_TYPE::PLAYER;
    //m_pTransformCom->Scaling(1.5f, 1.5f, 1.5f);
    m_strTimerTag = L"GameObject_Player_StateDeltaTime";
    
    m_pGameInstance->Add_Timer(m_strTimerTag);      // 마지막으로 상태가 바뀐지 지난 시간을 측정할 타이머
    m_pGameInstance->Compute_TimeDelta(m_strTimerTag);
    m_pGameInstance->Subscribe(ENUM_CLASS(EVENT_TYPE::UICHANGE), this);

	return S_OK;
}

void CPlayer::Priority_Update(_float fTimeDelta)
{

}

void CPlayer::Update(_float fTimeDelta)
{    
    //m_pCollider->Update_Collider();
    if (m_pTerrainBox != nullptr) {
        m_pTerrainBox->SetUp_OnTerrainBox(m_pTransformCom, _float3(0.f, 0.1f, 0.f));
    }

    CPlayerStats::PLAYERSTAT_DESC playerStat = m_pPlayerStatsCom->Get_Stats();

    _float fPointY = 0.f;       // 교차 평면의 기준이 될 Y값
    _float3 vRayPoint = {};     // fPointY 값 기준 마우스 Ray와 교차하는 좌표
    m_pGameInstance->Get_IntersectAtY(fPointY, vRayPoint);

    _float3 vPlayerPos = {};    // 플레이어 좌표
    vPlayerPos = m_pTransformCom->Get_State(STATE::POSITION);

    _wstring strCurStateTag = m_pAnimatorCom->Get_CurStateTag();
    
    // ******** 상태 변화 분기들

    wstring strStateTag = {}; // 임시 저장용 현재 상태

    /*
    // *** 무기 트랜스폼 관련 변수설정
	CTransform* pWeaponTransform = static_cast<CTransform*>(m_pGameInstance->Get_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), L"Layer_Weapon", L"Com_Transform"));
    _float4x4 matWeaponWorld = *pWeaponTransform->Get_WorldMatrix();

    // 이펙트 크기 변경
    _float4x4 matWeaponScaleWorld;
    D3DXMatrixIdentity(&matWeaponScaleWorld);
    D3DXMatrixScaling(&matWeaponScaleWorld, 1.5f, 1.5f, 1.5f);    // 크기 변경
    matWeaponWorld *= matWeaponScaleWorld;

    // 이펙트 회전값 변경 (인자)
    _float fWeaponOffsetDegree = 40.f;
	_float fWeaponOffsetRadian = (vRayPoint.x < vPlayerPos.x) ?
        D3DXToRadian(-fWeaponOffsetDegree) :
        D3DXToRadian(fWeaponOffsetDegree);
    _float4x4 matWeaponRotateWorld;
    _float3 vAxis = { 0, 1, 0 };
    D3DXMatrixIdentity(&matWeaponRotateWorld);
    D3DXMatrixRotationAxis(&matWeaponRotateWorld, &vAxis, fWeaponOffsetRadian);
    matWeaponWorld *= matWeaponRotateWorld;

    // 이펙트 위치 변경
    _float4x4 matWeaponTranslateWorld;
    D3DXMatrixIdentity(&matWeaponTranslateWorld);
    _float3 vDiff = - vPlayerPos + vRayPoint;       // 플레이어 위치에서 마우스 교차좌표로 가는 벡터
    D3DXVec3Normalize(&vDiff, &vDiff);              // 를 단위벡터화
    _float fDistanceOffset = 0.f;                   // ** ksta : 중점으로부터 떨어져 있을 거리 **
    vDiff *= fDistanceOffset;
    D3DXMatrixTranslation(&matWeaponTranslateWorld, vDiff.x, vDiff.y, vDiff.z);
    matWeaponWorld *= matWeaponTranslateWorld;
    */

    // ***************************************
    // * [마우스 좌클] 일반 공격
    // ***************************************
    if (m_pGameInstance->IsKeyDown(VK_LBUTTON))
    {
        // 이전 상태 공격 + 현재 상태 비공격 + 1번째 공격한 지 0.3f초 이하
        if (!   (strCurStateTag == L"Attack_Upper" ||
                strCurStateTag == L"Attack_Lower"   ) &&
                m_fStackedTime <= 0.3f)
        {
            // 2번째 공격으로.
            strStateTag = (vRayPoint.z > vPlayerPos.z)?     L"Attack_Upper2" :
                                                            L"Attack_Lower2";

            // 바꾸는 데에 성공시 2타공격 이펙트 출력
            if (m_pAnimatorCom->Change_State(strStateTag, true));
            //    CEffect_Factory::GetInstance()->Create_Effect(L"Prototype_Component_Texture_Effect_Blade0_Swing1",
            //        *m_pTransformCom->Get_WorldMatrix(), matWeaponWorld, true);
        }

        // 1번째 공격으로.
        strStateTag = (vRayPoint.z > vPlayerPos.z) ?    L"Attack_Upper" :
                                                        L"Attack_Lower";

        // 바꾸는 데에 성공시 1타공격 이펙트 출력
        if (m_pAnimatorCom->Change_State(strStateTag, true));
        //    CEffect_Factory::GetInstance()->Create_Effect(L"Prototype_Component_Texture_Effect_Blade0_Swing0",
        //        *m_pTransformCom->Get_WorldMatrix(), matWeaponWorld);
    }


    // ***************************************
    // * [WASD] 이동
    // ***************************************

    // 상태전환
    if (m_pGameInstance->IsKeyHold('W') ||
        m_pGameInstance->IsKeyHold('S') ||
        m_pGameInstance->IsKeyHold('A') ||
        m_pGameInstance->IsKeyHold('D'))
    {
        // 이전에 Move 이었다면 프레임 초기화X
        if (vRayPoint.z > vPlayerPos.z)                 // 상단
        {
            if (strCurStateTag == L"Move_Lower" ||
                strCurStateTag == L"Move_Upper")
                m_pAnimatorCom->Change_State(L"Move_Upper", false);
            else
                m_pAnimatorCom->Change_State(L"Move_Upper", true);
        }
        else                                            // 하단
        {
            if (strCurStateTag == L"Move_Lower" ||
                strCurStateTag == L"Move_Upper")
                m_pAnimatorCom->Change_State(L"Move_Lower", false);
            else
                m_pAnimatorCom->Change_State(L"Move_Lower", true);
        }
    }
    else
    {
        // 이전에 Idle 이었다면 프레임 초기화 X
        if (vRayPoint.z > vPlayerPos.z)                 // 상단
        {
            if (strCurStateTag == L"Idle_Lower" ||
                strCurStateTag == L"Idle_Upper")
                m_pAnimatorCom->Change_State(L"Idle_Upper", false);
            else
                m_pAnimatorCom->Change_State(L"Idle_Upper", true);
        }
        else                                            // 하단
        {
            if (strCurStateTag == L"Idle_Lower" ||
                strCurStateTag == L"Idle_Upper")
                m_pAnimatorCom->Change_State(L"Idle_Lower", false);
            else
                m_pAnimatorCom->Change_State(L"Idle_Lower", true);
        }
    }
    // [이동 이동]
    if (m_pAnimatorTransCom->Get_CurStateTag() != L"Dash" ||
        m_pAnimatorTransCom->Get_CurStateTag() != L"Parry")
    {
        if (m_pGameInstance->IsKeyHold('W'))
            {m_pTransformCom->Go_Straight(fTimeDelta);   m_vDashDir = { 0, 0, 1 };}
        if (m_pGameInstance->IsKeyHold('S'))
            {m_pTransformCom->Go_Backward(fTimeDelta);   m_vDashDir = { 0, 0, -1 };}
        if (m_pGameInstance->IsKeyHold('A'))
            {m_pTransformCom->Go_Left(fTimeDelta);       m_vDashDir = { -1, 0, 0 };}
        if (m_pGameInstance->IsKeyHold('D'))
            {m_pTransformCom->Go_Right(fTimeDelta);      m_vDashDir = { 1, 0, 0 };}

        if (m_pGameInstance->IsKeyHold('A') && m_pGameInstance->IsKeyHold('W'))
            m_vDashDir = { -1, 0, 1 };
        if (m_pGameInstance->IsKeyHold('W') && m_pGameInstance->IsKeyHold('D'))
            m_vDashDir = { 1, 0, 1 };
        if (m_pGameInstance->IsKeyHold('D') && m_pGameInstance->IsKeyHold('S'))
            m_vDashDir = { 1, 0, -1 };
        if (m_pGameInstance->IsKeyHold('S') && m_pGameInstance->IsKeyHold('A'))
            m_vDashDir = { -1, 0, -1 };

    }

    
    // ***************************************
    // * [좌 Shift] 대쉬
    // ***************************************
    if (m_pGameInstance->IsKeyDown(VK_SPACE))
    {
        // 스페이스바 누를 시 Dash 상태로 바꾸기 시도
        if (playerStat.fDash >= 1 &&
            m_pAnimatorTransCom->Change_State(L"Dash"))
        {
            D3DXVec3Normalize(&m_vDashDir, &m_vDashDir);
            CPlayerStats::PLAYERSTAT_DESC playerStat = m_pPlayerStatsCom->Get_Stats();
            playerStat.fDash -= 1; // 이거왜안줄지????????? 디버그로확인했을땐줄긴하는데
            playerStat.isGodMode = true;
            m_pPlayerStatsCom->Set_Stats(playerStat);
        }
    }
    else
    {
        if (m_pAnimatorTransCom->Change_State(L"Idle"))
        {
            playerStat.isGodMode = false;
            m_pPlayerStatsCom->Set_Stats(playerStat);
        }
    }

    // [Dash 이동]
    if (m_pAnimatorTransCom->Get_CurStateTag() == L"Dash")
    {
        _float3 playerPos = vPlayerPos;
        playerPos += m_vDashDir * fTimeDelta * 15.f;        // 마지막으로 누른 방향으로 이동
        m_pTransformCom->Set_State(STATE::POSITION, playerPos);
    }
        

    // ***************************************
    // * [마우스 우클] 임시 : 패리, 퓨리 (PARRY, FURY)
    // ***************************************
    if (m_pGameInstance->IsKeyDown(VK_RBUTTON))
    {
        if ((m_pAnimatorCom->Get_CurStateTag() != L"Attack_Lower" &&
            m_pAnimatorCom->Get_CurStateTag() != L"Attack_Upper" &&
            m_pAnimatorCom->Get_CurStateTag() != L"Attack_Lower2" &&
            m_pAnimatorCom->Get_CurStateTag() != L"Attack_Upper2") &&
            (m_pAnimatorTransCom->Get_CurStateTag() != L"Parry" &&
            m_pAnimatorTransCom->Get_CurStateTag() != L"Fury"))
        {
            if (m_pAnimatorTransCom->Get_CurStateTag() == L"Fury_Ready")
                m_pAnimatorTransCom->Change_State(L"Fury");
            else if (m_pAnimatorTransCom->Get_CurStateTag() == L"Idle")
            {
                m_pAnimatorTransCom->Change_State(L"Parry");
                m_pAnimatorCom->Change_State(L"Parry");
            }
        }
    }
    else
    {
        m_pAnimatorTransCom->Change_State(L"Idle");
    }

    // [Parry 이동]
    if (m_pAnimatorTransCom->Get_CurStateTag() == L"Parry")
    {
        _float3 playerPos = vPlayerPos;

        if (m_pAnimatorCom->Get_CurStackedFrame() >= 10)
        {
            playerPos += m_vCursorDir * fTimeDelta * 20.f;        // 커서 방향으로 이동
            m_pTransformCom->Set_State(STATE::POSITION, playerPos);
        }
        // if ( 공격 막는 데에 성공하면)
        // {
        //  m_pAnimatorTransCom->Set_State("Fury_Ready");
        // }
    }

    // [Fury 이동]
    if (m_pAnimatorTransCom->Get_CurStateTag() == L"Fury_Ready")
    {
        _float3 playerPos = vPlayerPos;
        playerPos += m_vCursorDir * fTimeDelta * 30.f;        // 커서 방향으로 이동
        m_pTransformCom->Set_State(STATE::POSITION, playerPos);
    }

    // 이미지 넣고, 이동 및 상태 전이조건 확인하고, 애니메이션도 만들어 넣어줘야 함


    // 상태가 바뀌었다면, 공격 타이머 초기화
    if (strCurStateTag == L"Attack_Upper" ||
        strCurStateTag == L"Attack_Lower" )
        m_fStackedTime = 0.f;

    // 이동 관련 상태가 Idle 일 때만 커서 위치 갱신
    if (m_pAnimatorTransCom->Get_CurStateTag() == L"Idle")
    {
        _float3 playerPos = vPlayerPos;
        m_vCursorDir = _float3{ vRayPoint.x, playerPos.y, vRayPoint.z } - playerPos;
        D3DXVec3Normalize(&m_vCursorDir, &m_vCursorDir);
    }
        


    m_pGameInstance->Compute_TimeDelta(m_strTimerTag);
    m_fStackedTime += m_pGameInstance->Get_TimeDelta(m_strTimerTag);

}



void CPlayer::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_BLEND, this);
}

HRESULT CPlayer::Render()
{
    //SetUp_RenderState();

    _float fPointY = 0.f;       // 교차 평면의 기준이 될 Y값
    _float3 vRayPoint = {};     // fPointY 값 기준 마우스 Ray와 교차하는 좌표
    m_pGameInstance->Get_IntersectAtY(fPointY, vRayPoint);

    _float3 vPlayerPos = {};    // 플레이어 좌표
    vPlayerPos = m_pTransformCom->Get_State(STATE::POSITION);

    // 좌우반전
    if (vRayPoint.x < vPlayerPos.x && !m_isFlippedX)   // 좌측
    {
        m_pVIBufferCom->ChangeUV_FlipX(true);
        m_isFlippedX = true;
        //std::cout << "[CPlayer::Update] FlippedX Changed to True." << std::endl;
    }
    else if (vRayPoint.x > vPlayerPos.x && m_isFlippedX)    // 우측
    {
        m_pVIBufferCom->ChangeUV_FlipX(false);
        m_isFlippedX = false;
        //std::cout << "[CPlayer::Update] FlippedX Changed to False." << std::endl;
    }


    m_pTransformCom->Bind_Matrix();

    m_pAnimatorCom->Update_State(); // Bind_Texture 이 포함되어, 현재 State에 맞는 이미지 출력
    m_pAnimatorTransCom->Update_State(); // 단순 이동관련 상태만 체크할 애니메이터

    m_pVIBufferCom->Bind_Buffers();
    
    m_pVIBufferCom->Render();
    if(m_isFlippedX)
    {
        m_pVIBufferCom->ResetUV_FlipX();
        m_isFlippedX = false;
    }

    //Reset_RenderState();
	return S_OK;
}

void CPlayer::OnCollision(CGameObject* pGameObject)
{
    switch (pGameObject->Get_ObjType())
    {
    case GAMEOBJ_TYPE::MONSTER:
        {
            //pGameObject->Set_IsDead(true);
            break;
        }
        
    }
}

void CPlayer::ChangeStat(STAT_INFO eStat, float fValue)
{
    //STATCHANGE tEvent{};
    //tEvent.eStatType = eStat;
    //tEvent.fValue = fValue;

    //m_pGameInstance->Broadcast(ENUM_CLASS(EVENT_TYPE::PLAYERTSTATCHANGE), &tEvent);
}

void CPlayer::OnEvent(_uint iTypeindex, const EVENTDATA* pData)
{
    //if (static_cast<EVENT_TYPE>(iTypeindex) == EVENT_TYPE::UICHANGE) {
    //    auto pStat = static_cast<const STATCHANGE*>(pData);
    //    if (pStat->eStatType == STAT_INFO::CULDAMAGE)
    //    {
    //        //데미지 처리

    //    }
    //}
}


HRESULT CPlayer::Ready_Components()
{
    /* For Com_VIBuffer */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;


    /* For.Com_Texture */
    // Roll
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Player_Roll"),
        TEXT("Com_Texture_Roll"), reinterpret_cast<CComponent**>(&m_pTextureCom_Roll))))
        return E_FAIL;
    // Air
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Player_Air"),
        TEXT("Com_Texture_Air"), reinterpret_cast<CComponent**>(&m_pTextureCom_Air))))
        return E_FAIL;
    // Down
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Player_Down"),
        TEXT("Com_Texture_Down"), reinterpret_cast<CComponent**>(&m_pTextureCom_Down))))
        return E_FAIL;
    // Idle (Lower, Upper)
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Player_Idle_Lower"),
        TEXT("Com_Texture_Idle_Lower"), reinterpret_cast<CComponent**>(&m_pTextureCom_Idle_Lower))))
        return E_FAIL;
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Player_Idle_Upper"),
        TEXT("Com_Texture_Idle_Upper"), reinterpret_cast<CComponent**>(&m_pTextureCom_Idle_Upper))))
        return E_FAIL;
    // Move (Lower, Upper)
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Player_Move_Lower"),
        TEXT("Com_Texture_Move_Lower"), reinterpret_cast<CComponent**>(&m_pTextureCom_Move_Lower))))
        return E_FAIL;
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Player_Move_Upper"),
        TEXT("Com_Texture_Move_Upper"), reinterpret_cast<CComponent**>(&m_pTextureCom_Move_Upper))))
        return E_FAIL;
    // Attack (Lower, Upper)
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Player_Attack_Lower"),
        TEXT("Com_Texture_Attack_Lower"), reinterpret_cast<CComponent**>(&m_pTextureCom_Attack_Lower))))
        return E_FAIL;
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Player_Attack_Upper"),
        TEXT("Com_Texture_Attack_Upper"), reinterpret_cast<CComponent**>(&m_pTextureCom_Attack_Upper))))
        return E_FAIL;
    // GreatSwordHeavyAttack (Lower, Upper)
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Player_GreatSwordHeavyAttack_Lower"),
        TEXT("Com_Texture_GreatSwordHeavyAttack_Lower"), reinterpret_cast<CComponent**>(&m_pTextureCom_GreatSwordHeavyAttack_Lower))))
        return E_FAIL;
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Player_GreatSwordHeavyAttack_Upper"),
        TEXT("Com_Texture_GreatSwordHeavyAttack_Upper"), reinterpret_cast<CComponent**>(&m_pTextureCom_GreatSwordHeavyAttack_Upper))))
        return E_FAIL;
    // WhirlWind_Ready
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Player_WhirlWind_Ready"),
        TEXT("Com_Texture_WhirlWind_Ready"), reinterpret_cast<CComponent**>(&m_pTextureCom_WhirlWind_Ready))))
        return E_FAIL;
    // WhirlWind_Cycle
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Player_WhirlWind_Cycle"),
        TEXT("Com_Texture_WhirlWind_Cycle"), reinterpret_cast<CComponent**>(&m_pTextureCom_WhirlWind_Cycle))))
        return E_FAIL;



    /* For Com_Transform */
    CTransform::TRANSFORM_DESC TransformDesc{};
    TransformDesc.fSpeedPerSec = 5.f;
    TransformDesc.fRotationPerSec = D3DXToRadian(90.f);

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
        return E_FAIL;

    _float3 vPlayerPos = m_pTransformCom->Get_State(STATE::POSITION);
    m_pTransformCom->Set_State(STATE::POSITION, vPlayerPos + _float3{0, 0.5, 0});
    m_pTransformCom->Scaling(float(18) / 19, 1, 1);


    /* For Com_PlayerStats */
    CPlayerStats::PLAYERSTAT_DESC PlayerStatDesc{};

    // 만약 기존 플레이 저장 정보가 존재한다면
    // 해당 정보를 불러오도록 나중에 수정
    PlayerStatDesc.strName  = L"테스트";
    PlayerStatDesc.iLevel   = 1;
    PlayerStatDesc.iExp     = 0;
    PlayerStatDesc.fHp      = 50.f;
    PlayerStatDesc.fMp      = 50.f;
    PlayerStatDesc.fDash    = 2.f;
    PlayerStatDesc.fAtkSpeed    = 1.0f;
    PlayerStatDesc.fMoveSpeed   = 1.0f;
    PlayerStatDesc.fHpRegen     = 0.f;
    PlayerStatDesc.fMpRegen     = 0.f;
    PlayerStatDesc.fDashRegen   = 0.f;
    PlayerStatDesc.fEvade   = 0.f;
    PlayerStatDesc.fDef     = 0.f;
    PlayerStatDesc.fExpMultiply     = 1.0f;
    PlayerStatDesc.fGoldMultiply    = 1.0f;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_PlayerStats"),
        TEXT("Com_PlayerStats"), reinterpret_cast<CComponent**>(&m_pPlayerStatsCom), &PlayerStatDesc)))
        return E_FAIL;



    /* For Com_Animator */
    CAnimator::ANIMSTATE_DESC StartAnimStateDesc{};
    StartAnimStateDesc.strTimerTag = L"Animator_Player_Main";   // 해당 애니메이터가 타이머에서 사용할 태그 key값
    StartAnimStateDesc.pParentTransform = m_pTransformCom;
    StartAnimStateDesc.pChildTransform = static_cast<CTransform*>(m_pGameInstance->Get_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), L"Layer_Weapon", L"Com_Transform"));

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Animator"),
        TEXT("Com_Animator"), reinterpret_cast<CComponent**>(&m_pAnimatorCom), &StartAnimStateDesc)))
        return E_FAIL;

    // 요소 삽입 : m_pAnimatorCom->Add_State(L"태그명", { m_pTextureCom_상태명, 프레임단위 이미지전환간격, 도중 나갈수있는지});
    // 애니메이션 찾기 :  m_pGameInstance->Find_Animation(L"태그명");
    // 애니메이션은 Level_GamePlay.cpp 에서 Create 함.
    
    m_pAnimatorCom->Add_State(L"Roll",              { m_pTextureCom_Roll, 3, true });
    m_pAnimatorCom->Add_State(L"Air",               { m_pTextureCom_Air, 4, true });
    m_pAnimatorCom->Add_State(L"Down",              { m_pTextureCom_Down, 4, true });
    m_pAnimatorCom->Add_State(L"Idle_Lower",        { m_pTextureCom_Idle_Lower, 4, true, m_pGameInstance->Find_Animation(L"Player_Idle") });
    m_pAnimatorCom->Add_State(L"Idle_Upper",        { m_pTextureCom_Idle_Upper, 4, true, m_pGameInstance->Find_Animation(L"Player_Idle") });
    m_pAnimatorCom->Add_State(L"Move_Lower",        { m_pTextureCom_Move_Lower, 4, true });
    m_pAnimatorCom->Add_State(L"Move_Upper",        { m_pTextureCom_Move_Upper, 4, true });
    m_pAnimatorCom->Add_State(L"Attack_Lower",      { m_pTextureCom_Attack_Lower, 3, false, m_pGameInstance->Find_Animation(L"Player_Attack") });
    m_pAnimatorCom->Add_State(L"Attack_Upper",      { m_pTextureCom_Attack_Upper, 3, false, m_pGameInstance->Find_Animation(L"Player_Attack") });
    m_pAnimatorCom->Add_State(L"Attack_Lower2",     { m_pTextureCom_Attack_Lower, 3, false, m_pGameInstance->Find_Animation(L"Player_Attack2") });
    m_pAnimatorCom->Add_State(L"Attack_Upper2",     { m_pTextureCom_Attack_Upper, 3, false, m_pGameInstance->Find_Animation(L"Player_Attack2") });
    m_pAnimatorCom->Add_State(L"GreatSwordHeavyAttack_Lower", { m_pTextureCom_GreatSwordHeavyAttack_Lower, 4, false});
    m_pAnimatorCom->Add_State(L"GreatSwordHeavyAttack_Upper", { m_pTextureCom_GreatSwordHeavyAttack_Upper, 4, false});
    m_pAnimatorCom->Add_State(L"WhirlWind_Ready",   { m_pTextureCom_WhirlWind_Ready, 4, false });
    m_pAnimatorCom->Add_State(L"WhirlWind_Cycle",   { m_pTextureCom_WhirlWind_Cycle, 4, false });
    m_pAnimatorCom->Add_State(L"Parry",             { m_pTextureCom_Roll, 3, false, m_pGameInstance->Find_Animation(L"Player_Parry") });
    m_pAnimatorCom->Add_State(L"Fury",              { m_pTextureCom_Roll, 3, false/* 맞는 애니메이션 제작하여 삽입 */ });

    /* For Com_Animator (Dash) */
    CAnimator::ANIMSTATE_DESC StartAnimStateDesc2{};
    StartAnimStateDesc2.strTimerTag = L"Animator_Player_Dash";   // 해당 애니메이터가 타이머에서 사용할 태그 key값
    StartAnimStateDesc2.pParentTransform = nullptr; // m_pTransformCom;
    StartAnimStateDesc2.pChildTransform = m_pTransformCom;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Animator"),
        TEXT("Com_Animator_Dash"), reinterpret_cast<CComponent**>(&m_pAnimatorTransCom), &StartAnimStateDesc2)))
        return E_FAIL;
    
    m_pAnimatorTransCom->Add_State(L"Idle",         { nullptr, 1, true });
    m_pAnimatorTransCom->Add_State(L"Dash",         { nullptr, 10, false });
    m_pAnimatorTransCom->Add_State(L"Attack",       { nullptr, 4, false });
    m_pAnimatorTransCom->Add_State(L"Parry",        { nullptr, 18, false });
    m_pAnimatorTransCom->Add_State(L"Fury",         { nullptr, 6, false });
    
    // collider
    CCollider_OBB::OBB_DESC tColliderDesc;
    tColliderDesc.vScale = _float3(1.f, 0.001f, 1.f);
    tColliderDesc.pOwner = this;
    tColliderDesc.pTransform = m_pTransformCom;
    CCollider_OBB* pCol = dynamic_cast<CCollider_OBB*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::COMPONENT, ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Collider_OBB"), &tColliderDesc));
    m_pGameInstance->Add_Collider(pCol);
    return S_OK;
}

void CPlayer::SetUp_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
}

void CPlayer::Reset_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

    m_pGraphic_Device->SetTexture(0, NULL);
}



CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CPlayer* pInstance = new CPlayer(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Create : CPlayer"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CPlayer::Clone(void* pArg)
{
    CPlayer* pInstance = new CPlayer(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Clone : CPlayer"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CPlayer::Free()
{
    m_pGameInstance->Unsubscribe(ENUM_CLASS(EVENT_TYPE::UICHANGE), this);
    __super::Free();

    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTransformCom);

    Safe_Release(m_pTextureCom_Roll);
    Safe_Release(m_pTextureCom_Air);
    Safe_Release(m_pTextureCom_Down);
    Safe_Release(m_pTextureCom_Idle_Lower);
    Safe_Release(m_pTextureCom_Idle_Upper);
    Safe_Release(m_pTextureCom_Move_Lower);
    Safe_Release(m_pTextureCom_Move_Upper);
    Safe_Release(m_pTextureCom_Attack_Lower);
    Safe_Release(m_pTextureCom_Attack_Upper);
    Safe_Release(m_pTextureCom_GreatSwordHeavyAttack_Lower);
    Safe_Release(m_pTextureCom_GreatSwordHeavyAttack_Upper);
    Safe_Release(m_pTextureCom_WhirlWind_Ready);
    Safe_Release(m_pTextureCom_WhirlWind_Cycle);

    Safe_Release(m_pPlayerStatsCom);
    Safe_Release(m_pAnimatorCom);
    Safe_Release(m_pAnimatorTransCom);
    Safe_Release(m_pTerrainBox);
    
    /*if (m_pCollider)
    {
        m_pCollider->Set_Owner(nullptr);
        Safe_Release(m_pCollider);
    }*/
    

    CEffect_Factory::GetInstance()->Free();
    
}

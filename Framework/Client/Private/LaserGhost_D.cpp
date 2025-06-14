#include "LaserGhost_D.h"
#include "Effect_Factory.h"

CLaserGhost_D::CLaserGhost_D(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CMonster{ pGraphic_Device }
{

}

CLaserGhost_D::CLaserGhost_D(const CLaserGhost_D& Prototype)
    : CMonster{ Prototype }
{

}

HRESULT CLaserGhost_D::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CLaserGhost_D::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(this->Ready_Components(pArg)))
        return E_FAIL;

    Ready_Object();

    m_isRunOut = false;
    //m_iAtkCooldownFrames = static_cast<_int>(m_pGameInstance->Compute_Random(0, 300));


    m_iMaxHp = 120;
    m_iCulHp = 120;
    m_eMonsterType = MONSTER_TYPE::LASERGHOST;
    m_pTransformCom->Scaling(1.5f, 1.5f, 1.5f);
    return S_OK;
}

void CLaserGhost_D::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
    if (m_pHpBar != nullptr &&
        m_isSummoned)
        m_pHpBar->Render_HP_Progress(m_pTransformCom, m_iCulHp, m_iMaxHp);
}

void CLaserGhost_D::Update(_float fTimeDelta)
{
    if (!m_pTerrainBox || !m_pTransformCom)
        return;



    // 처음 타겟 잡은 곳 (플레이어가 위치하던 곳) 을 기준좌표로 조준하다가
    // 이후 초당 20~30도 정도 속도로 반시계방향 회전하며 레이저 발사
    //
    // 평시 Idle
    // 너무 멀어지면 추적하며 Move
    // 공격 시 플레이어가 위치했던 좌표 조준하며 AttackReady
    // 이후 Attack_Start - Attack_Cycle (레이저, 약 3.5초간) - Attack_End
    // 이후 Attack_Standby 로 Exit, 후 다시 사이클 실행
    // 너무 가까우면 좀 떨어지도록 패턴 구성?



    _float fMinDist = 2.f;      // 원거리 공격 할 기준 거리
    _float fFollowDist = 8.f;   // 멀어서 추적해올 기준 거리
    _float fMaxDist = 15.f;     // 어그로가 풀리는 기준 거리

    _float fLaserLifeTime = 2.f;    // 레이저 지속 시간
    _float fLaserTurnAngle = 20.f;  // 레이저 회전 속도 (초당, degree 단위)

    //m_iAtkCooldownFrames++;

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
    D3DXMatrixScaling(&matScale, -1.f, 1.f, 1.f);

    // 3. 자전
    _float4x4 matRotateChild = {};
    D3DXMatrixIdentity(&matRotateChild);
    //D3DXMatrixRotationX(&matRotateChild, D3DXToRadian(-90)); // 안되면 -90도도 해보기

    _float4x4 matRotateChildtoPlayer = {};
    D3DXMatrixIdentity(&matRotateChildtoPlayer);
    _float fAngle = atan2f(vTargetPos.x - vMonsterPos.x, vTargetPos.z - vMonsterPos.z);
    _float fDegree = D3DXToDegree(fAngle) + 190;
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
    // ***********************


    // 최초 소환
#pragma region Summon States

    if (m_pAnimatorCom->Get_CurStateTag() == L"Summon_Standby")
    {
        if (m_pAnimatorCom->Change_State(L"Summon_Ready"))
        {
#pragma region Summon Effect Matrix Setting
            D3DXMatrixScaling(&matScale, -2.4f, 1.5f, 1.5f);
            D3DXMatrixRotationX(&matRotateChild, D3DXToRadian(-90));
            D3DXMatrixTranslation(&matTransOffset, 0, (m_pTransformCom->Get_Scaled().y / -2.6f), 0);

            matMonsterWorld = matTransToOrigin * matScale * matRotateChild * matRotateChildtoPlayer * matTransReturn * matTransOffset * matTransAddition;
#pragma endregion

            CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::NORMAL_EFFECT, L"Prototype_Component_Texture_LaserGhost_D_Summon",
                *m_pTransformCom->Get_WorldMatrix(), matMonsterWorld);

#pragma region Reset Effect Matrix Setting
            D3DXMatrixIdentity(&matScale);
            D3DXMatrixIdentity(&matRotateChild);
            D3DXMatrixIdentity(&matTransOffset);

            matMonsterWorld = matTransToOrigin * matScale * matRotateChild * matRotateChildtoPlayer * matTransReturn * matTransOffset * matTransAddition;
#pragma endregion

        }
    }
    else if (m_pAnimatorCom->Get_CurStateTag() == L"Summon_Ready")
    {
        if (m_pAnimatorCom->Change_State(L"Summon"))
            m_isSummoned = true;
    }
    else if (m_pAnimatorCom->Get_CurStateTag() == L"Summon")
    {
        m_pAnimatorCom->Change_State(L"Idle");
    }

#pragma endregion


    // 행동 분기
#pragma region Other States

    if (!(m_pAnimatorCom->Get_CurStateTag() == L"AttackReady" ||
        m_pAnimatorCom->Get_CurStateTag() == L"Attack_Start" ||
        m_pAnimatorCom->Get_CurStateTag() == L"Attack_Cycle" ||
        m_pAnimatorCom->Get_CurStateTag() == L"Attack_End" ||
        m_pAnimatorCom->Get_CurStateTag() == L"Attack_Standby"  ))
    {
        // 거리가 아주 가깝다면 반대로 도망
        if (fDistance <= fMinDist)
        {
            if (m_pAnimatorCom->Change_State(L"Move"))
            {
                m_isTracking = true;
                m_isRunOut = true;
            }
        }
        // 거리가 충분히 떨어져있다면 원거리 공격 준비
        else if (fMinDist <= fDistance &&
            fDistance <= fFollowDist)
        {
            if (m_pAnimatorCom->Change_State(L"AttackReady"))
            {
                m_pGameInstance->StopSound(ENUM_CLASS(CHANNELID::SOUND_MONSTER_LONG_EFFECT));
                m_pGameInstance->PlaySoundW(L"GolemLaser.wav", ENUM_CLASS(CHANNELID::SOUND_MONSTER_LONG_EFFECT), g_fEFFECTVolume - 0.9f);
                CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::NORMAL_EFFECT, L"Prototype_Component_Texture_LaserGhost_D_Effect_AttackReady",
                    *m_pTransformCom->Get_WorldMatrix(), matMonsterWorld);
                m_vLockedOnPos = vTargetPos;
                m_isTracking = false;
            }
        }
        // 거리가 적당히 떨어져있다면 추적 ON
        else if (fFollowDist <= fDistance &&
            fDistance <= fMaxDist)
        {
            if (m_pAnimatorCom->Change_State(L"Move"))
            {
                m_isTracking = true;
                m_isRunOut = false;
            }
        }
        // 충분히 멀리 떨어지면 Idle 상태
        else if (fMaxDist <= fDistance)
        {
            if (m_pAnimatorCom->Change_State(L"Idle"))
                m_isTracking = false;
        }
    }



    // 각 분기점마다의 행동
    if (m_isTracking &&
        m_pAnimatorCom->Get_CurStateTag() == L"Move")
    {
        float fMoveSpeed = 0.6f;

        // 플레이어 추적
        _float3 vPosDiff = (m_isRunOut)? - vTargetPos + vMonsterPos : vTargetPos - vMonsterPos;
        D3DXVec3Normalize(&vPosDiff, &vPosDiff);
        _float3 vNewMonsterPos = vMonsterPos + vPosDiff * fTimeDelta * fMoveSpeed;
        m_pTransformCom->Set_State(STATE::POSITION, vNewMonsterPos);
    }

    // 공격 패턴 분기
    if (m_pAnimatorCom->Get_CurStateTag() == L"AttackReady")
    {
        if (m_pAnimatorCom->Change_State(L"Attack_Start"))
        {
            CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::NORMAL_EFFECT, L"Prototype_Component_Texture_LaserGhost_D_Effect_Attack_Start",
                *m_pTransformCom->Get_WorldMatrix(), matMonsterWorld);
        }
    }
    else if (m_pAnimatorCom->Get_CurStateTag() == L"Attack_Start")
    {
        if (m_pAnimatorCom->Change_State(L"Attack_Cycle", true, fLaserLifeTime))
        {
            CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::NORMAL_EFFECT, L"Prototype_Component_Texture_LaserGhost_D_Effect_Attack_Cycle",
                *m_pTransformCom->Get_WorldMatrix(), matMonsterWorld);

            // 레이저용 세팅
#pragma region Laser Setting Change
            D3DXMatrixScaling(&matScale, -1.f, 10.f, 10.f);

            matRotateChild = {};
            D3DXMatrixIdentity(&matRotateChild);
            D3DXMatrixRotationX(&matRotateChild, D3DXToRadian(-90));

            matRotateChildtoPlayer = {};
            D3DXMatrixIdentity(&matRotateChildtoPlayer);
            fAngle = atan2f(m_vLockedOnPos.x - vMonsterPos.x, m_vLockedOnPos.z - vMonsterPos.z);
            fDegree = D3DXToDegree(fAngle) + 180;
            D3DXMatrixRotationY(&matRotateChildtoPlayer, D3DXToRadian(fDegree));  // 플레이어 방향을 보게 하고 싶다면 이쪽 주석을 해제

            D3DXMatrixIdentity(&matTransOffset);

            _float4x4 matTransAddition = {};
            D3DXMatrixIdentity(&matTransAddition);
            //_float3 vDiff = -vPlayerPos + vMonsterPos;       // 플레이어 위치에서 마우스 교차좌표로 가는 벡터 
            vDiff = -vMonsterPos + m_vLockedOnPos;
            D3DXVec3Normalize(&vDiff, &vDiff);              // 를 단위벡터화, 안되면 vDiff 순서 바꿔보기
            _float fDistanceOffset = 7.5f;                   // ** ksta : 중점으로부터 떨어져 있을 거리 **
            vDiff *= fDistanceOffset;
            D3DXMatrixTranslation(&matTransAddition, vDiff.x, 0, vDiff.z);

            matMonsterWorld = matTransToOrigin * matScale * matRotateChild * matRotateChildtoPlayer * matTransReturn * matTransOffset * matTransAddition;
#pragma endregion

            // m_vLockedOnPos 와 m_pTransformCom->Get_State(STATE::POSITION), pTargetTransform->Get_State(STATE::POSITION) 의 비교
            // 이후 플레이어와 가장 가까운 방향으로 회전

            _float3 vCross, vNormal = vMonsterPos;
            D3DXVec3Cross(&vCross, &m_vLockedOnPos, &vTargetPos);
            m_fTurnDir = D3DXVec3Dot(&vCross, &vNormal);
            m_fTurnDir = (m_fTurnDir > 0.f)? 1.f : -1.f;


            // 공격 이펙트 출력
            CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::MONSTER_EFFECT, L"Prototype_Component_Texture_LaserGhost_D_Effect_Laser_Progress",
                *m_pTransformCom->Get_WorldMatrix(), matMonsterWorld, { 0, 0, 0 }, 0.f, fLaserLifeTime, D3DXToRadian(fLaserTurnAngle * m_fTurnDir));
            // 세팅 리셋
#pragma region Laser Setting Reset
            D3DXMatrixScaling(&matScale, -1.f, 1.f, 1.f);
            D3DXMatrixIdentity(&matRotateChild);
            fAngle = atan2f(vTargetPos.x - vMonsterPos.x, vTargetPos.z - vMonsterPos.z);
            fDegree = D3DXToDegree(fAngle) + 190;
            D3DXMatrixIdentity(&matRotateChildtoPlayer);

            D3DXMatrixIdentity(&matTransOffset);
            D3DXMatrixTranslation(&matTransOffset, 0, -0.12f, 0);

            matTransAddition = {};
            vDiff = -vMonsterPos + vTargetPos;
            D3DXVec3Normalize(&vDiff, &vDiff);              // 를 단위벡터화, 안되면 vDiff 순서 바꿔보기
            fDistanceOffset = 1.0f;                   // ** ksta : 중점으로부터 떨어져 있을 거리 **
            vDiff *= fDistanceOffset;
            D3DXMatrixIdentity(&matTransAddition);

            matMonsterWorld = matTransToOrigin * matScale * matRotateChild * matRotateChildtoPlayer * matTransReturn * matTransOffset * matTransAddition;
#pragma endregion
        }
    }
    else if (m_pAnimatorCom->Get_CurStateTag() == L"Attack_Cycle")
    {

        if (m_pAnimatorCom->Change_State(L"Attack_End"))
        {
            CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::NORMAL_EFFECT, L"Prototype_Component_Texture_LaserGhost_D_Effect_Attack_End",
                *m_pTransformCom->Get_WorldMatrix(), matMonsterWorld, true);
            // 레이저용 세팅
#pragma region Laser Setting Change
            D3DXMatrixScaling(&matScale, -1.f, 10.f, 10.f);

            matRotateChild = {};
            D3DXMatrixIdentity(&matRotateChild);
            D3DXMatrixRotationX(&matRotateChild, D3DXToRadian(-90));

            matRotateChildtoPlayer = {};
            D3DXMatrixIdentity(&matRotateChildtoPlayer);
            fAngle = atan2f(m_vLockedOnPos.x - vMonsterPos.x, m_vLockedOnPos.z - vMonsterPos.z);
            fDegree = D3DXToDegree(fAngle) + 180;
            D3DXMatrixRotationY(&matRotateChildtoPlayer, D3DXToRadian(fDegree));  // 플레이어 방향을 보게 하고 싶다면 이쪽 주석을 해제

            D3DXMatrixIdentity(&matTransOffset);

            _float4x4 matTransAddition = {};
            D3DXMatrixIdentity(&matTransAddition);
            //_float3 vDiff = -vPlayerPos + vMonsterPos;       // 플레이어 위치에서 마우스 교차좌표로 가는 벡터 
            vDiff = -vMonsterPos + m_vLockedOnPos;
            D3DXVec3Normalize(&vDiff, &vDiff);              // 를 단위벡터화, 안되면 vDiff 순서 바꿔보기
            _float fDistanceOffset = 7.5f;                   // ** ksta : 중점으로부터 떨어져 있을 거리 **
            vDiff *= fDistanceOffset;
            D3DXMatrixTranslation(&matTransAddition, vDiff.x, 0, vDiff.z);

#pragma region Laser End Angle Calc
            _float3 vTurnAxis = { 0, 1, 0 };
            // 기준점 설정
            _float4x4 matTrackTarget = *m_pTransformCom->Get_WorldMatrix();
            // 원점으로 중심축 이동
            _float4x4 matToOrigin;
            D3DXMatrixTranslation(&matToOrigin,
                -matTrackTarget._41,
                -matTrackTarget._42,
                -matTrackTarget._43);
            // 축 기준 회전행렬
            _float4x4 matRot;
            D3DXMatrixRotationAxis(&matRot, &vTurnAxis, D3DXToRadian(fLaserTurnAngle * fLaserLifeTime * m_fTurnDir));
            // 다시 제자리로
            _float4x4 matFromOrigin;
            D3DXMatrixTranslation(&matFromOrigin,
                matTrackTarget._41,
                matTrackTarget._42,
                matTrackTarget._43);
            // 다 합치기
            _float4x4 matRotationTotal = matToOrigin * matRot * matFromOrigin;
#pragma endregion


            matMonsterWorld = matTransToOrigin * matScale * matRotateChild * matRotateChildtoPlayer * matTransReturn * matTransOffset * matTransAddition * matRotationTotal;
#pragma endregion
            // 공격 끝 이펙트 출력
            CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::MONSTER_EFFECT, L"Prototype_Component_Texture_LaserGhost_D_Effect_Laser_End",
                *m_pTransformCom->Get_WorldMatrix(), matMonsterWorld);
            // 세팅 리셋
#pragma region Laser Setting Reset
            D3DXMatrixScaling(&matScale, -1.f, 1.f, 1.f);
            D3DXMatrixIdentity(&matRotateChild);
            fAngle = atan2f(vTargetPos.x - vMonsterPos.x, vTargetPos.z - vMonsterPos.z);
            fDegree = D3DXToDegree(fAngle) + 190;
            D3DXMatrixIdentity(&matRotateChildtoPlayer);

            D3DXMatrixIdentity(&matTransOffset);
            D3DXMatrixTranslation(&matTransOffset, 0, -0.12f, 0);

            matTransAddition = {};
            vDiff = -vMonsterPos + vTargetPos;
            D3DXVec3Normalize(&vDiff, &vDiff);              // 를 단위벡터화, 안되면 vDiff 순서 바꿔보기
            fDistanceOffset = 1.0f;                   // ** ksta : 중점으로부터 떨어져 있을 거리 **
            vDiff *= fDistanceOffset;
            D3DXMatrixIdentity(&matTransAddition);

            matMonsterWorld = matTransToOrigin * matScale * matRotateChild * matRotateChildtoPlayer * matTransReturn * matTransOffset * matTransAddition;
#pragma endregion
        }
    }
    else if (m_pAnimatorCom->Get_CurStateTag() == L"Attack_End")
        m_pAnimatorCom->Change_State(L"Attack_Standby");
        


    if (m_pAnimatorCom->Get_CurStateTag() == L"Airborne")
        m_pAnimatorCom->Change_State(L"Attack_Standby");

    // 딜레이 Exit 역할
    if (m_pAnimatorCom->Get_CurStateTag() == L"Attack_Standby")
        m_pAnimatorCom->Change_State(L"Idle");

#pragma endregion



    if (m_pTerrainBox != nullptr) {
        m_pTerrainBox->SetUp_OnTerrainBox(m_pTransformCom, _float3(0.05f, 0.4f, 0.05f));
    }
}

void CLaserGhost_D::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
}

HRESULT CLaserGhost_D::Render()
{
    if (!m_pTransformCom)
        return S_OK;

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

    if (m_isSummoned)
    {
        m_pVIBufferCom->Render();
    }
    
    m_pTerrainBox->Render();

    if (m_isFlippedX)
    {
        m_pVIBufferCom->ResetUV_FlipX();
        m_isFlippedX = false;
    }

    Reset_RenderState();

    return S_OK;
}

HRESULT CLaserGhost_D::Ready_Components(void* pArg)
{
    MONSTERDESC* desc = static_cast<MONSTERDESC*>(pArg);

    /* For.Com_Texture */
    // Idle
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Texture_LaserGhost_D_Idle"),
        TEXT("Com_Texture_Idle"), reinterpret_cast<CComponent**>(&m_pTextureCom_Idle))))
        return E_FAIL;
    // Move
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Texture_LaserGhost_D_Move"),
        TEXT("Com_Texture_Move"), reinterpret_cast<CComponent**>(&m_pTextureCom_Move))))
        return E_FAIL;
    // Down
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Texture_LaserGhost_D_Down"),
        TEXT("Com_Texture_Down"), reinterpret_cast<CComponent**>(&m_pTextureCom_Down))))
        return E_FAIL;
    // AttackReady
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Texture_LaserGhost_D_AttackReady"),
        TEXT("Com_Texture_AttackReady"), reinterpret_cast<CComponent**>(&m_pTextureCom_AttackReady))))
        return E_FAIL;
    // Attack_Start
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Texture_LaserGhost_D_Attack_Start"),
        TEXT("Com_Texture_Attack_Start"), reinterpret_cast<CComponent**>(&m_pTextureCom_Attack_Start))))
        return E_FAIL;
    // Attack_Cycle
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Texture_LaserGhost_D_Attack_Cycle"),
        TEXT("Com_Texture_Attack_Cycle"), reinterpret_cast<CComponent**>(&m_pTextureCom_Attack_Cycle))))
        return E_FAIL;
    // Attack_End
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Texture_LaserGhost_D_Attack_End"),
        TEXT("Com_Texture_Attack_End"), reinterpret_cast<CComponent**>(&m_pTextureCom_Attack_End))))
        return E_FAIL;
    // Airborne
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Texture_LaserGhost_D_Airborne"),
        TEXT("Com_Texture_Airborne"), reinterpret_cast<CComponent**>(&m_pTextureCom_Airborne))))
        return E_FAIL;

    /* For Com_Animator */
    CAnimator::ANIMSTATE_DESC StartAnimStateDesc{};
    StartAnimStateDesc.strTimerTag = L"Animator_Monster_LaserGhost_D_Main";   // 해당 애니메이터가 타이머에서 사용할 태그 key값
    StartAnimStateDesc.pParentTransform = nullptr;
    StartAnimStateDesc.pChildTransform = nullptr;
    
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Animator"),
        TEXT("Com_Animator"), reinterpret_cast<CComponent**>(&m_pAnimatorCom), &StartAnimStateDesc)))
        return E_FAIL;
    
    // State 삽입
    m_pAnimatorCom->Add_State(L"Summon_Standby",{ nullptr, 90 /* 나중에 랜덤값 삽입 */, false});  // 소환 딜레이용
    m_pAnimatorCom->Add_State(L"Summon_Ready",  { nullptr, 30, false});  // 소환 시작 모션
    m_pAnimatorCom->Add_State(L"Summon",        { m_pTextureCom_Idle, 4, false});   // 소환

    m_pAnimatorCom->Add_State(L"Idle",          { m_pTextureCom_Idle, 4, true });           // 14
    m_pAnimatorCom->Add_State(L"Move",          { m_pTextureCom_Move, 4, true });           // 14
    m_pAnimatorCom->Add_State(L"Down",          { m_pTextureCom_Down, 4, true });           // 3
    m_pAnimatorCom->Add_State(L"AttackReady",   { m_pTextureCom_AttackReady, 7, false });   // 28
    m_pAnimatorCom->Add_State(L"Attack_Start",  { m_pTextureCom_Attack_Start, 4, false });  // 1
    m_pAnimatorCom->Add_State(L"Attack_Cycle",  { m_pTextureCom_Attack_Cycle, 5, false });  // 7
    m_pAnimatorCom->Add_State(L"Attack_End",    { m_pTextureCom_Attack_End, 4, false });    // 8
    m_pAnimatorCom->Add_State(L"Airborne",      { m_pTextureCom_Airborne, 4, false });      // 3
    m_pAnimatorCom->Add_State(L"Attack_Standby",{ m_pTextureCom_Idle, 4, false });          // 14




    return S_OK;
}

HRESULT CLaserGhost_D::Ready_Object()
{
    m_pHpBar = dynamic_cast<CField_Hp*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_UI_Field_Hp")));

    return S_OK;
}

void CLaserGhost_D::OnCollision(CGameObject* pGameObject)
{
    __super::OnCollision(pGameObject);

    //m_isTracking = true;


    switch (pGameObject->Get_ObjType())
    {
    case GAMEOBJ_TYPE::PLAYER_EFFECT:
        if (!m_bIsHit) {

            if (m_pAnimatorCom->Get_CurStateTag() != TEXT("AttackReady") &&
                m_pAnimatorCom->Get_CurStateTag() != TEXT("Attack_Start" &&
                    m_pAnimatorCom->Get_CurStateTag() != TEXT("Attack_Cycle") &&
                    m_pAnimatorCom->Get_CurStateTag() != TEXT("Attack_End")))
            {
                wstring strStateTag = {};
                _float fPointY = 0.f;       // 교차 평면의 기준이 될 Y값
                _float3 vRayPoint = {};     // fPointY 값 기준 마우스 Ray와 교차하는 좌표
                m_pGameInstance->Get_IntersectAtY(fPointY, vRayPoint);

                _float3 vThisPos = {};    // 플레이어 좌표
                vThisPos = m_pTransformCom->Get_State(STATE::POSITION);

                //strStateTag = (vRayPoint.z > vPlayerPos.z)?     L"Idle_Upper":
                                                                //L"Idle_Lower";

                //m_pAnimatorCom->Change_State(strStateTag, true, 2);
                m_pAnimatorCom->Change_State(L"Airborne", false, 0.5, true); // 도중 Exit 불가 State라 상태변환이 안됨


                CTransform* pEnemyTransform = dynamic_cast<CTransform*>(pGameObject->Find_Component(L"Com_Transform"));
                _float3 vEnemyPos = pEnemyTransform->Get_State(STATE::POSITION);
                _float3 vStunDir = vThisPos - vEnemyPos;
                D3DXVec3Normalize(&vStunDir, &vStunDir);

                _float3 vResult = vThisPos + vStunDir * 0.5f;    // 밀려날 정도 테스트
                m_pTransformCom->Set_State(STATE::POSITION, vResult);

                //m_bIsHit = true;
            }
            
        }
    }
}

CLaserGhost_D* CLaserGhost_D::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CLaserGhost_D* pInstance = new CLaserGhost_D(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Create : CLaserGhost_D"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CLaserGhost_D::Clone(void* pArg)
{
    CLaserGhost_D* pInstance = new CLaserGhost_D(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Clone : CLaserGhost_D"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CLaserGhost_D::Free()
{
    __super::Free();

    Safe_Release(m_pTextureCom_Idle);
    Safe_Release(m_pTextureCom_Move);
    Safe_Release(m_pTextureCom_Down);
    Safe_Release(m_pTextureCom_AttackReady);
    Safe_Release(m_pTextureCom_Attack_Start);
    Safe_Release(m_pTextureCom_Attack_Cycle);
    Safe_Release(m_pTextureCom_Attack_End);
    Safe_Release(m_pTextureCom_Airborne);

    Safe_Release(m_pAnimatorCom);
}
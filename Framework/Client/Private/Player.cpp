#include "Player.h"
#include "TerrainBox.h"
#include "GameInstance.h"
#include "Collider_OBB.h"
#include "Event_Manager.h"
#include "Effect_Factory.h"
#include "Room_Manager.h"
#include "Stat_Manager.h"
#include "ChapMap.h"
#include "EXP_Ball.h"
#include "Field_Font.h"
#include "Level_Loading.h"
#include "Client_Defines_Event.h"
#include "Field_Item.h"
#include "GoldLeaf.h"

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
 	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

    m_eObjType = GAMEOBJ_TYPE::PLAYER;
    m_pTransformCom->Scaling(1.5f, 1.5f, 1.5f);
    m_strTimerTag = L"GameObject_Player_StateDeltaTime";
    
    m_pGameInstance->Add_Timer(m_strTimerTag);      // 마지막으로 상태가 바뀐지 지난 시간을 측정할 타이머
    m_pGameInstance->Compute_TimeDelta(m_strTimerTag);
    m_pGameInstance->Subscribe(ENUM_CLASS(EVENT_TYPE::UICHANGE), this);
    Ready_Object();
    m_dwHitTime = 0.f;
    
    return S_OK;
}

void CPlayer::Priority_Update(_float fTimeDelta)
{
    //필드 HP바 Priority_Update에서 호출 필요
    _int m_iCulHp = CStat_Manager::GetInstance()->Get_CurStats()[ENUM_CLASS(STAT_INFO::CULHP)];
    _int m_iMaxHp = CStat_Manager::GetInstance()->Get_CurStats()[ENUM_CLASS(STAT_INFO::MAXHP)];

    /*if (m_pHpBar != nullptr)
        m_pHpBar->Render_HP_Progress(m_pTransformCom, m_iCulHp, m_iMaxHp);*/

    // isHit  은 무적 관리,
    // IsStun 은 경직 관리
    if (m_bIsHit)
        m_dwHitTime += 1.f;

    if (m_dwHitTime >= 20.f)
    {
        m_bIsStun = false;
    }
    if (m_dwHitTime >= 30.f)
    {
        m_dwHitTime = 0.f;
        m_bIsHit = false;
    }

    m_fGodModeTime += 1.f;


    //if (m_bIsHit)       std::wcout << "TRUE" << std::endl;
    //else                std::wcout << "FALSE" << std::endl; // 왜 TRUE인데도 공격이 잘 되지??
    //
    //std::wcout << m_pAnimatorCom->Get_CurStateTag() << std::endl;
}


void CPlayer::Update(_float fTimeDelta)
{    
    if (m_pGameInstance->IsKeyDown(VK_DOWN))
    {
        m_pChat->On_Chat(0, true);
    }
    if (m_pGameInstance->IsKeyDown(VK_LEFT))
    {
        m_pChat->Cinematic_Chat(0, true);
    }
    if (m_pGameInstance->IsKeyDown(VK_RIGHT))
    {
        m_pChat->Off_Chat();
    }
    //m_pCollider->Update_Collider();
    if (m_pTerrainBox != nullptr) {
        m_pTerrainBox->SetUp_OnTerrainBox(m_pTransformCom, _float3(0.f, 0.3f, 0.f));
    }

    // 경직 중이라면 Update 실행 X (키 조작 불가)
    if (m_bIsStun)
        return;

    // 아래에서 사용할 변수들
#pragma region Variables Setting

    _float fPointY = m_pTransformCom->Get_State(STATE::POSITION).y - m_pTransformCom->Get_Scaled().y / 2.0f;       // 교차 평면의 기준이 될 Y값
    _float3 vRayPoint = {};     // fPointY 값 기준 마우스 Ray와 교차하는 좌표
    m_pGameInstance->Get_IntersectAtY(fPointY, vRayPoint);

    _float3 vPlayerPos = {};    // 플레이어 좌표
    vPlayerPos = m_pTransformCom->Get_State(STATE::POSITION);

    _wstring strCurStateTag = m_pAnimatorCom->Get_CurStateTag();

    wstring strStateTag = {}; // 임시 저장용 현재 상태
    CStat_Manager* pPlayerStat = CStat_Manager::GetInstance();

    _float4x4 matPlayerWorld = *m_pTransformCom->Get_WorldMatrix();
    CStat_Manager* pStats = CStat_Manager::GetInstance();

#pragma endregion

    // **** 이펙트 크기조절용 초기설정
#pragma region Effect Setting

    // 1. 원점으로 이동
    _float4x4 matTransToOrigin = {};
    D3DXMatrixIdentity(&matTransToOrigin);
    D3DXMatrixTranslation(&matTransToOrigin, -matPlayerWorld._41, -matPlayerWorld._42, -matPlayerWorld._43);

    // 2. 크기
    _float4x4 matScale = {};
    D3DXMatrixIdentity(&matScale);
    D3DXMatrixScaling(&matScale, -3.5f, 3.5f, 3.5f);

    // 3. 자전
    _float4x4 matRotateChild = {};
    D3DXMatrixIdentity(&matRotateChild);
    D3DXMatrixRotationX(&matRotateChild, D3DXToRadian(-90)); // 안되면 -90도도 해보기

    _float4x4 matRotateChildtoCursor = {};
    D3DXMatrixIdentity(&matRotateChildtoCursor);
    _float fAngle = atan2f(vRayPoint.x - vPlayerPos.x, vRayPoint.z - vPlayerPos.z);
    _float fDegree = D3DXToDegree(fAngle) + 190;
    D3DXMatrixRotationY(&matRotateChildtoCursor, D3DXToRadian(fDegree)); // ksta2 : 커서각 어케구함

    // 4. 원래 위치(플레이어)로 재이동
    _float4x4 matTransReturn = {};
    D3DXMatrixIdentity(&matTransReturn);
    D3DXMatrixTranslation(&matTransReturn, matPlayerWorld._41, matPlayerWorld._42, matPlayerWorld._43);

    // 5. 거기에 추가 이동
    _float4x4 matTransAddition = {};
    D3DXMatrixIdentity(&matTransAddition);
    _float3 vDiff = -vPlayerPos + vRayPoint;       // 플레이어 위치에서 마우스 교차좌표로 가는 벡터
    D3DXVec3Normalize(&vDiff, &vDiff);              // 를 단위벡터화, 안되면 vDiff 순서 바꿔보기
    _float fDistanceOffset = 1.2f;                   // ** ksta : 중점으로부터 떨어져 있을 거리 **
    _float3 vDiffResult = vDiff * fDistanceOffset;
    D3DXMatrixTranslation(&matTransAddition, vDiffResult.x, 0, vDiffResult.z);

    matPlayerWorld = matTransToOrigin * matScale * matRotateChild * matRotateChildtoCursor * matTransReturn * matTransAddition;

#pragma endregion
    // ***********************


    // ***************************************
    // * [마우스 좌클] 일반 공격
    // ***************************************
#pragma region [Mouse L] Basic Attack

    if (m_pGameInstance->IsKeyDown(VK_LBUTTON))
    {
        if (!CStat_Manager::GetInstance()->Get_UIOpen()) {

            // 이전 상태 공격 + 현재 상태 비공격 + 1번째 공격한 지 0.3f초 이하
            if (!(strCurStateTag == L"Attack_Upper" ||
                strCurStateTag == L"Attack_Lower") &&
                m_fStackedTime <= 0.3f)
            {
                // 2번째 공격으로.
                strStateTag = (vRayPoint.z > vPlayerPos.z) ? L"Attack_Upper2" :
                    L"Attack_Lower2";

                m_pGameInstance->StopSound(ENUM_CLASS(CHANNELID::SOUND_PLAYER_EFFECT));
                m_pGameInstance->PlaySoundW(L"DaggerAttack.wav", ENUM_CLASS(CHANNELID::SOUND_PLAYER_EFFECT), g_fEFFECTVolume - 0.6f);

                // 바꾸는 데에 성공시 2타공격 이펙트 출력
                if (m_pAnimatorCom->Change_State(strStateTag, true))
                    CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::PLAYER_EFFECT, L"Prototype_Component_Texture_Effect_Blade0_Swing1",
                        *m_pTransformCom->Get_WorldMatrix(), matPlayerWorld, m_pTransformCom, 0.f, true);
            }

            // 1번째 공격으로.
            strStateTag = (vRayPoint.z > vPlayerPos.z) ? L"Attack_Upper" :
                L"Attack_Lower";
            m_pAnimatorTransCom->Change_State(L"Attack");

            // 바꾸는 데에 성공시 1타공격 이펙트 출력
            if (m_pAnimatorCom->Change_State(strStateTag, true))
            {
                m_pGameInstance->StopSound(ENUM_CLASS(CHANNELID::SOUND_PLAYER_EFFECT));
                m_pGameInstance->PlaySoundW(L"DaggerAttack.wav", ENUM_CLASS(CHANNELID::SOUND_PLAYER_EFFECT), g_fEFFECTVolume - 0.6f);
                CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::PLAYER_EFFECT, L"Prototype_Component_Texture_Effect_Blade0_Swing0",
                    *m_pTransformCom->Get_WorldMatrix(), matPlayerWorld, m_pTransformCom, 0.f);
            }
                

            //std::cout << "[Player::Update] PlayerPos : " << vPlayerPos.x << ", " << vPlayerPos.y << ", " << vPlayerPos.z << std::endl;
        }

    }

#pragma endregion

#pragma region Transform Change
    // [일반 공격 이동]
    if (m_pAnimatorTransCom->Get_CurStateTag() == L"Attack")
    {
        _float3 playerPos = vPlayerPos;
        playerPos += m_vCursorDir * fTimeDelta * 10.f * (-0.5f * cosf(0.69f * m_pAnimatorCom->Get_CurStackedFrame()) + 1.f);        // 커서 방향으로 이동
        m_pTransformCom->Set_State(STATE::POSITION, playerPos);
    }

#pragma endregion




    // ***************************************
    // * [WASD] 이동
    // ***************************************
#pragma region [WASD] Moving

    // 상태전환
    if (m_pGameInstance->IsKeyHold('W') ||
        m_pGameInstance->IsKeyHold('S') ||
        m_pGameInstance->IsKeyHold('A') ||
        m_pGameInstance->IsKeyHold('D'))
    {
        if (!CStat_Manager::GetInstance()->Get_UIOpen()) {
            m_pGameInstance->PlaySoundW(L"footstepRoad01.wav", ENUM_CLASS(CHANNELID::SOUND_PLAYER_WALK), g_fWALKVolume);
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
    }
    else
    {
        g_fWALKVolume = 0.f;
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

#pragma endregion

#pragma region Transform Change

    // [이동 이동]

    if (!(m_pAnimatorTransCom->Get_CurStateTag() == L"Dash" ||
        m_pAnimatorTransCom->Get_CurStateTag() == L"Parry" ||
        m_pAnimatorTransCom->Get_CurStateTag() == L"Fury" ||
        m_pAnimatorTransCom->Get_CurStateTag() == L"Attack"))
    {
        if (!CStat_Manager::GetInstance()->Get_UIOpen()) {
            if (m_pGameInstance->IsKeyHold('W'))
            {
                m_pTransformCom->Go_Straight(fTimeDelta);   m_vDashDir = { 0, 0, 1 };
            }
            if (m_pGameInstance->IsKeyHold('S'))
            {
                m_pTransformCom->Go_Backward(fTimeDelta);   m_vDashDir = { 0, 0, -1 };
            }
            if (m_pGameInstance->IsKeyHold('A'))
            {
                m_pTransformCom->Go_Left(fTimeDelta);       m_vDashDir = { -1, 0, 0 };
            }
            if (m_pGameInstance->IsKeyHold('D'))
            {
                m_pTransformCom->Go_Right(fTimeDelta);      m_vDashDir = { 1, 0, 0 };
            }

            if (m_pGameInstance->IsKeyHold('A') && m_pGameInstance->IsKeyHold('W'))
                m_vDashDir = { -1, 0, 1 };
            if (m_pGameInstance->IsKeyHold('W') && m_pGameInstance->IsKeyHold('D'))
                m_vDashDir = { 1, 0, 1 };
            if (m_pGameInstance->IsKeyHold('D') && m_pGameInstance->IsKeyHold('S'))
                m_vDashDir = { 1, 0, -1 };
            if (m_pGameInstance->IsKeyHold('S') && m_pGameInstance->IsKeyHold('A'))
                m_vDashDir = { -1, 0, -1 };

            if (!(m_pGameInstance->IsKeyHold('A') ||
                m_pGameInstance->IsKeyHold('S') ||
                m_pGameInstance->IsKeyHold('W') ||
                m_pGameInstance->IsKeyHold('D')))
                m_vDashDir = m_vCursorDir;
        }
    }

#pragma endregion


    // ***************************************
    // * [Space] 대쉬
    // ***************************************
#pragma region [Space] Dash

    if (m_pGameInstance->IsKeyDown(VK_SPACE))
    {
        if (!CStat_Manager::GetInstance()->Get_UIOpen()) {
            // 스페이스바 누를 시 Dash 상태로 바꾸기 시도
            if (pStats->Get_CurStats()[ENUM_CLASS(STAT_INFO::CULDASH)] >= 1 &&
                m_pAnimatorTransCom->Change_State(L"Dash"))
            {
                m_pGameInstance->StopSound(ENUM_CLASS(CHANNELID::SOUND_PLAYER));
                m_pGameInstance->PlaySoundW(L"dashMove.wav", ENUM_CLASS(CHANNELID::SOUND_PLAYER), g_fEFFECTVolume - 0.6f);
                D3DXVec3Normalize(&m_vDashDir, &m_vDashDir);
                pStats->Cal_Stats(STAT_INFO::CULDASH, -1);
                // 무적 설정..
                m_bIsHit = true;
                m_fGodModeTime = 0.f;
            }
        }
    }
    else
    {
        if (m_pAnimatorTransCom->Change_State(L"Idle"))
        {
            // 무적 해제...
        }
    }

#pragma endregion

#pragma region Transform Change

    // [Dash 이동]
    if (m_pAnimatorTransCom->Get_CurStateTag() == L"Dash")
    {
        _float3 playerPos = vPlayerPos;
        playerPos += m_vDashDir * 60.f * (-0.04f * pow((fTimeDelta - 5.f), 2.f) + 1.f);        // 마지막으로 누른 방향으로 이동
        m_pTransformCom->Set_State(STATE::POSITION, playerPos);
    }

#pragma endregion


    // ***************************************
    // * [마우스 우클] 임시 : 패리, 퓨리 (PARRY, FURY)
    // ***************************************
#pragma region [Mouse R] Parry, Fury

    if (m_pGameInstance->IsKeyDown(VK_RBUTTON))
    {
        if (!CStat_Manager::GetInstance()->Get_UIOpen()) {
            if ((m_pAnimatorCom->Get_CurStateTag() != L"Attack_Lower" &&
                m_pAnimatorCom->Get_CurStateTag() != L"Attack_Upper" &&
                m_pAnimatorCom->Get_CurStateTag() != L"Attack_Lower2" &&
                m_pAnimatorCom->Get_CurStateTag() != L"Attack_Upper2") &&
                (m_pAnimatorTransCom->Get_CurStateTag() != L"Parry"))
            {
                if (pStats->Get_CurStats()[ENUM_CLASS(STAT_INFO::FURYREADY)])
                {
                    if (m_pAnimatorTransCom->Change_State(L"Fury"))
                    {
                        m_pGameInstance->StopSound(ENUM_CLASS(CHANNELID::SOUND_PLAYER_EFFECT));
                        m_pGameInstance->PlaySoundW(L"DaggerAttack_Fury.wav", ENUM_CLASS(CHANNELID::SOUND_PLAYER_EFFECT), g_fEFFECTVolume - 0.6f);
                        CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::PLAYER_EFFECT, L"Prototype_Component_Texture_Effect_Blade0_NFury",
                            *m_pTransformCom->Get_WorldMatrix(), matPlayerWorld, m_pTransformCom, 0.f, true);
                        CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::PLAYER_EFFECT, L"Prototype_Component_Texture_Effect_Blade0_NFury_Back",
                            *m_pTransformCom->Get_WorldMatrix(), matPlayerWorld, m_pTransformCom, 0.f, true);
                    }
                    strStateTag = (vRayPoint.z > vPlayerPos.z) ? L"Fury_Upper" :
                        L"Fury_Lower";
                    m_pAnimatorCom->Change_State(strStateTag);
                    m_isReadyFury = false;
                    pStats->Set_Stats(STAT_INFO::FURYREADY, false);
                }
                else if (m_pAnimatorTransCom->Get_CurStateTag() == L"Idle" &&
                    pStats->Get_CurStats()[ENUM_CLASS(STAT_INFO::CULMP)] >= 10)
                {
                    if (m_pAnimatorTransCom->Change_State(L"Parry"))
                    {
                        // Parry 시에만 재조정
                        fDistanceOffset = 0.f;
                        vDiffResult = vDiff * fDistanceOffset;
                        D3DXMatrixTranslation(&matTransAddition, vDiffResult.x, 0, vDiffResult.z);
                        matPlayerWorld = matTransToOrigin * matScale * matRotateChild * matRotateChildtoCursor * matTransReturn * matTransAddition;

                        m_pGameInstance->StopSound(ENUM_CLASS(CHANNELID::SOUND_PLAYER_EFFECT));
                        m_pGameInstance->PlaySoundW(L"DaggerAttack_Parry.wav", ENUM_CLASS(CHANNELID::SOUND_PLAYER_EFFECT), g_fEFFECTVolume - 0.6f);

                        // 이펙트 적용
                        CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::PLAYER_EFFECT, L"Prototype_Component_Texture_Effect_Blade0_Parry",
                            *m_pTransformCom->Get_WorldMatrix(), matPlayerWorld, m_pTransformCom, 0.f, true);

                        // 다시 초기값대로 초기화
                        fDistanceOffset = 1.2f;
                        vDiffResult = vDiff * fDistanceOffset;
                        D3DXMatrixTranslation(&matTransAddition, vDiffResult.x, 0, vDiffResult.z);
                        matPlayerWorld = matTransToOrigin * matScale * matRotateChild * matRotateChildtoCursor * matTransReturn * matTransAddition;

                        _float3 playerPos = vPlayerPos;
                        playerPos += m_vCursorDir * fTimeDelta * 120.f;        // 커서 방향으로 이동
                        m_pTransformCom->Set_State(STATE::POSITION, playerPos);
                    }
                    if (m_pAnimatorCom->Change_State(L"Parry"))
                        pStats->Cal_Stats(STAT_INFO::CULMP, -10);
                }
            }
        }
    }
    else
    {
        m_pAnimatorTransCom->Change_State(L"Idle");
    }

#pragma endregion

#pragma region Transform Change (Parry)

    //// [Parry 이동]
    //if (m_pAnimatorTransCom->Get_CurStateTag() == L"Parry")
    //{
    //    if (m_pAnimatorCom->Get_CurStackedFrame() >= 8)
    //    {
    //        _float3 playerPos = vPlayerPos;
    //        playerPos += m_vCursorDir * fTimeDelta * 15.f * (-1 * cosf(0.4f * m_pAnimatorCom->Get_CurStackedFrame() - 0.7) + 1);        // 커서 방향으로 이동
    //        m_pTransformCom->Set_State(STATE::POSITION, playerPos);
    //        // 무적 설정...
    //        m_bIsHit = true;
    //        // 공격 막는 데에 성공 시 Fury_Ready로 넘어갈 준비
    //        if (m_pGameInstance->IsKeyDown('M'))    // ksta : 조건은 나중에 수정
    //        {
    //            if (!CStat_Manager::GetInstance()->Get_UIOpen()) {
    //                m_isReadyFury = true;
    //                pStats->Set_Stats(STAT_INFO::FURYREADY, true);
    //            }
    //        }
    //    }
    //}
    //else
    //{
    //    // 무적 해제...
    //}
    //
#pragma endregion   

#pragma region Transform Change (Fury)

    // [Fury 이동]
    if (m_pAnimatorTransCom->Get_CurStateTag() == L"Fury")
    {
        if (m_pAnimatorCom->Get_CurStackedFrame() >= 6)
        {
            _float3 playerPos = vPlayerPos;
            playerPos += m_vCursorDir * fTimeDelta * 14.f * (-1 * cosf(0.4f * m_pAnimatorCom->Get_CurStackedFrame() - 0.7f) + 1.f);        // 커서 방향으로 이동
            m_pTransformCom->Set_State(STATE::POSITION, playerPos);

            // 무적 설정...
            m_bIsHit = true;
        }
    }
    else
    {
        // 무적 해제...
    }

#pragma endregion









    // 상태가 바뀌었다면, 공격 타이머 초기화
    if (strCurStateTag == L"Attack_Upper" ||
        strCurStateTag == L"Attack_Lower")
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



    //std::cout << "[Player::Update] PlayerPos : " << vPlayerPos.x << ", " << vPlayerPos.y << ", " << vPlayerPos.z << std::endl;
}



void CPlayer::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_BLEND, this);

}

HRESULT CPlayer::Render()
{
    SetUp_RenderState();

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
    if (m_isFlippedX)
    {
        m_pVIBufferCom->ResetUV_FlipX();
        m_isFlippedX = false;
    }

    Reset_RenderState();
    return S_OK;
}

void CPlayer::Change_TerrainBox(CTerrainBox* pTerrainBox, _int iIndex)
{
    Safe_Release(m_pTerrainBox);
    m_pTerrainBox = pTerrainBox;
    Safe_AddRef(m_pTerrainBox);
}

void CPlayer::OnCollision(CGameObject* pGameObject)
{
    switch (pGameObject->Get_ObjType())
    {
    case GAMEOBJ_TYPE::POTAL:
    {
        if (pGameObject->Get_IsActive()) {
            _float3 vPos;
            CRoom_Manager::GetInstance()->Check_Potal_Coll(dynamic_cast<CPotal*>(pGameObject)->Get_PotalType(), vPos);

            m_pTransformCom->Set_State(STATE::POSITION, vPos);
            m_pGameInstance->StopSound(ENUM_CLASS(CHANNELID::SOUND_EFFECT));
            m_pGameInstance->PlaySoundW(L"breakBulletIce.wav", ENUM_CLASS(CHANNELID::SOUND_EFFECT), g_fEFFECTVolume - 0.6f);
        }

        break;
    }
    case GAMEOBJ_TYPE::END_POTAL:
    {
        if (pGameObject->Get_IsActive()) {
            if (!CStat_Manager::GetInstance()->Get_UIOpen()) {
                if (m_pGameInstance->IsKeyDown('F'))
                {
                    dynamic_cast<CChapMap*>(m_pGameInstance->Get_GameObject(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Layer_ChapMap")))->Open_Ui();
                    m_pGameInstance->StopSound(ENUM_CLASS(CHANNELID::SOUND_EFFECT));
                    m_pGameInstance->PlaySoundW(L"breakBulletIce.wav", ENUM_CLASS(CHANNELID::SOUND_EFFECT), g_fEFFECTVolume - 0.6f);
                }
            }
        }
        break;
    }
    case GAMEOBJ_TYPE::STAGE_POTAL:
    {

        _uint CurrentLevel = m_pGameInstance->Get_CurrentLevel();
        /*m_pGameInstance->StopSound(ENUM_CLASS(CHANNELID::SOUND_EFFECT));
        m_pGameInstance->PlaySoundW(L"breakBulletIce.wav", ENUM_CLASS(CHANNELID::SOUND_EFFECT), g_fEFFECTVolume - 0.6f);*/
        if (CurrentLevel == ENUM_CLASS(LEVEL::LEVEL_TOWN))  //타운일 때 스테이지1로 이동
            m_pGameInstance->Open_Level(static_cast<_uint>(LEVEL::LEVEL_LOADING), CLevel_Loading::Create(m_pGraphic_Device, LEVEL::LEVEL_STAGE1));
        else if (CurrentLevel == ENUM_CLASS(LEVEL::LEVEL_BOSS1)) // 보스1일 때 쉼터로 이동
            m_pGameInstance->Open_Level(static_cast<_uint>(LEVEL::LEVEL_LOADING), CLevel_Loading::Create(m_pGraphic_Device, LEVEL::LEVEL_SHELTER));
        else if (CurrentLevel == ENUM_CLASS(LEVEL::LEVEL_SHELTER)) // 쉼터일 때 보스2로 이동
            m_pGameInstance->Open_Level(static_cast<_uint>(LEVEL::LEVEL_LOADING), CLevel_Loading::Create(m_pGraphic_Device, LEVEL::LEVEL_BOSS2));
        break;
    }
    case GAMEOBJ_TYPE::EXPBALL:
    {
        CStat_Manager::GetInstance()->Cal_Stats(STAT_INFO::EXP, dynamic_cast<CEXP_Ball*>(pGameObject)->Get_EXP());
        ACTIONEVENT desc{};
        desc.strActionName = TEXT("GET_EXP");
        m_pGameInstance->Broadcast(ENUM_CLASS(EVENT_TYPE::EXP), &desc);
        pGameObject->Set_IsDead(TRUE);
        m_pGameInstance->StopSound(ENUM_CLASS(CHANNELID::SOUND_EFFECT));
        m_pGameInstance->PlaySoundW(L"changeConfirm.wav", ENUM_CLASS(CHANNELID::SOUND_EFFECT), g_fEFFECTVolume - 0.6f);
        break;
    }
    case GAMEOBJ_TYPE::OBJECT:
    {

        _float3 vPlayerPos = m_pTransformCom->Get_State(STATE::POSITION);

        CTransform* pObjectTransform = dynamic_cast<CTransform*>(pGameObject->Find_Component(L"Com_Transform"));
        _float3 vObjectPos = pObjectTransform->Get_State(STATE::POSITION);

        _float3 vPushDir = vPlayerPos - vObjectPos;
        _float fDist = D3DXVec3Length(&vPushDir);

        if (fDist < 0.001f)
            break;

        D3DXVec3Normalize(&vPushDir, &vPushDir);

        const _float fMinDistance = 2.0f;
        _float fOverlap = fMinDistance - fDist;

        if (fOverlap > 0.f)
        {
            _float3 vTargetPos = vPlayerPos + vPushDir * fOverlap;

            _float fLerpSpeed = 0.25f;
            _float3 vNewPos;
            vNewPos.x = vPlayerPos.x + (vTargetPos.x - vPlayerPos.x) * fLerpSpeed;
            vNewPos.y = vPlayerPos.y + (vTargetPos.y - vPlayerPos.y) * fLerpSpeed;
            vNewPos.z = vPlayerPos.z + (vTargetPos.z - vPlayerPos.z) * fLerpSpeed;

            m_pTransformCom->Set_State(STATE::POSITION, vNewPos);
        }
        break;
    }

    case GAMEOBJ_TYPE::MONSTER_EFFECT:
    {
        if (!m_bIsHit) {
            _float3 vPlayerPos = {};    // 플레이어 좌표
            vPlayerPos = m_pTransformCom->Get_State(STATE::POSITION);

            CTransform* pEnemyTransform = dynamic_cast<CTransform*>(pGameObject->Find_Component(L"Com_Transform"));
            _float3 vEnemyPos = pEnemyTransform->Get_State(STATE::POSITION);
            _float3 vStunDir = vPlayerPos - vEnemyPos;
            D3DXVec3Normalize(&vStunDir, &vStunDir);

            _float3 vResult = vPlayerPos + vStunDir * 0.2f;    // 밀려날 정도 테스트
            m_pTransformCom->Set_State(STATE::POSITION, vResult);
        }
        break;
    }

    case GAMEOBJ_TYPE::SHOP_ITEM:
    {
        if (pGameObject != nullptr)
        {
            if(m_pGameInstance->IsKeyDown('F'))
                dynamic_cast<CField_Item*>(pGameObject)->Buy_Item();
        }
        break;
    }

    case GAMEOBJ_TYPE::GOLDLEAF:
    {
        CStat_Manager::GetInstance()->Cal_Stats(STAT_INFO::GOLD, dynamic_cast<CGoldLeaf*>(pGameObject)->Get_Gold());
        /*ACTIONEVENT desc{};
        desc.strActionName = TEXT("GET_GOLD");
        m_pGameInstance->Broadcast(ENUM_CLASS(EVENT_TYPE::GOLD), &desc);*/
        pGameObject->Set_IsDead(TRUE);
        m_pGameInstance->StopSound(ENUM_CLASS(CHANNELID::SOUND_EFFECT));
        m_pGameInstance->PlaySoundW(L"goldCollect.wav", ENUM_CLASS(CHANNELID::SOUND_EFFECT), g_fEFFECTVolume - 0.6f);
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

void CPlayer::Hit(_int iDamage)
{
    m_bIsHit = true;
    m_bIsStun = true;

    _wstring strStateTag = {};
    _float fPointY = 0.f;       // 교차 평면의 기준이 될 Y값
    _float3 vRayPoint = {};     // fPointY 값 기준 마우스 Ray와 교차하는 좌표
    m_pGameInstance->Get_IntersectAtY(fPointY, vRayPoint);

    //strStateTag = (vRayPoint.z > vPlayerPos.z)?     L"Idle_Upper":
                                                    //L"Idle_Lower";

    //m_pAnimatorCom->Change_State(strStateTag, true, 2);
    m_pAnimatorCom->Change_State(L"Air", false, 0.2, true);
    Render_Font(iDamage);
}


HRESULT CPlayer::Ready_Components(void* pArg)
{
    PLAYERDESC* pDesc = static_cast<PLAYERDESC*>(pArg);

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
    m_pTransformCom->Set_State(STATE::POSITION, vPlayerPos + _float3{ 0, 0.5, 0 });
    m_pTransformCom->Scaling(float(18) / 19, 1, 1);


    /* For Com_Animator */
    CAnimator::ANIMSTATE_DESC StartAnimStateDesc{};
    StartAnimStateDesc.strTimerTag = L"Animator_Player_Main";   // 해당 애니메이터가 타이머에서 사용할 태그 key값
    StartAnimStateDesc.pParentTransform = m_pTransformCom;
    StartAnimStateDesc.pChildTransform = static_cast<CTransform*>(m_pGameInstance->Get_Component(pDesc->iLayerIndex, L"Layer_Weapon", L"Com_Transform"));

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Animator"),
        TEXT("Com_Animator"), reinterpret_cast<CComponent**>(&m_pAnimatorCom), &StartAnimStateDesc)))
        return E_FAIL;

    // 요소 삽입 : m_pAnimatorCom->Add_State(L"태그명", { m_pTextureCom_상태명, 프레임단위 이미지전환간격, 도중 나갈수있는지});
    // 애니메이션 찾기 :  m_pGameInstance->Find_Animation(L"태그명");
    // 애니메이션은 Level_GamePlay.cpp 에서 Create 함.

    m_pAnimatorCom->Add_State(L"Roll", { m_pTextureCom_Roll, 3, true });
    m_pAnimatorCom->Add_State(L"Air", { m_pTextureCom_Air, 4, true });
    m_pAnimatorCom->Add_State(L"Down", { m_pTextureCom_Down, 4, true });
    m_pAnimatorCom->Add_State(L"Idle_Lower", { m_pTextureCom_Idle_Lower, 4, true, m_pGameInstance->Find_Animation(L"Player_Idle") });
    m_pAnimatorCom->Add_State(L"Idle_Upper", { m_pTextureCom_Idle_Upper, 4, true, m_pGameInstance->Find_Animation(L"Player_Idle") });
    //m_pAnimatorCom->Add_State(L"Idle_Lower_Damaged",{ m_pTextureCom_Idle_Lower, 4, false });
    //m_pAnimatorCom->Add_State(L"Idle_Upper_Damaged",{ m_pTextureCom_Idle_Upper, 4, false });
    m_pAnimatorCom->Add_State(L"Move_Lower", { m_pTextureCom_Move_Lower, 4, true });
    m_pAnimatorCom->Add_State(L"Move_Upper", { m_pTextureCom_Move_Upper, 4, true });
    m_pAnimatorCom->Add_State(L"Attack_Lower", { m_pTextureCom_Attack_Lower, 3, false, m_pGameInstance->Find_Animation(L"Player_Attack") });
    m_pAnimatorCom->Add_State(L"Attack_Upper", { m_pTextureCom_Attack_Upper, 3, false, m_pGameInstance->Find_Animation(L"Player_Attack") });
    m_pAnimatorCom->Add_State(L"Attack_Lower2", { m_pTextureCom_Attack_Lower, 3, false, m_pGameInstance->Find_Animation(L"Player_Attack2") });
    m_pAnimatorCom->Add_State(L"Attack_Upper2", { m_pTextureCom_Attack_Upper, 3, false, m_pGameInstance->Find_Animation(L"Player_Attack2") });
    m_pAnimatorCom->Add_State(L"GreatSwordHeavyAttack_Lower", { m_pTextureCom_GreatSwordHeavyAttack_Lower, 4, false });
    m_pAnimatorCom->Add_State(L"GreatSwordHeavyAttack_Upper", { m_pTextureCom_GreatSwordHeavyAttack_Upper, 4, false });
    m_pAnimatorCom->Add_State(L"WhirlWind_Ready", { m_pTextureCom_WhirlWind_Ready, 4, false });
    m_pAnimatorCom->Add_State(L"WhirlWind_Cycle", { m_pTextureCom_WhirlWind_Cycle, 4, false });
    m_pAnimatorCom->Add_State(L"Parry", { m_pTextureCom_Roll, 3, false, m_pGameInstance->Find_Animation(L"Player_Parry") });
    m_pAnimatorCom->Add_State(L"Fury_Lower", { m_pTextureCom_Attack_Lower, 6, false, m_pGameInstance->Find_Animation(L"Player_Fury") });
    m_pAnimatorCom->Add_State(L"Fury_Upper", { m_pTextureCom_Attack_Upper, 6, false, m_pGameInstance->Find_Animation(L"Player_Fury") });

    /* For Com_Animator (이동 관련) */
    CAnimator::ANIMSTATE_DESC StartAnimStateDesc2{};
    StartAnimStateDesc2.strTimerTag = L"Animator_Player_Dash";   // 해당 애니메이터가 타이머에서 사용할 태그 key값
    StartAnimStateDesc2.pParentTransform = nullptr; // m_pTransformCom;
    StartAnimStateDesc2.pChildTransform = m_pTransformCom;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Animator"),
        TEXT("Com_Animator_Dash"), reinterpret_cast<CComponent**>(&m_pAnimatorTransCom), &StartAnimStateDesc2)))
        return E_FAIL;

    m_pAnimatorTransCom->Add_State(L"Idle", { nullptr, 1, true });
    m_pAnimatorTransCom->Add_State(L"Dash", { nullptr, 10, false });
    m_pAnimatorTransCom->Add_State(L"Attack", { nullptr, 4, false });
    m_pAnimatorTransCom->Add_State(L"Parry", { nullptr, 18, false });
    m_pAnimatorTransCom->Add_State(L"Fury", { nullptr, 18, false });

    // collider
    CCollider_OBB::OBB_DESC tColliderDesc;
    tColliderDesc.vScale = _float3(0.7f, 0.5f, 0.7f);
    tColliderDesc.pOwner = this;
    tColliderDesc.pTransform = m_pTransformCom;
    tColliderDesc.eType = m_eObjType;
    CCollider_OBB* pCol = dynamic_cast<CCollider_OBB*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::COMPONENT, ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Collider_OBB"), &tColliderDesc));
    m_pGameInstance->Add_Collider(pCol);
    return S_OK;
}

HRESULT CPlayer::Ready_Object()
{
    //m_pHpBar = dynamic_cast<CField_Hp*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_UI_Field_Hp")));
   
    CField_Npc_Chat::FIELD_CHAT_DESC ChatDesc{};

    _wstring szTag = TEXT("Test_Chat");

    //NPC 트랜스폼
    ChatDesc.pTransform = m_pTransformCom;
    //채팅 UI 태그
    ChatDesc.szChatTag = szTag;
    //NPC 현재 레벨 넣어주면 됩니다.
    ChatDesc.m_iLevel = ENUM_CLASS(LEVEL::LEVEL_TOWN);
    //NPC 머리 위로 얼만큼 띄울거지 음수 값 넣어주면 됩니다.
    ChatDesc.fY = -100;
    //생성
    if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_TOWN), TEXT("Layer_UI_Chat"),
        ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_UI_Field_Npc_Chat"), &ChatDesc)))
        return E_FAIL;

    //멤버 변수의 채팅 클래스 주소 연결(만든 레벨, UI 태그)
    m_pChat = static_cast<CField_Npc_Chat*>(m_pGameInstance->Find_UIObj(ENUM_CLASS(LEVEL::LEVEL_TOWN), TEXT("Test_Chat")));

    m_pChat->Add_Chat(TEXT("일이삼사오육칠팔구십일이"));
    m_pChat->Add_Chat(TEXT("안녕하세요2"));
    m_pChat->Add_Chat(TEXT("안녕하세요3"));
    m_pChat->Add_Chat(TEXT("안녕하세요4"));
    return S_OK;
}

HRESULT CPlayer::Render_Font(_int iDamage)
{
    CField_Font::FIELD_FONT_DESC Desc = {};

    Desc.eType = CField_Font::FIELD_FONT_TYPE::PLAYER_DAMAGE;   //데미지 타입
    Desc.iValue = iDamage;                                           //넣을 수치 값
    Desc.pTransform = m_pTransformCom;                          //현재 객체 트랜스폼

    if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(m_pGameInstance->Get_CurrentLevel(), TEXT("Layer_Field_Font"),
        ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_UI_Field_Font"), &Desc)))
        return E_FAIL;
}

HRESULT CPlayer::Render_Font_Parry()
{
    CField_Font::FIELD_FONT_DESC Desc = {};

    Desc.eType = CField_Font::FIELD_FONT_TYPE::PARING;
    //Desc.iValue = iDamage;                                           //넣을 수치 값
    Desc.pTransform = m_pTransformCom;                          //현재 객체 트랜스폼

    if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(m_pGameInstance->Get_CurrentLevel(), TEXT("Layer_Field_Font"),
        ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_UI_Field_Font"), &Desc)))
        return E_FAIL;
}

void CPlayer::Ready_Parry()
{
    // [Parry 이동]
    if (!m_isReadyFury) {
        if (m_pAnimatorTransCom->Get_CurStateTag() == L"Parry")
        {
            
            if (m_pAnimatorCom->Get_CurStackedFrame() >= 8)
            {
                m_pGameInstance->StopSound(ENUM_CLASS(CHANNELID::SOUND_PLAYER_EFFECT));
                m_pGameInstance->PlaySoundW(L"DaggerAttack_ParrySuccess_2.wav", ENUM_CLASS(CHANNELID::SOUND_PLAYER_EFFECT), g_fEFFECTVolume - 0.6f);
                m_fGodModeTime = 0.f;
                Render_Font_Parry();
                _float3 playerPos = m_pTransformCom->Get_State(STATE::POSITION);
                playerPos += m_vCursorDir * 0.01f * 15.f * (-1 * cosf(0.4f * m_pAnimatorCom->Get_CurStackedFrame() - 0.7) + 1);        // 커서 방향으로 이동
                m_pTransformCom->Set_State(STATE::POSITION, playerPos);
                // 무적 설정...
                // 무적 바꿔야함 패리 순간만 무적임
                m_bIsHit = true;
                // 공격 막는 데에 성공 시 Fury_Ready로 넘어갈 준비
                if (!CStat_Manager::GetInstance()->Get_UIOpen()) {
                    m_isReadyFury = true;
                    CStat_Manager::GetInstance()->Set_Stats(STAT_INFO::FURYREADY, true);
                }

            }
        }
    }
    

}

_bool CPlayer::Get_IsGodMode()
{
    if (m_fGodModeTime <= 31)
        return true;

    return false;
}

void CPlayer::SetUp_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
}

void CPlayer::Reset_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

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
    
    CEffect_Factory::GetInstance()->Free();

    __super::Free();
}

#include "Erma_Hand_R.h"
#include "Effect_Factory.h"

#include "Erma.h"
#include "Room_Manager.h"
#include "Camera_Follow.h"

CErma_Hand_R::CErma_Hand_R(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CMonster{ pGraphic_Device }
{
}

CErma_Hand_R::CErma_Hand_R(const CErma_Hand_R& Prototype)
    : CMonster{ Prototype }
{
}

HRESULT CErma_Hand_R::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CErma_Hand_R::Initialize(void* pArg)
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
        fTerrainPos.x + 6.f,
        0.f,
        fTerrainPos.z + fTerrainScale.z / 2 - 4.5f));

    // 크기 조정
    m_pTransformCom->Scaling(4.5f, 4.5f, 4.5f);



    // ksta : 테스트중.. 패턴 완성 후 삭제
    //PlayPattern(PATTERN_HAND_R::PT_LASER);



    m_isSummoned = true;
    //Ready_Object();

    // 임시
    m_iMaxHp = 999999;
    m_iCulHp = 999999;
    m_eMonsterType = MONSTER_TYPE::ERMA_HAND_R;
    return S_OK;
}

void CErma_Hand_R::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
    //if (m_pHpBar != nullptr &&
    //    m_isSummoned)
    //    m_pHpBar->Render_HP_Progress(m_pTransformCom, m_iCulHp, m_iMaxHp);

    //if (m_iCulHp <= 0)
    //    m_bDead = true;
}

void CErma_Hand_R::Update(_float fTimeDelta)
{
    if (m_isAllStop)
        return;
    // Update
    // 


    // Idle
    // Strike   // 7s (420)
    // Laser    // 10s (600)

    // 아래에서 사용할 변수들
#pragma region Variables Setting

    _uint iCurLevel = m_pGameInstance->Get_CurrentLevel();

    CTransform* pTargetTransform = dynamic_cast<CTransform*>(m_pGameInstance->GetInstance()->Get_Component(iCurLevel, TEXT("Layer_Player"), TEXT("Com_Transform")));
    _float3 vTerrainPos = m_pTerrainTransformCom->Get_State(STATE::POSITION);
    _float3 vTerrainScale = m_pTerrainTransformCom->Get_Scaled();

    _float3 vMonsterPos = m_pTransformCom->Get_State(STATE::POSITION);
    _float3 vTargetPos = pTargetTransform->Get_State(STATE::POSITION);

    _float3 vDiff = -vMonsterPos + vTargetPos;
    _float fDistance = D3DXVec3Length(&vDiff);

    _int iCurPatternFrame = m_pAnimatorPatternCom->Get_CurStackedFrame();

    // ********* matMonster 구하기
    _float4x4 matMonsterWorld = *m_pTransformCom->Get_WorldMatrix();

    _float3 vTerrainOffset = { 0.f, 1.f, 0.f };

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
    D3DXMatrixRotationX(&matRotateChild, D3DXToRadian(-90)); // 안되면 -90도도 해보기

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

    if (m_isPatternPlaying)
    {

        switch (m_ePattern)
        {
        case Client::CErma_Hand_R::PATTERN_HAND_R::PT_IDLE:
        {
            _float3 vPos = {
                    vTerrainPos.x + 10.f,
                    0.f,
                    vTerrainPos.z + vTerrainScale.z / 2 - 6.f
                };
            m_pTransformCom->Move_To(vPos + vTerrainPos + vTerrainOffset, fTimeDelta * 10.f, 0.01f);
            m_pTerrainBox->SetUp_OnTerrainBox(m_pTransformCom, vTerrainOffset);
        }
            break;
        case Client::CErma_Hand_R::PATTERN_HAND_R::PT_STRIKE:
        {
#pragma region PT_STRIKE Pattern
            // 트래킹 상태일 때 추적 
            if (m_isTracking)
                m_pTransformCom->Move_To(vTargetPos, fTimeDelta * 4.f, 0.01f);

            _float fROffset = 80;

            if (IS_BETWEEN(iCurPatternFrame, 0 + fROffset, 40 + fROffset))
            {
                _int iFrame = iCurPatternFrame - (0 + fROffset);
                m_isTracking = true;
                _float fY = -0.00125 * pow((iFrame - 40), 2) + 2;
                m_pTerrainBox->SetUp_OnTerrainBox(m_pTransformCom, _float3(0.0f, fY, 0.0f) + vTerrainOffset);
            }
            else if (IS_BETWEEN(iCurPatternFrame, 40 + fROffset, 100 + fROffset))
            {
                _int iFrame = iCurPatternFrame - (40 + fROffset);
                // tracking..
                m_pTerrainBox->SetUp_OnTerrainBox(m_pTransformCom, _float3(0.0f, 2.f, 0.0f) + vTerrainOffset);
            }
            else if (IS_BETWEEN(iCurPatternFrame, 100 + fROffset, 110 + fROffset))
            {
                _int iFrame = iCurPatternFrame - (100 + fROffset);
                m_isTracking = false;
                _float fY = -0.02 * pow((iFrame), 2) + 2;
                m_pTerrainBox->SetUp_OnTerrainBox(m_pTransformCom, _float3(0.0f, fY, 0.0f) + vTerrainOffset);
            }
            else if (IS_BETWEEN(iCurPatternFrame, 110 + fROffset, 160 + fROffset))
            {
                _int iFrame = iCurPatternFrame - (110 + fROffset);
                m_pTerrainBox->SetUp_OnTerrainBox(m_pTransformCom, vTerrainOffset);
#pragma region Effect Setting

                D3DXMatrixIdentity(&matScale);
                D3DXMatrixScaling(&matScale, -1.5f, 1.f, 1.f);

                D3DXMatrixIdentity(&matRotateChild);
                D3DXMatrixRotationX(&matRotateChild, D3DXToRadian(90)); // 안되면 -90도도 해보기

                D3DXMatrixIdentity(&matTransAddition);
                D3DXMatrixTranslation(&matTransAddition, 0, -m_pTransformCom->Get_Scaled().y / 2, 0);

                matMonsterWorld = matTransToOrigin * matScale * matRotateChild * matRotateChildtoPlayer * matTransReturn * matTransAddition;

#pragma endregion
                if (iCurPatternFrame == (110 + fROffset))
                {
                    m_pGameInstance->StopSound(ENUM_CLASS(CHANNELID::SOUND_MONSTER_EFFECT));
                    m_pGameInstance->PlaySoundW(L"GolemHandStomp.wav", ENUM_CLASS(CHANNELID::SOUND_MONSTER_EFFECT), g_fEFFECTVolume - 0.8f);
                    dynamic_cast<CCamera_Follow*>(m_pGameInstance->Get_GameObject(m_pGameInstance->Get_CurrentLevel(), TEXT("Layer_Camera")))->Start_Shake(0.4f, 0.3);
                    CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::MONSTER_EFFECT, L"Prototype_Component_Boss_Erma_BigGolem_Stmap_FX",
                        *m_pTransformCom->Get_WorldMatrix(), matMonsterWorld);
                }
                    
#pragma region Effect Reset

                D3DXMatrixIdentity(&matScale);
                D3DXMatrixScaling(&matScale, -1.f, 1.f, 1.f);

                D3DXMatrixIdentity(&matRotateChild);
                D3DXMatrixRotationX(&matRotateChild, D3DXToRadian(90)); // 안되면 -90도도 해보기

                D3DXMatrixIdentity(&matTransAddition);

                matMonsterWorld = matTransToOrigin * matScale * matRotateChild * matRotateChildtoPlayer * matTransReturn * matTransAddition;

#pragma endregion
            }
            else if (IS_BETWEEN(iCurPatternFrame, 0 + 160 + fROffset, 40 + 160 + fROffset))
            {
                _int iFrame = iCurPatternFrame - (0 + 160 + fROffset);
                m_isTracking = true;
                _float fY = -0.00125 * pow((iFrame - 40), 2) + 2;
                m_pTerrainBox->SetUp_OnTerrainBox(m_pTransformCom, _float3(0.0f, fY, 0.0f) + vTerrainOffset);
            }
            else if (IS_BETWEEN(iCurPatternFrame, 40 + 160 + fROffset, 100 + 160 + fROffset))
            {
                _int iFrame = iCurPatternFrame - (40 + 160 + fROffset);
                // tracking..
                m_pTerrainBox->SetUp_OnTerrainBox(m_pTransformCom, _float3(0.0f, 2.f, 0.0f) + vTerrainOffset);
            }
            else if (IS_BETWEEN(iCurPatternFrame, 100 + 160 + fROffset, 110 + 160 + fROffset))
            {
                _int iFrame = iCurPatternFrame - (100 + 160 + fROffset);
                m_isTracking = false;
                _float fY = -0.02 * pow((iFrame), 2) + 2;
                m_pTerrainBox->SetUp_OnTerrainBox(m_pTransformCom, _float3(0.0f, fY, 0.0f) + vTerrainOffset);
            }
            else if (IS_BETWEEN(iCurPatternFrame, 110 + 160 + fROffset, 160 + 160 + fROffset))
            {
#pragma region Effect Setting

                D3DXMatrixIdentity(&matScale);
                D3DXMatrixScaling(&matScale, -1.5f, 1.f, 1.f);

                D3DXMatrixIdentity(&matRotateChild);
                D3DXMatrixRotationX(&matRotateChild, D3DXToRadian(90)); // 안되면 -90도도 해보기

                D3DXMatrixIdentity(&matTransAddition);
                D3DXMatrixTranslation(&matTransAddition, 0, -m_pTransformCom->Get_Scaled().y / 2, 0);

                matMonsterWorld = matTransToOrigin * matScale * matRotateChild * matRotateChildtoPlayer * matTransReturn * matTransAddition;

#pragma endregion
                if (iCurPatternFrame == (110 + 160 + fROffset))
                {
                    m_pGameInstance->StopSound(ENUM_CLASS(CHANNELID::SOUND_MONSTER_EFFECT));
                    m_pGameInstance->PlaySoundW(L"GolemHandStomp.wav", ENUM_CLASS(CHANNELID::SOUND_MONSTER_EFFECT), g_fEFFECTVolume - 0.8f);
                    dynamic_cast<CCamera_Follow*>(m_pGameInstance->Get_GameObject(m_pGameInstance->Get_CurrentLevel(), TEXT("Layer_Camera")))->Start_Shake(0.4f, 0.3);
                    CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::MONSTER_EFFECT, L"Prototype_Component_Boss_Erma_BigGolem_Stmap_FX",
                        *m_pTransformCom->Get_WorldMatrix(), matMonsterWorld);
                }
                    
#pragma region Effect Reset

                D3DXMatrixIdentity(&matScale);
                D3DXMatrixScaling(&matScale, -1.f, 1.f, 1.f);

                D3DXMatrixIdentity(&matRotateChild);
                D3DXMatrixRotationX(&matRotateChild, D3DXToRadian(-90)); // 안되면 -90도도 해보기

                D3DXMatrixIdentity(&matTransAddition);

                matMonsterWorld = matTransToOrigin * matScale * matRotateChild * matRotateChildtoPlayer * matTransReturn * matTransAddition;

#pragma endregion
                _int iFrame = iCurPatternFrame - (110 + 160 + fROffset);
                m_pTerrainBox->SetUp_OnTerrainBox(m_pTransformCom, vTerrainOffset);
            }
            else
            {
                _float3 vPos = {
                    vTerrainPos.x + 10.f,
                    0.f,
                    vTerrainPos.z + vTerrainScale.z / 2 - 6.f
                };

                m_pTransformCom->Move_To(vPos + vTerrainPos + vTerrainOffset, fTimeDelta * 10.f, 0.01f);
                m_pTerrainBox->SetUp_OnTerrainBox(m_pTransformCom, vTerrainOffset);

                if (iCurPatternFrame == 479)
                {
                    m_isPatternPlaying = false;
                    m_ePattern = PATTERN_HAND_R::PT_IDLE;
                    m_pAnimatorPatternCom->Change_State(L"Idle");
                }
            }
#pragma endregion
        }
        break;
        case Client::CErma_Hand_R::PATTERN_HAND_R::PT_LASER:
        {
#pragma region PT_LASER Pattern
            _bool isLaserEnd = false;

            // Pattern
            if (iCurPatternFrame == 1)
            {
                m_pGameInstance->StopSound(ENUM_CLASS(CHANNELID::SOUND_MONSTER_LONG_EFFECT2));
                m_pGameInstance->PlaySoundW(L"GolemLaser.wav", ENUM_CLASS(CHANNELID::SOUND_MONSTER_LONG_EFFECT2), g_fEFFECTVolume - 0.85f);
                m_pAnimatorCom->Change_State(L"LaserStart");
            }
            if (IS_BETWEEN(iCurPatternFrame, 0, 50))
            {
                _float3 vPos = {
                    vTerrainPos.x + vTerrainScale.x / 2,
                    0.f,
                    vTerrainPos.z - vTerrainScale.z / 2
                };
                m_pTransformCom->Move_To(vPos + vTerrainOffset, fTimeDelta * 20.f, 0.01f);
                m_pTerrainBox->SetUp_OnTerrainBox(m_pTransformCom, vTerrainOffset);
            }
            else if (IS_BETWEEN(iCurPatternFrame, 50, 350))
            {
                _int iFrame = iCurPatternFrame - 50;
                _float3 vPos = {
                    vTerrainPos.x + vTerrainScale.x / 2,
                    0.f,
                    vTerrainPos.z - vTerrainScale.z / 2.f + vTerrainScale.z * iFrame / 300.f
                };
                m_pTransformCom->Move_To(vPos + vTerrainOffset, fTimeDelta * 3.f, 0.01f);
                m_pTerrainBox->SetUp_OnTerrainBox(m_pTransformCom, vTerrainOffset);
            }
            else if (IS_BETWEEN(iCurPatternFrame, 350, 550))
            {
                _int iFrame = iCurPatternFrame - 350;
                _float3 vPos = {
                    vTerrainPos.x + vTerrainScale.x / 2,
                    0.f,
                    vTerrainPos.z + vTerrainScale.z / 2.f - vTerrainScale.z * iFrame / 300.f
                };
                m_pTransformCom->Move_To(vPos + vTerrainOffset, fTimeDelta * 3.f, 0.01f);
                m_pTerrainBox->SetUp_OnTerrainBox(m_pTransformCom, vTerrainOffset);
            }
            else
            {
                isLaserEnd = true;

                _float3 vPos = {
                    vTerrainPos.x + vTerrainScale.x / 2 - 4.5f,
                    0.f,
                    vTerrainPos.z
                };
                m_pTransformCom->Move_To(vPos + vTerrainOffset, fTimeDelta * 4.f, 0.01f);
                m_pTerrainBox->SetUp_OnTerrainBox(m_pTransformCom, vTerrainOffset);

                if (iCurPatternFrame == 599)
                {
                    m_isPatternPlaying = false;
                    m_ePattern = PATTERN_HAND_R::PT_IDLE;
                    m_pAnimatorPatternCom->Change_State(L"Idle");
                }
            }
            
#pragma region Laser Effect Setting

                D3DXMatrixIdentity(&matScale);
                D3DXMatrixScaling(&matScale, -0.75f, 6.f, 0.75f);

                D3DXMatrixIdentity(&matRotateChild);
                _float4x4 matRotateChild2 = {};
                D3DXMatrixIdentity(&matRotateChild2);
                D3DXMatrixRotationX(&matRotateChild, D3DXToRadian(-90)); // 안되면 -90도도 해보기
                D3DXMatrixRotationY(&matRotateChild2, D3DXToRadian(90)); // 안되면 -90도도 해보기

                D3DXMatrixIdentity(&matTransAddition);
                D3DXMatrixTranslation(&matTransAddition, -13.5f, 0, 0);

                matMonsterWorld = matTransToOrigin * matScale * matRotateChild * matRotateChild2 * matRotateChildtoPlayer * matTransReturn * matTransAddition;
#pragma endregion
            // Effect
            if (iCurPatternFrame == 81)
            {
                CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::MONSTER_EFFECT, L"Prototype_Component_Boss_Erma_BigGolem_LaserStart",
                    *m_pTransformCom->Get_WorldMatrix(), matMonsterWorld, m_pTransformCom);

            }
            else if (IS_BETWEEN(iCurPatternFrame, 100, 550) &&
                (iCurPatternFrame - 1) % 9 == 0)
            {
                CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::MONSTER_EFFECT, L"Prototype_Component_Boss_Erma_BigGolem_LaserProgress",
                    *m_pTransformCom->Get_WorldMatrix(), matMonsterWorld, m_pTransformCom);
            }
            else if (iCurPatternFrame == 550)
            {
                CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::MONSTER_EFFECT, L"Prototype_Component_Boss_Erma_BigGolem_LaserEnd",
                    *m_pTransformCom->Get_WorldMatrix(), matMonsterWorld, m_pTransformCom);
            }
#pragma region Effect Reset

                D3DXMatrixIdentity(&matScale);
                D3DXMatrixScaling(&matScale, -1.f, 1.f, 1.f);

                D3DXMatrixIdentity(&matRotateChild);
                D3DXMatrixRotationX(&matRotateChild, D3DXToRadian(-90)); // 안되면 -90도도 해보기

                D3DXMatrixIdentity(&matTransAddition);

                matMonsterWorld = matTransToOrigin * matScale * matRotateChild * matRotateChildtoPlayer * matTransReturn * matTransAddition;

#pragma endregion

            if (m_pAnimatorCom->Get_CurStateTag() == L"LaserStart")
                m_pAnimatorCom->Change_State(L"LaserProgress_Start");
            else if (m_pAnimatorCom->Get_CurStateTag() == L"LaserProgress_Start")
                m_pAnimatorCom->Change_State(L"LaserProgress_Cycle");
            else if (m_pAnimatorCom->Get_CurStateTag() == L"LaserProgress_Cycle" &&
                isLaserEnd)
                m_pAnimatorCom->Change_State(L"Laser_End");
            else if (m_pAnimatorCom->Get_CurStateTag() == L"Laser_End")
                m_pAnimatorCom->Change_State(L"Idle");

#pragma endregion
        }
            break;
        case Client::CErma_Hand_R::PATTERN_HAND_R::PT_KEYPATTERN:
        {
#pragma region PT_KEYPATTERN

            _float3 vMovePos = {
                vTerrainPos.x + 7.f,
                0,
                vTerrainPos.z
            };

            if (m_isTracking)
                m_pTransformCom->Move_To(vMovePos, fTimeDelta * 4.f, 0.01f);

            if (IS_BETWEEN(iCurPatternFrame, 0, 40))
            {
                _int iFrame = iCurPatternFrame - (0);
                m_isTracking = true;
                _float fY = -0.00125 * pow((iFrame - 40), 2) + 2;
                m_pTerrainBox->SetUp_OnTerrainBox(m_pTransformCom, _float3(0.0f, fY, 0.0f) + vTerrainOffset);
            }
            else if (IS_BETWEEN(iCurPatternFrame, 40, 100))
            {
                _int iFrame = iCurPatternFrame - (40);
                // tracking..
                m_pTerrainBox->SetUp_OnTerrainBox(m_pTransformCom, _float3(0.0f, 2.f, 0.0f) + vTerrainOffset);
            }
            else if (IS_BETWEEN(iCurPatternFrame, 100, 110))
            {
                _int iFrame = iCurPatternFrame - (100);
                m_isTracking = false;
                _float fY = -0.02 * pow((iFrame), 2) + 2;
                m_pTerrainBox->SetUp_OnTerrainBox(m_pTransformCom, _float3(0.0f, fY, 0.0f) + vTerrainOffset);
            }
            else if (IS_BETWEEN(iCurPatternFrame, 110, 160))
            {
                _int iFrame = iCurPatternFrame - (110);
                m_pTerrainBox->SetUp_OnTerrainBox(m_pTransformCom, vTerrainOffset);

                if (iCurPatternFrame == 110)
                {
#pragma region Effect Setting

                    D3DXMatrixIdentity(&matScale);
                    D3DXMatrixScaling(&matScale, -1.5f, 1.f, 1.f);

                    D3DXMatrixIdentity(&matRotateChild);
                    D3DXMatrixRotationX(&matRotateChild, D3DXToRadian(90)); // 안되면 -90도도 해보기

                    D3DXMatrixIdentity(&matTransAddition);
                    D3DXMatrixTranslation(&matTransAddition, 0, -m_pTransformCom->Get_Scaled().y / 2, 0);

                    matMonsterWorld = matTransToOrigin * matScale * matRotateChild * matRotateChildtoPlayer * matTransReturn * matTransAddition;

#pragma endregion
                    m_pGameInstance->StopSound(ENUM_CLASS(CHANNELID::SOUND_MONSTER_EFFECT));
                    m_pGameInstance->PlaySoundW(L"GolemHandStomp.wav", ENUM_CLASS(CHANNELID::SOUND_MONSTER_EFFECT), g_fEFFECTVolume - 0.8f);
                    CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::MONSTER_EFFECT, L"Prototype_Component_Boss_Erma_BigGolem_Stmap_FX",
                        *m_pTransformCom->Get_WorldMatrix(), matMonsterWorld);
#pragma region Effect Reset

                    D3DXMatrixIdentity(&matScale);
                    D3DXMatrixScaling(&matScale, -1.f, 1.f, 1.f);

                    D3DXMatrixIdentity(&matRotateChild);
                    D3DXMatrixRotationX(&matRotateChild, D3DXToRadian(-90)); // 안되면 -90도도 해보기

                    D3DXMatrixIdentity(&matTransAddition);

                    matMonsterWorld = matTransToOrigin * matScale * matRotateChild * matRotateChildtoPlayer * matTransReturn * matTransAddition;

#pragma endregion
                    dynamic_cast<CCamera_Follow*>(m_pGameInstance->Get_GameObject(m_pGameInstance->Get_CurrentLevel(), TEXT("Layer_Camera")))->Start_Shake(2.5f, 0.5f);
                    dynamic_cast<CErma*>(CRoom_Manager::GetInstance()->Find_CurrentRoom_Monster(MONSTER_TYPE::ERMA))->ChangeKeyInputPattern();

                    m_isPatternPlaying = false;
                }
            }

#pragma endregion
        }
            break;
        default:
            break;
        }

    }
    else
    {
        _float3 vDefaultPos = {
            vTerrainPos.x + 10.f,
            0.f,
            vTerrainPos.z + vTerrainScale.z / 2 - 6.f
        };
        m_pTransformCom->Move_To(vDefaultPos + vTerrainPos + vTerrainOffset, fTimeDelta * 10.f, 0.01f);
    }











    // 이걸 이용해서 Y축 값 바꿔야함
    //m_pTerrainBox->SetUp_OnTerrainBox(m_pTransformCom, _float3(0.05f, 0.5f, 0.05f));





    if (m_pTerrainBox != nullptr &&
        !m_isPatternPlaying) {
        m_pTerrainBox->SetUp_OnTerrainBox(m_pTransformCom, vTerrainOffset);
    }
}

void CErma_Hand_R::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
}

HRESULT CErma_Hand_R::Render()
{
    if (!m_pTransformCom)
        return S_OK;

    m_pVIBufferCom->ChangeUV_FlipX(true);

    SetUp_RenderState();

    m_pTransformCom->Bind_Matrix();

    if (!m_isAllStop)
    {
        m_pAnimatorCom->Update_State(); // Bind_Texture
        m_pAnimatorPatternCom->Update_State(); // Bind_Texture
    }
    else
    {
        _uint iImageMaxIndex = m_pAnimatorCom->Get_CurState()->pTextureCom->Get_NumTextures();
        _uint iImageCurIndex = m_pAnimatorCom->Get_CurStackedFrame() / m_pAnimatorCom->Get_CurState()->iFramePerImage;

        iImageCurIndex %= iImageMaxIndex;
        m_pAnimatorCom->Get_CurState()->pTextureCom->Bind_Texture();
    }

    m_pVIBufferCom->Bind_Buffers();


    m_pVIBufferCom->Render();

    m_pTerrainBox->Render();

    m_pVIBufferCom->ResetUV_FlipX();

    Reset_RenderState();

    return S_OK;
}

HRESULT CErma_Hand_R::Ready_Components(void* pArg)
{
    MONSTERDESC* desc = static_cast<MONSTERDESC*>(pArg);

    // 컴포넌트 추가
    /* For.Com_Texture */
    // Normal (1)
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Boss_Erma_BigGolem_Hand_Normal"),
        TEXT("Com_Texture_Normal"), reinterpret_cast<CComponent**>(&m_pTextureCom_Normal))))
        return E_FAIL;
    // LaserStart (5)
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Boss_Erma_BigGolem_Hand_LaserStart"),
        TEXT("Com_Texture_LaserStart"), reinterpret_cast<CComponent**>(&m_pTextureCom_LaserStart))))
        return E_FAIL;
    // LaserProgress_Start (5)
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Boss_Erma_BigGolem_Hand_LaserProgress_Start"),
        TEXT("Com_Texture_LaserProgress_Start"), reinterpret_cast<CComponent**>(&m_pTextureCom_LaserProgress_Start))))
        return E_FAIL;
    // LaserProgress_Cycle (4)
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Boss_Erma_BigGolem_Hand_LaserProgress_Cycle"),
        TEXT("Com_Texture_LaserProgress_Cycle"), reinterpret_cast<CComponent**>(&m_pTextureCom_LaserProgress_Cycle))))
        return E_FAIL;
    // Laser_End (4)
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Boss_Erma_BigGolem_Hand_LaserEnd"),
        TEXT("Com_Texture_Laser_End"), reinterpret_cast<CComponent**>(&m_pTextureCom_Laser_End))))
        return E_FAIL;




    /* For Com_Animator */
    CAnimator::ANIMSTATE_DESC StartAnimStateDesc{};
    StartAnimStateDesc.strTimerTag = L"Animator_Boss_Arma_Hand_R";   // 해당 애니메이터가 타이머에서 사용할 태그 key값
    StartAnimStateDesc.pParentTransform = nullptr;
    StartAnimStateDesc.pChildTransform = nullptr;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Animator"),
        TEXT("Com_Animator"), reinterpret_cast<CComponent**>(&m_pAnimatorCom), &StartAnimStateDesc)))
        return E_FAIL;


    // State 삽입
    m_pAnimatorCom->Add_State(L"Idle",                  { m_pTextureCom_Normal, 4, true });
    m_pAnimatorCom->Add_State(L"LaserStart",            { m_pTextureCom_LaserStart, 16, false });
    m_pAnimatorCom->Add_State(L"LaserProgress_Start",   { m_pTextureCom_LaserProgress_Start, 4, false });
    m_pAnimatorCom->Add_State(L"LaserProgress_Cycle",   { m_pTextureCom_LaserProgress_Cycle, 4, true });
    m_pAnimatorCom->Add_State(L"Laser_End",             { m_pTextureCom_Laser_End, 8, false });


    CAnimator::ANIMSTATE_DESC StartAnimStateDesc2{};
    StartAnimStateDesc2.strTimerTag = L"Animator_Boss_Arma_Hand_R_Pattern";   // 해당 애니메이터가 타이머에서 사용할 태그 key값
    StartAnimStateDesc2.pParentTransform = nullptr;
    StartAnimStateDesc2.pChildTransform = nullptr;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Animator"),
        TEXT("Com_AnimatorPattern"), reinterpret_cast<CComponent**>(&m_pAnimatorPatternCom), &StartAnimStateDesc)))
        return E_FAIL;

    m_pAnimatorPatternCom->Add_State(L"Idle",       { nullptr, 4, true });
    m_pAnimatorPatternCom->Add_State(L"Strike",     { nullptr, 480, true });    // 8s
    m_pAnimatorPatternCom->Add_State(L"Laser",      { nullptr, 600, true });    // 10s
    m_pAnimatorPatternCom->Add_State(L"KeyPattern", { nullptr, 180, true });    // 3s

    // ..



    return S_OK;
}

HRESULT CErma_Hand_R::Ready_Object()
{
    m_pHpBar = dynamic_cast<CField_Hp*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_UI_Field_Hp")));

    return S_OK;
}

void CErma_Hand_R::OnCollision(CGameObject* pGameObject)
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

void CErma_Hand_R::PlayPattern(PATTERN_HAND_R ePattern, _bool isForced)
{
    if (m_isPatternPlaying == true && !isForced)
        return;



    _float fPatternTime = 0.f;
    _wstring strPatternTag = {};
    m_isPatternPlaying = true;

    switch (ePattern)
    {
    case Client::CErma_Hand_R::PATTERN_HAND_R::PT_IDLE:
        m_pAnimatorCom->Change_State(L"Idle", true, 0.f, true);
        m_isPatternPlaying = false;
        break;
    case Client::CErma_Hand_R::PATTERN_HAND_R::PT_STRIKE:  
        fPatternTime = 8.f;
        strPatternTag = L"Strike";  // 이거 확인하고 패턴마다 바뀌도록,
        //그리고 이게 패턴 switch 문 내에서 현재 프레임 구분의 조건이 되도록만들어야 함
        break;
    case Client::CErma_Hand_R::PATTERN_HAND_R::PT_LASER:    
        fPatternTime = 10.f;
        strPatternTag = L"Laser";
        break;
    case Client::CErma_Hand_R::PATTERN_HAND_R::PT_KEYPATTERN:
        fPatternTime = 3.f;
        strPatternTag = L"KeyPattern";
        break;
        break;
    default:
        m_isPatternPlaying = false;
        break;
    }



    m_ePattern = ePattern;
    if (!strPatternTag.empty())
        m_pAnimatorPatternCom->Change_State(strPatternTag, true, fPatternTime, true);
}

CErma_Hand_R* CErma_Hand_R::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CErma_Hand_R* pInstance = new CErma_Hand_R(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Create : CErma_Hand_R"));
        Safe_Release(pInstance);
    }

    return pInstance;
}


CGameObject* CErma_Hand_R::Clone(void* pArg)
{
    CErma_Hand_R* pInstance = new CErma_Hand_R(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Clone : CErma_Hand_R"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CErma_Hand_R::Free()
{
    __super::Free();

    Safe_Release(m_pTextureCom_Normal);
    Safe_Release(m_pTextureCom_LaserStart);
    Safe_Release(m_pTextureCom_LaserProgress_Start);
    Safe_Release(m_pTextureCom_LaserProgress_Cycle);
    Safe_Release(m_pTextureCom_Laser_End);


    Safe_Release(m_pAnimatorCom);
    Safe_Release(m_pAnimatorPatternCom);
}
#include "Erma_Body.h"
#include "Effect_Factory.h"

CErma_Body::CErma_Body(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CMonster{ pGraphic_Device }
{
}

CErma_Body::CErma_Body(const CErma_Body& Prototype)
    : CMonster{ Prototype }
{
}

HRESULT CErma_Body::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CErma_Body::Initialize(void* pArg)
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
        fTerrainPos.z + fTerrainScale.z / 2 - 1.8f));

    // 크기 조정
    m_pTransformCom->Scaling(9.f, 4.5f, 4.5f);


    // ksta : 테스트중.. 패턴 완성 후 삭제
    //PlayPattern(PATTERN_BODY::PT_MISSILE_R);






    m_isSummoned = true;
    //Ready_Object();

    // 임시
    m_iMaxHp = 5000;
    m_iCulHp = 5000;

    m_eMonsterType = MONSTER_TYPE::ERMA_BODY;

    return S_OK;
}

void CErma_Body::Priority_Update(_float fTimeDelta)
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



    //if (m_pHpBar != nullptr &&
    //    m_isSummoned)
    //    m_pHpBar->Render_HP_Progress(m_pTransformCom, m_iCulHp, m_iMaxHp);

    //if (m_iCulHp <= 0)
    //    m_bDead = true;
}

void CErma_Body::Update(_float fTimeDelta)
{
    if (m_isAllStop)
        return;
    // Update
    // 


    // Idle
    // Missile  // 5s (300)


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

    if (m_isPatternPlaying)
    {
        switch (m_ePattern)
        {
        case Client::CErma_Body::PATTERN_BODY::PT_IDLE:
        {

        }
            break;
        case Client::CErma_Body::PATTERN_BODY::PT_MISSILE_L:
        {
#pragma region PT_MISSILE_L Pattern
            // 0~300
            // 5프레임 간격으로 미사일 조준 및 발사?
            
            // 이펙트 두 개 생성
            // 하나는 맵 구석에서 발사하는 미사일 이펙트
            // 하나는 조준점 이펙트
            
            if (iCurPatternFrame == 9)
            {
                m_pGameInstance->StopSound(ENUM_CLASS(CHANNELID::SOUND_MONSTER_LONG_EFFECT3));
                m_pGameInstance->PlaySoundW(L"GolemMissileStart.wav", ENUM_CLASS(CHANNELID::SOUND_MONSTER_LONG_EFFECT3), g_fEFFECTVolume - 0.6f);
            }

            if (IS_BETWEEN(iCurPatternFrame, 10, 60))
            {
                if (iCurPatternFrame % 5 == 0)
                {
                    _float fRandX = m_pGameInstance->Compute_Random(-1.f, 1.f);
                    _float fRandZ = m_pGameInstance->Compute_Random(-1.f, 1.f);

                    CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::NORMAL_EFFECT, L"Prototype_Component_Boss_Erma_Missile_Upper",
                        { vTerrainPos.x - vTerrainScale.x / 2 + fRandX,
                        vTerrainPos.y + 3,
                        vTerrainPos.z - vTerrainScale.z / 2 + fRandZ },
                        { 0, 0, 0, 1 },
                        { 2.f, 12.46f, 2.f });
                    CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::NORMAL_EFFECT, L"Prototype_Component_Boss_Erma_Missile_Upper_Light",
                        { vTerrainPos.x - vTerrainScale.x / 2 + fRandX,
                        vTerrainPos.y + 3,
                        vTerrainPos.z - vTerrainScale.z / 2 + fRandZ },
                        { 0, 0, 0, 1 },
                        { 2.f, 12.46f, 2.f });

                }
            }

            if (IS_BETWEEN(iCurPatternFrame, 30, 80))
            {
                if (iCurPatternFrame % 5 == 0)
                {
                    _int iCount = (iCurPatternFrame - 30) / 5;  // 0 ~ 9

                    D3DXQUATERNION qRot = { 0 ,0, 0, 1 };
                    _float3 vAxis = { 1, 0, 0 };
                    D3DXQuaternionRotationAxis(&qRot, &vAxis, D3DXToRadian(-90));

                    CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::NORMAL_EFFECT, L"Prototype_Component_Boss_Erma_Missile_Alert",
                        { vTerrainPos.x - 5 + iCount * 1.1f,
                        vTerrainPos.y + 1.01f,
                        vTerrainPos.z + vTerrainScale.z / 2 - 2.8f * (1 + iCount)},
                        qRot,
                        { 4.2f, 2.8f, 2.8f });
                }
            }

            if (IS_BETWEEN(iCurPatternFrame, 80, 130))
            {
                if (iCurPatternFrame % 5 == 0)
                {
                    m_pGameInstance->StopSound(ENUM_CLASS(CHANNELID::SOUND_MONSTER_LONG_EFFECT3));
                    m_pGameInstance->PlaySoundW(L"GolemMissile.wav", ENUM_CLASS(CHANNELID::SOUND_MONSTER_LONG_EFFECT3), g_fEFFECTVolume - 0.6f);
                    _int iCount = (iCurPatternFrame - 80) / 5;  // 0 ~ 9

                    CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::MONSTER_EFFECT, L"Prototype_Component_Boss_Erma_Missile_Lower",
                        { vTerrainPos.x - 5 + iCount * 1.1f,
                        vTerrainPos.y + 5.f,
                        vTerrainPos.z + vTerrainScale.z / 2 - 2.8f * (1 + iCount) },
                        {0, 0, 0, 1},
                        { 5.f, 10.f, 5.f });
                    CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::MONSTER_EFFECT, L"Prototype_Component_Boss_Erma_Missile_Lower_Light",
                        { vTerrainPos.x - 5 + iCount * 1.1f,
                        vTerrainPos.y + 5.f,
                        vTerrainPos.z + vTerrainScale.z / 2 - 2.8f * (1 + iCount) },
                        { 0, 0, 0, 1 },
                        { 5.f, 10.f, 5.f });
                }
            }

            if (iCurPatternFrame == 299)
            {
                m_isPatternPlaying = false;
                m_ePattern = PATTERN_BODY::PT_IDLE;
                m_pAnimatorPatternCom->Change_State(L"Idle");
            }

#pragma endregion
        }
            break;
        case Client::CErma_Body::PATTERN_BODY::PT_MISSILE_R:
        {
#pragma region PT_MISSILE_R Pattern
            // 0~300
            // 5프레임 간격으로 미사일 조준 및 발사?

            // 이펙트 두 개 생성
            // 하나는 맵 구석에서 발사하는 미사일 이펙트
            // 하나는 조준점 이펙트

            if (IS_BETWEEN(iCurPatternFrame, 10, 60))
            {
                if (iCurPatternFrame % 5 == 0)
                {
                    _float fRandX = m_pGameInstance->Compute_Random(-1.f, 1.f);
                    _float fRandZ = m_pGameInstance->Compute_Random(-1.f, 1.f);

                    CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::NORMAL_EFFECT, L"Prototype_Component_Boss_Erma_Missile_Upper",
                        { vTerrainPos.x + vTerrainScale.x / 2 + fRandX,
                        vTerrainPos.y + 3,
                        vTerrainPos.z - vTerrainScale.z / 2 + fRandZ },
                        { 0, 0, 0, 1 },
                        { 2.f, 12.46f, 2.f });
                    CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::NORMAL_EFFECT, L"Prototype_Component_Boss_Erma_Missile_Upper_Light",
                        { vTerrainPos.x + vTerrainScale.x / 2 + fRandX,
                        vTerrainPos.y + 3,
                        vTerrainPos.z - vTerrainScale.z / 2 + fRandZ },
                        { 0, 0, 0, 1 },
                        { 2.f, 12.46f, 2.f });

                }
            }

            if (IS_BETWEEN(iCurPatternFrame, 30, 80))
            {
                if (iCurPatternFrame % 5 == 0)
                {
                    _int iCount = (iCurPatternFrame - 30) / 5;  // 0 ~ 9

                    D3DXQUATERNION qRot = { 0 ,0, 0, 1 };
                    _float3 vAxis = { 1, 0, 0 };
                    D3DXQuaternionRotationAxis(&qRot, &vAxis, D3DXToRadian(-90));

                    CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::NORMAL_EFFECT, L"Prototype_Component_Boss_Erma_Missile_Alert",
                        { vTerrainPos.x + 5 - iCount * 1.1f,
                        vTerrainPos.y + 1.01f,
                        vTerrainPos.z + vTerrainScale.z / 2 - 2.8f * (1 + iCount) },
                        qRot,
                        { 4.2f, 2.8f, 2.8f });
                }
            }

            if (IS_BETWEEN(iCurPatternFrame, 80, 130))
            {
                if (iCurPatternFrame % 5 == 0)
                {
                    _int iCount = (iCurPatternFrame - 80) / 5;  // 0 ~ 9

                    CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::MONSTER_EFFECT, L"Prototype_Component_Boss_Erma_Missile_Lower",
                        { vTerrainPos.x + 5 - iCount * 1.1f,
                        vTerrainPos.y + 5.f,
                        vTerrainPos.z + vTerrainScale.z / 2 - 2.8f * (1 + iCount) },
                        { 0, 0, 0, 1 },
                        { 5.f, 10.f, 5.f });
                    CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::MONSTER_EFFECT, L"Prototype_Component_Boss_Erma_Missile_Lower_Light",
                        { vTerrainPos.x + 5 - iCount * 1.1f,
                        vTerrainPos.y + 5.f,
                        vTerrainPos.z + vTerrainScale.z / 2 - 2.8f * (1 + iCount) },
                        { 0, 0, 0, 1 },
                        { 5.f, 10.f, 5.f });
                }
            }

            if (iCurPatternFrame == 299)
            {
                m_isPatternPlaying = false;
                m_ePattern = PATTERN_BODY::PT_IDLE;
                m_pAnimatorPatternCom->Change_State(L"Idle");
            }

#pragma endregion
        }
            break;
        default:
            break;
        }
    }




    if (m_pTerrainBox != nullptr) {
        m_pTerrainBox->SetUp_OnTerrainBox(m_pTransformCom, _float3(0.05f, 0.7f, 0.05f));
    }
}

void CErma_Body::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
}

HRESULT CErma_Body::Render()
{
    if (!m_pTransformCom)
        return S_OK;

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


    Reset_RenderState();

    return S_OK;
}

HRESULT CErma_Body::Ready_Components(void* pArg)
{
    MONSTERDESC* desc = static_cast<MONSTERDESC*>(pArg);

    // 컴포넌트 추가
    /* For.Com_Texture */
    // Body_Broken
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Boss_Erma_BigGolem_Body_Broken"),
        TEXT("Com_Texture_Body_Broken"), reinterpret_cast<CComponent**>(&m_pTextureCom_Body_Broken))))
        return E_FAIL;
    // Body
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Boss_Erma_BigGolem_Body"),
        TEXT("Com_Texture_Body"), reinterpret_cast<CComponent**>(&m_pTextureCom_Body))))
        return E_FAIL;


    /* For Com_Animator */
    CAnimator::ANIMSTATE_DESC StartAnimStateDesc{};
    StartAnimStateDesc.strTimerTag = L"Animator_Boss_Arma_Body";   // 해당 애니메이터가 타이머에서 사용할 태그 key값
    StartAnimStateDesc.pParentTransform = nullptr;
    StartAnimStateDesc.pChildTransform = nullptr;
    
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Animator"),
        TEXT("Com_Animator"), reinterpret_cast<CComponent**>(&m_pAnimatorCom), &StartAnimStateDesc)))
        return E_FAIL;

    // State 삽입
    m_pAnimatorCom->Add_State(L"Idle",      { m_pTextureCom_Body, 4, true });
    m_pAnimatorCom->Add_State(L"Broken",    { m_pTextureCom_Body_Broken, 4, true });


    CAnimator::ANIMSTATE_DESC StartAnimStateDesc2{};
    StartAnimStateDesc2.strTimerTag = L"Animator_Boss_Arma_Body_Pattern";   // 해당 애니메이터가 타이머에서 사용할 태그 key값
    StartAnimStateDesc2.pParentTransform = nullptr;
    StartAnimStateDesc2.pChildTransform = nullptr;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Animator"),
        TEXT("Com_AnimatorPattern"), reinterpret_cast<CComponent**>(&m_pAnimatorPatternCom), &StartAnimStateDesc)))
        return E_FAIL;

    m_pAnimatorPatternCom->Add_State(L"Idle",       { nullptr, 4, true });
    m_pAnimatorPatternCom->Add_State(L"Missile",    { nullptr, 300, true });    // 5s

    // ..




    return S_OK;
}

HRESULT CErma_Body::Ready_Object()
{
    m_pHpBar = dynamic_cast<CField_Hp*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_UI_Field_Hp")));

    return S_OK;
}

void CErma_Body::OnCollision(CGameObject* pGameObject)
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

void CErma_Body::PlayPattern(PATTERN_BODY ePattern, _bool isForced)
{
    if (m_isPatternPlaying == true && !isForced)
        return;


    _float fPatternTime = 0.f;
    _wstring strPatternTag = {};
    m_isPatternPlaying = true;

    switch (ePattern)
    {
    case Client::CErma_Body::PATTERN_BODY::PT_MISSILE_L:
        fPatternTime = 5.f;
        strPatternTag = L"Missile";
        break;
    case Client::CErma_Body::PATTERN_BODY::PT_MISSILE_R:
        fPatternTime = 5.f;
        strPatternTag = L"Missile";
        break;
    case Client::CErma_Body::PATTERN_BODY::PT_BROKEN:
        m_isPatternPlaying = false;
        m_pAnimatorCom->Change_State(L"Broken");
        strPatternTag = L"Idle";
        break;
    case Client::CErma_Body::PATTERN_BODY::PT_IDLE:
        m_isPatternPlaying = false;
        m_pAnimatorCom->Change_State(L"Idle");
        break;
    default:
        break;
    }



    m_ePattern = ePattern;
    if (!strPatternTag.empty())
        m_pAnimatorPatternCom->Change_State(strPatternTag, true, fPatternTime, true);
}

CErma_Body* CErma_Body::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CErma_Body* pInstance = new CErma_Body(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Create : CErma_Body"));
        Safe_Release(pInstance);
    }

    return pInstance;
}


CGameObject* CErma_Body::Clone(void* pArg)
{
    CErma_Body* pInstance = new CErma_Body(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Clone : CErma_Body"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CErma_Body::Free()
{
    m_pGameInstance->Remove_Collider_ByOwner(this);
    __super::Free();

    Safe_Release(m_pTextureCom_Body);
    Safe_Release(m_pTextureCom_Body_Broken);


    Safe_Release(m_pAnimatorCom);
    Safe_Release(m_pAnimatorPatternCom);
}
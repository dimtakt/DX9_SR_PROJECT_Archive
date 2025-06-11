#include "Erma_Head.h"
#include "Effect_Factory.h"

CErma_Head::CErma_Head(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CMonster{ pGraphic_Device }
{
}

CErma_Head::CErma_Head(const CErma_Head& Prototype)
    : CMonster{ Prototype }
{
}

HRESULT CErma_Head::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CErma_Head::Initialize(void* pArg)
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
        fTerrainPos.z + fTerrainScale.z / 2 - 2.f));

    // 크기 조정
    m_pTransformCom->Scaling(7.5f, 7.5f, 7.5f);



    // ksta : 테스트중.. 패턴 완성 후 삭제
    //PlayPattern(PATTERN_HEAD::PT_BULLETFIRE);


    m_isSummoned = true;
    //Ready_Object();

    // 임시
    m_iMaxHp = 999999;
    m_iCulHp = 999999;
    m_eMonsterType = MONSTER_TYPE::ERMA_HEAD;
    return S_OK;
}

void CErma_Head::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
    //if (m_pHpBar != nullptr &&
    //    m_isSummoned)
    //    m_pHpBar->Render_HP_Progress(m_pTransformCom, m_iCulHp, m_iMaxHp);

    //if (m_iCulHp <= 0)
    //    m_bDead = true;
}

void CErma_Head::Update(_float fTimeDelta)
{
    if (m_isAllStop)
        return;
    // Update
    // 


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
    D3DXMatrixScaling(&matScale, -0.1f, 0.1f, 0.1f);

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
        case Client::CErma_Head::PATTERN_HEAD::PT_IDLE:
        {

        }
            break;
        case Client::CErma_Head::PATTERN_HEAD::PT_BULLETFIRE:
        {
#pragma region PT_BULLETFIRE Pattern
            _bool isBulletFireEnd = false;

            D3DXMatrixIdentity(&matTransAddition);
            D3DXMatrixTranslation(&matTransAddition, 0.f, -1.0f, 0.f);
            matMonsterWorld = matTransToOrigin * matScale * matRotateChild * matRotateChildtoPlayer * matTransReturn * matTransAddition;

            // 360frame (6s)
            if (iCurPatternFrame == 1)
                m_pAnimatorCom->Change_State(L"AttackStart");


            if (m_pAnimatorCom->Get_CurStateTag() == L"AttackProgress")
            {   
                _float fLifeTime = 2.8f;

                // 한번에 발사 시 렉이 크게 걸려서 프레임에 걸쳐 생성하도록
                if (IS_BETWEEN(iCurPatternFrame % 50, 0, 15))
                {
                    for (int i = 0; i < 5; i++)
                    {
                        _float fAngle = 150;    // 방사 각도 수정용

                        _float fAngleRand = m_pGameInstance->Compute_Random(-fAngle / 2.f, fAngle / 2.f);
                        _float fRad = D3DXToRadian(fAngleRand);

                        _float3 vThrownDir = {
                            sinf(fRad),
                            0.f,
                            -cosf(fRad)
                        };

                        _float fThrownPower = m_pGameInstance->Compute_Random(7.f, 12.f);
                        _float fThrownAtkLifeTime = fLifeTime;

                        CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::MONSTER_EFFECT, L"Prototype_Component_Boss_Erma_Bullet",
                            *m_pTransformCom->Get_WorldMatrix(), matMonsterWorld, vThrownDir, fThrownPower, fThrownAtkLifeTime, 0.f, true);
                    
                        //_float3 vStartPos = m_pTransformCom->Get_State(STATE::POSITION);
                        //_float3 vFinalPos = vStartPos + vThrownDir * fThrownPower * fThrownAtkLifeTime;

                        //m_effectVecList.push_back(vFinalPos);
                    }
                }

                if (iCurPatternFrame == 330)
                    isBulletFireEnd = true;
            }

            if (iCurPatternFrame == 359)
            {
                m_isPatternPlaying = false;
                m_ePattern = PATTERN_HEAD::PT_IDLE;
                m_pAnimatorPatternCom->Change_State(L"Idle");
            }



            if (m_pAnimatorCom->Get_CurStateTag() == L"AttackStart")
                m_pAnimatorCom->Change_State(L"AttackProgress");
            else if (m_pAnimatorCom->Get_CurStateTag() == L"AttackProgress" &&
                isBulletFireEnd)
                m_pAnimatorCom->Change_State(L"AttackEnd");
            else if (m_pAnimatorCom->Get_CurStateTag() == L"AttackEnd")
                m_pAnimatorCom->Change_State(L"Idle");
    
            D3DXMatrixIdentity(&matTransAddition);
            matMonsterWorld = matTransToOrigin * matScale * matRotateChild * matRotateChildtoPlayer * matTransReturn * matTransAddition;
#pragma endregion
        }
            break;
        default:
            break;
        }
    }











    if (m_pTerrainBox != nullptr) {
        m_pTerrainBox->SetUp_OnTerrainBox(m_pTransformCom, _float3(0.0f, 2.f, 0.0f));
    }
}

void CErma_Head::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
}

HRESULT CErma_Head::Render()
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

HRESULT CErma_Head::Ready_Components(void* pArg)
{
    MONSTERDESC* desc = static_cast<MONSTERDESC*>(pArg);

    // 컴포넌트 추가
    /* For.Com_Texture */
    // Standby
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Boss_Erma_BigGolem_Head_Standby"),
        TEXT("Com_Texture_Standby"), reinterpret_cast<CComponent**>(&m_pTextureCom_Standby))))
        return E_FAIL;
    // Normal (1)
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Boss_Erma_BigGolem_Head_Normal"),
        TEXT("Com_Texture_Normal"), reinterpret_cast<CComponent**>(&m_pTextureCom_Normal))))
        return E_FAIL;
    // AttackStart (9)
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Boss_Erma_BigGolem_Head_AttackStart"),
        TEXT("Com_Texture_AttackStart"), reinterpret_cast<CComponent**>(&m_pTextureCom_AttackStart))))
        return E_FAIL;
    // AttackProgress (1)
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Boss_Erma_BigGolem_Head_AttackProgress"),
        TEXT("Com_Texture_AttackProgress"), reinterpret_cast<CComponent**>(&m_pTextureCom_AttackProgress))))
        return E_FAIL;
    // AttackEnd (5)
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Boss_Erma_BigGolem_Head_AttackEnd"),
        TEXT("Com_Texture_AttackEnd"), reinterpret_cast<CComponent**>(&m_pTextureCom_AttackEnd))))
        return E_FAIL;
    // DramaticDying (1)
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Boss_Erma_BigGolem_Head_DramaticDying"),
        TEXT("Com_Texture_DramaticDying"), reinterpret_cast<CComponent**>(&m_pTextureCom_DramaticDying))))
        return E_FAIL;
    // Broken (1)
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Boss_Erma_BigGolem_Head_Broken"),
        TEXT("Com_Texture_Broken"), reinterpret_cast<CComponent**>(&m_pTextureCom_Broken))))
        return E_FAIL;


    /* For Com_Animator */
    CAnimator::ANIMSTATE_DESC StartAnimStateDesc{};
    StartAnimStateDesc.strTimerTag = L"Animator_Boss_Erma_Head";   // 해당 애니메이터가 타이머에서 사용할 태그 key값
    StartAnimStateDesc.pParentTransform = nullptr;
    StartAnimStateDesc.pChildTransform = nullptr;
    
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Animator"),
        TEXT("Com_Animator"), reinterpret_cast<CComponent**>(&m_pAnimatorCom), &StartAnimStateDesc)))
        return E_FAIL;
    // State 삽입
    m_pAnimatorCom->Add_State(L"Standby",           { m_pTextureCom_Standby, 4, true });
    m_pAnimatorCom->Add_State(L"Idle",              { m_pTextureCom_Normal, 4, true });
    m_pAnimatorCom->Add_State(L"AttackStart",       { m_pTextureCom_AttackStart, 4, true });
    m_pAnimatorCom->Add_State(L"AttackProgress",    { m_pTextureCom_AttackProgress, 4, true });
    m_pAnimatorCom->Add_State(L"AttackEnd",         { m_pTextureCom_AttackEnd, 4, true });
    m_pAnimatorCom->Add_State(L"DramaticDying",     { m_pTextureCom_DramaticDying, 4, true });
    m_pAnimatorCom->Add_State(L"Broken",            { m_pTextureCom_Broken, 4, true });

    CAnimator::ANIMSTATE_DESC StartAnimStateDesc2{};
    StartAnimStateDesc2.strTimerTag = L"Animator_Boss_Arma_Head_Pattern";   // 해당 애니메이터가 타이머에서 사용할 태그 key값
    StartAnimStateDesc2.pParentTransform = nullptr;
    StartAnimStateDesc2.pChildTransform = nullptr;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Animator"),
        TEXT("Com_AnimatorPattern"), reinterpret_cast<CComponent**>(&m_pAnimatorPatternCom), &StartAnimStateDesc)))
        return E_FAIL;

    m_pAnimatorPatternCom->Add_State(L"Idle",           { nullptr, 4, true });
    m_pAnimatorPatternCom->Add_State(L"BulletFire",     { nullptr, 360, true });    // 6s

    // ..




    return S_OK;
}

HRESULT CErma_Head::Ready_Object()
{
    m_pHpBar = dynamic_cast<CField_Hp*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_UI_Field_Hp")));

    return S_OK;
}

void CErma_Head::OnCollision(CGameObject* pGameObject)
{
    __super::OnCollision(pGameObject);

    switch (pGameObject->Get_ObjType())
    {
    case GAMEOBJ_TYPE::PLAYER_EFFECT:
        break;
    }
}

void CErma_Head::PlayPattern(PATTERN_HEAD ePattern, _bool isForced)
{
    if (m_isPatternPlaying == true && !isForced)
        return;


    _float fPatternTime = 0.f;
    _wstring strPatternTag = {};
    m_isPatternPlaying = true;

    switch (ePattern)
    {
    case Client::CErma_Head::PATTERN_HEAD::PT_IDLE:
        if (m_pAnimatorCom->Get_CurStateTag() == L"Idle")
            m_pAnimatorCom->Change_State(L"Standby");
        m_isPatternPlaying = false;
        break;
    case Client::CErma_Head::PATTERN_HEAD::PT_AWAKEN:
        if (m_pAnimatorCom->Get_CurStateTag() == L"Standby")
            m_pAnimatorCom->Change_State(L"Idle");
        m_isPatternPlaying = false;
        break;
    case Client::CErma_Head::PATTERN_HEAD::PT_BULLETFIRE:
        fPatternTime = 6.f;
        strPatternTag = L"BulletFire";
        break;
    case Client::CErma_Head::PATTERN_HEAD::PT_BROKEN:
        m_isPatternPlaying = false;
        m_pAnimatorCom->Change_State(L"Broken");
    default:
        break;
    }



    m_ePattern = ePattern;
    if (!strPatternTag.empty())
        m_pAnimatorPatternCom->Change_State(strPatternTag, true, fPatternTime, true);
}

CErma_Head* CErma_Head::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CErma_Head* pInstance = new CErma_Head(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Create : CErma_Head"));
        Safe_Release(pInstance);
    }

    return pInstance;
}


CGameObject* CErma_Head::Clone(void* pArg)
{
    CErma_Head* pInstance = new CErma_Head(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Clone : CErma_Head"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CErma_Head::Free()
{
    __super::Free();

    Safe_Release(m_pTextureCom_Standby);
    Safe_Release(m_pTextureCom_Normal);
    Safe_Release(m_pTextureCom_AttackStart);
    Safe_Release(m_pTextureCom_AttackProgress);
    Safe_Release(m_pTextureCom_AttackEnd);
    Safe_Release(m_pTextureCom_DramaticDying);
    Safe_Release(m_pTextureCom_Broken);


    Safe_Release(m_pAnimatorCom);
    Safe_Release(m_pAnimatorPatternCom);
}
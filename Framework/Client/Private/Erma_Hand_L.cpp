#include "Erma_Hand_L.h"
#include "Effect_Factory.h"

CErma_Hand_L::CErma_Hand_L(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CMonster{ pGraphic_Device }
{
}

CErma_Hand_L::CErma_Hand_L(const CErma_Hand_L& Prototype)
    : CMonster{ Prototype }
{
}

HRESULT CErma_Hand_L::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CErma_Hand_L::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(this->Ready_Components(pArg)))
        return E_FAIL;

    MONSTERDESC* desc = static_cast<MONSTERDESC*>(pArg);

    CTransform* pTerrainTransform = dynamic_cast<CTransform*>(desc->pTerrainBox->Find_Component(TEXT("Com_Transform_TerrainBox")));
    m_pTerrainTransformCom = pTerrainTransform;
    _float3 vTerrainPos = pTerrainTransform->Get_State(STATE::POSITION);
    _float3 vTerrainScale = pTerrainTransform->Get_Scaled();

    // 위치 정해지는 부분. from __super
    m_pTransformCom->Set_State(STATE::POSITION, _float3(
        vTerrainPos.x - 4.f,
        0.f,
        vTerrainPos.z + vTerrainScale.z / 2 - 3.f));

    // 크기 조정
    m_pTransformCom->Scaling(3.f, 3.f, 3.f);





    // ksta : 테스트중.. 패턴 완성 후 삭제
    PlayPattern(PATTERN_HAND_L::PT_STRIKE);



    m_isSummoned = true;
    //Ready_Object();

    // 임시
    m_iMaxHp = 500;
    m_iCulHp = 500;

    return S_OK;
}

void CErma_Hand_L::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
    //if (m_pHpBar != nullptr &&
    //    m_isSummoned)
    //    m_pHpBar->Render_HP_Progress(m_pTransformCom, m_iCulHp, m_iMaxHp);

    //if (m_iCulHp <= 0)
    //    m_bDead = true;
}

void CErma_Hand_L::Update(_float fTimeDelta)
{
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

    _int iCurPatternFrame = m_pAnimatorCom->Get_CurStackedFrame();

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
    D3DXMatrixScaling(&matScale, -1.3f, 1.f, 1.f);

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
        case Client::CErma_Hand_L::PATTERN_HAND_L::PT_IDLE:

            break;
        case Client::CErma_Hand_L::PATTERN_HAND_L::PT_STRIKE:
        {   
#pragma region PT_STRIKE Pattern

            // 트래킹 상태일 때 추적 
            if (m_isTracking)
                m_pTransformCom->Move_To(vTargetPos, fTimeDelta * 2.f, 0.01f);

            if (0 <= iCurPatternFrame && iCurPatternFrame <= 40)
            {
                _int iFrame = iCurPatternFrame - (0);
                m_isTracking = true;
                _float fY = -0.00125 * pow((iFrame - 40), 2) + 2;
                m_pTerrainBox->SetUp_OnTerrainBox(m_pTransformCom, _float3(0.0f, fY, 0.0f) + vTerrainOffset);
            }
            else if (40 < iCurPatternFrame && iCurPatternFrame <= 120)
            {
                _int iFrame = iCurPatternFrame - (40);
                // tracking..
                m_pTerrainBox->SetUp_OnTerrainBox(m_pTransformCom, _float3(0.0f, 2.f, 0.0f) + vTerrainOffset);
            }
            else if (120 < iCurPatternFrame && iCurPatternFrame <= 130)
            {
                _int iFrame = iCurPatternFrame - (120);
                m_isTracking = false;
                _float fY = -0.02 * pow((iFrame), 2) + 2;
                m_pTerrainBox->SetUp_OnTerrainBox(m_pTransformCom, _float3(0.0f, fY, 0.0f) + vTerrainOffset);
            }
            else if (130 < iCurPatternFrame && iCurPatternFrame <= 180)
            {
                _int iFrame = iCurPatternFrame - (130); 
                m_pTerrainBox->SetUp_OnTerrainBox(m_pTransformCom, vTerrainOffset);
            }
            else if (0 + 180 < iCurPatternFrame && iCurPatternFrame <= 40 + 180)
            {
                _int iFrame = iCurPatternFrame - (0 + 180);
                m_isTracking = true;
                _float fY = -0.00125 * pow((iFrame - 40), 2) + 2;
                m_pTerrainBox->SetUp_OnTerrainBox(m_pTransformCom, _float3(0.0f, fY, 0.0f) + vTerrainOffset);
            }
            else if (40 + 180 < iCurPatternFrame && iCurPatternFrame <= 120 + 180)
            {
                _int iFrame = iCurPatternFrame - (40 + 180);
                // tracking..
                m_pTerrainBox->SetUp_OnTerrainBox(m_pTransformCom, _float3(0.0f, 2.f, 0.0f) + vTerrainOffset);
            }
            else if (120 + 180 < iCurPatternFrame && iCurPatternFrame <= 130 + 180)
            {
                _int iFrame = iCurPatternFrame - (120 + 180);
                m_isTracking = false;
                _float fY = -0.02 * pow((iFrame), 2) + 2;
                m_pTerrainBox->SetUp_OnTerrainBox(m_pTransformCom, _float3(0.0f, fY, 0.0f) + vTerrainOffset);
            }
            else if (130 + 180 < iCurPatternFrame && iCurPatternFrame <= 180 + 180)
            {
                _int iFrame = iCurPatternFrame - (130 + 180);
                m_pTerrainBox->SetUp_OnTerrainBox(m_pTransformCom, vTerrainOffset);
            }
            else
            {
                _float3 vPos = {
                    vTerrainPos.x - 4.f,
                    0.f,
                    vTerrainPos.z + vTerrainScale.z / 2 - 3.f
                };

                m_pTransformCom->Move_To(vPos + vTerrainPos + vTerrainOffset, fTimeDelta * 4.f, 0.01f);
                m_pTerrainBox->SetUp_OnTerrainBox(m_pTransformCom, vTerrainOffset);
                
                if (iCurPatternFrame == 449)
                    m_isPatternPlaying = false;
            }



#pragma endregion
        }
            break;
        case Client::CErma_Hand_L::PATTERN_HAND_L::PT_LASER:
            break;
        default:
            break;
        }

    }







    // 타이머용 강제 상태변경






    // 이걸 이용해서 Y축 값 바꿔야함
    //m_pTerrainBox->SetUp_OnTerrainBox(m_pTransformCom, _float3(0.05f, 0.5f, 0.05f));





    if (m_pTerrainBox != nullptr &&
        !m_isPatternPlaying) {
        m_pTerrainBox->SetUp_OnTerrainBox(m_pTransformCom, vTerrainOffset);
    }
}

void CErma_Hand_L::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
}

HRESULT CErma_Hand_L::Render()
{
    if (!m_pTransformCom)
        return S_OK;


    m_pTransformCom->Bind_Matrix();

    m_pAnimatorCom->Update_State(); // Bind_Texture

    m_pVIBufferCom->Bind_Buffers();

    SetUp_RenderState();

    m_pVIBufferCom->Render();

    m_pTerrainBox->Render();


    Reset_RenderState();

    return S_OK;
}

HRESULT CErma_Hand_L::Ready_Components(void* pArg)
{
    MONSTERDESC* desc = static_cast<MONSTERDESC*>(pArg);

    // 컴포넌트 추가
    /* For.Com_Texture */
    // Normal
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Boss_Erma_BigGolem_Hand_Normal"),
        TEXT("Com_Texture_Normal"), reinterpret_cast<CComponent**>(&m_pTextureCom_Normal))))
        return E_FAIL;
    // LaserStart
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Boss_Erma_BigGolem_Hand_LaserStart"),
        TEXT("Com_Texture_LaserStart"), reinterpret_cast<CComponent**>(&m_pTextureCom_LaserStart))))
        return E_FAIL;
    // LaserProgress_Start
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Boss_Erma_BigGolem_Hand_LaserProgress_Start"),
        TEXT("Com_Texture_LaserProgress_Start"), reinterpret_cast<CComponent**>(&m_pTextureCom_LaserProgress_Start))))
        return E_FAIL;
    // LaserProgress_Cycle
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Boss_Erma_BigGolem_Hand_LaserProgress_Cycle"),
        TEXT("Com_Texture_LaserProgress_Cycle"), reinterpret_cast<CComponent**>(&m_pTextureCom_LaserProgress_Cycle))))
        return E_FAIL;
    // Laser_End
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Boss_Erma_BigGolem_Hand_LaserEnd"),
        TEXT("Com_Texture_Laser_End"), reinterpret_cast<CComponent**>(&m_pTextureCom_Laser_End))))
        return E_FAIL;




    /* For Com_Animator */
    CAnimator::ANIMSTATE_DESC StartAnimStateDesc{};
    StartAnimStateDesc.strTimerTag = L"Animator_Boss_Arma_Hand_L";   // 해당 애니메이터가 타이머에서 사용할 태그 key값
    StartAnimStateDesc.pParentTransform = nullptr;
    StartAnimStateDesc.pChildTransform = nullptr;
    
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Animator"),
        TEXT("Com_Animator"), reinterpret_cast<CComponent**>(&m_pAnimatorCom), &StartAnimStateDesc)))
        return E_FAIL;


    // State 삽입
    m_pAnimatorCom->Add_State(L"Idle",                  { m_pTextureCom_Normal, 4, true });
    m_pAnimatorCom->Add_State(L"LaserStart",            { m_pTextureCom_LaserStart, 4, false });
    m_pAnimatorCom->Add_State(L"LaserProgress_Start",   { m_pTextureCom_LaserProgress_Start, 4, false });
    m_pAnimatorCom->Add_State(L"LaserProgress_Cycle",   { m_pTextureCom_LaserProgress_Cycle, 4, true });
    m_pAnimatorCom->Add_State(L"Laser_End",             { m_pTextureCom_Laser_End, 4, false });


    CAnimator::ANIMSTATE_DESC StartAnimStateDesc2{};
    StartAnimStateDesc2.strTimerTag = L"Animator_Boss_Arma_Hand_L_Pattern";   // 해당 애니메이터가 타이머에서 사용할 태그 key값
    StartAnimStateDesc2.pParentTransform = nullptr;
    StartAnimStateDesc2.pChildTransform = nullptr;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Animator"),
        TEXT("Com_AnimatorPattern"), reinterpret_cast<CComponent**>(&m_pAnimatorPatternCom), &StartAnimStateDesc)))
        return E_FAIL;

    m_pAnimatorPatternCom->Add_State(L"Idle",       { nullptr, 4, true });
    m_pAnimatorPatternCom->Add_State(L"Strike",       { nullptr, 450, true });    // 7.5s
    m_pAnimatorPatternCom->Add_State(L"Laser",       { nullptr, 600, true });    // 10s

    // ..




    return S_OK;
}

HRESULT CErma_Hand_L::Ready_Object()
{
    m_pHpBar = dynamic_cast<CField_Hp*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_UI_Field_Hp")));

    return S_OK;
}

void CErma_Hand_L::OnCollision(CGameObject* pGameObject)
{
    __super::OnCollision(pGameObject);

    switch (pGameObject->Get_ObjType())
    {
    case GAMEOBJ_TYPE::PLAYER_EFFECT:
        break;
    }
}

void CErma_Hand_L::PlayPattern(PATTERN_HAND_L ePattern)
{
    _float fPatternTime = 0.f;

    switch (ePattern)
    {
    case Client::CErma_Hand_L::PATTERN_HAND_L::PT_STRIKE:   fPatternTime = 7.5f;
        break;
    case Client::CErma_Hand_L::PATTERN_HAND_L::PT_LASER:    fPatternTime = 10.f;
        break;
    default:
        break;
    }

    m_isPatternPlaying = true;
    m_ePattern = ePattern;
    m_pAnimatorPatternCom->Change_State(L"Idle", true, fPatternTime, true);
}

CErma_Hand_L* CErma_Hand_L::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CErma_Hand_L* pInstance = new CErma_Hand_L(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Create : CErma_Hand_L"));
        Safe_Release(pInstance);
    }

    return pInstance;
}


CGameObject* CErma_Hand_L::Clone(void* pArg)
{
    CErma_Hand_L* pInstance = new CErma_Hand_L(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Clone : CErma_Hand_L"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CErma_Hand_L::Free()
{
    __super::Free();

    Safe_Release(m_pTextureCom_Normal);
    Safe_Release(m_pTextureCom_LaserStart);
    Safe_Release(m_pTextureCom_LaserProgress_Start);
    Safe_Release(m_pTextureCom_LaserProgress_Cycle);
    Safe_Release(m_pTextureCom_Laser_End);


    Safe_Release(m_pAnimatorCom);
}
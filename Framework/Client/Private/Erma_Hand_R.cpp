#include "Erma_Hand_R.h"
#include "Effect_Factory.h"

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
    _float3 fTerrainPos = pTerrainTransform->Get_State(STATE::POSITION);
    _float3 fTerrainScale = pTerrainTransform->Get_Scaled();

    // 위치 정해지는 부분. from __super
    m_pTransformCom->Set_State(STATE::POSITION, _float3(
        fTerrainPos.x + 4.f,
        0.f,
        fTerrainPos.z + fTerrainScale.z / 2 - 3.f));

    // 크기 조정
    m_pTransformCom->Scaling(3.f, 3.f, 3.f);




    m_isSummoned = true;
    //Ready_Object();

    // 임시
    m_iMaxHp = 500;
    m_iCulHp = 500;

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






    if (m_pTerrainBox != nullptr) {
        m_pTerrainBox->SetUp_OnTerrainBox(m_pTransformCom, _float3(0.05f, 0.5f, 0.05f));
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

    m_pTransformCom->Bind_Matrix();

    m_pAnimatorCom->Update_State(); // Bind_Texture

    m_pVIBufferCom->Bind_Buffers();

    SetUp_RenderState();

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
    StartAnimStateDesc.strTimerTag = L"Animator_Boss_Arma_Hand_R";   // 해당 애니메이터가 타이머에서 사용할 태그 key값
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
    __super::OnCollision(pGameObject);

    switch (pGameObject->Get_ObjType())
    {
    case GAMEOBJ_TYPE::PLAYER_EFFECT:
        break;
    }
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
}
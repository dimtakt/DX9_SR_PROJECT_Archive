#include "Erma.h"
#include "Effect_Factory.h"

#include "Erma_Body.h"
#include "Erma_Hand_L.h"
#include "Erma_Hand_R.h"
#include "Erma_Head.h"

CErma::CErma(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CMonster{ pGraphic_Device }
{
}

CErma::CErma(const CErma& Prototype)
	: CMonster{ Prototype }
{
}

HRESULT CErma::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CErma::Initialize(void* pArg)
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
        fTerrainPos.x,
        0.f,
        fTerrainPos.z + fTerrainScale.z / 2 - 5));



    // Erma가 각 부속 객체들을 제어하도록 연결

    //m_pObj_Body->Set_IsDead(true);
    //m_pObj_Hand_L->Set_IsDead(true);
    //m_pObj_Hand_R->Set_IsDead(true);
    //m_pObj_Head->Set_IsDead(true);		





    m_isSummoned = true;
    Ready_Object();

    m_iMaxHp = 500;
    m_iCulHp = 500;

    m_eMonsterType = MONSTER_TYPE::ERMA;

    return S_OK;
}

void CErma::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
    if (m_pHpBar != nullptr &&
        m_isSummoned)
        m_pHpBar->Render_HP_Progress(m_pTransformCom, m_iCulHp, m_iMaxHp);

    if (m_iCulHp <= 0)
        m_bDead = true;
}

void CErma::Update(_float fTimeDelta)
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


    _wstring strCurStateTag = m_pAnimatorCom->Get_CurStateTag();

    if (strCurStateTag == L"Idle")
    {
        m_pAnimatorCom->Change_State(L"Enter_Progress");
    }
    else if (strCurStateTag == L"Enter_Progress")
    {
        m_pAnimatorCom->Change_State(L"Enter_End");
    }
    else if (strCurStateTag == L"Enter_End")
    {
        // ksta : 위치에 도달 할 때 발동 조건도 넣어야 함
        if (m_pAnimatorCom->Change_State(L"Entered"))
        {
            CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::NORMAL_EFFECT, L"Prototype_Component_Boss_Erma_Effect_HeadStart",
                *m_pTransformCom->Get_WorldMatrix(), matMonsterWorld);
        }
    }




    if (m_pTerrainBox != nullptr) {
        m_pTerrainBox->SetUp_OnTerrainBox(m_pTransformCom, _float3(0.05f, 0.2f, 0.05f));
    }
}

void CErma::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
}

HRESULT CErma::Render()
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

HRESULT CErma::Ready_Components(void* pArg)
{
    MONSTERDESC* desc = static_cast<MONSTERDESC*>(pArg);

    // 컴포넌트 추가
    /* For.Com_Texture */
    // Idle
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Boss_Erma_Idle"),
        TEXT("Com_Texture_Idle"), reinterpret_cast<CComponent**>(&m_pTextureCom_Idle))))
        return E_FAIL;
    // Move
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Boss_Erma_Move"),
        TEXT("Com_Texture_Move"), reinterpret_cast<CComponent**>(&m_pTextureCom_Move))))
        return E_FAIL;
    // Airborne
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Boss_Erma_Airborne"),
        TEXT("Com_Texture_Airborne"), reinterpret_cast<CComponent**>(&m_pTextureCom_Airborne))))
        return E_FAIL;
    // Down
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Boss_Erma_Down"),
        TEXT("Com_Texture_Down"), reinterpret_cast<CComponent**>(&m_pTextureCom_Down))))
        return E_FAIL;
    // Down_End
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Boss_Erma_Down_End"),
        TEXT("Com_Texture_Down_End"), reinterpret_cast<CComponent**>(&m_pTextureCom_Down_End))))
        return E_FAIL;
    // Stun
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Boss_Erma_Stun"),
        TEXT("Com_Texture_Stun"), reinterpret_cast<CComponent**>(&m_pTextureCom_Stun))))
        return E_FAIL;
    // Enter_Progress (Core로 변신)
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Boss_Erma_Enter_Progress"),
        TEXT("Com_Texture_Enter_Progress"), reinterpret_cast<CComponent**>(&m_pTextureCom_Enter_Progress))))
        return E_FAIL;
    // Enter_End (변신 완)
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Boss_Erma_Enter_End"),
        TEXT("Com_Texture_Enter_End"), reinterpret_cast<CComponent**>(&m_pTextureCom_Enter_End))))
        return E_FAIL;
    // Entered (합체 완 / texture은 nullptr로, 기체 탑승한 상태)
    // 이는 상태로 관리


    /* For Com_Animator */
    CAnimator::ANIMSTATE_DESC StartAnimStateDesc{};
    StartAnimStateDesc.strTimerTag = L"Animator_Boss_Erma";   // 해당 애니메이터가 타이머에서 사용할 태그 key값
    StartAnimStateDesc.pParentTransform = nullptr;
    StartAnimStateDesc.pChildTransform = nullptr;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Animator"),
        TEXT("Com_Animator"), reinterpret_cast<CComponent**>(&m_pAnimatorCom), &StartAnimStateDesc)))
        return E_FAIL;
    // State 삽입
    m_pAnimatorCom->Add_State(L"Idle",              { m_pTextureCom_Idle, 4, true });
    m_pAnimatorCom->Add_State(L"Move",              { m_pTextureCom_Move, 4, true });
    m_pAnimatorCom->Add_State(L"Airborne",          { m_pTextureCom_Airborne, 4, true });
    m_pAnimatorCom->Add_State(L"Down",              { m_pTextureCom_Down, 4, true });
    m_pAnimatorCom->Add_State(L"Down_End",          { m_pTextureCom_Down_End, 4, true });
    m_pAnimatorCom->Add_State(L"Stun",              { m_pTextureCom_Stun, 4, true });
    m_pAnimatorCom->Add_State(L"Enter_Progress",    { m_pTextureCom_Enter_Progress, 4, false });
    m_pAnimatorCom->Add_State(L"Enter_End",         { m_pTextureCom_Enter_End, 4, true });
    m_pAnimatorCom->Add_State(L"Entered",           { nullptr, 4, true });    // 기체 탑승 상태, 텍스쳐 사용 X


    // ..




    return S_OK;
}

HRESULT CErma::Ready_Object()
{
    m_pHpBar = dynamic_cast<CField_Hp*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_UI_Field_Hp")));

    return S_OK;
}

void CErma::OnCollision(CGameObject* pGameObject)
{
    __super::OnCollision(pGameObject);

    switch (pGameObject->Get_ObjType())
    {
    case GAMEOBJ_TYPE::PLAYER_EFFECT:
        break;
    }
}

CErma* CErma::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CErma* pInstance = new CErma(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Create : CErma"));
        Safe_Release(pInstance);
    }

    return pInstance;
}


CGameObject* CErma::Clone(void* pArg)
{
    CErma* pInstance = new CErma(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Clone : CErma"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CErma::Free()
{
    __super::Free();

    Safe_Release(m_pTextureCom_Idle);
    Safe_Release(m_pTextureCom_Move);
    Safe_Release(m_pTextureCom_Airborne);
    Safe_Release(m_pTextureCom_Down);
    Safe_Release(m_pTextureCom_Down_End);
    Safe_Release(m_pTextureCom_Stun);
    Safe_Release(m_pTextureCom_Enter_Progress);
    Safe_Release(m_pTextureCom_Enter_End);

    Safe_Release(m_pAnimatorCom);

    //Safe_Release(m_pAnimatorCom);
}
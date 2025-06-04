#include "Mole_A.h"
#include "Effect_Factory.h"

CMole_A::CMole_A(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CMonster{ pGraphic_Device }
{

}

CMole_A::CMole_A(const CMole_A& Prototype)
    : CMonster{ Prototype }
{

}

HRESULT CMole_A::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CMole_A::Initialize(void* pArg)
{
	__super::Initialize(pArg);

    if (FAILED(this->Ready_Components(pArg)))
        return E_FAIL;

	return S_OK;
}

void CMole_A::Priority_Update(_float fTimeDelta)
{

}

void CMole_A::Update(_float fTimeDelta)
{
    if (!m_pTerrainBox || !m_pTransformCom || !m_pTextureCom)
        return;

    _float fMinDist = 6.f;      // 추적 상태로 변할 기준 거리
    _float fMaxDist = 12.f;     // 어그로가 풀리는 기준 거리
    _float fAtkDist = 1.5f;     // 근접공격할 기준 거리




    _uint iCurLevel = CGameInstance::GetInstance()->Get_CurrentLevel();

    CTransform* pTargetTransform = dynamic_cast<CTransform*>(m_pGameInstance->GetInstance()->Get_Component(iCurLevel, TEXT("Layer_Player"), TEXT("Com_Transform")));

    _float3 vMonsterPos = m_pTransformCom->Get_State(STATE::POSITION);
    _float3 vTargetPos = pTargetTransform->Get_State(STATE::POSITION);

    _float3 vDiff = - vMonsterPos + vTargetPos;
    _float fDistance = D3DXVec3Length(&vDiff);


    // ********* matMonster 구하기
    _float4x4 matMonsterWorld = *m_pTransformCom->Get_WorldMatrix();

    
    
    // *** 상태 분기 정리
    _float fPointY = 0.f;       // 교차 평면의 기준이 될 Y값
    

    // 이펙트용
    // 1. 원점으로 이동
    _float4x4 matTransToOrigin = {};
    D3DXMatrixIdentity(&matTransToOrigin);
    D3DXMatrixTranslation(&matTransToOrigin, -matMonsterWorld._41, -matMonsterWorld._42, -matMonsterWorld._43);

    // 2. 크기
    _float4x4 matScale = {};
    D3DXMatrixIdentity(&matScale);
    D3DXMatrixScaling(&matScale, -2.f, 2.f, 2.f);

    // 3. 자전
    _float4x4 matRotateChild = {};
    D3DXMatrixIdentity(&matRotateChild);
    D3DXMatrixRotationX(&matRotateChild, D3DXToRadian(-90)); // 안되면 -90도도 해보기

    _float4x4 matRotateChildtoPlayer = {};
    D3DXMatrixIdentity(&matRotateChildtoPlayer);
    _float fAngle = atan2f(vTargetPos.x - vMonsterPos.x, vTargetPos.z - vMonsterPos.z);
    _float fDegree = D3DXToDegree(fAngle) + 190;
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
    _float fDistanceOffset = 1.2f;                   // ** ksta : 중점으로부터 떨어져 있을 거리 **
    vDiff *= fDistanceOffset;
    D3DXMatrixTranslation(&matTransAddition, vDiff.x, 0, vDiff.z);

    matMonsterWorld = matTransToOrigin * matScale * matRotateChild * matRotateChildtoPlayer * matTransReturn * matTransAddition;



    // 거리가 아주 가깝다면 공격 상태
    if (fDistance <= fAtkDist)
    {
        if (m_pAnimatorCom->Change_State(L"Attack"))
        {
            m_isTracking = false;

        }
    }
    // 거리가 적당히 떨어져있다면 추적 ON
    else if (fDistance <= fMinDist)
    {
        if(m_pAnimatorCom->Change_State(L"Move"))
            m_isTracking = true;
    }
    // 충분히 멀리 떨어지면 Idle 상태
    else if (fDistance >= fMaxDist)
    {
        if (m_pAnimatorCom->Change_State(L"Idle"))
            m_isTracking = false;
    }


    // 각 분기점마다의 행동
    if (m_isTracking)
    {
        float fMoveSpeed = 1.5f;

        // 플레이어 추적
        _float3 vPosDiff = vTargetPos - vMonsterPos;
        D3DXVec3Normalize(&vPosDiff, &vPosDiff);
        _float3 vNewMonsterPos = vMonsterPos + vPosDiff * fTimeDelta * fMoveSpeed;
        m_pTransformCom->Set_State(STATE::POSITION, vNewMonsterPos);
    }


    if (m_pAnimatorCom->Get_CurStateTag() == L"Attack" &&
        m_pAnimatorCom->Get_CurStackedFrame() == 20)
    {
        CEffect_Factory::GetInstance()->Create_Effect(L"Prototype_Component_Texture_Mole_A_Effect_Swing",
            *m_pTransformCom->Get_WorldMatrix(), matMonsterWorld, true);
    }
    else if (m_pAnimatorCom->Get_CurStateTag() == L"Attack" &&
        m_pAnimatorCom->Get_IsLastFrame())
    {
        if (m_pAnimatorCom->Change_State(L"Attack_Standby"));
    }
    
    if (m_pAnimatorCom->Get_CurStateTag() == L"Attack_Standby")
        m_pAnimatorCom->Change_State(L"Idle");
    



    if (m_pTerrainBox != nullptr) {
        m_pTerrainBox->SetUp_OnTerrainBox(m_pTransformCom, _float3(0.05f, 0.2f, 0.05f));
    }

    //if (m_isTracking)           m_pAnimatorCom->Change_State(L"Move");
    //else                        m_pAnimatorCom->Change_State(L"Idle");
    // m_isTracking = false;
}

void CMole_A::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CMole_A::Render()
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

    m_pVIBufferCom->Render();

    m_pTerrainBox->Render();

    if (m_isFlippedX)
    {
        m_pVIBufferCom->ResetUV_FlipX();
        m_isFlippedX = false;
    }

    return S_OK;
}

HRESULT CMole_A::Ready_Components(void* pArg)
{

    MONSTERDESC* desc = static_cast<MONSTERDESC*>(pArg);
    
    /* For.Com_Texture */
    // Idle
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Texture_Mole_A_Idle"),
        TEXT("Com_Texture_Idle"), reinterpret_cast<CComponent**>(&m_pTextureCom_Idle))))
        return E_FAIL;
    // Move
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Texture_Mole_A_Move"),
        TEXT("Com_Texture_Move"), reinterpret_cast<CComponent**>(&m_pTextureCom_Move))))
        return E_FAIL;
    // Down
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Texture_Mole_A_Down"),
        TEXT("Com_Texture_Down"), reinterpret_cast<CComponent**>(&m_pTextureCom_Down))))
        return E_FAIL;
    // Attack
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Texture_Mole_A_Attack"),
        TEXT("Com_Texture_Attack"), reinterpret_cast<CComponent**>(&m_pTextureCom_Attack))))
        return E_FAIL;
    // Airborne
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Texture_Mole_A_Airborne"),
        TEXT("Com_Texture_Airborne"), reinterpret_cast<CComponent**>(&m_pTextureCom_Airborne))))
        return E_FAIL;

	/* For Com_Animator */
    CAnimator::ANIMSTATE_DESC StartAnimStateDesc{};
    StartAnimStateDesc.strTimerTag = L"Animator_Monster_Mole_A_Main";   // 해당 애니메이터가 타이머에서 사용할 태그 key값
    StartAnimStateDesc.pParentTransform = nullptr;
    StartAnimStateDesc.pChildTransform = nullptr;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Animator"),
        TEXT("Com_Animator"), reinterpret_cast<CComponent**>(&m_pAnimatorCom), &StartAnimStateDesc)))
        return E_FAIL;
	// State 삽입
    m_pAnimatorCom->Add_State(L"Idle",          { m_pTextureCom_Idle, 4, true });       // 6
	m_pAnimatorCom->Add_State(L"Move",          { m_pTextureCom_Move, 4, true });       // 10
	m_pAnimatorCom->Add_State(L"Down",          { m_pTextureCom_Down, 4, false });      // 1
	m_pAnimatorCom->Add_State(L"Attack",        { m_pTextureCom_Attack, 6, false });    // 7
	m_pAnimatorCom->Add_State(L"Attack_Standby",{ m_pTextureCom_Idle, 4, false });      // 6
	m_pAnimatorCom->Add_State(L"Airborne",      { m_pTextureCom_Airborne, 4, false});   // 1

	return S_OK;
}

void CMole_A::OnCollision(CGameObject* pGameObject)
{
    __super::OnCollision(pGameObject);

    m_isTracking = true;
}

CMole_A* CMole_A::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CMole_A* pInstance = new CMole_A(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Create : CMole_A"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CMole_A::Clone(void* pArg)
{
    CMole_A* pInstance = new CMole_A(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Clone : CMole_A"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CMole_A::Free()
{
    Safe_Release(m_pTextureCom_Idle);
    Safe_Release(m_pTextureCom_Move);
    Safe_Release(m_pTextureCom_Down);
    Safe_Release(m_pTextureCom_Attack);
    Safe_Release(m_pTextureCom_Airborne);

	Safe_Release(m_pAnimatorCom);

    __super::Free();
}
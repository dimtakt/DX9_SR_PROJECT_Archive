#include "Player.h"
#include "TerrainBox.h"
#include "GameInstance.h"
#include "Collider_OBB.h"

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
    m_pTransformCom->Scaling(1.5f, 1.5f, 1.5f);

	return S_OK;
}

void CPlayer::Priority_Update(_float fTimeDelta)
{

}

void CPlayer::Update(_float fTimeDelta)
{    
    m_pCollider->Update_Collider(m_pTransformCom);
    if (m_pTerrainBox != nullptr) {
        m_pTerrainBox->SetUp_OnTerrainBox(m_pTransformCom, _float3(0.05f, 0.5f, 0.05f));
    }
        

    _float fPointY = 0.f;       // 교차 평면의 기준이 될 Y값
    _float3 vRayPoint = {};     // fPointY 값 기준 마우스 Ray와 교차하는 좌표
    m_pGameInstance->Get_IntersectAtY(fPointY, vRayPoint);

    _float3 vPlayerPos = {};    // 플레이어 좌표
    vPlayerPos = m_pTransformCom->Get_State(STATE::POSITION);

    //std::cout << "[Player::Update] RayPoint  : {" << vRayPoint.x << ", " << vRayPoint.y << ", " << vRayPoint.z << "}" << std::endl;
    //std::cout << "[Player::Update] PlayerPos : {" << vPlayerPos.x << ", " << vPlayerPos.y << ", " << vPlayerPos.z << "}" << std::endl;




    if (m_pGameInstance->IsKeyHold('W') ||
        m_pGameInstance->IsKeyHold('S') ||
        m_pGameInstance->IsKeyHold('A') ||
        m_pGameInstance->IsKeyHold('D'))
    {
        // 이동중
        // 이전에 Move 이었다면 프레임 초기화X
        if (vRayPoint.z > vPlayerPos.z)                     // 상단
        {
            if (m_pAnimatorCom->Get_CurStateTag() == L"Move_Lower" ||
                m_pAnimatorCom->Get_CurStateTag() == L"Move_Upper")
                m_pAnimatorCom->Change_State(L"Move_Upper", false);
            else
                m_pAnimatorCom->Change_State(L"Move_Upper", true);
        }
        else                                            // 하단
        {
            if (m_pAnimatorCom->Get_CurStateTag() == L"Move_Lower" ||
                m_pAnimatorCom->Get_CurStateTag() == L"Move_Upper")
                m_pAnimatorCom->Change_State(L"Move_Lower", false);
            else
                m_pAnimatorCom->Change_State(L"Move_Lower", true);
        }
    }
    else
    {
        // 이전에 Idle 이었다면 프레임 초기화 X
        if (vRayPoint.z > vPlayerPos.z)                     // 상단
        {
            if (m_pAnimatorCom->Get_CurStateTag() == L"Idle_Lower" ||
                m_pAnimatorCom->Get_CurStateTag() == L"Idle_Upper")
                m_pAnimatorCom->Change_State(L"Idle_Upper", false);
            else
                m_pAnimatorCom->Change_State(L"Idle_Upper", true);
        }
        else                                            // 하단
        {
            if (m_pAnimatorCom->Get_CurStateTag() == L"Idle_Lower" ||
                m_pAnimatorCom->Get_CurStateTag() == L"Idle_Upper")
                m_pAnimatorCom->Change_State(L"Idle_Lower", false);
            else
                m_pAnimatorCom->Change_State(L"Idle_Lower", true);
        }
    }



    if (m_pGameInstance->IsKeyHold('W'))
        m_pTransformCom->Go_Straight(fTimeDelta);

    if (m_pGameInstance->IsKeyHold('S'))
        m_pTransformCom->Go_Backward(fTimeDelta);

    if (m_pGameInstance->IsKeyHold('A'))
        m_pTransformCom->Go_Left(fTimeDelta);

    if (m_pGameInstance->IsKeyHold('D'))
        m_pTransformCom->Go_Right(fTimeDelta);

    

    if (m_pGameInstance->IsKeyDown(VK_LBUTTON))
    {
        if (vRayPoint.z > vPlayerPos.z)
            m_pAnimatorCom->Change_State(L"GreatSwordHeavyAttack_Upper");
        else
            m_pAnimatorCom->Change_State(L"GreatSwordHeavyAttack_Lower");
    }
    
    if (m_pGameInstance->IsKeyDown(VK_RBUTTON))
    {
        // 우클릭시 행동
    }

    /*if (GetKeyState(VK_LBUTTON) < 0)
    {
        _float3		vTmp = m_pVIBufferCom->Compute_PickedPosition(m_pTransformCom->Get_WorldMatrix_Inverse());
        int a = 10;
    }*/


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
            int a = 1;
            break;
        }
        
    }
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
    
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Animator"),
        TEXT("Com_Animator"), reinterpret_cast<CComponent**>(&m_pAnimatorCom), &StartAnimStateDesc)))
        return E_FAIL;

    // m_pAnimatorCom->Add_State(L"태그명", { m_pTextureCom_상태명, 프레임단위 이미지전환간격, 도중 나갈수있는지});
    m_pAnimatorCom->Add_State(L"Roll",              { m_pTextureCom_Roll, 4, true });
    m_pAnimatorCom->Add_State(L"Air",               { m_pTextureCom_Air, 4, true });
    m_pAnimatorCom->Add_State(L"Down",              { m_pTextureCom_Down, 4, true });
    m_pAnimatorCom->Add_State(L"Idle_Lower",        { m_pTextureCom_Idle_Lower, 4, true });
    m_pAnimatorCom->Add_State(L"Idle_Upper",        { m_pTextureCom_Idle_Upper, 4, true });
    m_pAnimatorCom->Add_State(L"Move_Lower",        { m_pTextureCom_Move_Lower, 4, true });
    m_pAnimatorCom->Add_State(L"Move_Upper",        { m_pTextureCom_Move_Upper, 4, true });
    m_pAnimatorCom->Add_State(L"Attack_Lower",      { m_pTextureCom_Attack_Lower, 4, false });
    m_pAnimatorCom->Add_State(L"Attack_Upper",      { m_pTextureCom_Attack_Upper, 4, false });
    m_pAnimatorCom->Add_State(L"GreatSwordHeavyAttack_Lower", { m_pTextureCom_GreatSwordHeavyAttack_Lower, 4, false });
    m_pAnimatorCom->Add_State(L"GreatSwordHeavyAttack_Upper", { m_pTextureCom_GreatSwordHeavyAttack_Upper, 4, false });
    m_pAnimatorCom->Add_State(L"WhirlWind_Ready",   { m_pTextureCom_WhirlWind_Ready, 4, false });
    m_pAnimatorCom->Add_State(L"WhirlWind_Cycle",   { m_pTextureCom_WhirlWind_Cycle, 4, false });



    // collider
    CCollider_OBB::OBB_DESC tColliderDesc;
    tColliderDesc.vScale = _float3(1.5f, 1.5f, 1.5f);
    tColliderDesc.pOwner = this;
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Collider_OBB"),
        TEXT("Com_Player_Collider"), reinterpret_cast<CComponent**>(&m_pCollider), &tColliderDesc)))
        return E_FAIL;


    m_pGameInstance->Add_Collider(m_pCollider);
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
        MSG_BOX(TEXT("Failed to Created : CPlayer"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CPlayer::Clone(void* pArg)
{
    CPlayer* pInstance = new CPlayer(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Cloned : CPlayer"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CPlayer::Free()
{
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
    //Safe_Release(m_pTerrainBox);
}

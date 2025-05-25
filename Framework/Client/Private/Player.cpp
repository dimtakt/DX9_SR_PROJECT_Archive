#include "Player.h"

#include "GameInstance.h"

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

	return S_OK;
}

void CPlayer::Priority_Update(_float fTimeDelta)
{

}

void CPlayer::Update(_float fTimeDelta)
{
    // ksta : 좌우 반전 방법 찾아서 분기 만들어줘야함
    
    
    POINT pt;
    GetCursorPos(&pt);
    ScreenToClient(g_hWnd, &pt);
    
    if (m_pGameInstance->IsKeyHold('W') ||
        m_pGameInstance->IsKeyHold('S') ||
        m_pGameInstance->IsKeyHold('A') ||
        m_pGameInstance->IsKeyHold('D'))
    {
        // 이동중
        // 이전에 Move 이었다면 프레임 초기화X
        if (g_iWinSizeY / 2 > pt.y)                     // 상단
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
        if (g_iWinSizeY / 2 > pt.y)                     // 상단
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
    
    
        // 좌우반전
        //if      (g_iWinSizeX / 2 > pt.x && !m_isFlippedX)   // 좌측
        //{
        //    m_pVIBufferCom->ChangeUV_FlipX(true);
        //    m_isFlippedX = true;
        //}
        //else if (g_iWinSizeX / 2 < pt.x && m_isFlippedX)    // 우측
        //{
        //    m_pVIBufferCom->ChangeUV_FlipX(false);
        //    m_isFlippedX = false;
        //}


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
        if (g_iWinSizeY / 2 > pt.y)
            m_pAnimatorCom->Change_State(L"GreatSwordHeavyAttack_Upper");
        else
            m_pAnimatorCom->Change_State(L"GreatSwordHeavyAttack_Lower");
    }
    
    if (m_pGameInstance->IsKeyDown(VK_RBUTTON))
    {
        // 우클릭시 행동
    }

    //if (GetKeyState('D') < 0)
    //{
    //    m_pTransformCom->Go_Right(fTimeDelta);
    //}

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
	m_pTransformCom->Bind_Matrix();

	//if (FAILED(m_pTextureCom->Bind_Texture(0)))
	//	return E_FAIL;

    m_pAnimatorCom->Update_State(); // Bind_Texture 이 포함되어, 현재 State에 맞는 이미지 출력

	m_pVIBufferCom->Bind_Buffers();
	m_pVIBufferCom->Render();

	return S_OK;
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


    return S_OK;
}

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CPlayer* pInstance = new CPlayer(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failde to Created : CPlayer"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CPlayer::Clone(void* pArg)
{
    CPlayer* pInstance = new CPlayer(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failde to Cloned : CPlayer"));
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
}

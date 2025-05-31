#include "Mole_A.h"

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

    if (FAILED(this->Ready_Components()))
        return E_FAIL;

	return S_OK;
}

void CMole_A::Priority_Update(_float fTimeDelta)
{

}

void CMole_A::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);

    if (m_isTracking)           m_pAnimatorCom->Change_State(L"Move");
    else                        m_pAnimatorCom->Change_State(L"Idle");

    m_isTracking = false;
}

void CMole_A::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CMole_A::Render()
{
    m_pTransformCom->Bind_Matrix();

    m_pAnimatorCom->Update_State(); // Bind_Texture

    m_pVIBufferCom->Bind_Buffers();

    SetUp_RenderState();

    m_pVIBufferCom->Render();

    m_pTerrainBox->Render();

    return S_OK;
}

HRESULT CMole_A::Ready_Components()
{
    /* For.Com_Texture */
    // Idle
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_Component_Texture_Mole_A_Idle"),
        TEXT("Com_Texture_Idle"), reinterpret_cast<CComponent**>(&m_pTextureCom_Idle))))
        return E_FAIL;
    // Move
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_Component_Texture_Mole_A_Move"),
        TEXT("Com_Texture_Move"), reinterpret_cast<CComponent**>(&m_pTextureCom_Move))))
        return E_FAIL;
    // Down
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_Component_Texture_Mole_A_Down"),
        TEXT("Com_Texture_Down"), reinterpret_cast<CComponent**>(&m_pTextureCom_Down))))
        return E_FAIL;
    // Attack
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_Component_Texture_Mole_A_Attack"),
        TEXT("Com_Texture_Attack"), reinterpret_cast<CComponent**>(&m_pTextureCom_Attack))))
        return E_FAIL;
    // Airborne
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_Component_Texture_Mole_A_Airborne"),
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
    m_pAnimatorCom->Add_State(L"Idle",          { m_pTextureCom_Idle, 4, true });
	m_pAnimatorCom->Add_State(L"Move",          { m_pTextureCom_Move, 4, true });
	m_pAnimatorCom->Add_State(L"Down",          { m_pTextureCom_Down, 4, false });
	m_pAnimatorCom->Add_State(L"Attack",        { m_pTextureCom_Attack, 4, false });
	m_pAnimatorCom->Add_State(L"Airborne",      { m_pTextureCom_Airborne, 4, false});

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
    __super::Free();

    Safe_Release(m_pTextureCom_Idle);
    Safe_Release(m_pTextureCom_Move);
    Safe_Release(m_pTextureCom_Down);
    Safe_Release(m_pTextureCom_Attack);
    Safe_Release(m_pTextureCom_Airborne);

	Safe_Release(m_pAnimatorCom);
}
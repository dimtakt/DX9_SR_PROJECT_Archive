#include "Oink_A.h"

COink_A::COink_A(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CMonster{ pGraphic_Device }
{

}

COink_A::COink_A(const COink_A& Prototype)
    : CMonster{ Prototype }
{

}

HRESULT COink_A::Initialize_Prototype()
{
    return S_OK;
}

HRESULT COink_A::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    if (FAILED(this->Ready_Components()))
        return E_FAIL;

    return S_OK;
}

void COink_A::Priority_Update(_float fTimeDelta)
{

}

void COink_A::Update(_float fTimeDelta)
{
    if (m_pTerrainBox != nullptr) {
        m_pTerrainBox->SetUp_OnTerrainBox(m_pTransformCom, _float3(0.05f, 0.2f, 0.05f));
    }

    if (m_isTracking)           m_pAnimatorCom->Change_State(L"Move");
	else                        m_pAnimatorCom->Change_State(L"Idle");

	m_isTracking = false;
}

void COink_A::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
}

HRESULT COink_A::Render()
{
    m_pTransformCom->Bind_Matrix();

    m_pAnimatorCom->Update_State(); // Bind_Texture

    m_pVIBufferCom->Bind_Buffers();

    SetUp_RenderState();

    m_pVIBufferCom->Render();

    m_pTerrainBox->Render();

    return S_OK;
}

HRESULT COink_A::Ready_Components()
{
    /* For.Com_Texture */
    // Idle
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_Component_Texture_Oink_A_Idle"),
        TEXT("Com_Texture_Idle"), reinterpret_cast<CComponent**>(&m_pTextureCom_Idle))))
        return E_FAIL;
    // Move
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_Component_Texture_Oink_A_Move"),
        TEXT("Com_Texture_Move"), reinterpret_cast<CComponent**>(&m_pTextureCom_Move))))
		return E_FAIL;
    // Attack
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_Component_Texture_Oink_A_Attack"),
		TEXT("Com_Texture_Attack"), reinterpret_cast<CComponent**>(&m_pTextureCom_Attack))))
		return E_FAIL;
    // ChargeReady
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_Component_Texture_Oink_A_ChargeReady"),
        TEXT("Com_Texture_ChargeReady"), reinterpret_cast<CComponent**>(&m_pTextureCom_ChargeReady))))
		return E_FAIL;
    // ChargeReady_Cycle
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_Component_Texture_Oink_A_ChargeReady_Cycle"),
		TEXT("Com_Texture_ChargeReady_Cycle"), reinterpret_cast<CComponent**>(&m_pTextureCom_ChargeReady_Cycle))))
		return E_FAIL;
    // Charge_Cycle
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_Component_Texture_Oink_A_Charge_Cycle"),
		TEXT("Com_Texture_Charge_Cycle"), reinterpret_cast<CComponent**>(&m_pTextureCom_Charge_Cycle))))
		return E_FAIL;
    // Charge_Airborne
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_Component_Texture_Oink_A_Charge_Airborne"),
		TEXT("Com_Texture_Charge_Airborne"), reinterpret_cast<CComponent**>(&m_pTextureCom_Charge_Airborne))))
		return E_FAIL;
    // Charge_Down
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_Component_Texture_Oink_A_Charge_Down"),
		TEXT("Com_Texture_Charge_Down"), reinterpret_cast<CComponent**>(&m_pTextureCom_Charge_Down))))
		return E_FAIL;
    // Charge_End
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_Component_Texture_Oink_A_Charge_End"),
        TEXT("Com_Texture_Charge_End"), reinterpret_cast<CComponent**>(&m_pTextureCom_Charge_End))))
		return E_FAIL;


    /* For Com_Animator */
    CAnimator::ANIMSTATE_DESC StartAnimStateDesc{};
    StartAnimStateDesc.strTimerTag = L"Animator_Monster_Oink_A_Main";   // 해당 애니메이터가 타이머에서 사용할 태그 key값
    StartAnimStateDesc.pParentTransform = nullptr;
    StartAnimStateDesc.pChildTransform = nullptr;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Animator"),
        TEXT("Com_Animator"), reinterpret_cast<CComponent**>(&m_pAnimatorCom), &StartAnimStateDesc)))
        return E_FAIL;

    // State 삽입
    m_pAnimatorCom->Add_State(L"Idle",                  { m_pTextureCom_Idle, 4, true });
	m_pAnimatorCom->Add_State(L"Move",                  { m_pTextureCom_Move, 4, true });
	m_pAnimatorCom->Add_State(L"Attack",                { m_pTextureCom_Attack, 4, false });
	m_pAnimatorCom->Add_State(L"ChargeReady",           { m_pTextureCom_ChargeReady, 4, false });
	m_pAnimatorCom->Add_State(L"ChargeReady_Cycle",     { m_pTextureCom_ChargeReady_Cycle, 4, true });
	m_pAnimatorCom->Add_State(L"Charge_Cycle",          { m_pTextureCom_Charge_Cycle, 4, true });
	m_pAnimatorCom->Add_State(L"Charge_Airborne",       { m_pTextureCom_Charge_Airborne, 4, false });
	m_pAnimatorCom->Add_State(L"Charge_Down",           { m_pTextureCom_Charge_Down, 4, false });
	m_pAnimatorCom->Add_State(L"Charge_End",            { m_pTextureCom_Charge_End, 4, false });
    
    return S_OK;
}

void COink_A::OnCollision(CGameObject* pGameObject)
{
	__super::OnCollision(pGameObject);

	m_isTracking = true;
}

COink_A* COink_A::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    COink_A* pInstance = new COink_A(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Create : COink_A"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* COink_A::Clone(void* pArg)
{
    COink_A* pInstance = new COink_A(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Clone : COink_A"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void COink_A::Free()
{
    __super::Free();

    Safe_Release(m_pTextureCom_Idle);
    Safe_Release(m_pTextureCom_Move);
    Safe_Release(m_pTextureCom_Attack);
    Safe_Release(m_pTextureCom_ChargeReady);
    Safe_Release(m_pTextureCom_ChargeReady_Cycle);
    Safe_Release(m_pTextureCom_Charge_Cycle);
    Safe_Release(m_pTextureCom_Charge_Airborne);
    Safe_Release(m_pTextureCom_Charge_Down);
    Safe_Release(m_pTextureCom_Charge_End);

    Safe_Release(m_pAnimatorCom);
}
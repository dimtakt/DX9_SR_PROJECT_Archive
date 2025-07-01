#include "EXP_Ball.h"

#include "GameInstance.h"
#include "Client_Struct.h"
#include "Stat_Manager.h"

CEXP_Ball::CEXP_Ball(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CGameObject{ pGraphic_Device }
{
}

CEXP_Ball::CEXP_Ball(const CEXP_Ball& Prototype)
    : CGameObject{ Prototype }
{
}

HRESULT CEXP_Ball::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CEXP_Ball::Initialize(void* pArg)
{
    if (FAILED(Ready_Components(pArg)))
        return E_FAIL;

    m_eObjType = GAMEOBJ_TYPE::EXPBALL;

    EXPBALLDESC* pDesc = static_cast<EXPBALLDESC*>(pArg);
    m_fEXPValue = pDesc->fValue;
    m_pTransformCom->Set_State(STATE::POSITION, pDesc->vPosition);
    m_pTransformCom->Scaling(0.3f, 0.3f, 0.3f);
    // collider
    CCollider_OBB::OBB_DESC tColliderDesc;
    tColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
    tColliderDesc.pOwner = this;
    tColliderDesc.pTransform = m_pTransformCom;
    tColliderDesc.eType = m_eObjType;
    CCollider_OBB* pCol = dynamic_cast<CCollider_OBB*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::COMPONENT, ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Collider_OBB"), &tColliderDesc));
    m_pGameInstance->Add_Collider(pCol);

    return S_OK;
}

void CEXP_Ball::Priority_Update(_float fTimeDelta)
{
    

}

void CEXP_Ball::Update(_float fTimeDelta)
{
    CTransform* pPlayerTransform = dynamic_cast<CTransform*>(m_pGameInstance->Get_GameObject(m_pGameInstance->Get_CurrentLevel(), TEXT("Layer_Player"))->Find_Component(TEXT("Com_Transform")));
    m_pTransformCom->Move_To(pPlayerTransform->Get_State(STATE::POSITION), fTimeDelta * 2.f, 0.f);
}

void CEXP_Ball::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_PRIORITY, this);
}

HRESULT CEXP_Ball::Render()
{
    SetUp_RenderState();

    m_pTransformCom->Bind_Matrix();

    m_pVIBufferCom->Bind_Buffers();
    m_pAnimatorCom->Update_State();
    m_pVIBufferCom->Render();
    

    Reset_RenderState();

    return S_OK;
}
void CEXP_Ball::SetUp_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
}

void CEXP_Ball::Reset_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

    m_pGraphic_Device->SetTexture(0, NULL);
}

HRESULT CEXP_Ball::Ready_Components(void* pArg)
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    /* For.Com_Texture */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_EXP_Ball"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    CTransform::TRANSFORM_DESC TransformDesc{};
    TransformDesc.fSpeedPerSec = 5.f;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
        return E_FAIL;

    CAnimator::ANIMSTATE_DESC StartAnimStateDesc{};
    StartAnimStateDesc.strTimerTag = L"Animator_EXP_Ball";
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Animator"),
        TEXT("Com_Animator"), reinterpret_cast<CComponent**>(&m_pAnimatorCom), &StartAnimStateDesc)))
        return E_FAIL;

    m_pAnimatorCom->Add_State(L"EXP_Ball", { m_pTextureCom, 4, true });

    return S_OK;
}

void CEXP_Ball::OnCollision(CGameObject* pGameObject)
{
    if (pGameObject == nullptr)
        return;
}

CEXP_Ball* CEXP_Ball::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CEXP_Ball* pInstance = new CEXP_Ball(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failde to Created : CBackGround"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CEXP_Ball::Clone(void* pArg)
{
    CEXP_Ball* pInstance = new CEXP_Ball(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failde to Cloned : CEXP_Ball"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CEXP_Ball::Free()
{
    m_pGameInstance->Remove_Collider_ByOwner(this);
    __super::Free();

    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTransformCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pAnimatorCom);

}

#include "ProjSword.h"

#include "GameInstance.h"
#include "Client_Struct.h"
#include "Stat_Manager.h"
#include "Effect_Factory.h"
#include "Room_Manager.h"

CProjSword::CProjSword(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CGameObject{ pGraphic_Device }
{
}

CProjSword::CProjSword(const CProjSword& Prototype)
    : CGameObject{ Prototype }
{
}

HRESULT CProjSword::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CProjSword::Initialize(void* pArg)
{
    if (FAILED(Ready_Components(pArg)))
        return E_FAIL;

    m_eObjType = GAMEOBJ_TYPE::NORMAL_EFFECT;

    PROJSWORDDESC* pDesc = static_cast<PROJSWORDDESC*>(pArg);
    m_pTargetTransform = pDesc->pTargetTransform;
    Safe_AddRef(m_pTargetTransform);
    m_pTransformCom_0->Set_State(STATE::POSITION, m_pTargetTransform->Get_State(STATE::POSITION) - _float3(1.5f, 0.f, 0.f));
    m_pTransformCom_1->Set_State(STATE::POSITION, m_pTargetTransform->Get_State(STATE::POSITION) - _float3(1.5f, 0.f, 0.f));
    m_pTransformCom_0->Scaling(2.f, 2.f, 2.f);
    m_pTransformCom_1->Scaling(2.f, 2.f, 2.f);


    Init_ItemRot(m_pTransformCom_0, SWORDDIR::LEFT);
    Init_ItemRot(m_pTransformCom_1, SWORDDIR::RIGHT);
    return S_OK;
}

void CProjSword::Priority_Update(_float fTimeDelta)
{
}

void CProjSword::Update(_float fTimeDelta)
{
    _float3 vDelta = m_pTargetTransform->Get_State(STATE::POSITION) - m_pTransformCom_0->Get_State(STATE::POSITION);
    _float fDistSq = D3DXVec3LengthSq(&vDelta);
    if (sqrt(fDistSq) > 10)
    {
        m_pTransformCom_0->Set_State(STATE::POSITION, m_pTargetTransform->Get_State(STATE::POSITION));
        m_pTransformCom_1->Set_State(STATE::POSITION, m_pTargetTransform->Get_State(STATE::POSITION));
    }
    Follow_Player_Side(m_pTransformCom_0, fTimeDelta, SWORDDIR::LEFT);
    Follow_Player_Side(m_pTransformCom_1, fTimeDelta, SWORDDIR::RIGHT);
    
}

void CProjSword::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CProjSword::Render()
{
    SetUp_RenderState();

    m_pTransformCom_0->Bind_Matrix();
    m_pVIBufferCom_0->Bind_Buffers();
    m_pAnimatorCom_0->Update_State();
    m_pVIBufferCom_0->Render();

    m_pTransformCom_1->Bind_Matrix();
    m_pVIBufferCom_1->Bind_Buffers();
    m_pAnimatorCom_1->Update_State();
    m_pVIBufferCom_1->Render();

    Reset_RenderState();


    return S_OK;
}

void CProjSword::SetUp_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
}

void CProjSword::Reset_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

    m_pGraphic_Device->SetTexture(0, NULL);
}

HRESULT CProjSword::Ready_Components(void* pArg)
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer_0"), reinterpret_cast<CComponent**>(&m_pVIBufferCom_0))))
        return E_FAIL;
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer_1"), reinterpret_cast<CComponent**>(&m_pVIBufferCom_1))))
        return E_FAIL;

    /* For.Com_Texture */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_ProjSword"),
        TEXT("Com_Texture_0"), reinterpret_cast<CComponent**>(&m_pTextureCom_0))))
        return E_FAIL;
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_ProjSword"),
        TEXT("Com_Texture_1"), reinterpret_cast<CComponent**>(&m_pTextureCom_1))))
        return E_FAIL;

    CTransform::TRANSFORM_DESC TransformDesc{};
    TransformDesc.fSpeedPerSec = 5.f;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform_0"), reinterpret_cast<CComponent**>(&m_pTransformCom_0), &TransformDesc)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform_1"), reinterpret_cast<CComponent**>(&m_pTransformCom_1), &TransformDesc)))
        return E_FAIL;

    CAnimator::ANIMSTATE_DESC StartAnimStateDesc_0{};
    StartAnimStateDesc_0.strTimerTag = L"Animator_ProjSword_0";
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Animator"),
        TEXT("Com_Animator_0"), reinterpret_cast<CComponent**>(&m_pAnimatorCom_0), &StartAnimStateDesc_0)))
        return E_FAIL;

    m_pAnimatorCom_0->Add_State(L"Planet", { m_pTextureCom_0, 4, true });

    CAnimator::ANIMSTATE_DESC StartAnimStateDesc_1{};
    StartAnimStateDesc_1.strTimerTag = L"Animator_ProjSword_1";
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Animator"),
        TEXT("Com_Animator_1"), reinterpret_cast<CComponent**>(&m_pAnimatorCom_1), &StartAnimStateDesc_1)))
        return E_FAIL;

    m_pAnimatorCom_1->Add_State(L"Planet", { m_pTextureCom_1, 4, true });

    return S_OK;
}

void CProjSword::Init_ItemRot(CTransform* pTransform, SWORDDIR eDir)
{
    if (eDir == SWORDDIR::LEFT)
        pTransform->Rotation(_float3(0.f, 0.f, 1.f), D3DXToRadian(-30.f));
    else if (eDir == SWORDDIR::RIGHT)
        pTransform->Rotation(_float3(0.f, 0.f, 1.f), D3DXToRadian(30.f));
  
}

HRESULT CProjSword::Follow_Player_Side(CTransform* pTransform, _float fDeltaTime, SWORDDIR eDir)
{
    _float3 vPlayerPos = m_pTargetTransform->Get_State(STATE::POSITION);
    _float3 vLook = m_pTargetTransform->Get_State(STATE::LOOK);
    _float3 vRight = m_pTargetTransform->Get_State(STATE::RIGHT);

    D3DXVec3Normalize(&vLook, &vLook);
    D3DXVec3Normalize(&vRight, &vRight);

    // 상대 위치 지정
    _float3 vOffset = +vLook * 0.8f;

    if (eDir == SWORDDIR::LEFT)        // 왼쪽
        vOffset -= vRight * 0.55f;
    else if (eDir == SWORDDIR::RIGHT)   // 오른쪽
        vOffset += vRight * 0.55f;
 

    _float3 vTargetPos = vPlayerPos + vOffset;

    // 부드럽게 따라가기
    _float3 vCurPos = pTransform->Get_State(STATE::POSITION);
    _float fSpeed = 6.f;
    _float fT = 1.f - expf(-fSpeed * fDeltaTime);
    _float3 vSmoothed;
    D3DXVec3Lerp(&vSmoothed, &vCurPos, &vTargetPos, fT);
    pTransform->Set_State(STATE::POSITION, vSmoothed);

    return S_OK;
}

CProjSword* CProjSword::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CProjSword* pInstance = new CProjSword(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failde to Created : CProjSword"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CProjSword::Clone(void* pArg)
{
    CProjSword* pInstance = new CProjSword(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failde to Cloned : CProjSword"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CProjSword::Free()
{
    __super::Free();

    Safe_Release(m_pVIBufferCom_0);
    Safe_Release(m_pTransformCom_0);
    Safe_Release(m_pVIBufferCom_1);
    Safe_Release(m_pTransformCom_1);
    Safe_Release(m_pTextureCom_0);
    Safe_Release(m_pTextureCom_1);
    Safe_Release(m_pAnimatorCom_0);
    Safe_Release(m_pAnimatorCom_1);
    Safe_Release(m_pTargetTransform);

}
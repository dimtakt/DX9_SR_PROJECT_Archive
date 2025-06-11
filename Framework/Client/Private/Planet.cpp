#include "Planet.h"

#include "GameInstance.h"
#include "Client_Struct.h"
#include "Stat_Manager.h"

CPlanet::CPlanet(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject { pGraphic_Device }
{
}

CPlanet::CPlanet(const CPlanet& Prototype)
	: CGameObject { Prototype }
{
}

HRESULT CPlanet::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlanet::Initialize(void* pArg)
{
    if (FAILED(Ready_Components(pArg)))
        return E_FAIL;

    m_eObjType = GAMEOBJ_TYPE::PLANET;

    PLANETDESC* pDesc = static_cast<PLANETDESC*>(pArg);
    m_eType = pDesc->eType;
    m_pTargetTransform = pDesc->pTargetTransform;
    m_pTransformCom->Set_State(STATE::POSITION, m_pTargetTransform->Get_State(STATE::POSITION) - _float3(1.5f, 0.f, 0.f));
    m_pTransformCom->Scaling(1.f, 1.f, 1.f);
 
	return S_OK;
}

void CPlanet::Priority_Update(_float fTimeDelta)
{
}

void CPlanet::Update(_float fTimeDelta)
{
}

void CPlanet::Late_Update(_float fTimeDelta)
{
}

HRESULT CPlanet::Render()
{
	return S_OK;
}

void CPlanet::SetUp_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
}

void CPlanet::Reset_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

    m_pGraphic_Device->SetTexture(0, NULL);
}

HRESULT CPlanet::Ready_Components(void* pArg)
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

CPlanet* CPlanet::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CPlanet* pInstance = new CPlanet(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failde to Created : CPlanet"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CPlanet::Clone(void* pArg)
{
    CPlanet* pInstance = new CPlanet(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failde to Cloned : CPlanet"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CPlanet::Free()
{
    __super::Free();

    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTransformCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pAnimatorCom);

}
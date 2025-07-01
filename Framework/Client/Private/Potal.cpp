#include "Potal.h"
#include "GameInstance.h"
#include "Client_Struct.h"
CPotal::CPotal(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CGameObject{ pGraphic_Device }
{
}

CPotal::CPotal(const CPotal& Prototype)
    : CGameObject{ Prototype }
{
}

HRESULT CPotal::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CPotal::Initialize(void* pArg)
{
    OBJECT_INTERACTION_DESC* pObject_Desc = static_cast<OBJECT_INTERACTION_DESC*>(pArg);
    m_eObjType = GAMEOBJ_TYPE::POTAL;

    m_eDirType = pObject_Desc->ePotalType;

    if (m_eDirType == POTAL_TYPE::END_POTAL)
        m_eObjType = GAMEOBJ_TYPE::END_POTAL;
    else if (m_eDirType == POTAL_TYPE::STAGE_POTAL)
        m_eObjType = GAMEOBJ_TYPE::STAGE_POTAL;

  
    if (FAILED(Ready_Components()))
        return E_FAIL;


    if (pArg == nullptr)
    {
        m_pTransformCom->Set_State(STATE::POSITION, _float3(0.f, 0.f, 0.f));
        return S_OK;
    }
    m_pTransformCom->Set_State(STATE::POSITION, pObject_Desc->vPos);

    //XÃà È¸Àü
    if (pObject_Desc->vRotate.x != 0.f || pObject_Desc->vRotate.y != 0.f || pObject_Desc->vRotate.z != 0.f)
        m_pTransformCom->ApplyEulerRotation(pObject_Desc->vRotate);
       
    if(pObject_Desc->vRotate != nullptr)
      m_pTransformCom->Set_RotationEuler(pObject_Desc->vRotate.x, pObject_Desc->vRotate.y,pObject_Desc->vRotate.z);

    if (pObject_Desc->vRotate != nullptr)
    m_pTransformCom->Scaling(pObject_Desc->vScale.x, pObject_Desc->vScale.y, pObject_Desc->vScale.z);

    m_iTextureIndex = pObject_Desc->iTextureIndex;


    return S_OK;
}

void CPotal::Priority_Update(_float fTimeDelta)
{

}

void CPotal::Update(_float fTimeDelta)
{
    //if (m_pAnimatorCom->Get_CurStateTag() == L"Potal_Start")
    //{
    //    m_pAnimatorCom->Change_State(L"Potal_Idle");
    //}
}

void CPotal::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_PRIORITY, this);
}

HRESULT CPotal::Render()
{
    SetUp_RenderState();

    m_pTransformCom->Bind_Matrix();

    m_pAnimatorCom->Update_State();

    m_pVIBufferCom->Bind_Buffers();

    m_pVIBufferCom->Render();

    Reset_RenderState();

    return S_OK;
}

void CPotal::SetUp_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 50);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
}

void CPotal::Reset_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

    m_pGraphic_Device->SetTexture(0, NULL);
}

HRESULT CPotal::Ready_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    
    if (m_eObjType == GAMEOBJ_TYPE::POTAL || m_eObjType == GAMEOBJ_TYPE::END_POTAL)
    {
        if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Potal_Idle"),
            TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
            return E_FAIL;
    }
    else if(m_eObjType == GAMEOBJ_TYPE::STAGE_POTAL)
    {
        if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Boss_Potal"),
            TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
            return E_FAIL;
    }

    CTransform::TRANSFORM_DESC TransformDesc{};
    TransformDesc.fSpeedPerSec = 5.f;
    TransformDesc.fRotationPerSec = D3DXToRadian(90.f);

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
        return E_FAIL;

    CAnimator::ANIMSTATE_DESC StartAnimStateDesc{};
    StartAnimStateDesc.strTimerTag = L"Animator_Potal";
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Animator"),
        TEXT("Com_Animator"), reinterpret_cast<CComponent**>(&m_pAnimatorCom), &StartAnimStateDesc)))
        return E_FAIL;

    m_pAnimatorCom->Add_State(L"Potal_Idle", { m_pTextureCom, 8, false });

    return S_OK;
}

CPotal* CPotal::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CPotal* pInstance = new CPotal(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failde to Created : CBackGround"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CPotal::Clone(void* pArg)
{
    CPotal* pInstance = new CPotal(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failde to Cloned : CPotal"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CPotal::Free()
{
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTransformCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pAnimatorCom);
    __super::Free();
}

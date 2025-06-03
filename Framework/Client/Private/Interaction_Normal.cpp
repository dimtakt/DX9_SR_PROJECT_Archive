#include "Interaction_Normal.h"

#include "GameInstance.h"
#include "Client_Struct.h"

CInteraction_Normal::CInteraction_Normal(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CGameObject{ pGraphic_Device }
{
}

CInteraction_Normal::CInteraction_Normal(const CInteraction_Normal& Prototype)
    : CGameObject{ Prototype }
{
}

HRESULT CInteraction_Normal::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CInteraction_Normal::Initialize(void* pArg)
{
    MAP_OBJECT_DESC* pObject_Desc = static_cast<MAP_OBJECT_DESC*>(pArg);
    m_eObjType = pObject_Desc->eType;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    if (pArg == nullptr)
    {
        m_pTransformCom->Set_State(STATE::POSITION, _float3(0.f, 0.f, 0.f));
        return S_OK;
    }

    /*MAP_OBJECT_DESC* pObject_Desc = static_cast<MAP_OBJECT_DESC*>(pArg);

    m_pTransformCom->Set_State(STATE::POSITION, pObject_Desc->vPos);*/

    m_pTransformCom->Set_State(STATE::POSITION, pObject_Desc->vPos);

    //X축 회전
    /*if (pObject_Desc->vRotate.x != 0.f || pObject_Desc->vRotate.y != 0.f || pObject_Desc->vRotate.z != 0.f)
        m_pTransformCom->ApplyEulerRotation(pObject_Desc->vRotate);

    m_pTransformCom->Set_RotationEuler(pObject_Desc->vRotate.x, pObject_Desc->vRotate.y, pObject_Desc->vRotate.z);

    m_pTransformCom->Scaling(pObject_Desc->vScale.x, pObject_Desc->vScale.y, pObject_Desc->vScale.z);*/

    m_pTransformCom->Scaling(pObject_Desc->vScale.x, pObject_Desc->vScale.y, pObject_Desc->vScale.z);

    /*m_iTextureIndex = pObject_Desc->iTextureIndex;
    m_pTextureCom->Bind_Texture(m_iTextureIndex);*/
    
    if (pObject_Desc->eType == GAMEOBJ_TYPE::EXP)
        EXP_Initialize();
    else if (pObject_Desc->eType == GAMEOBJ_TYPE::GOLD)
        Gold_Initialize();
    else if (pObject_Desc->eType == GAMEOBJ_TYPE::HP)
        HP_Initialize();
    else if (pObject_Desc->eType == GAMEOBJ_TYPE::STONE_TABLET)
        Stone_Tablet_Initialize();
    else if (pObject_Desc->eType == GAMEOBJ_TYPE::ATIFACT)
        Atifact_Initialize();
    else
        Merchant_Initialize();
    

    return S_OK;
}

void CInteraction_Normal::Priority_Update(_float fTimeDelta)
{
    if (m_eObjType == GAMEOBJ_TYPE::EXP)
        EXP_Priority_Update(fTimeDelta);
    else if (m_eObjType == GAMEOBJ_TYPE::GOLD)
        Gold_Priority_Update(fTimeDelta);
    else if (m_eObjType == GAMEOBJ_TYPE::HP)
        HP_Priority_Update(fTimeDelta);
    else if (m_eObjType == GAMEOBJ_TYPE::STONE_TABLET)
        Stone_Tablet_Priority_Update(fTimeDelta);
    else if (m_eObjType == GAMEOBJ_TYPE::ATIFACT)
        Atifact_Priority_Update(fTimeDelta);
    else
        Merchant_Priority_Update(fTimeDelta);

}

void CInteraction_Normal::Update(_float fTimeDelta)
{
    if (m_eObjType == GAMEOBJ_TYPE::EXP)
        EXP_Update(fTimeDelta);
    else if (m_eObjType == GAMEOBJ_TYPE::GOLD)
        Gold_Update(fTimeDelta);
    else if (m_eObjType == GAMEOBJ_TYPE::HP)
        HP_Update(fTimeDelta);
    else if (m_eObjType == GAMEOBJ_TYPE::STONE_TABLET)
        Stone_Tablet_Update(fTimeDelta);
    else if (m_eObjType == GAMEOBJ_TYPE::ATIFACT)
        Atifact_Update(fTimeDelta);
    else
        Merchant_Update(fTimeDelta);
}

void CInteraction_Normal::Late_Update(_float fTimeDelta)
{
    if (m_eObjType == GAMEOBJ_TYPE::EXP)
        EXP_Late_Update(fTimeDelta);
    else if (m_eObjType == GAMEOBJ_TYPE::GOLD)
        Gold_Late_Update(fTimeDelta);
    else if (m_eObjType == GAMEOBJ_TYPE::HP)
        HP_Late_Update(fTimeDelta);
    else if (m_eObjType == GAMEOBJ_TYPE::STONE_TABLET)
        Stone_Tablet_Late_Update(fTimeDelta);
    else if (m_eObjType == GAMEOBJ_TYPE::ATIFACT)
        Atifact_Late_Update(fTimeDelta);
    else
        Merchant_Late_Update(fTimeDelta);

    m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_PRIORITY, this);
}

HRESULT CInteraction_Normal::Render()
{
    SetUp_RenderState();

    if (m_eObjType == GAMEOBJ_TYPE::EXP)
        EXP_Render();
    else if (m_eObjType == GAMEOBJ_TYPE::GOLD)
        Gold_Render();
    else if (m_eObjType == GAMEOBJ_TYPE::HP)
        HP_Render();
    else if (m_eObjType == GAMEOBJ_TYPE::STONE_TABLET)
        Stone_Tablet_Render();
    else if (m_eObjType == GAMEOBJ_TYPE::ATIFACT)
        Atifact_Render();
    else
        Merchant_Render();

    Reset_RenderState();

    return S_OK;
}

void CInteraction_Normal::SetUp_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
}

void CInteraction_Normal::Reset_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

    m_pGraphic_Device->SetTexture(0, NULL);
}

#pragma region 경험치 뭉치

HRESULT CInteraction_Normal::EXP_Initialize()
{
    return S_OK;
}

HRESULT CInteraction_Normal::EXP_Priority_Update(_float fTimeDelta)
{
    return S_OK;
}

HRESULT CInteraction_Normal::EXP_Update(_float fTimeDelta)
{
    return S_OK;
}

HRESULT CInteraction_Normal::EXP_Late_Update(_float fTimeDelta)
{
    return S_OK;
}

HRESULT CInteraction_Normal::EXP_Render()
{
    m_pTransformCom->Bind_Matrix();

    m_pVIBufferCom_0->Bind_Buffers();

    m_pAnimatorCom_0->Update_State();
    m_pVIBufferCom_0->Render();

    return S_OK;
}

HRESULT CInteraction_Normal::EXP_Component()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom_0))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer_1"), reinterpret_cast<CComponent**>(&m_pVIBufferCom_1))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
        return E_FAIL;

    /* For.Com_Texture */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_EXP_Big"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom_0))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_EXP_Big_Fx"),
        TEXT("Com_Texture_1"), reinterpret_cast<CComponent**>(&m_pTextureCom_2))))
        return E_FAIL;

    

    CAnimator::ANIMSTATE_DESC StartAnimStateDesc_0{};
    StartAnimStateDesc_0.strTimerTag = L"Animator_EXP";
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Animator"),
        TEXT("Com_Animator"), reinterpret_cast<CComponent**>(&m_pAnimatorCom_0), &StartAnimStateDesc_0)))
        return E_FAIL;

    m_pAnimatorCom_0->Add_State(L"EXP_big", { m_pTextureCom_0, 10, true });
    m_pAnimatorCom_0->Add_State(L"EXP_Fx", { m_pTextureCom_2, 3, true });
    

    return S_OK;
}
#pragma endregion

#pragma region 골드

HRESULT CInteraction_Normal::Gold_Initialize()
{
    return S_OK;
}

HRESULT CInteraction_Normal::Gold_Priority_Update(_float fTimeDelta)
{
    return S_OK;
}

HRESULT CInteraction_Normal::Gold_Update(_float fTimeDelta)
{
    return S_OK;
}

HRESULT CInteraction_Normal::Gold_Late_Update(_float fTimeDelta)
{
    return S_OK;
}

HRESULT CInteraction_Normal::Gold_Render()
{
    m_pTransformCom->Bind_Matrix();
    if (FAILED(m_pTextureCom_0->Bind_Texture(0)))
        return E_FAIL;
    m_pVIBufferCom_0->Bind_Buffers();

    m_pVIBufferCom_0->Render();

    return S_OK;
}

HRESULT CInteraction_Normal::Gold_Component()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom_0))))
        return E_FAIL;

    /* For.Com_Texture */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Gold"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom_0))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
        return E_FAIL;

    CAnimator::ANIMSTATE_DESC StartAnimStateDesc{};
    StartAnimStateDesc.strTimerTag = L"Animator_Gold";
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Animator"),
        TEXT("Com_Animator"), reinterpret_cast<CComponent**>(&m_pAnimatorCom_0), &StartAnimStateDesc)))
        return E_FAIL;

    m_pAnimatorCom_0->Add_State(L"Gold", { m_pTextureCom_0, 1, true });

    return S_OK;
}
#pragma endregion

#pragma region HP
HRESULT CInteraction_Normal::HP_Initialize()
{
    return S_OK;
}

HRESULT CInteraction_Normal::HP_Priority_Update(_float fTimeDelta)
{
    return S_OK;
}

HRESULT CInteraction_Normal::HP_Update(_float fTimeDelta)
{
    return S_OK;
}

HRESULT CInteraction_Normal::HP_Late_Update(_float fTimeDelta)
{
    return S_OK;
}

HRESULT CInteraction_Normal::HP_Render()
{
    m_pTransformCom->Bind_Matrix();

    m_pVIBufferCom_0->Bind_Buffers();

    m_pAnimatorCom_0->Update_State();
    m_pVIBufferCom_0->Render();

    return S_OK;
}

HRESULT CInteraction_Normal::HP_Component()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom_0))))
        return E_FAIL;

    /* For.Com_Texture */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_HP"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom_0))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
        return E_FAIL;

    CAnimator::ANIMSTATE_DESC StartAnimStateDesc{};
    StartAnimStateDesc.strTimerTag = L"Animator_HP";
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Animator"),
        TEXT("Com_Animator"), reinterpret_cast<CComponent**>(&m_pAnimatorCom_0), &StartAnimStateDesc)))
        return E_FAIL;

    m_pAnimatorCom_0->Add_State(L"HP", { m_pTextureCom_0, 3, true });

    return S_OK;
}
#pragma endregion

#pragma region 석판
HRESULT CInteraction_Normal::Stone_Tablet_Initialize()
{
    return S_OK;
}

HRESULT CInteraction_Normal::Stone_Tablet_Priority_Update(_float fTimeDelta)
{
    return S_OK;
}

HRESULT CInteraction_Normal::Stone_Tablet_Update(_float fTimeDelta)
{
    return S_OK;
}

HRESULT CInteraction_Normal::Stone_Tablet_Late_Update(_float fTimeDelta)
{
    return S_OK;
}

HRESULT CInteraction_Normal::Stone_Tablet_Render()
{
    m_pTransformCom->Bind_Matrix();

    m_pVIBufferCom_0->Bind_Buffers();

    m_pAnimatorCom_0->Update_State();
    m_pVIBufferCom_0->Render();

    return S_OK;
}

HRESULT CInteraction_Normal::Stone_Tablet_Component()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom_0))))
        return E_FAIL;

    /* For.Com_Texture */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Stone_Tablet"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom_0))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
        return E_FAIL;

    CAnimator::ANIMSTATE_DESC StartAnimStateDesc{};
    StartAnimStateDesc.strTimerTag = L"Animator_Stone_Tablet";
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Animator"),
        TEXT("Com_Animator"), reinterpret_cast<CComponent**>(&m_pAnimatorCom_0), &StartAnimStateDesc)))
        return E_FAIL;

    m_pAnimatorCom_0->Add_State(L"Stone_Tablet", { m_pTextureCom_0, 3, true });
    return S_OK;
}
#pragma endregion

#pragma region 아티팩트
HRESULT CInteraction_Normal::Atifact_Initialize()
{
    return S_OK;
}

HRESULT CInteraction_Normal::Atifact_Priority_Update(_float fTimeDelta)
{
    return S_OK;
}

HRESULT CInteraction_Normal::Atifact_Update(_float fTimeDelta)
{
    return S_OK;
}

HRESULT CInteraction_Normal::Atifact_Late_Update(_float fTimeDelta)
{
    return S_OK;
}

HRESULT CInteraction_Normal::Atifact_Render()
{
    m_pTransformCom->Bind_Matrix();

    m_pVIBufferCom_0->Bind_Buffers();

    m_pAnimatorCom_0->Update_State();
    m_pVIBufferCom_0->Render();

    return S_OK;
}

HRESULT CInteraction_Normal::Atifact_Component()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom_0))))
        return E_FAIL;

    /* For.Com_Texture */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Atifact"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom_0))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
        return E_FAIL;

    CAnimator::ANIMSTATE_DESC StartAnimStateDesc{};
    StartAnimStateDesc.strTimerTag = L"Animator_Atifact";
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Animator"),
        TEXT("Com_Animator"), reinterpret_cast<CComponent**>(&m_pAnimatorCom_0), &StartAnimStateDesc)))
        return E_FAIL;

    m_pAnimatorCom_0->Add_State(L"Atifact", { m_pTextureCom_0, 3, true });
    
    return S_OK;
}
#pragma endregion

#pragma region 상인
HRESULT CInteraction_Normal::Merchant_Initialize()
{
    return S_OK;
}

HRESULT CInteraction_Normal::Merchant_Priority_Update(_float fTimeDelta)
{
    return S_OK;
}

HRESULT CInteraction_Normal::Merchant_Update(_float fTimeDelta)
{
    return S_OK;
}

HRESULT CInteraction_Normal::Merchant_Late_Update(_float fTimeDelta)
{
    return S_OK;
}

HRESULT CInteraction_Normal::Merchant_Render()
{
    m_pTransformCom->Bind_Matrix();

    m_pVIBufferCom_0->Bind_Buffers();

    m_pAnimatorCom_0->Update_State();
    m_pVIBufferCom_0->Render();

    return S_OK;
}

HRESULT CInteraction_Normal::Merchant_Component()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom_0))))
        return E_FAIL;

    /* For.Com_Texture */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Merchant"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom_0))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
        return E_FAIL;

    CAnimator::ANIMSTATE_DESC StartAnimStateDesc{};
    StartAnimStateDesc.strTimerTag = L"Animator_Merchant";
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Animator"),
        TEXT("Com_Animator"), reinterpret_cast<CComponent**>(&m_pAnimatorCom_0), &StartAnimStateDesc)))
        return E_FAIL;

    m_pAnimatorCom_0->Add_State(L"Merchant", { m_pTextureCom_0, 5, true });

    return S_OK;
}
#pragma endregion


HRESULT CInteraction_Normal::Ready_Components()
{

    if (m_eObjType == GAMEOBJ_TYPE::EXP)
    {
        if (FAILED(EXP_Component()))
            return E_FAIL;
    }
    else if (m_eObjType == GAMEOBJ_TYPE::GOLD)
    {
        if (FAILED(Gold_Component()))
            return E_FAIL;
    }
    else if (m_eObjType == GAMEOBJ_TYPE::HP)
    {
        if (FAILED(HP_Component()))
            return E_FAIL;
    }
    else if (m_eObjType == GAMEOBJ_TYPE::STONE_TABLET)
    {
        if (FAILED(Stone_Tablet_Component()))
            return E_FAIL;
    }
    else if (m_eObjType == GAMEOBJ_TYPE::ATIFACT)
    {
        if (FAILED(Atifact_Component()))
            return E_FAIL;
    }
    else
    {
        if (FAILED(Merchant_Component()))
            return E_FAIL;
    }
        

    return S_OK;

}

CInteraction_Normal* CInteraction_Normal::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CInteraction_Normal* pInstance = new CInteraction_Normal(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failde to Created : CBackGround"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CInteraction_Normal::Clone(void* pArg)
{
    CInteraction_Normal* pInstance = new CInteraction_Normal(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failde to Cloned : CInteraction_Normal"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CInteraction_Normal::Free()
{
    __super::Free();

    Safe_Release(m_pVIBufferCom_0);
    Safe_Release(m_pVIBufferCom_1);
    Safe_Release(m_pTransformCom);
    Safe_Release(m_pTextureCom_0);
    Safe_Release(m_pTextureCom_1);
    Safe_Release(m_pTextureCom_2);
    Safe_Release(m_pAnimatorCom_0);
    Safe_Release(m_pAnimatorCom_1);

}

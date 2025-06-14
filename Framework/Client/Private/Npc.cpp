#include "Npc.h"

#include "GameInstance.h"
#include "Client_Struct.h"


CNpc::CNpc(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CGameObject{ pGraphic_Device }
{
}

CNpc::CNpc(const CNpc& Prototype)
    : CGameObject{ Prototype }
{
}

HRESULT CNpc::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CNpc::Initialize(void* pArg)
{
    NPCDESC* desc = static_cast<NPCDESC*>(pArg);
    m_eType = desc->eType;
    m_eObjType = GAMEOBJ_TYPE::NPC;
    if (FAILED(Ready_Components()))
        return E_FAIL;

    

    CField_Npc_Chat::FIELD_CHAT_DESC chatDesc{};

    chatDesc.pTransform = m_pTransformCom;
    
    
    chatDesc.fY = -100;


    

    if (m_eType == NPC_TYPE::BLACKSMITH)
    {
        m_pTransformCom->Scaling(3.f, 3.f, 3.f);
        m_pTransformCom->Set_State(STATE::POSITION, _float3(6.f, 1.8f, -7.3f));
        chatDesc.m_iLevel = ENUM_CLASS(LEVEL::LEVEL_TOWN);
        chatDesc.szChatTag = TEXT("BlackSmith_CHAT");
        if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_TOWN), TEXT("Layer_UI_Chat"), ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_UI_Field_Npc_Chat"), &chatDesc)))
            return E_FAIL;

        m_pChat = static_cast<CField_Npc_Chat*>(m_pGameInstance->Find_UIObj(ENUM_CLASS(LEVEL::LEVEL_TOWN), TEXT("BlackSmith_CHAT")));

        m_pChat->Add_Chat(TEXT("에베베"));
        m_pChat->Add_Chat(TEXT("스미스"));
    }
    else if (m_eType == NPC_TYPE::ELDER)
    {
        m_pTransformCom->Scaling(3.f, 3.f, 3.f);
        m_pTransformCom->Set_State(STATE::POSITION, _float3(5.5f, 1.8f, 1.0f));
        chatDesc.m_iLevel = ENUM_CLASS(LEVEL::LEVEL_TOWN);
        chatDesc.szChatTag = TEXT("Elder_CHAT");
        if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_TOWN), TEXT("Layer_UI_Chat"), ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_UI_Field_Npc_Chat"), &chatDesc)))
            return E_FAIL;

        m_pChat = static_cast<CField_Npc_Chat*>(m_pGameInstance->Find_UIObj(ENUM_CLASS(LEVEL::LEVEL_TOWN), TEXT("Elder_CHAT")));
        m_pChat->Add_Chat(TEXT("에베베"));
        m_pChat->Add_Chat(TEXT("장로"));
    }
    else if (m_eType == NPC_TYPE::SHIELDDOG)
    {
        m_pTransformCom->Scaling(3.f, 3.f, 3.f);
        m_pTransformCom->Set_State(STATE::POSITION, _float3(-5.2f, 1.8f, 2.7f));
        chatDesc.m_iLevel = ENUM_CLASS(LEVEL::LEVEL_SHELTER);
        chatDesc.szChatTag = TEXT("ShieldDog_CHAT");
        if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_SHELTER), TEXT("Layer_UI_Chat"), ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_UI_Field_Npc_Chat"), &chatDesc)))
            return E_FAIL;

        m_pChat = static_cast<CField_Npc_Chat*>(m_pGameInstance->Find_UIObj(ENUM_CLASS(LEVEL::LEVEL_SHELTER), TEXT("ShieldDog_CHAT")));
        m_pChat->Add_Chat(TEXT("에베베"));
        m_pChat->Add_Chat(TEXT("갑옷개"));
    }
    else if (m_eType == NPC_TYPE::SWORDSHIELD)
    {
        m_pTransformCom->Scaling(3.f, 3.f, 3.f);
        m_pTransformCom->Set_State(STATE::POSITION, _float3(-5.5f, 1.8f, -3.5f));
        chatDesc.m_iLevel = ENUM_CLASS(LEVEL::LEVEL_TOWN);
        chatDesc.szChatTag = TEXT("SwordShield_CHAT");
        if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_TOWN), TEXT("Layer_UI_Chat"), ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_UI_Field_Npc_Chat"), &chatDesc)))
            return E_FAIL;

        m_pChat = static_cast<CField_Npc_Chat*>(m_pGameInstance->Find_UIObj(ENUM_CLASS(LEVEL::LEVEL_TOWN), TEXT("SwordShield_CHAT")));
        m_pChat->Add_Chat(TEXT("에베베"));
        m_pChat->Add_Chat(TEXT("검방패"));
    }

    CCollider_OBB::OBB_DESC tColliderDesc;
    tColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
    tColliderDesc.pOwner = this;
    tColliderDesc.pTransform = m_pTransformCom;
    tColliderDesc.eType = m_eObjType;
    CCollider_OBB* pCol = dynamic_cast<CCollider_OBB*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::COMPONENT, ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Collider_OBB"), &tColliderDesc));
    m_pGameInstance->Add_Collider(pCol);
    

    return S_OK;
}

void CNpc::Priority_Update(_float fTimeDelta)
{

}

void CNpc::Update(_float fTimeDelta)
{

}

void CNpc::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_PRIORITY, this);
}

HRESULT CNpc::Render()
{
    SetUp_RenderState();

    m_pTransformCom->Bind_Matrix();

    m_pAnimatorCom->Update_State();

    m_pVIBufferCom->Bind_Buffers();

    m_pVIBufferCom->Render();

    Reset_RenderState();

    return S_OK;
}

void CNpc::SetUp_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
}

void CNpc::Reset_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

    m_pGraphic_Device->SetTexture(0, NULL);
}


HRESULT CNpc::Ready_Components()
{

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    CTransform::TRANSFORM_DESC TransformDesc{};
    TransformDesc.fSpeedPerSec = 5.f;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
        return E_FAIL;

    
    CAnimator::ANIMSTATE_DESC StartAnimStateDesc{};
    StartAnimStateDesc.strTimerTag = L"Animator_Npc";
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Animator"),
        TEXT("Com_Animator"), reinterpret_cast<CComponent**>(&m_pAnimatorCom), &StartAnimStateDesc)))
        return E_FAIL;

    

    if (m_eType == NPC_TYPE::BLACKSMITH)
    {
        /* For.Com_Texture */
        if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_TOWN), TEXT("Prototype_Component_Texture_BlackSmith"),
            TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
            return E_FAIL;
        m_pAnimatorCom->Add_State(L"BlackSmith", { m_pTextureCom, 4, true });
    }
    else if (m_eType == NPC_TYPE::ELDER)
    {
        /* For.Com_Texture */
        if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_TOWN), TEXT("Prototype_Component_Texture_Elder"),
            TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
            return E_FAIL;
        m_pAnimatorCom->Add_State(L"Elder", { m_pTextureCom, 4, true });
    }
    else if (m_eType == NPC_TYPE::SHIELDDOG)
    {
        /* For.Com_Texture */
        if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_SHELTER), TEXT("Prototype_Component_Texture_ShieldDog"),
            TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
            return E_FAIL;
        m_pAnimatorCom->Add_State(L"ShieldDog", { m_pTextureCom, 4, true });
    }   
    else if (m_eType == NPC_TYPE::SWORDSHIELD)
    {
        /* For.Com_Texture */
        if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_TOWN), TEXT("Prototype_Component_Texture_SwordShield"),
            TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
            return E_FAIL;
        m_pAnimatorCom->Add_State(L"SwordShield", { m_pTextureCom, 4, true });
    }
    

    return S_OK;

}

void CNpc::OnCollision(CGameObject* pGameObject)
{
    if(pGameObject->Get_ObjType() == GAMEOBJ_TYPE::PLAYER)
    {
        if (m_eType == NPC_TYPE::BLACKSMITH)
        {
            m_pChat->On_Chat(2, true);
            if (m_pGameInstance->IsKeyDown('F'))
            {
                m_pChat->Cinematic_Chat(2, true);
            }
        }
        else if (m_eType == NPC_TYPE::ELDER)
        {
            m_pChat->On_Chat(3, true);
            if (m_pGameInstance->IsKeyDown('F'))
            {
                m_pChat->Cinematic_Chat(3, true);
            }
        }
        else if (m_eType == NPC_TYPE::SHIELDDOG)
        {
            m_pChat->On_Chat(0, true);
            if (m_pGameInstance->IsKeyDown('F'))
            {
                m_pChat->Cinematic_Chat(0, true);
            }
        }
        else if (m_eType == NPC_TYPE::SWORDSHIELD)
        {
            m_pChat->On_Chat(4, true);
            if (m_pGameInstance->IsKeyDown('F'))
            {
                m_pChat->Cinematic_Chat(4, true);
            }
        }
    }
    
}

void CNpc::OffCollision(CGameObject* pGameObject)
{
    if (pGameObject->Get_ObjType() == GAMEOBJ_TYPE::PLAYER)
    {
        //m_pChat->Off_Chat();
    }
}

CNpc* CNpc::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CNpc* pInstance = new CNpc(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failde to Created : CBackGround"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CNpc::Clone(void* pArg)
{
    CNpc* pInstance = new CNpc(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failde to Cloned : CNpc"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CNpc::Free()
{
    __super::Free();

    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTransformCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pAnimatorCom);
    Safe_Release(m_pChat);
}

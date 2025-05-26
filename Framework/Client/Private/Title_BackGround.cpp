#include "Title_BackGround.h"
#include "GameInstance.h"
#include "Animator.h"
#include "Title_Tree.h"
#include "Title_Logo.h"
#include "Title_Star.h"
#include "Title_Logo_Wave.h"

CTitle_BackGround::CTitle_BackGround(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CUIObject{ pGraphic_Device }
{
}

CTitle_BackGround::CTitle_BackGround(const CTitle_BackGround& Prototype)
    : CUIObject{ Prototype }
    , m_eLevel{ Prototype.m_eLevel }
{
}

HRESULT CTitle_BackGround::Initialize_Prototype(LEVEL eLevel)
{
    m_eLevel = eLevel;

    if (FAILED(Ready_ChildPrototype(eLevel)))
        return E_FAIL;
     
    return S_OK;
}

HRESULT CTitle_BackGround::Initialize(void* pArg)
{
    m_fSizeX = g_iWinSizeX*3;
    m_fSizeY = g_iWinSizeY*5;
    m_fX = g_iWinSizeX >> 1;
    m_fY = 330.f;
    m_fZ = 0.1f;
    m_iWinSizeX = g_iWinSizeX;
    m_iWinSizeY = g_iWinSizeY;

    if (FAILED(__super::Initialize()))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
     __super::Update_Position();

    if (FAILED(Ready_Children()))
        return E_FAIL;

    return S_OK;
}

void CTitle_BackGround::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

void CTitle_BackGround::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);
}

void CTitle_BackGround::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
    __super::Late_Update(fTimeDelta);
}

HRESULT CTitle_BackGround::Render()
{
    if (FAILED(m_pTextureCom_Title_Back->Bind_Texture(0)))
        return E_FAIL;

    m_pVIBufferCom->Bind_Buffers();

    __super::Begin();

    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", m_pTransformCom->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;

    m_pTextureCom_Title_Back->Bind_Texture(m_pShaderCom, "g_Texture", 0);

    m_pShaderCom->Begin(0);

    _float fTime = static_cast<_float>(GetTickCount()) * 0.001f;
    m_pShaderCom->Set_Float("g_Time", fTime);

    m_pVIBufferCom->Render();

    m_pShaderCom->End();

    __super::End();
    return S_OK;
}

HRESULT CTitle_BackGround::Ready_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
         return E_FAIL;

    /* For.Com_Texture */

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_LOGO), TEXT("Prototype_Component_Texture_Title_Back"),
        TEXT("Com_Texture_Title_Back"), reinterpret_cast<CComponent**>(&m_pTextureCom_Title_Back))))
        return E_FAIL;


    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform_Title_BackGround"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
        return E_FAIL;

    /* For.Com_Shader */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_LOGO), TEXT("Prototype_Component_Shader_TitleFadeIn"),
        TEXT("Com_Shader_Title_Star"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    return S_OK;
}

HRESULT CTitle_BackGround::Ready_ChildPrototype(LEVEL eLevel)
{
    if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_Title_Tree"),
        CTitle_Tree::Create(m_pGraphic_Device))))
        return E_FAIL;

    if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_Title_Logo"),
        CTitle_Logo::Create(m_pGraphic_Device))))
        return E_FAIL;

    if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_Title_Star"),
        CTitle_Star::Create(m_pGraphic_Device))))
        return E_FAIL;

    if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_Title_Logo_Wave"),
        CTitle_Logo_Wave::Create(m_pGraphic_Device))))
        return E_FAIL;

    return S_OK;
}

HRESULT CTitle_BackGround::Ready_Children()
{
    CUIObject* pGameObject = nullptr;

    pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Title_Star")));
    if (nullptr == pGameObject)
        return E_FAIL;
    Add_Child(pGameObject);
    
    pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Title_Tree")));
    if (nullptr == pGameObject)
        return E_FAIL;
    Add_Child(pGameObject);

    pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Title_Logo")));
    if (nullptr == pGameObject)
        return E_FAIL;
    Add_Child(pGameObject);

    pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Title_Logo_Wave")));
    if (nullptr == pGameObject)
        return E_FAIL;
    Add_Child(pGameObject);

    return S_OK;
}

CTitle_BackGround* CTitle_BackGround::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
    CTitle_BackGround* pInstance = new CTitle_BackGround(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype(eLevel)))
    {
        MSG_BOX(TEXT("Failde to Created : CTitle_BackGround"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CTitle_BackGround::Clone(void* pArg)
{
    CTitle_BackGround* pInstance = new CTitle_BackGround(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failde to Cloned : CTitle_BackGround"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CTitle_BackGround::Free()
{
    __super::Free();

    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom_Title_Back);
    Safe_Release(m_pTextureCom_Title_ReverseTree);
}

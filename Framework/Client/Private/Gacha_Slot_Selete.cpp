#include "Gacha_Slot_Selete.h"
#include "GameInstance.h"
CGacha_Slot_Selete::CGacha_Slot_Selete(LPDIRECT3DDEVICE9 pGraphic_Device) : CButton{ pGraphic_Device }
{
}

CGacha_Slot_Selete::CGacha_Slot_Selete(const CGacha_Slot_Selete& Prototype) : CButton(Prototype), m_eLevel(Prototype.m_eLevel), m_bIsOver(false)
{
}

HRESULT CGacha_Slot_Selete::Initialize_Prototype(LEVEL eLevel)
{
    m_eLevel = eLevel;

    if (FAILED(Ready_ChildPrototype(eLevel)))
        return E_FAIL;

    return S_OK;
}

HRESULT CGacha_Slot_Selete::Initialize(void* pArg)
{
    CUIObject::UIOBJECT_DESC* Desc = static_cast<UIOBJECT_DESC*>(pArg);

    m_fSizeX = 86;
    m_fSizeY = 86;
    m_fX = 0.f;
    m_fY = 4.f;
    m_fZ = UI_DEPTH::INVEN_SLOT_SELETE;
    m_iWinSizeX = g_iWinSizeX;
    m_iWinSizeY = g_iWinSizeY;

    if (FAILED(CUIObject::Initialize()))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
    __super::Update_Position();

    if (FAILED(Ready_Children()))
        return E_FAIL;

    return S_OK;
}

void CGacha_Slot_Selete::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

void CGacha_Slot_Selete::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);
}

void CGacha_Slot_Selete::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
    __super::Late_Update(fTimeDelta);
}

HRESULT CGacha_Slot_Selete::Render()
{
    if (FAILED(CButton::Bind_ButtonTex_Single(g_hWnd, 4)))
        return E_FAIL;
    return S_OK;
}

HRESULT CGacha_Slot_Selete::Ready_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Rect_GachaSlot"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    return S_OK;
}

HRESULT CGacha_Slot_Selete::Ready_ChildPrototype(LEVEL eLevel)
{
    return S_OK;
}

HRESULT CGacha_Slot_Selete::Ready_Children()
{
    return S_OK;
}

CGacha_Slot_Selete* CGacha_Slot_Selete::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
    CGacha_Slot_Selete* pInstance = new CGacha_Slot_Selete(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype(eLevel)))
    {
        MSG_BOX(TEXT("Failed to Created : CGacha_Slot_Selete"));
        Safe_Release(pInstance);
    }
    return pInstance;
}

CGameObject* CGacha_Slot_Selete::Clone(void* pArg)
{
    CGacha_Slot_Selete* pInstance = new CGacha_Slot_Selete(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Clone : CGacha_Slot_Selete"));
        Safe_Release(pInstance);
    }
    return pInstance;
}

void CGacha_Slot_Selete::Free()
{
    __super::Free();
}

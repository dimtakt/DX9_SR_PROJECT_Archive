#include "Hud_Exp_Checkout.h"
#include "GameInstance.h"

CHud_Exp_Checkout::CHud_Exp_Checkout(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CHud_Exp_Checkout::CHud_Exp_Checkout(const CHud_Exp_Checkout& Prototype) : CUIObject(Prototype)
{
}

HRESULT CHud_Exp_Checkout::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CHud_Exp_Checkout::Initialize(void* pArg)
{
    UIHUD_CHECKOUT_DESC* DescMy = static_cast<UIHUD_CHECKOUT_DESC*>(pArg);

    UIOBJECT_DESC Desc {};


    Desc.fSizeX = 3;
    Desc.fSizeY = 9;
    
    if (DescMy->fX <= -(g_iWinSizeX * 0.5))
        Desc.fX = (Desc.fSizeX * 0.5) - (g_iWinSizeX * 0.5);
    else if (DescMy->fX >= g_iWinSizeX * 0.5)
        Desc.fX = (g_iWinSizeX * 0.5) - (Desc.fSizeX * 0.5);
    else
        Desc.fX = DescMy->fX;

    Desc.fY = 0.f;
    Desc.fZ = 0.f;
    Desc.iWinSizeX = g_iWinSizeX;
    Desc.iWinSizeY = g_iWinSizeY;

    if (FAILED(__super::Initialize(&Desc)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
    __super::Update_Position();

    return S_OK;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    return S_OK;
}

void CHud_Exp_Checkout::Priority_Update(_float fTimeDelta)
{
}

void CHud_Exp_Checkout::Update(_float fTimeDelta)
{
}

void CHud_Exp_Checkout::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CHud_Exp_Checkout::Render()
{
    m_pTransformCom->Bind_Matrix();
    m_pVIBufferCom->Bind_Buffers();

    __super::Begin();
    m_pVIBufferCom->Render();
    __super::End();

    return S_OK;
}

HRESULT CHud_Exp_Checkout::Ready_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Rect_UI_Hud_Exp_Fream_1"),
		TEXT("Com_VIBuffer1"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
        return E_FAIL;

    return S_OK;
}

CHud_Exp_Checkout* CHud_Exp_Checkout::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CHud_Exp_Checkout* pInstance = new CHud_Exp_Checkout(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CHud_Exp_Checkout"));
        Safe_Release(pInstance);
    }
    return pInstance;
}

CGameObject* CHud_Exp_Checkout::Clone(void* pArg)
{
    CHud_Exp_Checkout* pInstance = new CHud_Exp_Checkout(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Clone : CHud_Exp_Checkout"));
        Safe_Release(pInstance);
    }
    return pInstance;
}

void CHud_Exp_Checkout::Free()
{
    __super::Free();
    Safe_Release(m_pVIBufferCom);
}

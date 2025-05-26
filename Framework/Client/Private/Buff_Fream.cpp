#include "Buff_Fream.h"
#include "GameInstance.h"

CBuff_Fream::CBuff_Fream(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CBuff_Fream::CBuff_Fream(const CBuff_Fream& Prototype) : CUIObject(Prototype)
{
}

HRESULT CBuff_Fream::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CBuff_Fream::Initialize(void* pArg)
{
    UIOBJECT_DESC Desc{};
    Desc.fSizeX = 32;
    Desc.fSizeY = 32;
    Desc.fX = 0;
    Desc.fY = 0;
    Desc.iWinSizeX = g_iWinSizeX;
    Desc.iWinSizeY = g_iWinSizeY;

    if (FAILED(CUIObject::Initialize(&Desc)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
    __super::Update_Position();

    return S_OK;
}

void CBuff_Fream::Priority_Update(_float fTimeDelta)
{
}

void CBuff_Fream::Update(_float fTimeDelta)
{
}

void CBuff_Fream::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CBuff_Fream::Render()
{
    SetUp_RenderState();

    m_pTransformCom->Bind_Matrix();
    if (FAILED(m_pTextureCom->Bind_Texture(0)))
        return E_FAIL;
    m_pVIBufferCom->Bind_Buffers();

    __super::Begin();
    m_pVIBufferCom->Render();
    __super::End();

    Reset_RenderState();
    return S_OK;
}

HRESULT CBuff_Fream::Ready_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_Texture_Rect_UI_Hud_Buff_Fream"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    return S_OK;
}

void CBuff_Fream::SetUp_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
}

void CBuff_Fream::Reset_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

    m_pGraphic_Device->SetTexture(0, NULL);
}

CBuff_Fream* CBuff_Fream::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CBuff_Fream* pInstance = new CBuff_Fream(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CBuff_Fream"));
        Safe_Release(pInstance);
    }
    return pInstance;
}

CGameObject* CBuff_Fream::Clone(void* pArg)
{
    CBuff_Fream* pInstance = new CBuff_Fream(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Clone : CBuff_Fream"));
        Safe_Release(pInstance);
    }
    return pInstance;
}

void CBuff_Fream::Free()
{
    __super::Free();
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
}

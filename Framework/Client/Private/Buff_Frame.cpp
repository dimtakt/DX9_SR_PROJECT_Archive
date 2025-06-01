#include "Buff_Frame.h"
#include "GameInstance.h"

CBuff_Frame::CBuff_Frame(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CBuff_Frame::CBuff_Frame(const CBuff_Frame& Prototype) : CUIObject(Prototype)
{
}

HRESULT CBuff_Frame::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CBuff_Frame::Initialize(void* pArg)
{
    m_fSizeX = 32;
    m_fSizeY = 32;
    m_fX = 0;
    m_fY = 0;
    m_fZ = UI_DEPTH::HUD_BUFF;
    m_iWinSizeX = g_iWinSizeX;
    m_iWinSizeY = g_iWinSizeY;

    if (FAILED(CUIObject::Initialize()))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
    __super::Update_Position();

    return S_OK;
}

void CBuff_Frame::Priority_Update(_float fTimeDelta)
{
}

void CBuff_Frame::Update(_float fTimeDelta)
{
}

void CBuff_Frame::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CBuff_Frame::Render()
{
    SetUp_RenderState();

    if (FAILED(m_pTextureCom->Bind_Texture(0)))
        return E_FAIL;
    m_pVIBufferCom->Bind_Buffers();

    __super::Begin();
    m_pVIBufferCom->Render();
    __super::End();

    Reset_RenderState();
    return S_OK;
}

HRESULT CBuff_Frame::Ready_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Rect_UI_Hud_Buff_Frame"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    return S_OK;
}

void CBuff_Frame::SetUp_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
}

void CBuff_Frame::Reset_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

    m_pGraphic_Device->SetTexture(0, NULL);
}

CBuff_Frame* CBuff_Frame::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CBuff_Frame* pInstance = new CBuff_Frame(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CBuff_Frame"));
        Safe_Release(pInstance);
    }
    return pInstance;
}

CGameObject* CBuff_Frame::Clone(void* pArg)
{
    CBuff_Frame* pInstance = new CBuff_Frame(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Clone : CBuff_Frame"));
        Safe_Release(pInstance);
    }
    return pInstance;
}

void CBuff_Frame::Free()
{
    __super::Free();
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
}

#include "ChapMap_MapSymbol.h"
#include "GameInstance.h"

CChapMap_MapSymbol::CChapMap_MapSymbol(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CChapMap_MapSymbol::CChapMap_MapSymbol(const CChapMap_MapSymbol& Prototype) : CUIObject(Prototype)
{
}

HRESULT CChapMap_MapSymbol::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CChapMap_MapSymbol::Initialize(void* pArg)
{
    MAP_EVENT_DESC* Desc = static_cast<MAP_EVENT_DESC*>(pArg);

    m_iMapTex = ENUM_CLASS(Desc->eMap_EVENT);
    m_fSizeX = 64;
    m_fSizeY = 64;

    if (Desc->iMax_Event_Count == 1)
        m_fX = 0.f;
    else if(Desc->iEvent_Count % 2 == 1)
        m_fX = -m_fSizeX * 0.5;
    else
        m_fX = m_fSizeX * 0.5;

    m_fY = -70.f;
    m_fZ = UI_DEPTH::CHATERMAP;
    m_iWinSizeX = g_iWinSizeX;
    m_iWinSizeY = g_iWinSizeY;

    if (FAILED(__super::Initialize()))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
    __super::Update_Position();

    return S_OK;
}

void CChapMap_MapSymbol::Priority_Update(_float fTimeDelta)
{
}

void CChapMap_MapSymbol::Update(_float fTimeDelta)
{
}

void CChapMap_MapSymbol::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CChapMap_MapSymbol::Render()
{
    SetUp_RenderState();

    if (FAILED(m_pTextureCom->Bind_Texture(m_iMapTex)))
        return E_FAIL;
    m_pVIBufferCom->Bind_Buffers();

    __super::Begin();
    m_pVIBufferCom->Render();
    __super::End();

    Reset_RenderState();
    return S_OK;
}

HRESULT CChapMap_MapSymbol::Ready_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_Texture_Rect_ChapMap_Node_Symbol"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    return S_OK;
}

void CChapMap_MapSymbol::SetUp_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
}

void CChapMap_MapSymbol::Reset_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

    m_pGraphic_Device->SetTexture(0, NULL);
}

CChapMap_MapSymbol* CChapMap_MapSymbol::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CChapMap_MapSymbol* pInstance = new CChapMap_MapSymbol(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Create : CChapMap_MapSymbol"));
        Safe_Release(pInstance);
    }
    return pInstance;
}

CGameObject* CChapMap_MapSymbol::Clone(void* pArg)
{
    CChapMap_MapSymbol* pInstance = new CChapMap_MapSymbol(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Clone : CChapMap_MapSymbol"));
        Safe_Release(pInstance);
    }
    return pInstance;
}

void CChapMap_MapSymbol::Free()
{
    __super::Free();

    Safe_Release(m_pTextureCom);
    Safe_Release(m_pVIBufferCom);
}

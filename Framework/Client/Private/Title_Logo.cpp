#include "Title_Logo.h"
#include "GameInstance.h"

CTitle_Logo::CTitle_Logo(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CUIObject{ pGraphic_Device }
{
}

CTitle_Logo::CTitle_Logo(const CTitle_Logo& Prototype)
    : CUIObject{ Prototype }
{
}

HRESULT CTitle_Logo::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CTitle_Logo::Initialize(void* pArg)
{
    UIOBJECT_DESC Desc{};

    Desc.fSizeX = 311.f*1.5f;
    Desc.fSizeY = 100.f*1.5f;
    Desc.fX = 0.f;
    Desc.fY = 100.f;
    Desc.fZ = 0.1f;
    Desc.iWinSizeX = g_iWinSizeX;
    Desc.iWinSizeY = g_iWinSizeY;

    if (FAILED(__super::Initialize(&Desc)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
    __super::Update_Position();

    return S_OK;
}

void CTitle_Logo::Priority_Update(_float fTimeDelta)
{

}

void CTitle_Logo::Update(_float fTimeDelta)
{

}

void CTitle_Logo::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CTitle_Logo::Render()
{
    SetUp_RenderState();

    m_pTransformCom->Bind_Matrix();

    if (FAILED(m_pTextureCom_Title_Logo->Bind_Texture(0)))
        return E_FAIL;

    m_pVIBufferCom->Bind_Buffers();

    __super::Begin();
    m_pVIBufferCom->Render();
    __super::End();

    Reset_RenderState();

    return S_OK;
}

HRESULT CTitle_Logo::Ready_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer_Tree"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    /* For.Com_Texture */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_LOGO), TEXT("Prototype_Component_Texture_Title_Logo"),
        TEXT("Com_Texture_Title"), reinterpret_cast<CComponent**>(&m_pTextureCom_Title_Logo))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform_Title_Logo"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
        return E_FAIL;

    return S_OK;
}

void CTitle_Logo::SetUp_RenderState()
{
    // 알파 블렌딩 활성화
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

    // 알파 테스트로 투명한 배경 제거
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 80);  // 이 값보다 낮은 알파는 버림
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
}

void CTitle_Logo::Reset_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
    // 색을 섞어서 처리(알파블렌딩)
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);


    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
}

CTitle_Logo* CTitle_Logo::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CTitle_Logo* pInstance = new CTitle_Logo(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failde to Created : CTitle_Logo"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CTitle_Logo::Clone(void* pArg)
{
    CTitle_Logo* pInstance = new CTitle_Logo(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failde to Cloned : CTitle_Logo"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CTitle_Logo::Free()
{
    __super::Free();

    Safe_Release(m_pVIBufferCom);
}

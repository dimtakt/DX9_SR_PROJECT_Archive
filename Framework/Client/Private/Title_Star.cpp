#include "Title_Star.h"
#include "GameInstance.h"

CTitle_Star::CTitle_Star(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CUIObject{ pGraphic_Device }
{
}

CTitle_Star::CTitle_Star(const CTitle_Star& Prototype)
    : CUIObject{ Prototype }
{
}

HRESULT CTitle_Star::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CTitle_Star::Initialize(void* pArg)
{
    m_fSizeX = 640.f * 2.5f;
    m_fSizeY = 90.f * 2.5f;
    m_fX = 0.f;
    m_fY = -120.f;
    m_fZ = 0.1f;
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

void CTitle_Star::Priority_Update(_float fTimeDelta)
{

}

void CTitle_Star::Update(_float fTimeDelta)
{

}

void CTitle_Star::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CTitle_Star::Render()
{
    //SetUp_RenderState();

    if (FAILED(m_pTextureCom_Title_Star->Bind_Texture(0)))
        return E_FAIL;

    m_pVIBufferCom->Bind_Buffers();

    __super::Begin();

    if (FAILED(m_pShaderCom_Title_Star->Bind_Matrix("g_WorldMatrix", m_pTransformCom->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom_Title_Star->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom_Title_Star->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;

    m_pTextureCom_Title_Star->Bind_Texture(m_pShaderCom_Title_Star, "g_Texture", 0);

    m_pShaderCom_Title_Star->Begin(0);

    m_pVIBufferCom->Render();

    m_pShaderCom_Title_Star->End();

    __super::End();
    //Reset_RenderState();

    return S_OK;
}

HRESULT CTitle_Star::Ready_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer_Tree"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    /* For.Com_Texture */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_LOGO), TEXT("Prototype_Component_Texture_Title_Star"),
        TEXT("Com_Texture_Title"), reinterpret_cast<CComponent**>(&m_pTextureCom_Title_Star))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform_Title_Star"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
        return E_FAIL;

    /* For.Com_Shader */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_LOGO), TEXT("Prototype_Component_Shader_StarColor"),
        TEXT("Com_Shader_Title_Star"), reinterpret_cast<CComponent**>(&m_pShaderCom_Title_Star))))
        return E_FAIL;

    return S_OK;
}

void CTitle_Star::SetUp_RenderState()
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

void CTitle_Star::Reset_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
    // 색을 섞어서 처리(알파블렌딩)
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);


    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
}

CTitle_Star* CTitle_Star::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CTitle_Star* pInstance = new CTitle_Star(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failde to Created : CTitle_Star"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CTitle_Star::Clone(void* pArg)
{
    CTitle_Star* pInstance = new CTitle_Star(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failde to Cloned : CTitle_Star"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CTitle_Star::Free()
{
    __super::Free();

    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom_Title_Star);
    Safe_Release(m_pShaderCom_Title_Star);
}

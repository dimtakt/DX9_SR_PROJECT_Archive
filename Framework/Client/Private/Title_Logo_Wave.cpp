#include "Title_Logo_Wave.h"
#include "GameInstance.h"

CTitle_Logo_Wave::CTitle_Logo_Wave(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CUIObject{ pGraphic_Device }
{
}

CTitle_Logo_Wave::CTitle_Logo_Wave(const CTitle_Logo_Wave& Prototype)
    : CUIObject( Prototype )
{
}

HRESULT CTitle_Logo_Wave::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CTitle_Logo_Wave::Initialize(void* pArg)
{
    m_fSizeX = 311.f * 1.5f;
    m_fSizeY = 100.f * 1.5f;
    m_fX = 0.f;
    m_fY = 230.f;
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

void CTitle_Logo_Wave::Priority_Update(_float fTimeDelta)
{

}

void CTitle_Logo_Wave::Update(_float fTimeDelta)
{

}

void CTitle_Logo_Wave::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CTitle_Logo_Wave::Render()
{
    //SetUp_RenderState();

    if (FAILED(m_pTextureCom_Title_Logo_Wave->Bind_Texture(0)))
        return E_FAIL;

    m_pVIBufferCom->Bind_Buffers();

    __super::Begin();

    if (FAILED(m_pShaderCom_Title_Logo_Wave->Bind_Matrix("g_WorldMatrix", m_pTransformCom->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom_Title_Logo_Wave->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom_Title_Logo_Wave->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;

    m_pTextureCom_Title_Logo_Wave->Bind_Texture(m_pShaderCom_Title_Logo_Wave, "g_Texture", 0);

    m_pShaderCom_Title_Logo_Wave->Begin(0);

    _float fTime = static_cast<_float>(GetTickCount()) * 0.001f;
    m_pShaderCom_Title_Logo_Wave->Set_Float("g_Time", fTime);

    m_pVIBufferCom->Render();

    m_pShaderCom_Title_Logo_Wave->End();

    __super::End();

    //Reset_RenderState();

    return S_OK;
}

HRESULT CTitle_Logo_Wave::Ready_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer_Tree"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    /* For.Com_Texture */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_LOGO), TEXT("Prototype_Component_Texture_Title_Logo_Wave"),
        TEXT("Com_Texture_Title_Logo_Wave"), reinterpret_cast<CComponent**>(&m_pTextureCom_Title_Logo_Wave))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform_Title_Logo_Wave"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
        return E_FAIL;

    /* For.Com_Shader */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_LOGO), TEXT("Prototype_Component_Shader_LogoRipple"),
        TEXT("Com_Shader_Title_Logo_Wave"), reinterpret_cast<CComponent**>(&m_pShaderCom_Title_Logo_Wave))))
        return E_FAIL;

    return S_OK;
}

void CTitle_Logo_Wave::SetUp_RenderState()
{
    // 알파 블렌딩 활성화
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

    // 알파 테스트로 투명한 배경 제거
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 80);  // 이 값보다 낮은 알파는 버림
}

void CTitle_Logo_Wave::Reset_RenderState()
{
    // 색을 섞어서 처리(알파블렌딩)
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
}

CTitle_Logo_Wave* CTitle_Logo_Wave::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CTitle_Logo_Wave* pInstance = new CTitle_Logo_Wave(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failde to Created : CTitle_Logo_Wave"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CTitle_Logo_Wave::Clone(void* pArg)
{
    CTitle_Logo_Wave* pInstance = new CTitle_Logo_Wave(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failde to Cloned : CTitle_Logo_Wave"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CTitle_Logo_Wave::Free()
{
    __super::Free();

    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pShaderCom_Title_Logo_Wave);
    Safe_Release(m_pTextureCom_Title_Logo_Wave);
}

#include "Exit_Button.h"
#include "GameInstance.h"
#include "Level_Loading.h"
CExit_Button::CExit_Button(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CUIObject{ pGraphic_Device }
{
}

CExit_Button::CExit_Button(const CExit_Button& Prototype)
    : CUIObject{ Prototype }
{
}

HRESULT CExit_Button::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CExit_Button::Initialize(void* pArg)
{
    m_fSizeX = 60.f;
    m_fSizeY = 35.f;
    m_fX = 235.f;
    m_fY = 235.f;
    m_fZ = 0.1f;
    m_iWinSizeX = g_iWinSizeX;
    m_iWinSizeY = g_iWinSizeY;

    m_FontColor = { 1.f, 1.f, 1.f, 1.f };

    if (FAILED(__super::Initialize()))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);

    __super::Update_Position();

    return S_OK;
}

void CExit_Button::Priority_Update(_float fTimeDelta)
{

}

void CExit_Button::Update(_float fTimeDelta)
{
    if (isPick(g_hWnd))
    {
        m_FontColor = { 1.f, 1.f, 0.2f, 1.f };
        if (m_pGameInstance->IsKeyDown(VK_LBUTTON))
        {
            PostQuitMessage(0);
        }
    }
    else {
        m_FontColor = { 1.f, 1.f, 1.f, 1.f };
    }

}

void CExit_Button::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CExit_Button::Render()
{
    SetUp_RenderState();

    //m_pGraphic_Device->SetTexture(0, nullptr);
    m_pVIBufferCom->Bind_Buffers();
    __super::Begin();
    //m_pVIBufferCom->Render();
    Render_Font();
    __super::End();

    Reset_RenderState();

    return S_OK;
}

HRESULT CExit_Button::Render_Font()
{
    TCHAR szText[64];
    _stprintf_s(szText, TEXT("종료"));

    Font_Rect_Update();
    m_vTexRect.left = 780.f;
    m_vTexRect.top = 450.f;
    m_vTexRect.right = 980.f;
    m_vTexRect.bottom = 650.f;
    m_pGameInstance->Render_Font(TEXT("UI_Font_Logo"), szText, m_vTexRect, m_FontColor, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    return S_OK;
}

HRESULT CExit_Button::Ready_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer_Tree"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform_Title_Logo"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
        return E_FAIL;

    return S_OK;
}

void CExit_Button::SetUp_RenderState()
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

void CExit_Button::Reset_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
    // 색을 섞어서 처리(알파블렌딩)
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);


    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
}

CExit_Button* CExit_Button::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CExit_Button* pInstance = new CExit_Button(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failde to Created : CExit_Button"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CExit_Button::Clone(void* pArg)
{
    CExit_Button* pInstance = new CExit_Button(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failde to Cloned : CExit_Button"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CExit_Button::Free()
{
    __super::Free();

    Safe_Release(m_pVIBufferCom);
}

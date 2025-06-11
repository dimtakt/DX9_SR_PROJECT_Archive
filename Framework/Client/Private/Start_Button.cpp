#include "Start_Button.h"
#include "GameInstance.h"
#include "Level_Loading.h"
CStart_Button::CStart_Button(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CUIObject{ pGraphic_Device }
{
}

CStart_Button::CStart_Button(const CStart_Button& Prototype)
    : CUIObject(Prototype)
{
}

HRESULT CStart_Button::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CStart_Button::Initialize(void* pArg)
{
    m_fSizeX = 142.f;
    m_fSizeY = 35.f;
    m_fX = -245.f;
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

void CStart_Button::Priority_Update(_float fTimeDelta)
{

}

void CStart_Button::Update(_float fTimeDelta)
{
    if (isPick(g_hWnd))
    {
        m_FontColor = { 1.f, 1.f, 0.2f, 1.f };
        if (m_pGameInstance->IsKeyDown(VK_LBUTTON))
        {
            m_pGameInstance->PlaySoundW(L"sephiriteSelect.wav", ENUM_CLASS(CHANNELID::SOUND_EFFECT), g_fEFFECTVolume - 0.9f);
            if (FAILED(m_pGameInstance->Open_Level(ENUM_CLASS(LEVEL::LEVEL_LOADING), CLevel_Loading::Create(m_pGraphic_Device, LEVEL::LEVEL_TOWN))))
                return;
        }
    }
    else {
        m_FontColor = { 1.f, 1.f, 1.f, 1.f };
    }

}

void CStart_Button::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CStart_Button::Render()
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

HRESULT CStart_Button::Render_Font()
{
    TCHAR szText[64];
    _stprintf_s(szText, TEXT("게임 시작"));

    Font_Rect_Update();
    m_vTexRect.left = 300.f;
    m_vTexRect.top = 450.f;
    m_vTexRect.right = 500.f;
    m_vTexRect.bottom = 650.f;
    m_pGameInstance->Render_Font(TEXT("UI_Font_Logo"), szText, m_vTexRect, m_FontColor, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    return S_OK;
}

HRESULT CStart_Button::Ready_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
        return E_FAIL;

    return S_OK;
}

void CStart_Button::SetUp_RenderState()
{
    // 알파 블렌딩 활성화
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

    // 알파 테스트로 투명한 배경 제거
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 80);  // 이 값보다 낮은 알파는 버림
}

void CStart_Button::Reset_RenderState()
{
    // 색을 섞어서 처리(알파블렌딩)
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
}

CStart_Button* CStart_Button::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CStart_Button* pInstance = new CStart_Button(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failde to Created : CStart_Button"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CStart_Button::Clone(void* pArg)
{
    CStart_Button* pInstance = new CStart_Button(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failde to Cloned : CStart_Button"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CStart_Button::Free()
{
    __super::Free();

    Safe_Release(m_pVIBufferCom);
}

#include "Hud_Exp.h"
#include "GameInstance.h"

CHud_Exp::CHud_Exp(LPDIRECT3DDEVICE9 pGraphic_Device) : CProgressBar(pGraphic_Device)
{
}

CHud_Exp::CHud_Exp(const CHud_Exp& Prototype) : CProgressBar(Prototype)
{
}

HRESULT CHud_Exp::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CHud_Exp::Initialize(void* pArg)
{
    m_iCulMaxValue = 100;
    m_iCulValue = 0;

    UIOBJECT_DESC Desc{};
    Desc.fSizeX = g_iWinSizeX;
    Desc.fSizeY = 9;
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

	return S_OK;
}

void CHud_Exp::Priority_Update(_float fTimeDelta)
{
    if (GetKeyState('Z') < 0)
    {
        --m_iCulValue;
    }
    if (GetKeyState('X') < 0)
    {
        ++m_iCulValue;
    }
    if (GetKeyState('O') < 0)
    {
        --m_iCulMaxValue;
    }
    if (GetKeyState('P') < 0)
    {
        ++m_iCulMaxValue;
    }
}

void CHud_Exp::Update(_float fTimeDelta)
{
}

void CHud_Exp::Late_Update(_float fTimeDelta)
{
    Progress_UpdateX();
    m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CHud_Exp::Render()
{

    m_pTransformCom->Bind_Matrix();
    m_pVIBufferCom->Bind_Buffers();

    SetUp_RenderState();
    __super::Begin();
    m_pVIBufferCom->Render();
    __super::End();
    Reset_RenderState();
 
    return S_OK;
}

HRESULT CHud_Exp::Ready_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Rect_UI_Hud_Exp_Player"),
        TEXT("Com_VIBuffer1"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
        return E_FAIL;

    return S_OK;
}

void CHud_Exp::SetUp_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
}

void CHud_Exp::Reset_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

CHud_Exp* CHud_Exp::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CHud_Exp* pInstance = new CHud_Exp(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CHud_Exp"));
        Safe_Release(pInstance);
    }
    return pInstance;
}

CGameObject* CHud_Exp::Clone(void* pArg)
{
    CHud_Exp* pInstance = new CHud_Exp(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Clone : CHud_Exp"));
        Safe_Release(pInstance);
    }
    return pInstance;
}

void CHud_Exp::Free()
{
    __super::Free();
    Safe_Release(m_pVIBufferCom);
}

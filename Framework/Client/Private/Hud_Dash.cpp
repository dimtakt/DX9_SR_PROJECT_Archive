#include "Hud_Dash.h"
#include "GameInstance.h"
#include "Dash_Gauge_Fream.h"

CHud_Dash::CHud_Dash(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CHud_Dash::CHud_Dash(const CHud_Dash& Prototype) : CUIObject(Prototype), m_eLevel( Prototype.m_eLevel), m_iDashMaxValue(Prototype.m_iDashMaxValue)
{
}

HRESULT CHud_Dash::Initialize_Prototype(LEVEL eLevel)
{
    m_eLevel = eLevel;

    if (FAILED(Ready_ChildPrototype(eLevel)))
        return E_FAIL;

    return S_OK;
}

HRESULT CHud_Dash::Initialize(void* pArg)
{
    m_iDashMaxValue = 5;

    UIOBJECT_DESC Desc{};

    Desc.fSizeX = 0;
    Desc.fSizeY = 0;
    Desc.fX = 30;
    Desc.fY = 100.f;
    Desc.fZ = 0.f;
    Desc.iWinSizeX = g_iWinSizeX;
    Desc.iWinSizeY = g_iWinSizeY;

    if (FAILED(__super::Initialize(&Desc)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
    __super::Update_Position();

    if (FAILED(Ready_Children()))
        return E_FAIL;

    return S_OK;
}

void CHud_Dash::Priority_Update(_float fTimeDelta)
{
    if (GetKeyState('N') < 0)
    {
        --m_iDashMaxValue;
    }
    if (GetKeyState('M') < 0)
    {
        ++m_iDashMaxValue;
    }
    
    if (m_iDashMaxValue > 5)
        m_iDashMaxValue = 5;

    if (m_iDashMaxValue <= 0)
    {
        m_iDashMaxValue = 0;
    }
    else
    {
        for (_int i = 0; i < m_iDashMaxValue; ++i)
            m_vecChildren[i]->Priority_Update(fTimeDelta);
    }
}

void CHud_Dash::Update(_float fTimeDelta)
{
    if (m_iDashMaxValue <= 0)
    {
        m_iDashMaxValue = 0;
    }
    else
    {
        
        for (_int i = 0; i < m_iDashMaxValue; ++i)
            m_vecChildren[i]->Update(fTimeDelta);
    }
}

void CHud_Dash::Late_Update(_float fTimeDelta)
{
    if (m_iDashMaxValue <= 0)
    {
        m_iDashMaxValue = 0;
    }
    else
    {
        for (_int i = 0; i < m_iDashMaxValue; ++i)
            m_vecChildren[i]->Late_Update(fTimeDelta);
    }
}

HRESULT CHud_Dash::Render()
{
    return S_OK;
}

HRESULT CHud_Dash::Ready_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
        return E_FAIL;
    return S_OK;
}

HRESULT CHud_Dash::Ready_ChildPrototype(LEVEL eLevel)
{
    if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_Dash_Gauge_Fream"),
        CDash_Gauge_Fream::Create(m_pGraphic_Device, eLevel))))
        return E_FAIL;

    return S_OK;
}

HRESULT CHud_Dash::Ready_Children()
{
    CUIObject* pGameObject = nullptr;
    CDash_Gauge_Fream::UIHUD_DASH_FREAM_DESC Desc{};

    for (_int i = 0; i < 5; ++i)
    {
        Desc.fX = i;
        pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Dash_Gauge_Fream"), &Desc));
        if (nullptr == pGameObject)
            return E_FAIL;
        Add_Child(pGameObject);
    }
    

    return S_OK;
}

CHud_Dash* CHud_Dash::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
    CHud_Dash* pInstance = new CHud_Dash(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype(eLevel)))
    {
        MSG_BOX(TEXT("Failed to Create : CHud_Dash"));
        Safe_Release(pInstance);
    }
    return pInstance;
}

CGameObject* CHud_Dash::Clone(void* pArg)
{
    CHud_Dash* pInstance = new CHud_Dash(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Clone : CHud_Dash"));
        Safe_Release(pInstance);
    }
    return pInstance;
}

void CHud_Dash::Free()
{
    __super::Free();
}

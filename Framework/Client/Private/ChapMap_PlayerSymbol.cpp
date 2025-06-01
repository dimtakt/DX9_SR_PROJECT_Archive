#include "ChapMap_PlayerSymbol.h"
#include "GameInstance.h"
CChapMap_PlayerSymbol::CChapMap_PlayerSymbol(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CChapMap_PlayerSymbol::CChapMap_PlayerSymbol(const CChapMap_PlayerSymbol& Prototype) : CUIObject(Prototype)
{
}

HRESULT CChapMap_PlayerSymbol::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CChapMap_PlayerSymbol::Initialize(void* pArg)
{

    m_fRotSpeed = 90.f;

    m_fSizeX = 100;
    m_fSizeY = 100;
    m_fX = 0.f;
    m_fY = 0.f;
    m_fZ = UI_DEPTH::CHATERMAP_PLAYER;
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

void CChapMap_PlayerSymbol::Priority_Update(_float fTimeDelta)
{
}

void CChapMap_PlayerSymbol::Update(_float fTimeDelta)
{
}

void CChapMap_PlayerSymbol::Late_Update(_float fTimeDelta)
{
    _float3 vRotation = { 0.f, 0.f, 1.f };
    m_fAngle += m_fRotSpeed * fTimeDelta;
    m_pTransformCom->Rotation(vRotation, D3DXToRadian(m_fAngle));
    m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CChapMap_PlayerSymbol::Render()
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

void CChapMap_PlayerSymbol::Player_Move(_float fX, _float fY)
{
    m_fX = fX;
    m_fY = fY;
}

HRESULT CChapMap_PlayerSymbol::Ready_Components()
{
 
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_Texture_Rect_ChapMap_Node_Player"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    return S_OK;
}

void CChapMap_PlayerSymbol::SetUp_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
}

void CChapMap_PlayerSymbol::Reset_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

    m_pGraphic_Device->SetTexture(0, NULL);
}

CChapMap_PlayerSymbol* CChapMap_PlayerSymbol::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CChapMap_PlayerSymbol* pInstance = new CChapMap_PlayerSymbol(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Create : CChapMap_PlayerSymbol"));
        Safe_Release(pInstance);
    }
    return pInstance;
}

CGameObject* CChapMap_PlayerSymbol::Clone(void* pArg)
{
    CChapMap_PlayerSymbol* pInstance = new CChapMap_PlayerSymbol(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Clone : CChapMap_PlayerSymbol"));
        Safe_Release(pInstance);
    }
    return pInstance;
}

void CChapMap_PlayerSymbol::Free()
{
    __super::Free();

    Safe_Release(m_pTextureCom);
    Safe_Release(m_pVIBufferCom);
}

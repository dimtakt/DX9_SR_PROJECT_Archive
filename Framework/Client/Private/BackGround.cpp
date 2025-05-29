#include "BackGround.h"

#include "GameInstance.h"

CBackGround::CBackGround(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CGameObject { pGraphic_Device }
{
}

CBackGround::CBackGround(const CBackGround& Prototype)
    : CGameObject { Prototype }
{
}

HRESULT CBackGround::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CBackGround::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;

    return S_OK;
}

void CBackGround::Priority_Update(_float fTimeDelta)
{

}

void CBackGround::Update(_float fTimeDelta)
{
    if (GetKeyState(VK_UP) < 0)
    {
        m_pTransformCom->Go_Straight(fTimeDelta);
    }

    if (GetKeyState(VK_DOWN) < 0)
    {
        m_pTransformCom->Go_Backward(fTimeDelta);
    }

    if (GetKeyState(VK_LEFT) < 0)
    {
        m_pTransformCom->Turn(_float3(0.f, 1.f, 0.f), fTimeDelta * -1.f);
    }

    if (GetKeyState(VK_RIGHT) < 0)
    {
        m_pTransformCom->Turn(_float3(0.f, 1.f, 0.f), fTimeDelta);
    }
}

void CBackGround::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_PRIORITY, this);
}

HRESULT CBackGround::Render()
{

    m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    m_pTransformCom->Bind_Matrix();

    _float4x4			ViewMatrix, ProjMatrix;
    _float3				vEye{ 0.f, 0.f, -1.f }, vAt{ 0.f, 0.f, 0.f }, vUpDir{ 0.f, 1.f, 0.f };

    m_pGraphic_Device->SetTransform(D3DTS_VIEW, D3DXMatrixLookAtLH(&ViewMatrix, &vEye, &vAt, &vUpDir));
    m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, D3DXMatrixPerspectiveFovLH(&ProjMatrix, D3DXToRadian(60.0f), static_cast<_float>(g_iWinSizeX) / g_iWinSizeY, 0.1f, 1000.f));

    if (FAILED(m_pTextureCom->Bind_Texture(0)))
        return E_FAIL;

    m_pVIBufferCom->Bind_Buffers();

    m_pVIBufferCom->Render();

    return S_OK;
}

HRESULT CBackGround::Ready_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    /* For.Com_Texture */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_LOGO), TEXT("Prototype_Component_Texture_BackGround"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    CTransform::TRANSFORM_DESC TransformDesc {};
    TransformDesc.fSpeedPerSec = 5.f;
    TransformDesc.fRotationPerSec = D3DXToRadian(90.f);

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
        return E_FAIL;

    /*m_pVIBufferCom = dynamic_cast<CVIBuffer_Rect*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::COMPONENT, ));
    
    if (nullptr == m_pVIBufferCom)
        return E_FAIL;

    if (nullptr == Get_Component(TEXT("Com_VIBuffer")))
        m_Components.emplace(TEXT("Com_VIBuffer"), m_pVIBufferCom);

    Safe_AddRef(m_pVIBufferCom);*/

    return S_OK;
}

CBackGround* CBackGround::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CBackGround* pInstance = new CBackGround(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failde to Created : CBackGround"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CBackGround::Clone(void* pArg)
{
    CBackGround* pInstance = new CBackGround(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failde to Cloned : CBackGround"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CBackGround::Free()
{
    __super::Free();

    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTransformCom);
    Safe_Release(m_pTextureCom);
}

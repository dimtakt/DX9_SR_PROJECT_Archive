#include "Light.h"

CLight::CLight(LPDIRECT3DDEVICE9 pGraphic_Device)
    : m_pGraphic_Device { pGraphic_Device }
    , m_iIndex(0)
{
    Safe_AddRef(m_pGraphic_Device);
    ZeroMemory(&m_LightDesc, sizeof(D3DLIGHT9));
}


HRESULT CLight::Ready_Light(const D3DLIGHT9* pLightInfo, const _uint& iIndex)
{
    memcpy(&m_LightDesc, pLightInfo, sizeof(D3DLIGHT9));

    m_iIndex = iIndex;

    m_pGraphic_Device->SetLight(iIndex, pLightInfo);

    m_pGraphic_Device->LightEnable(iIndex, TRUE);

    return S_OK;
}

CLight* CLight::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const D3DLIGHT9* pLightInfo, const _uint& iIndex)
{
    CLight* pLight = new CLight(pGraphic_Device);

    if (FAILED(pLight->Ready_Light(pLightInfo, iIndex)))
    {
        Safe_Release(pLight);
        MSG_BOX(TEXT("Light Create Failed"));
        return nullptr;
    }

    return pLight;
}

void CLight::Free()
{
    m_pGraphic_Device->LightEnable(m_iIndex, FALSE);

    Safe_Release(m_pGraphic_Device);
}

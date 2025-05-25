#include "Light_Manager.h"
#include "Light.h"
CLight_Manager::CLight_Manager(LPDIRECT3DDEVICE9 pGraphic_Device)
	:m_pGraphic_Device { pGraphic_Device }
{
	Safe_AddRef(pGraphic_Device);
}

HRESULT CLight_Manager::Ready_Light(const D3DLIGHT9* pLightInfo, const _uint& iIndex)
{
	CLight* pLight = CLight::Create(m_pGraphic_Device, pLightInfo, iIndex);
	NULL_CHECK_RETURN(pLight, E_FAIL);

	m_LightList.push_back(pLight);

	return S_OK;
}

CLight_Manager* CLight_Manager::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	return new CLight_Manager(pGraphic_Device);
}

void CLight_Manager::Free()
{
	__super::Free();
	for (auto& light : m_LightList)
	{
		Safe_Release(light);
	}
	m_LightList.clear();
	Safe_Release(m_pGraphic_Device);
}

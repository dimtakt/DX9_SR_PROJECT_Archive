#include "Ending_Light.h"
#include "GameInstance.h"

CEnding_Light::CEnding_Light(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CEnding_Light::CEnding_Light(const CEnding_Light& Prototype) : CUIObject(Prototype)
{
}

HRESULT CEnding_Light::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEnding_Light::Initialize(void* pArg)
{
	m_fSizeX = 900;
	m_fSizeY = 900;
	m_fX = 0;
	m_fY = -150;
	m_fZ = 0.32f;
	m_iWinSizeX = g_iWinSizeX;
	m_iWinSizeY = g_iWinSizeY;

	if (FAILED(CUIObject::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
	__super::Update_Position();

	return S_OK;
}

void CEnding_Light::Priority_Update(_float fTimeDelta)
{
}

void CEnding_Light::Update(_float fTimeDelta)
{

}

void CEnding_Light::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CEnding_Light::Render()
{
	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;
	m_pVIBufferCom->Bind_Buffers();

	__super::Begin();
	m_pVIBufferCom->Render();

	__super::End();

	return S_OK;
}

HRESULT CEnding_Light::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Rect_Light_Ending"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

CEnding_Light* CEnding_Light::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CEnding_Light* pInstance = new CEnding_Light(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CEnding_Light"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CEnding_Light::Clone(void* pArg)
{
	CEnding_Light* pInstance = new CEnding_Light(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CEnding_Light"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEnding_Light::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}

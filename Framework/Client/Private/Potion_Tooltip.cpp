#include "Potion_Tooltip.h"
#include "GameInstance.h"

CPotion_Tooltip::CPotion_Tooltip(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CPotion_Tooltip::CPotion_Tooltip(const CPotion_Tooltip& Prototype) : CUIObject(Prototype), m_eLevel(Prototype.m_eLevel)
{
}

HRESULT CPotion_Tooltip::Initialize_Prototype(LEVEL eLevel)
{
	m_eLevel = eLevel;

	return S_OK;
}

HRESULT CPotion_Tooltip::Initialize(void* pArg)
{

	m_fSizeX = 470;
	m_fSizeY = 470;
	m_fX = 0;
	m_fY = 0;
	m_fZ = UI_DEPTH::TOOLTIP;
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

void CPotion_Tooltip::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CPotion_Tooltip::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CPotion_Tooltip::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CPotion_Tooltip::Render()
{
	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;
	m_pVIBufferCom->Bind_Buffers();

	__super::Begin();
	m_pVIBufferCom->Render();
	return S_OK;
}

HRESULT CPotion_Tooltip::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Rect_Item_Tooltip"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

CPotion_Tooltip* CPotion_Tooltip::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
	CPotion_Tooltip* pInstance = new CPotion_Tooltip(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eLevel)))
	{
		MSG_BOX(TEXT("Failed to Created : CPotion_Tooltip"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CPotion_Tooltip::Clone(void* pArg)
{
	CPotion_Tooltip* pInstance = new CPotion_Tooltip(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CPotion_Tooltip"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPotion_Tooltip::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}

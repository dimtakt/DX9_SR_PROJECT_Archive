#include "Item_Tooltip_Mid.h"
#include "GameInstance.h"

CItem_Tooltip_Mid::CItem_Tooltip_Mid(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CItem_Tooltip_Mid::CItem_Tooltip_Mid(const CItem_Tooltip_Mid& Prototype) : CUIObject(Prototype), m_eLevel(Prototype.m_eLevel)
{
}

HRESULT CItem_Tooltip_Mid::Initialize_Prototype(LEVEL eLevel)
{
	m_eLevel = eLevel;

	return S_OK;
}

HRESULT CItem_Tooltip_Mid::Initialize(void* pArg)
{
	UIOBJECT_DESC* Desc = static_cast<UIOBJECT_DESC*>(pArg);

	switch ((int)Desc->fZ)
	{
	case 0:
		m_fSizeX = 350;
		m_fSizeY = 350;
		m_fX = 0;
		m_fY = 97;
		break;
	case 1:
		m_fSizeX = 350;
		m_fSizeY = 600;
		m_fX = 0;
		m_fY = 127;
		break;
	case 2:
		m_fSizeX = 350;
		m_fSizeY = 170;
		m_fX = 0;
		m_fY = 67;
		break;
	}

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

void CItem_Tooltip_Mid::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CItem_Tooltip_Mid::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CItem_Tooltip_Mid::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI_BLEND, this);
	__super::Late_Update(fTimeDelta);
}

HRESULT CItem_Tooltip_Mid::Render()
{
	if (FAILED(m_pTextureCom->Bind_Texture(1)))
		return E_FAIL;
	m_pVIBufferCom->Bind_Buffers();

	__super::Begin();
	m_pVIBufferCom->Render();
	return S_OK;
}

HRESULT CItem_Tooltip_Mid::Ready_Components()
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

CItem_Tooltip_Mid* CItem_Tooltip_Mid::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
	CItem_Tooltip_Mid* pInstance = new CItem_Tooltip_Mid(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eLevel)))
	{
		MSG_BOX(TEXT("Failed to Created : CItem_Tooltip_Mid"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CItem_Tooltip_Mid::Clone(void* pArg)
{
	CItem_Tooltip_Mid* pInstance = new CItem_Tooltip_Mid(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CItem_Tooltip_Mid"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CItem_Tooltip_Mid::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}

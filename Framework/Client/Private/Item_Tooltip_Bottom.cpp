#include "Item_Tooltip_Bottom.h"
#include "GameInstance.h"

CItem_Tooltip_Bottom::CItem_Tooltip_Bottom(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CItem_Tooltip_Bottom::CItem_Tooltip_Bottom(const CItem_Tooltip_Bottom& Prototype) : CUIObject(Prototype), m_eLevel(Prototype.m_eLevel)
{
}

HRESULT CItem_Tooltip_Bottom::Initialize_Prototype(LEVEL eLevel)
{
	m_eLevel = eLevel;

	return S_OK;
}

HRESULT CItem_Tooltip_Bottom::Initialize(void* pArg)
{

	m_fSizeX = 350;
	m_fSizeY = 350;
	m_fX = 0;
	m_fY = 170;
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

void CItem_Tooltip_Bottom::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CItem_Tooltip_Bottom::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CItem_Tooltip_Bottom::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI_BLEND, this);
	__super::Late_Update(fTimeDelta);
}

HRESULT CItem_Tooltip_Bottom::Render()
{
	if (FAILED(m_pTextureCom->Bind_Texture(2)))
		return E_FAIL;
	m_pVIBufferCom->Bind_Buffers();

	__super::Begin();
	m_pVIBufferCom->Render();
	return S_OK;
}

HRESULT CItem_Tooltip_Bottom::Ready_Components()
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

CItem_Tooltip_Bottom* CItem_Tooltip_Bottom::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
	CItem_Tooltip_Bottom* pInstance = new CItem_Tooltip_Bottom(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eLevel)))
	{
		MSG_BOX(TEXT("Failed to Created : CItem_Tooltip_Bottom"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CItem_Tooltip_Bottom::Clone(void* pArg)
{
	CItem_Tooltip_Bottom* pInstance = new CItem_Tooltip_Bottom(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CItem_Tooltip_Bottom"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CItem_Tooltip_Bottom::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}

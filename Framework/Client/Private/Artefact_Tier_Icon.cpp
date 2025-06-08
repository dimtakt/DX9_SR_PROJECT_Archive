#include "Artefact_Tier_Icon.h"
#include "GameInstance.h"
CArtefact_Tier_Icon::CArtefact_Tier_Icon(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CArtefact_Tier_Icon::CArtefact_Tier_Icon(const CArtefact_Tier_Icon& Prototype) : CUIObject(Prototype), m_eLevel(Prototype.m_eLevel)
{
}

HRESULT CArtefact_Tier_Icon::Initialize_Prototype(LEVEL eLevel)
{
	m_eLevel = eLevel;

	return S_OK;
}

HRESULT CArtefact_Tier_Icon::Initialize(void* pArg)
{
	UIOBJECT_DESC* Desc = static_cast<UIOBJECT_DESC*>(pArg);
	m_fSizeX = 20;
	m_fSizeY = 20;
	m_fX = -30 + Desc->fX * 20 - (Desc->fZ - 1)*20/2;
	m_fY = -30;

	m_iTextIndex = Desc->fY;

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

void CArtefact_Tier_Icon::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CArtefact_Tier_Icon::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CArtefact_Tier_Icon::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI_BLEND, this);

	__super::Late_Update(fTimeDelta);
}

HRESULT CArtefact_Tier_Icon::Render()
{
	if (FAILED(m_pTextureCom->Bind_Texture(m_iTextIndex)))
		return E_FAIL;
	m_pVIBufferCom->Bind_Buffers();

	__super::Begin();
	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CArtefact_Tier_Icon::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Rect_Item_Tooltip_Tier"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

CArtefact_Tier_Icon* CArtefact_Tier_Icon::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
	CArtefact_Tier_Icon* pInstance = new CArtefact_Tier_Icon(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eLevel)))
	{
		MSG_BOX(TEXT("Failed to Created : CArtefact_Tier_Icon"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CArtefact_Tier_Icon::Clone(void* pArg)
{
	CArtefact_Tier_Icon* pInstance = new CArtefact_Tier_Icon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CArtefact_Tier_Icon"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CArtefact_Tier_Icon::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}

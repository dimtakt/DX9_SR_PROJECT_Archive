#include "Slate_Tooltip.h"
#include "GameInstance.h"
#include "Item_Tooltip_Mid.h"
#include "Item_Tooltip_Bottom.h"
#include "Button.h"

CSlate_Tooltip::CSlate_Tooltip(LPDIRECT3DDEVICE9 pGraphic_Device) : CTooltip(pGraphic_Device)
{
}

CSlate_Tooltip::CSlate_Tooltip(const CSlate_Tooltip& Prototype) : CTooltip(Prototype), m_eLevel(Prototype.m_eLevel)
{
}

HRESULT CSlate_Tooltip::Initialize_Prototype(LEVEL eLevel)
{
	m_eLevel = eLevel;

	if (FAILED(Ready_ChildPrototype(m_eLevel)))
		return E_FAIL;

	return S_OK;
}

HRESULT CSlate_Tooltip::Initialize(void* pArg)
{
	m_fSizeX = 350;
	m_fSizeY = 350;
	m_fX = 195;
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

	if (FAILED(Ready_Children()))
		return E_FAIL;

	return S_OK;
}

void CSlate_Tooltip::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CSlate_Tooltip::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CSlate_Tooltip::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);

	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI_BLEND, this);

	_float3 vRenderPos;
	vRenderPos = m_pTransformCom->Get_State(STATE::POSITION);

	vRenderPos.x += 105;
	vRenderPos.z = 0;

	if (m_pItemObject != nullptr)
		static_cast<CItem_Base*>(m_pItemObject)->IsTooltip(vRenderPos);
}

HRESULT CSlate_Tooltip::Render()
{
	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;
	m_pVIBufferCom->Bind_Buffers();

	__super::Begin();
	m_pVIBufferCom->Render();
	Render_Font();
	return S_OK;
}

HRESULT CSlate_Tooltip::Ready_Components()
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

HRESULT CSlate_Tooltip::Ready_ChildPrototype(LEVEL eLevel)
{
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_Slate_Tooltip_Mid"),
		CItem_Tooltip_Mid::Create(m_pGraphic_Device, eLevel))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_Slate_Tooltip_Bottom"),
		CItem_Tooltip_Bottom::Create(m_pGraphic_Device, eLevel))))
		return E_FAIL;

	return S_OK;
}

HRESULT CSlate_Tooltip::Ready_Children()
{
	CUIObject* pGameObject = nullptr;

	UIOBJECT_DESC Desc{};
	Desc.fZ = 1;
	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_ArteFact_Tooltip_Mid"), &Desc));
	if (nullptr == pGameObject)
		return E_FAIL;
	Add_Child(pGameObject);
	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_ArteFact_Tooltip_Bottom"), &Desc));
	if (nullptr == pGameObject)
		return E_FAIL;
	Add_Child(pGameObject);

	return S_OK;
}

void CSlate_Tooltip::Render_Font()
{
	TCHAR szText[MAX_PATH];
	D3DXCOLOR TexColor{};

	Font_Rect_Update();
	CUIObject::Font_Rect_Update();

	_uint iItemID = m_pItemObject->Item_Info()->iItemID;

	m_vTexRect.left -= 30;
	switch (g_ItemDataBase[iItemID].m_eRarity)
	{
	case ITEM_RARITY::NORMAL:
		_stprintf_s(szText, TEXT("[ÀÏ¹Ý]"));
		TexColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		break;
	case ITEM_RARITY::RARE:
		_stprintf_s(szText, TEXT("[°í±Þ]"));
		TexColor = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);
		break;
	case ITEM_RARITY::EPIC:
		_stprintf_s(szText, TEXT("[Èñ±Í]"));
		TexColor = D3DXCOLOR(0.f, 0.f, 1.f, 1.f);
		break;
	case ITEM_RARITY::LEGENDARY:
		_stprintf_s(szText, TEXT("[Àü¼³]"));
		TexColor = D3DXCOLOR(1.f, 1.f, 0.f, 1.f);
		break;
	}
	m_vTexRect.right -= 30;
	m_pGameInstance->Render_Font(TEXT("UI_Font_22_Damage"), g_ItemDataBase[iItemID].m_szName, m_vTexRect, TexColor, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	CUIObject::Font_Rect_Update();
	m_vTexRect.top += 220;
	m_vTexRect.left += 50;

	m_pGameInstance->Render_Font(TEXT("UI_Font_16_Tooltip"), szText, m_vTexRect, TexColor, DT_LEFT | DT_TOP);

}

CSlate_Tooltip* CSlate_Tooltip::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
	CSlate_Tooltip* pInstance = new CSlate_Tooltip(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eLevel)))
	{
		MSG_BOX(TEXT("Failed to Created : CSlate_Tooltip"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CSlate_Tooltip::Clone(void* pArg)
{
	CSlate_Tooltip* pInstance = new CSlate_Tooltip(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CSlate_Tooltip"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSlate_Tooltip::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}

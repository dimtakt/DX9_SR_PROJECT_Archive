#include "Artefact_Tooltip.h"
#include "GameInstance.h"
#include "Item_Tooltip_Mid.h"
#include "Item_Tooltip_Bottom.h"
#include "Button.h"
#include "Artefact_Tier_Icon.h"
CArtefact_Tooltip::CArtefact_Tooltip(LPDIRECT3DDEVICE9 pGraphic_Device) : CTooltip(pGraphic_Device)
{
}

CArtefact_Tooltip::CArtefact_Tooltip(const CArtefact_Tooltip& Prototype) : CTooltip(Prototype), m_eLevel(Prototype.m_eLevel)
{
}

HRESULT CArtefact_Tooltip::Initialize_Prototype(LEVEL eLevel)
{
	m_eLevel = eLevel;

	if (FAILED(Ready_ChildPrototype(m_eLevel)))
		return E_FAIL;

	return S_OK;
}

HRESULT CArtefact_Tooltip::Initialize(void* pArg)
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

void CArtefact_Tooltip::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CArtefact_Tooltip::Update(_float fTimeDelta)
{
	if (m_pItemObject == nullptr )
		return;

	if (m_pOldItem != m_pItemObject)
	{
		for (auto iter = m_vecChildren.begin() + 2; iter != m_vecChildren.end(); ++iter)
		{
			Safe_Release(*(iter));
		}
		m_vecChildren.erase(m_vecChildren.begin() + 2, m_vecChildren.end());
		m_pOldItem = m_pItemObject;
		m_bTier = false;
	}

	if (m_pItemObject->Item_Info()->iItemValue != 0 && !m_bTier)
	{
		m_iUpgradeValue = static_cast<CButton*>(m_pParent)->Get_Value();
		CUIObject* pGameObject = nullptr;
		UIOBJECT_DESC Desc{};
		Desc.fZ = m_pItemObject->Item_Info()->iItemValue;
		for (_int i = 0; i < m_pItemObject->Item_Info()->iItemValue; ++i)
		{
			Desc.fX = i;
			pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_ArteFact_Tier"), &Desc.fX));
			if (nullptr == pGameObject)
				return;
			Add_Child(pGameObject);
		}
		m_bTier = true;
	}
	if (m_bTier)
	{
		for (_int i = 2; i < m_vecChildren.size(); ++i)
		{
			if (i < m_iUpgradeValue + 2)
				static_cast<CArtefact_Tier_Icon*>(m_vecChildren[i])->Set_Textuer(1);
			else
				static_cast<CArtefact_Tier_Icon*>(m_vecChildren[i])->Set_Textuer(0);
		}
	}
__super::Update(fTimeDelta);
}

void CArtefact_Tooltip::Late_Update(_float fTimeDelta)
{
	if (m_pItemObject == nullptr)
		return;

	for (_int i = 0; i < 2; ++i)
		m_vecChildren[i]->Late_Update(fTimeDelta);
		
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI_BLEND, this);

	_float3 vRenderPos;
	vRenderPos = m_pTransformCom->Get_State(STATE::POSITION);

	vRenderPos.x += 105;
	vRenderPos.z = 0;

	if (m_pItemObject != nullptr)
		static_cast<CItem_Base*>(m_pItemObject)->IsTooltip(vRenderPos);

	if (1 < m_vecChildren.size())
	{
		for (_int i = 2; i < m_vecChildren.size(); ++i)
			m_vecChildren[i]->Late_Update(fTimeDelta);
	}
}

HRESULT CArtefact_Tooltip::Render()
{
	Render_Pos();
	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;
	m_pVIBufferCom->Bind_Buffers();

	__super::Begin();
	m_pVIBufferCom->Render();
	__super::End();
	Render_Font();
	return S_OK;
}

HRESULT CArtefact_Tooltip::Ready_Components()
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

HRESULT CArtefact_Tooltip::Ready_ChildPrototype(LEVEL eLevel)
{
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_ArteFact_Tooltip_Mid"),
		CItem_Tooltip_Mid::Create(m_pGraphic_Device, eLevel))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_ArteFact_Tooltip_Bottom"),
		CItem_Tooltip_Bottom::Create(m_pGraphic_Device, eLevel))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_ArteFact_Tier"),
		CArtefact_Tier_Icon::Create(m_pGraphic_Device, eLevel))))
		return E_FAIL;

	return S_OK;
}

HRESULT CArtefact_Tooltip::Ready_Children()
{
	CUIObject* pGameObject = nullptr;

	UIOBJECT_DESC Desc{};
	Desc.fZ = 0;
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

void CArtefact_Tooltip::Render_Font()
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
		TexColor = D3DXCOLOR(0.3f, 1.f, 0.f, 1.f);
		break;
	case ITEM_RARITY::EPIC:
		_stprintf_s(szText, TEXT("[Èñ±Í]"));
		TexColor = D3DXCOLOR(0.f, 0.75f, 1.f, 1.f);
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

	if (g_ItemEffect[g_ItemDataBase[iItemID].m_iARTEFACT_Value].m_eType == ITEM_EFFECT::VALUE_TYPE)
	{
		for (_int i = 0; i < g_ItemEffect[g_ItemDataBase[iItemID].m_iARTEFACT_Value].m_vecValue.size(); ++i)
		{
			m_vTexRect.top += 20;
			_stprintf_s(szText, TEXT(""));
			_tcscpy_s(szText, g_ItemEffect[g_ItemDataBase[iItemID].m_iARTEFACT_Value].m_vecValue[i].m_szText.c_str());
			
			_float iStat_Value = g_ItemEffect[g_ItemDataBase[iItemID].m_iARTEFACT_Value].m_vecValue[i].m_fStat_Value1;
			_float iUP_Value = g_ItemEffect[g_ItemDataBase[iItemID].m_iARTEFACT_Value].m_vecValue[i].m_fStat_Value2;

			iStat_Value = iStat_Value + iUP_Value * static_cast<CButton*>(m_pParent)->Get_Value();
			
			_stprintf_s(szText, szText, int(iStat_Value));

			m_pGameInstance->Render_Font(TEXT("UI_Font_16_Tooltip"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), DT_LEFT | DT_TOP);
		}
	}
	else
	{
		m_vTexRect.top += 20;
		m_pGameInstance->Render_Font(TEXT("UI_Font_16_Tooltip"), g_ItemEffect[g_ItemDataBase[iItemID].m_iARTEFACT_Value].m_vecValue[0].m_szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), DT_LEFT | DT_TOP);
	}

	m_vTexRect.top += 70;
	m_pGameInstance->Render_Font(TEXT("UI_Font_16_Tooltip"), g_ItemDataBase[iItemID].m_szDescription, m_vTexRect, D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.f), DT_LEFT | DT_TOP);
	return;

}

void CArtefact_Tooltip::Render_Pos()
{
	POINT mousePos;
	GetCursorPos(&mousePos);
	ScreenToClient(g_hWnd, &mousePos);

	if (mousePos.x < 970)
		m_fX = 195;
	else
		m_fX = -195;

	__super::Update_Position();
}

CArtefact_Tooltip* CArtefact_Tooltip::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
	CArtefact_Tooltip* pInstance = new CArtefact_Tooltip(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eLevel)))
	{
		MSG_BOX(TEXT("Failed to Created : CArtefact_Tooltip"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CArtefact_Tooltip::Clone(void* pArg)
{
	CArtefact_Tooltip* pInstance = new CArtefact_Tooltip(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CArtefact_Tooltip"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CArtefact_Tooltip::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}

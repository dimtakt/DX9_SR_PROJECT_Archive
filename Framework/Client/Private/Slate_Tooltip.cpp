#include "Slate_Tooltip.h"
#include "GameInstance.h"
#include "Item_Tooltip_Mid.h"
#include "Item_Tooltip_Bottom.h"
#include "Button.h"
#include "Slate_Tooltip_Slot.h"
#include "UI_KeyGuide.h"
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
	if (m_pItemObject == nullptr)
		return;
	__super::Update(fTimeDelta);
}

void CSlate_Tooltip::Late_Update(_float fTimeDelta)
{
	if (m_pItemObject == nullptr)
		return;

	Set_UpGrade();

	for (_int i = 0; i < 2; ++i)
	{
		m_vecChildren[i]->Late_Update(fTimeDelta);
	}

	if(g_SlateDataBase[ static_cast<CItem_Base*>(m_pItemObject)->Item_Info()->iItemValue].m_bRotation)
		m_vecChildren[2]->Late_Update(fTimeDelta);

	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI_BLEND, this);

	_float3 vRenderPos;
	vRenderPos = m_pTransformCom->Get_State(STATE::POSITION);

	vRenderPos.x += 105;
	vRenderPos.z = 0;

	if (m_pItemObject != nullptr)
		static_cast<CItem_Base*>(m_pItemObject)->IsTooltip(vRenderPos);

	for (_int i = 3; i < 33; ++i)
	{
		m_vecChildren[i]->Late_Update(fTimeDelta);
	}

}

HRESULT CSlate_Tooltip::Render()
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

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_Slate_Tooltip_Slot"),
		CSlate_Tooltip_Slot::Create(m_pGraphic_Device, eLevel))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_Slate_Guide"),
		CUI_KeyGuide::Create(m_pGraphic_Device))))
		return E_FAIL;



	return S_OK;
}

HRESULT CSlate_Tooltip::Ready_Children()
{
	CUIObject* pGameObject = nullptr;

	UIOBJECT_DESC Desc{};
	Desc.fZ = 1;
	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Slate_Tooltip_Mid"), &Desc));
	if (nullptr == pGameObject)
		return E_FAIL;
	Add_Child(pGameObject);

	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Slate_Tooltip_Bottom"), &Desc));
	if (nullptr == pGameObject)
		return E_FAIL;
	Add_Child(pGameObject);

	CUI_KeyGuide::KEYGUIDE_DESC KeyDesc{};

	KeyDesc.strKey = TEXT("R");
	KeyDesc.Default = false;
	KeyDesc.fX = 55;
	KeyDesc.fY = 217;
	KeyDesc.fZ = 4;
	KeyDesc.bTarget = false;
	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Slate_Guide"), &KeyDesc));
	if (nullptr == pGameObject)
		return E_FAIL;
	Add_Child(pGameObject);

	for (_int i = -2; i < 3; ++i)
	{
		for (_int j = -2; j < 4; ++j)
		{
			Desc.fX = j;
			Desc.fY = i;
			pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Slate_Tooltip_Slot"), &Desc));
			if (nullptr == pGameObject)
				return E_FAIL;
			Add_Child(pGameObject);
		}
	}
	
	return S_OK;
}

void CSlate_Tooltip::Render_Font()
{
	TCHAR szText[MAX_PATH];
	D3DXCOLOR TexColor{};

	CUIObject::Font_Rect_Update();

	_uint iItemID = m_pItemObject->Item_Info()->iItemID;

	m_vTexRect.left -= 30;
	switch (g_ItemDataBase[iItemID].m_eRarity)
	{
	case ITEM_RARITY::NORMAL:
		_stprintf_s(szText, TEXT("[일반]"));
		TexColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		break;
	case ITEM_RARITY::RARE:
		_stprintf_s(szText, TEXT("[고급]"));
		TexColor = D3DXCOLOR(0.3f, 1.f, 0.f, 1.f);
		break;
	case ITEM_RARITY::EPIC:
		_stprintf_s(szText, TEXT("[희귀]"));
		TexColor = D3DXCOLOR(0.f, 0.75f, 1.f, 1.f);
		break;
	case ITEM_RARITY::LEGENDARY:
		_stprintf_s(szText, TEXT("[전설]"));
		TexColor = D3DXCOLOR(1.f, 1.f, 0.f, 1.f);
		break;
	}
	m_vTexRect.right -= 30;
	m_pGameInstance->Render_Font(TEXT("UI_Font_22_Damage"), g_ItemDataBase[iItemID].m_szName, m_vTexRect, TexColor, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	CUIObject::Font_Rect_Update();
	m_vTexRect.right -= 50;
	m_vTexRect.top += 220;
	m_vTexRect.left += 50;

	m_pGameInstance->Render_Font(TEXT("UI_Font_16_Tooltip"), szText, m_vTexRect, TexColor, DT_CENTER | DT_TOP);

	m_vTexRect.top += 163;
	m_vTexRect.bottom += 161;
	if (g_SlateDataBase[g_ItemDataBase[iItemID].m_iItemValue].m_bRotation)
	{
		_stprintf_s(szText, TEXT("[회전 가능]"));
		m_pGameInstance->Render_Font(TEXT("UI_Font_16_Tooltip"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f) , DT_CENTER | DT_TOP);
	}
	else
	{
		_stprintf_s(szText, TEXT("[회전 불가능]"));
		m_pGameInstance->Render_Font(TEXT("UI_Font_16_Tooltip"), szText, m_vTexRect, D3DXCOLOR(1.f, 0.f, 0.f, 1.f), DT_CENTER | DT_TOP);
	}

}

void CSlate_Tooltip::Render_Pos()
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

void CSlate_Tooltip::Set_UpGrade()
{
	if (m_pItemObject == nullptr)
		return;


	_uint iItemID = m_pItemObject->Item_Info()->iItemID;
	_int iAngle = m_pItemObject->Item_Info()->fAngle;

	_int iIndex = g_ItemDataBase[iItemID].m_iItemValue;
	_int iX = 0;
	_int iY = 0;
	

	for (_int i = 3; i < 33; ++i)
	{
		static_cast<CSlate_Tooltip_Slot*>(m_vecChildren[i])->Set_Vlaue(0);
	}
		
	static_cast<CSlate_Tooltip_Slot*>(m_vecChildren[18])->Set_Item(m_pItemObject);

	for (_int i = 0; i < g_SlateDataBase[iIndex].m_vecGardeValue.size(); ++i)
	{
		_int myX = 15 % 6;
		_int myY = 15 / 6;

		_int iValue = g_SlateDataBase[iIndex].m_vecGardeValue[i].m_iValue;
		
		switch (iAngle)
		{
		case 0:
			iX = g_SlateDataBase[iIndex].m_vecGardeValue[i].m_iX;
			iY = g_SlateDataBase[iIndex].m_vecGardeValue[i].m_iY;
			break;
		case 90:
			iX = -g_SlateDataBase[iIndex].m_vecGardeValue[i].m_iY;
			iY = g_SlateDataBase[iIndex].m_vecGardeValue[i].m_iX;
			break;
		case 180:
			iX = -g_SlateDataBase[iIndex].m_vecGardeValue[i].m_iX;
			iY = -g_SlateDataBase[iIndex].m_vecGardeValue[i].m_iY;
			break;
		case 270:
			iX = g_SlateDataBase[iIndex].m_vecGardeValue[i].m_iY;
			iY = -g_SlateDataBase[iIndex].m_vecGardeValue[i].m_iX;
			break;
		}

		_int iYouIndexX = myX + iX;
		_int iYouIndexY = myY + iY;

		if (iYouIndexX < 0 || iYouIndexX > 5 || iYouIndexY < 0 || iYouIndexY > 4)
			continue;

		_int YouIndex = iYouIndexX + iYouIndexY * 6;

		if (YouIndex > 30 || YouIndex < 0)
			continue;
		static_cast<CSlate_Tooltip_Slot*>(m_vecChildren[YouIndex+3])->Set_Vlaue(iValue);
	}
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

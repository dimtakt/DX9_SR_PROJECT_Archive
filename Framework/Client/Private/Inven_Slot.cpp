#include "Inven_Slot.h"
#include "GameInstance.h"
#include "Inven_Slot_Selete.h"
#include "Client_Defines_Item.h"
#include "Inventory.h"
#include "Gacha.h"
#include "Gacha_Slot.h"
#include "Tooltip.h"
#include "Hud_Slot.h"
CInven_Slot::CInven_Slot(LPDIRECT3DDEVICE9 pGraphic_Device) : CButton{ pGraphic_Device }
{
}

CInven_Slot::CInven_Slot(const CInven_Slot& Prototype) : CButton(Prototype), m_eLevel(Prototype.m_eLevel), m_bIsOver(false)
{
}

HRESULT CInven_Slot::Initialize_Prototype(LEVEL eLevel)
{
	m_eLevel = eLevel;

	if (FAILED(Ready_ChildPrototype(eLevel)))
		return E_FAIL;

	return S_OK;
}

HRESULT CInven_Slot::Initialize(void* pArg)
{
	INVEN_SLOT_DESC* Desc = static_cast<INVEN_SLOT_DESC*>(pArg);
	m_iSlotIndex = Desc->iSlotInedx;

	m_fSizeX = 76;
	m_fSizeY = 76;
	m_fX = -203 + Desc->fX * (m_fSizeX + 5);
	m_fY = -151 + Desc->fY * (m_fSizeY + 5);
	m_fZ = UI_DEPTH::INVEN_SLOT;
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

void CInven_Slot::Priority_Update(_float fTimeDelta)
{
	if (m_pSlotItem == nullptr)
		m_iItemCount = 0;

	m_iSlotGradeCount = 0;
	if (m_bIsOver)
		__super::Priority_Update(fTimeDelta);
}

void CInven_Slot::Update(_float fTimeDelta)
{
	if (CUIObject::isPick(g_hWnd) && m_pGameInstance->IsKeyDown('R') && m_pSlotItem != nullptr)
		m_pSlotItem->IsRotation_Slate();
	
	if (m_pOldSlotItem != m_pSlotItem)
	{
		for (_int i = 0; i < 3; ++i)
		{
			static_cast<CTooltip*>(m_vecChildren[i])->Change_Item(m_pSlotItem);
		}
		m_pOldSlotItem = m_pSlotItem;
	}
	Item_Selete();
	Subscribe_Item();
	Setting_Item();
	if (m_bIsOver)
		__super::Update(fTimeDelta);
}

void CInven_Slot::Late_Update(_float fTimeDelta)
{
	if (CUIObject::isPick(g_hWnd))
		m_bIsOver = true;
	else
		m_bIsOver = false;

	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);

	if (m_bIsOver && m_pSlotItem != nullptr)
	{

		m_vecChildren[3]->Late_Update(fTimeDelta);
		//ARTEFACT, STONE, POTION, SKILLBOOK
		switch (m_pSlotItem->Item_Info()->iItemType)
		{
		case 0:
			m_vecChildren[0]->Late_Update(fTimeDelta);
			break;
		case 1:
			m_vecChildren[1]->Late_Update(fTimeDelta);
			break;
		case 2:
			m_vecChildren[2]->Late_Update(fTimeDelta);
			break;
		case 3:
			m_vecChildren[0]->Late_Update(fTimeDelta);
			break;
		}
	}
	else if(m_bIsOver)
	{
		m_vecChildren[3]->Late_Update(fTimeDelta);
	}
	if (m_pSlotItem != nullptr)
	{
		m_pSlotItem->Late_Update(fTimeDelta, m_pTransformCom->Get_State(STATE::POSITION));
	}
	
	if (m_bIsPick)
		m_pSlotItem->IsSelete();

	m_iValue = m_iSlotGradeCount;
}

HRESULT CInven_Slot::Render()
{
	if (FAILED(CButton::Bind_ButtonTex_Single(g_hWnd, m_iSlotItem_Tex)))
		return E_FAIL;

	Render_Font();
	return S_OK;
}

void CInven_Slot::Add_Item(CItem_Base* pItem)
{
	m_pSlotItem = pItem;
	m_iItemCount += 1;
}

_bool CInven_Slot::Down_Item_Count()
{
	--m_iItemCount;

	if (m_iItemCount <= 0)
	{
		Safe_Release(m_pSlotItem);
		Safe_Release(m_pOldSlotItem);
		return true;
	}
	return false;
}

void CInven_Slot::Push_Item(CItemObject* pItem)
{
	m_pSlotItem = static_cast<CItem_Base*>(pItem);
}

CItem_Base* CInven_Slot::Pop_Item()
{
	return m_pSlotItem;
}

_int CInven_Slot::Slot_Info(ITEM_INFO eInfo)
{
	switch (eInfo)
	{
	case Client::ITEM_INFO::ITEM_TYPE:
		return static_cast<_int>(m_eItemType);
	case Client::ITEM_INFO::ITEM_VALUE:
		return m_iItemValue;
	case ITEM_INFO::ITEM_COUNT:
		return m_iItemCount;
	}
}

void CInven_Slot::Add_GradeCount(_int iValue)
{
	m_iSlotGradeCount += iValue;
	
}

void CInven_Slot::Setting_Item()
{
	if (m_pSlotItem == nullptr)
	{
		m_iSlotItem_MaxGrade = 0;
		m_iSlotItem_Tex = 0;
		m_iItemValue = 0;
		m_eItemType = ITEM_TYPE::ITEM_TYPE_END;
		return;
	}
	else if (m_pSlotItem != nullptr)
	{
		m_eItemType = static_cast<ITEM_TYPE>(m_pSlotItem->Item_Info()->iItemType);
		m_iItemValue = m_pSlotItem->Item_Info()->iItemValue;

		switch (m_eItemType)
		{
		case ITEM_TYPE::ARTEFACT:
			m_iSlotItem_MaxGrade = m_iItemValue;
			switch (m_pSlotItem->Item_Info()->iRarity)
			{
			case ENUM_CLASS(ITEM_RARITY::NORMAL):
				m_iSlotItem_Tex = 2;
				break;
			case ENUM_CLASS(ITEM_RARITY::RARE):
				m_iSlotItem_Tex = 3;
				break;
			case ENUM_CLASS(ITEM_RARITY::EPIC):
				m_iSlotItem_Tex = 4;
				break;
			case ENUM_CLASS(ITEM_RARITY::LEGENDARY):
				m_iSlotItem_Tex = 5;
				break;
			}
			break;
		case ITEM_TYPE::SKILLBOOK:
			m_iSlotItem_MaxGrade = m_iItemValue;
			switch (m_pSlotItem->Item_Info()->iRarity)
			{
			case ENUM_CLASS(ITEM_RARITY::NORMAL):
				m_iSlotItem_Tex = 2;
				break;
			case ENUM_CLASS(ITEM_RARITY::RARE):
				m_iSlotItem_Tex = 3;
				break;
			case ENUM_CLASS(ITEM_RARITY::EPIC):
				m_iSlotItem_Tex = 4;
				break;
			case ENUM_CLASS(ITEM_RARITY::LEGENDARY):
				m_iSlotItem_Tex = 5;
				break;
			}
			break;
		case ITEM_TYPE::STONE:
			m_iSlotItem_Tex = 6;
			break;
		case ITEM_TYPE::POTION:
			m_iSlotItem_MaxGrade = g_PotionDataBase[m_iItemValue].m_iMaxPotion;
			m_iSlotItem_Tex = 0;
			break;
		}
	}
}

void CInven_Slot::Item_Selete()
{

	if (Check_Key_Down(g_hWnd, VK_LBUTTON) && m_pSlotItem != nullptr)
	{
		m_pGameInstance->Pick_ItemSlot(m_pSlotItem, this, m_iItemCount, 1);
		m_bIsPick = true;
		m_pGameInstance->StopSound(ENUM_CLASS(CHANNELID::SOUND_UI));
		m_pGameInstance->PlaySoundW(L"InventorySelect001.wav", ENUM_CLASS(CHANNELID::SOUND_UI), g_fUIVolume - 0.8f);
	}


	if (m_pGameInstance->Pop_Item() != nullptr && Check_Key_UP(g_hWnd, VK_LBUTTON))
	{
		if (m_pGameInstance->Pop_ISlot_Type() == 1)
		{
			m_pGameInstance->Pop_Slot()->Push_Item(m_pSlotItem);
			m_pGameInstance->Pop_Slot()->Push_Item_Count(m_iItemCount);
			m_pGameInstance->Pop_Slot()->IsPick_off();
			m_pSlotItem = static_cast<CItem_Base*>(m_pGameInstance->Pop_Item());
			m_iItemCount = m_pGameInstance->Pop_Item_Count();
		}
		else if (m_pGameInstance->Pop_ISlot_Type() == 2 && m_pSlotItem != nullptr)
		{
			static_cast<CInventory*>(m_pGameInstance->Get_GameObject(ENUM_CLASS(LEVEL::LEVEL_STATIC),
				TEXT("Layer_Inventory")))->Push_Item_Slot(static_cast<CItem_Base*>(m_pGameInstance->Pop_Item()), 
					m_pGameInstance->Pop_Item_Count());

			m_pGameInstance->AcquiredItem_List_Add(static_cast<CItem_Base*>(m_pGameInstance->Pop_Item())->Item_Info()->iItemID);
			static_cast<CHud_Slot*>(m_pGameInstance->Find_UIObj(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Hud_Slot")))->SlotToItem(static_cast<CItem_Base*>(m_pGameInstance->Pop_Item()));

			static_cast<CGacha_Slot*>(m_pGameInstance->Pop_Slot())->Release_Pop();

			static_cast<CGacha*>(m_pGameInstance->Get_GameObject(ENUM_CLASS(LEVEL::LEVEL_STATIC),
				TEXT("Layer_Gacha")))->Release_Slot();
			static_cast<CGacha*>(m_pGameInstance->Get_GameObject(ENUM_CLASS(LEVEL::LEVEL_STATIC),
				TEXT("Layer_Gacha")))->Random_off();
			static_cast<CGacha*>(m_pGameInstance->Get_GameObject(ENUM_CLASS(LEVEL::LEVEL_STATIC),
				TEXT("Layer_Gacha")))->UI_Switch();

		}
		else if (m_pGameInstance->Pop_ISlot_Type() == 2 && m_pSlotItem == nullptr)
		{
		
			m_pSlotItem = static_cast<CItem_Base*>(m_pGameInstance->Pop_Item());
			m_pGameInstance->AcquiredItem_List_Add(static_cast<CItem_Base*>(m_pGameInstance->Pop_Item())->Item_Info()->iItemID);
			m_iItemCount = m_pGameInstance->Pop_Item_Count();
			static_cast<CHud_Slot*>(m_pGameInstance->Find_UIObj(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Hud_Slot")))->SlotToItem(static_cast<CItem_Base*>(m_pGameInstance->Pop_Item()));

			static_cast<CGacha_Slot*>(m_pGameInstance->Pop_Slot())->Release_Pop();

			static_cast<CGacha*>(m_pGameInstance->Get_GameObject(ENUM_CLASS(LEVEL::LEVEL_STATIC),
				TEXT("Layer_Gacha")))->Release_Slot();
			static_cast<CGacha*>(m_pGameInstance->Get_GameObject(ENUM_CLASS(LEVEL::LEVEL_STATIC),
				TEXT("Layer_Gacha")))->Random_off();
			static_cast<CGacha*>(m_pGameInstance->Get_GameObject(ENUM_CLASS(LEVEL::LEVEL_STATIC),
				TEXT("Layer_Gacha")))->UI_Switch();
		}
		m_pGameInstance->StopSound(ENUM_CLASS(CHANNELID::SOUND_UI));
		m_pGameInstance->PlaySoundW(L"InventorySelect002.wav", ENUM_CLASS(CHANNELID::SOUND_UI), g_fUIVolume - 0.8f);
		m_pGameInstance->Pop_Slot()->IsPick_off();
		
	}
	
}

void CInven_Slot::Subscribe_Item()
{
	if (Check_Key_Down(g_hWnd, VK_RBUTTON) && m_pSlotItem != nullptr)
	{
		if (m_pSlotItem->Item_Info()->iItemType == ENUM_CLASS(ITEM_TYPE::POTION) || m_pSlotItem->Item_Info()->iItemType == ENUM_CLASS(ITEM_TYPE::SKILLBOOK))
		{
			static_cast<CHud_Slot*>(m_pGameInstance->Find_UIObj(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Hud_Slot")))->Subscribe_Item(m_pSlotItem);
		}
	}
}

HRESULT CInven_Slot::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Rect_Slot_Inven"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CInven_Slot::Ready_ChildPrototype(LEVEL eLevel)
{
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_Inven_Slot_Selete"),
		CInven_Slot_Selete::Create(m_pGraphic_Device, eLevel))))
		return E_FAIL;

	return S_OK;
}

HRESULT CInven_Slot::Ready_Children()
{
	CUIObject* pGameObject = nullptr;

	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_Artefact_Tooltip")));
	if (nullptr == pGameObject)
		return E_FAIL;
	Add_Child(pGameObject);

	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_Slate_Tooltip")));
	if (nullptr == pGameObject)
		return E_FAIL;
	Add_Child(pGameObject);

	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_Potion_Tooltip")));
	if (nullptr == pGameObject)
		return E_FAIL;
	Add_Child(pGameObject);

	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Inven_Slot_Selete")));
	if (nullptr == pGameObject)
		return E_FAIL;
	Add_Child(pGameObject);


	return S_OK;
}

void CInven_Slot::Render_Font()
{
	Font_Rect_Update();
	m_vTexRect.left += 11;
	m_vTexRect.top += 11;
	m_vTexRect.right -= 11;
	m_vTexRect.bottom -= 11;
	TCHAR szText[64];
	D3DXCOLOR TexColor{};
	if (m_pSlotItem == nullptr)
	{
		if (m_iSlotGradeCount == 0)
			return;
		if (m_iSlotGradeCount > 0)
		{
			_stprintf_s(szText, TEXT("+%d"),m_iSlotGradeCount);
			m_pGameInstance->Render_Font(TEXT("UI_Font_12"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), DT_TOP | DT_LEFT);
			return;
		}
		else
		{
			_stprintf_s(szText, TEXT("%d"),m_iSlotGradeCount);
			m_pGameInstance->Render_Font(TEXT("UI_Font_12"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), DT_TOP | DT_LEFT);
			return;
		}
	}
	
	switch (m_eItemType)
	{
	case Client::ITEM_TYPE::ARTEFACT:
		if (m_iSlotGradeCount < 0)
			TexColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);
		else if (m_iSlotGradeCount > m_iSlotItem_MaxGrade)
			TexColor = D3DXCOLOR(1.f, 0.647f, 0.f, 1.f);
		else if(m_iSlotGradeCount == m_iSlotItem_MaxGrade)
			TexColor = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);
		else
			TexColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		
		_stprintf_s(szText, TEXT("%d/%d"), m_iSlotGradeCount, m_iSlotItem_MaxGrade);
		m_pGameInstance->Render_Font(TEXT("UI_Font_12"), szText, m_vTexRect, TexColor, DT_TOP | DT_LEFT);
		break;
	case Client::ITEM_TYPE::STONE:
		break;
	case Client::ITEM_TYPE::POTION:
		if(m_iItemCount >= m_iSlotItem_MaxGrade)
			TexColor = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);
		else
			TexColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		_stprintf_s(szText, TEXT("%d/%d"),m_iItemCount, m_iSlotItem_MaxGrade);
		m_pGameInstance->Render_Font(TEXT("UI_Font_12"), szText, m_vTexRect, TexColor, DT_RIGHT | DT_BOTTOM);
		break;
	case Client::ITEM_TYPE::SKILLBOOK:
		if (m_iSlotGradeCount < 0)
			TexColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);
		else if (m_iSlotGradeCount > m_iSlotItem_MaxGrade)
			TexColor = D3DXCOLOR(1.f, 0.647f, 0.f, 1.f);
		else if (m_iSlotGradeCount == m_iSlotItem_MaxGrade)
			TexColor = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);
		else
			TexColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

		_stprintf_s(szText, TEXT("%d/%d"),m_iSlotGradeCount, m_iSlotItem_MaxGrade);
		m_pGameInstance->Render_Font(TEXT("UI_Font_12"), szText, m_vTexRect, TexColor, DT_TOP | DT_LEFT);
		break;
	}

}


CInven_Slot* CInven_Slot::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
	CInven_Slot* pInstance = new CInven_Slot(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eLevel)))
	{
		MSG_BOX(TEXT("Failed to Created : CInven_Slot"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CInven_Slot::Clone(void* pArg)
{
	CInven_Slot* pInstance = new CInven_Slot(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CInven_Slot"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CInven_Slot::Free()
{
	Safe_Release(m_pSlotItem);
	Safe_Release(m_pOldSlotItem);
	__super::Free();
}

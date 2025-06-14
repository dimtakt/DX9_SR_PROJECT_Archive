#include "Hud_Quick_Slot.h"
#include "GameInstance.h"
#include "Hud_Slot_Guide.h"
#include "Stat_Manager.h"
#include "Inventory.h"
#include "Hud_Slot_CoolTime.h"
#include "Hud_Slot_EatPotion.h"

CHud_Quick_Slot::CHud_Quick_Slot(LPDIRECT3DDEVICE9 pGraphic_Device) : CButton{ pGraphic_Device }
{
}

CHud_Quick_Slot::CHud_Quick_Slot(const CHud_Quick_Slot& Prototype) : CButton(Prototype), m_eLevel(Prototype.m_eLevel)
{
}

HRESULT CHud_Quick_Slot::Initialize_Prototype(LEVEL eLevel)
{
	m_eLevel = eLevel;

	if (FAILED(Ready_ChildPrototype(eLevel)))
		return E_FAIL;

	return S_OK;
}

HRESULT CHud_Quick_Slot::Initialize(void* pArg)
{
	UIOBJECT_DESC* Desc = static_cast<UIOBJECT_DESC*>(pArg);

	m_iIndex = Desc->fY;
	m_szVkKey = TEXT('0') + m_iIndex;
	m_fPotionCoolTime = 60;
	m_fEatSpeed = 0;
	
	if (m_iIndex == 1)
		m_bSelete = true;
	m_fSizeX = 50;
	m_fSizeY = 50;
	m_fX = Desc->fX * 60 - (Desc->fZ - 1) * 60 / 2;
	m_fY = 0;
	m_fZ = UI_DEPTH::HUD_Qyick_Slot;
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

void CHud_Quick_Slot::Priority_Update(_float fTimeDelta)
{
}

void CHud_Quick_Slot::Update(_float fTimeDelta)
{
	if (m_pSlotItem == nullptr)
	{
		m_fItemCooltime = 0;
		m_fItemCulCool = 0;
		return;
	}

	if (m_pSlotItem->Item_Info()->iItemType == ENUM_CLASS(ITEM_TYPE::POTION))
	{
		if (Use_Potion())
		{
			if (m_pSlotItem != nullptr)
				m_fPotionCulCool = 60;
		}
		ItemCount();
		if (m_fPotionCulCool > 0)
		{
			static_cast<CHud_Slot_CoolTime*>(m_vecChildren[0])->Progerss_Set(m_fPotionCulCool, m_fPotionCoolTime);
			m_fPotionCulCool -= 1;
		}
	}
	else if (m_pSlotItem->Item_Info()->iItemType == ENUM_CLASS(ITEM_TYPE::SKILLBOOK))
	{

		_int iEffectIndex = m_pSlotItem->Item_Info()->iArtefact_Value;
		if (g_ItemEffect[iEffectIndex].m_eType != ITEM_EFFECT::VALUE_TYPE)
		{
			m_fItemCooltime = m_pGameInstance->Get_ItemCool(true, g_ItemEffect[iEffectIndex].m_szEffectTag);
			m_fItemCulCool = m_pGameInstance->Get_ItemCool(false, g_ItemEffect[iEffectIndex].m_szEffectTag);
		}

		if (m_fItemCulCool > 0)
		{
			static_cast<CHud_Slot_CoolTime*>(m_vecChildren[0])->Progerss_Set(m_fItemCulCool, m_fItemCooltime);
		}
		
		if (m_fItemCulCool <= 0 && m_pGameInstance->IsKeyHold('R') && m_bSelete)
		{
			
			if (g_ItemEffect[m_pSlotItem->Item_Info()->iArtefact_Value].m_szEffectTag == TEXT("Ice Bolt"))
			{
				m_pGameInstance->Broadcast(ENUM_CLASS(EVENT_TYPE::ICEBOLT), &pEvent);
			}
			else if(g_ItemEffect[m_pSlotItem->Item_Info()->iArtefact_Value].m_szEffectTag == TEXT("Meteor"))
			{
				m_pGameInstance->Broadcast(ENUM_CLASS(EVENT_TYPE::METEOR), &pEvent);
			}
		}
	}

	if (m_bEating)
	{
		m_fEatSpeed += 1;
		static_cast<CHud_Slot_CoolTime*>(m_vecChildren[1])->Progerss_Set(m_fEatSpeed, 60);
		m_pGameInstance->PlayLoopSound(TEXT("Drinking01.wav"), ENUM_CLASS(CHANNELID::UI_QUICKSLOT), g_fUIVolume - 0.4f);
		//if (m_fEatSpeed == 0 || m_fEatSpeed == 20 || m_fEatSpeed == 40 || m_fEatSpeed == 60)
		//{
		//	m_pGameInstance->StopSound(ENUM_CLASS(CHANNELID::UI_QUICKSLOT));
		//	m_pGameInstance->PlayLoopSound(TEXT("Drinking01.wav"), ENUM_CLASS(CHANNELID::UI_QUICKSLOT), g_fUIVolume - 0.4f);
		//}

		if (m_fEatSpeed > 60)
		{
			m_fEatSpeed = 0;
			m_bEatPotion = true;
			m_pGameInstance->StopSound(ENUM_CLASS(CHANNELID::UI_QUICKSLOT));
		}
	}
}

void CHud_Quick_Slot::Late_Update(_float fTimeDelta)
{

	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
	if (m_pSlotItem != nullptr)
	{
		if (m_pSlotItem->Item_Info()->iItemType == ENUM_CLASS(ITEM_TYPE::POTION))
		{
			if (m_fPotionCulCool > 0)
				m_vecChildren[0]->Late_Update(fTimeDelta);
		}
		else if (m_pSlotItem->Item_Info()->iItemType == ENUM_CLASS(ITEM_TYPE::SKILLBOOK))
		{
			if (m_fItemCulCool > 0)
				m_vecChildren[0]->Late_Update(fTimeDelta);
		}
	}
	if (m_bEating)
	{
		m_vecChildren[1]->Late_Update(fTimeDelta);
		m_pSlotItem->IsEat_Render(static_cast<CTransform*>(m_pGameInstance->Get_Component(m_pGameInstance->Get_CurrentLevel(), TEXT("Layer_Player"), TEXT("Com_Transform"))));
	}

	if (m_pSlotItem != nullptr)
		m_pSlotItem->IsQuickSlot_Render(m_vWorldPos);
	
	m_vecChildren[2]->Late_Update(fTimeDelta);
}

HRESULT CHud_Quick_Slot::Render()
{
	if (m_bSelete)
	{
		if (FAILED(CButton::Bind_ButtonTex_Single(g_hWnd, 1)))
			return E_FAIL;
	}
	else
	{
		if (FAILED(CButton::Bind_ButtonTex_Single(g_hWnd, 0)))
			return E_FAIL;
	}

	if (m_pSlotItem != nullptr && m_pSlotItem->Item_Info()->iItemType == ENUM_CLASS(ITEM_TYPE::POTION))
		Render_Font();

	return S_OK;
}

void CHud_Quick_Slot::Selete_Slot(_int iKey)
{
	if (iKey == m_iIndex)
		m_bSelete = true;
	else
		m_bSelete = false;
}


HRESULT CHud_Quick_Slot::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Rect_UI_Hud_Quick_Slot"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CHud_Quick_Slot::Ready_ChildPrototype(LEVEL eLevel)
{
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_Slot_Cool"),
		CHud_Slot_CoolTime::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_Slot_Eat"),
		CHud_Slot_EatPotion::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_Slot_Guide"),
		CHud_Slot_Guide::Create(m_pGraphic_Device))))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CHud_Quick_Slot::Ready_Children()
{
	CUIObject* pGameObject = nullptr;
	
	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Slot_Cool")));
	if (nullptr == pGameObject)
		return E_FAIL;
	Add_Child(pGameObject);

	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Slot_Eat")));
	if (nullptr == pGameObject)
		return E_FAIL;
	Add_Child(pGameObject);

	CHud_Slot_Guide::SLOT_KEYGUIDE_DESC Desc{};
	
	Desc.strKey = to_wstring(m_iIndex);
	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Slot_Guide"), &Desc));
	if (nullptr == pGameObject)
		return E_FAIL;
	Add_Child(pGameObject);

	return S_OK;
}

void CHud_Quick_Slot::Render_Font()
{
	CUIObject::Font_Rect_Update();
	
	TCHAR szText[MAX_PATH];
	_int iValue = m_pSlotItem->Item_Info()->iItemValue;
	_int iMaxItem = g_PotionDataBase[iValue].m_iMaxPotion;
	m_vTexRect.top += -4;

	m_vTexRect.left		+= 1;
	m_vTexRect.top		+= 1;
	m_vTexRect.right	+= 1;
	m_vTexRect.bottom	+= 1;
	_stprintf_s(szText, TEXT("%d/%d"), m_iItemCount, iMaxItem);
	m_pGameInstance->Render_Font(TEXT("UI_Font_12_QuickSlot"), szText, m_vTexRect, D3DXCOLOR(0.f, 0.f, 0.f, 1.f), DT_CENTER | DT_TOP);

	m_vTexRect.left		-= 1;
	m_vTexRect.top		-= 1;
	m_vTexRect.right	-= 1;
	m_vTexRect.bottom	-= 1;
	_stprintf_s(szText, TEXT("%d/%d"), m_iItemCount, iMaxItem);
	m_pGameInstance->Render_Font(TEXT("UI_Font_12_QuickSlot"), szText, m_vTexRect, D3DXCOLOR(0.6f, 1.f, 0.f, 1.f), DT_CENTER | DT_TOP);
}

void CHud_Quick_Slot::ItemCount()
{
	if (m_pSlotItem != nullptr)
	{
		if (m_pSlotItem->Item_Info()->iItemType == ENUM_CLASS(ITEM_TYPE::POTION))
		{
			m_iItemCount = static_cast<CInventory*>(m_pGameInstance->Find_UIObj(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("UI_Inven")))->Item_Count(m_pSlotItem);
		}
	}
}

_bool CHud_Quick_Slot::Use_Potion()
{
	if (!m_bSelete)
		return false;
	if (m_fPotionCulCool <= 0 && m_pGameInstance->IsKeyHold('R'))
	{
		m_bEating = true;
		CStat_Manager::GetInstance()->Set_UIOpen(true);
		if (m_bEatPotion)
		{
			_int iValue = m_pSlotItem->Item_Info()->iItemValue;
			_float fHealValue = g_PotionDataBase[iValue].m_iValue;
			_float fMaxValue{};

			switch (g_PotionDataBase[iValue].m_iPotionType)
			{
			case 1:
				fMaxValue = CStat_Manager::GetInstance()->Get_CurStats()[ENUM_CLASS(STAT_INFO::MAXHP)];
				CStat_Manager::GetInstance()->Cal_Stats(STAT_INFO::CULHP, fMaxValue * fHealValue * 0.01f);
				break;
			case 2:
				fMaxValue = CStat_Manager::GetInstance()->Get_CurStats()[ENUM_CLASS(STAT_INFO::MAXMP)];
				CStat_Manager::GetInstance()->Cal_Stats(STAT_INFO::CULMP, fMaxValue * fHealValue * 0.01f);
				break;
			}

			if (static_cast<CInventory*>(m_pGameInstance->Find_UIObj(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("UI_Inven")))->Use_Item(m_pSlotItem))
			{
				m_iItemCount = 0;
				m_pSlotItem = nullptr;
			}
			m_bEating = false;
			m_fEatSpeed = 0;
			m_bEatPotion = false;
			CStat_Manager::GetInstance()->Set_UIOpen(false);
			return true;
		}
	}
	else if (m_pGameInstance->IsKeyUp('R'))
	{
		if(CStat_Manager::GetInstance()->Get_UIOpen())
			CStat_Manager::GetInstance()->Set_UIOpen(false);
		m_bEating = false;
		m_fEatSpeed = 0;
	}
	return false;
}

CHud_Quick_Slot* CHud_Quick_Slot::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
	CHud_Quick_Slot* pInstance = new CHud_Quick_Slot(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eLevel)))
	{
		MSG_BOX(TEXT("Failed to Created : CHud_Quick_Slot"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CHud_Quick_Slot::Clone(void* pArg)
{
	CHud_Quick_Slot* pInstance = new CHud_Quick_Slot(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CHud_Quick_Slot"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CHud_Quick_Slot::Free()
{
	m_pSlotItem = nullptr;
	__super::Free();

}

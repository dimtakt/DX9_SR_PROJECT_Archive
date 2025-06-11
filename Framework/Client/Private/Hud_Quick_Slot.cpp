#include "Hud_Quick_Slot.h"
#include "GameInstance.h"
#include "Hud_Slot_Guide.h"
#include "Stat_Manager.h"
#include "Inventory.h"
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
	Use_Item();
}

void CHud_Quick_Slot::Late_Update(_float fTimeDelta)
{

	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
	__super::Late_Update(fTimeDelta);

	if (m_pSlotItem != nullptr)
		m_pSlotItem->IsQuickSlot_Render(m_vWorldPos);
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
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_Slot_Guide"),
		CHud_Slot_Guide::Create(m_pGraphic_Device))))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CHud_Quick_Slot::Ready_Children()
{
	CUIObject* pGameObject = nullptr;

	CHud_Slot_Guide::SLOT_KEYGUIDE_DESC Desc{};
	
	Desc.strKey = to_wstring(m_iIndex);
	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Slot_Guide"), &Desc));
	if (nullptr == pGameObject)
		return E_FAIL;
	Add_Child(pGameObject);

	return S_OK;
}

void CHud_Quick_Slot::Use_Item()
{
	if (!m_bSelete)
		return;
	if (m_pSlotItem == nullptr)
		return;
	if (m_pSlotItem->Item_Info()->iItemType != ENUM_CLASS(ITEM_TYPE::POTION))
		return;
	
	if (m_pGameInstance->IsKeyDown('R'))
	{
		_int iValue = m_pSlotItem->Item_Info()->iItemValue;
		_float fHealValue = g_PotionDataBase[iValue].m_iValue;
		_float fMaxValue{};
	
		switch (g_PotionDataBase[iValue].m_iPotionType)
		{
		case 1:
			fMaxValue = CStat_Manager::GetInstance()->Get_CurStats()[ENUM_CLASS(STAT_INFO::MAXHP)];
			CStat_Manager::GetInstance()->Cal_Stats(STAT_INFO::CULHP, fMaxValue * fHealValue * 0.1f);
			break;
		case 2:
			fMaxValue = CStat_Manager::GetInstance()->Get_CurStats()[ENUM_CLASS(STAT_INFO::MAXMP)];
			CStat_Manager::GetInstance()->Cal_Stats(STAT_INFO::CULMP, fMaxValue * fHealValue * 0.1f);
			break;
		}

		if (static_cast<CInventory*>(m_pGameInstance->Find_UIObj(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("UI_Inven")))->Use_Item(m_pSlotItem))
		{
			m_pSlotItem = nullptr;
		}
	}
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

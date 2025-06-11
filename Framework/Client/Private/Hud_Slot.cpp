#include "Hud_Slot.h"
#include "GameInstance.h"

CHud_Slot::CHud_Slot(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CHud_Slot::CHud_Slot(const CHud_Slot& Prototype) : CUIObject(Prototype), m_eLevel{ Prototype.m_eLevel }
{
}

HRESULT CHud_Slot::Initialize_Prototype(LEVEL eLevel)
{
	m_eLevel = eLevel;

	if (FAILED(Ready_ChildPrototype(eLevel)))
		return E_FAIL;

	return S_OK;
}

HRESULT CHud_Slot::Initialize(void* pArg)
{
	UIOBJECT_DESC* Desc = static_cast<UIOBJECT_DESC*>(pArg);

	m_fSizeX = 0;
	m_fSizeY = 0;
	m_fX = g_iWinSizeX * 0.5;
	m_fY = g_iWinSizeY - 50.f;;
	m_fZ = UI_DEPTH::HUD_Qyick_Slot;
	m_iWinSizeX = g_iWinSizeX;
	m_iWinSizeY = g_iWinSizeY;

	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
	__super::Update_Position();

	if (FAILED(Ready_Children()))
		return E_FAIL;

	m_pGameInstance->Add_UIObject(ENUM_CLASS(m_eLevel), TEXT("Hud_Slot"), this);

	return S_OK;
}

void CHud_Slot::Priority_Update(_float fTimeDelta)
{
	if (m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_LOADING) || m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_LOGO) || m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_MAPEDIT))
		return;

	if (!m_bIsUpdate)
		return;

	__super::Priority_Update(fTimeDelta);
}

void CHud_Slot::Update(_float fTimeDelta)
{
	if (m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_LOADING) || m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_LOGO) || m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_MAPEDIT))
		return;

	if (!m_bIsUpdate)
		return;
	
	Selete_Slot();
	__super::Update(fTimeDelta);
}

void CHud_Slot::Late_Update(_float fTimeDelta)
{
	if (m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_LOADING) || m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_LOGO) || m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_MAPEDIT))
		return;

	if (!m_bIsUpdate)
		return;

	__super::Late_Update(fTimeDelta);
}

HRESULT CHud_Slot::Render()
{
	return S_OK;
}

void CHud_Slot::SlotToItem(CItem_Base* pItem)
{
	for (_int i = 0; i < m_vecQuickSlot.size(); ++i)
	{
		if (pItem->Item_Info()->iItemType == ENUM_CLASS(ITEM_TYPE::ARTEFACT)
			|| pItem->Item_Info()->iItemType == ENUM_CLASS(ITEM_TYPE::STONE))
			return;

		if (m_vecQuickSlot[i]->Get_Itme() != nullptr)
		{
			if (m_vecQuickSlot[i]->Get_Itme()->Item_Info()->iItemID == pItem->Item_Info()->iItemID)
				return;
		}
		else
		{
			m_vecQuickSlot[i]->Push_Item(pItem);
			return;
		}
	}
}

void CHud_Slot::Subscribe_Item(CItem_Base* pItem)
{
	for (_int i = 0; i < m_vecQuickSlot.size(); ++i)
	{
		if (m_vecQuickSlot[i]->Get_Itme() == pItem)
		{
			m_vecQuickSlot[i]->Release_Item();
			return;
		}
	}

	for (_int i = 0; i < m_vecQuickSlot.size(); ++i)
	{
		if (m_vecQuickSlot[i]->Get_Itme() == nullptr)
		{
			m_vecQuickSlot[i]->Push_Item(pItem);
			return;
		}
	}
}

HRESULT CHud_Slot::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CHud_Slot::Ready_ChildPrototype(LEVEL eLevel)
{
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_UI_Hud_QuickSlot"),
		CHud_Quick_Slot::Create(m_pGraphic_Device, LEVEL::LEVEL_STATIC))))
		return E_FAIL;

	return S_OK;
}

HRESULT CHud_Slot::Ready_Children()
{
	CUIObject* pGameObject = nullptr;
	UIOBJECT_DESC Desc{};

	_int iSlotValue = 5;
	Desc.fZ = iSlotValue;
	for (_int i = 0; i < iSlotValue; ++i)
	{
		Desc.fY = i + 1;
		Desc.fX = i;
		pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Hud_QuickSlot"), &Desc));
		if (nullptr == pGameObject)
			return E_FAIL;
		Add_Child(pGameObject);
		m_vecQuickSlot.push_back(static_cast<CHud_Quick_Slot*>(pGameObject));
		Safe_AddRef(m_vecQuickSlot[i]);
	}
	return S_OK;
}

void CHud_Slot::Selete_Slot()
{
	if (m_pGameInstance->IsKeyDown('1'))
	{
		for (_int i = 0; i < 5; ++i)
			m_vecQuickSlot[i]->Selete_Slot(1);
	}
	else if (m_pGameInstance->IsKeyDown('2'))
	{
		for (_int i = 0; i < 5; ++i)
			m_vecQuickSlot[i]->Selete_Slot(2);
	}
	else if (m_pGameInstance->IsKeyDown('3'))
	{
		for (_int i = 0; i < 5; ++i)
			m_vecQuickSlot[i]->Selete_Slot(3);
	}
	else if (m_pGameInstance->IsKeyDown('4'))
	{
		for (_int i = 0; i < 5; ++i)
			m_vecQuickSlot[i]->Selete_Slot(4);
	}
	else if (m_pGameInstance->IsKeyDown('5'))
	{
		for (_int i = 0; i < 5; ++i)
			m_vecQuickSlot[i]->Selete_Slot(5);
	}
}

CHud_Slot* CHud_Slot::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
	CHud_Slot* pInstance = new CHud_Slot(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eLevel)))
	{
		MSG_BOX(TEXT("Failed to Create : CHud_Slot"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CHud_Slot::Clone(void* pArg)
{
	CHud_Slot* pInstance = new CHud_Slot(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CHud_Slot"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CHud_Slot::Free()
{
	for (auto& pItemObject : m_vecQuickSlot)
		Safe_Release(pItemObject);
	m_vecQuickSlot.clear();

	__super::Free();
}

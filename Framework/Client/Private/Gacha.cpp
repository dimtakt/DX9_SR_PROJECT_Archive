#include "Gacha.h"
#include "GameInstance.h"
#include "Gacha_Slot.h"
#include "Inventory.h"
#include "Gacha_Pass.h"
#include "Gacha_Reroll.h"
#include "Gacha_FX.h"

CGacha::CGacha(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CGacha::CGacha(const CGacha& Prototype) : CUIObject(Prototype), m_eLevel(Prototype.m_eLevel)
{
}

HRESULT CGacha::Initialize_Prototype(LEVEL eLevel)
{
	m_eLevel = eLevel;

	if (FAILED(Ready_ChildPrototype(eLevel)))
		return E_FAIL;

	return S_OK;
}

HRESULT CGacha::Initialize(void* pArg)
{
	m_fSizeX = g_iWinSizeX;
	m_fSizeY = g_iWinSizeY;
	m_fX = m_fSizeX * 0.5f;
	m_fY = m_fSizeY * 0.5f;
	m_fZ = UI_DEPTH::GACHA;
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

	m_pGameInstance->Add_UIObject(ENUM_CLASS(m_eLevel), TEXT("UI_Gacha"), this);

	return S_OK;
}

void CGacha::Priority_Update(_float fTimeDelta)
{
	if (m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_LOADING) || m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_LOGO) || m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_MAPEDIT))
		return;

	if (!m_bIsUpdate)
		return;

	if (!m_bIsOpen)
		return;

	if (!m_bIsReandom)
		Rand_Itme(GACHA_TYPE::STONE);

	__super::Priority_Update(fTimeDelta);
}

void CGacha::Update(_float fTimeDelta)
{
	if (m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_LOADING) || m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_LOGO) || m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_MAPEDIT))
		return;
	
	if (!m_bIsUpdate)
		return;

	if (m_pGameInstance->IsKeyDown(VK_UP))
		UI_Switch();

	if (!m_bIsOpen)
		return;

	if (m_pGameInstance->IsKeyDown(VK_ESCAPE))
	{
		static_cast<CInventory*>(m_pGameInstance->Get_GameObject(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Layer_Inventory")))->Close_UI();
		m_pGameInstance->All_Update_On();
		m_bIsOpen = false;

		return;
	}
	
	__super::Update(fTimeDelta);
}

void CGacha::Late_Update(_float fTimeDelta)
{
	if (m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_LOADING) || m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_LOGO) || m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_MAPEDIT))
		return;
	
	if (!m_bIsUpdate)
		return;

	if (!m_bIsOpen)
		return;

	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
	__super::Late_Update(fTimeDelta);
	
}

HRESULT CGacha::Render()
{
	m_pGraphic_Device->SetTexture(0, NULL);
	m_pVIBufferCom->Bind_Buffers();
	__super::Begin();
	m_pVIBufferCom->Render();
	__super::End();
	return S_OK;
}

void CGacha::UI_Switch()
{
	if (m_bIsOpen)
	{
		static_cast<CInventory*>(m_pGameInstance->Get_GameObject(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Layer_Inventory")))->Close_UI();
		m_pGameInstance->All_Update_On();
		m_bIsOpen = false;
	}
	else
	{
		m_pGameInstance->All_Update_Off();
		m_pGameInstance->Update_On(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("UI_Inven"));
		static_cast<CInventory*>(m_pGameInstance->Get_GameObject(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Layer_Inventory")))->Open_UI(250, 0);
		m_bIsUpdate = true;
		m_bIsOpen = true;
	}
}

void CGacha::Rand_Item_Set(GACHA_TYPE eType)
{
	Rand_Itme(eType);
}

HRESULT CGacha::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Rect_UI_BalckRect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CGacha::Ready_ChildPrototype(LEVEL eLevel)
{
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_Gacha_Slot"),
		CGacha_Slot::Create(m_pGraphic_Device, eLevel))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_Gacha_Reroll"),
		CGacha_Reroll::Create(m_pGraphic_Device, eLevel))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_Gacha_Pass"),
		CGacha_Pass::Create(m_pGraphic_Device, eLevel))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_Gacha_FX"),
		CGacha_FX::Create(m_pGraphic_Device))))
		return E_FAIL;

	return S_OK;
}

HRESULT CGacha::Ready_Children()
{
	CUIObject* pGameObject = nullptr;

	UIOBJECT_DESC Desc{};

	_float fDeg = 72.f;
	_int fRange = 150;
		
	for (_int i = 0; i < 5; i++)
	{
		Desc.fZ = i;
		Desc.fX = fRange * cosf(D3DXToRadian(fDeg * i - 90));
		Desc.fY = fRange * sinf(D3DXToRadian(fDeg * i - 90));

		pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Gacha_Slot"), &Desc));
		if (nullptr == pGameObject)
			return E_FAIL;
		Add_Child(pGameObject);

	}
	Ready_Fx();

	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Gacha_Reroll"), &Desc));
	if (nullptr == pGameObject)
		return E_FAIL;
	Add_Child(pGameObject);

	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Gacha_Pass"), &Desc));
	if (nullptr == pGameObject)
		return E_FAIL;
	Add_Child(pGameObject);

	

	return S_OK;
}

void CGacha::Ready_Fx()
{
	CUIObject* pGameObject = nullptr;

	UIOBJECT_DESC Desc{};

	Desc.fX = -100;
	Desc.fY = 0;
	Desc.fZ = 0;
	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Gacha_FX"), &Desc));
	Add_Child(pGameObject);

	Desc.fX = -120;
	Desc.fY = 50;
	Desc.fZ = 0;
	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Gacha_FX"), &Desc));
	Add_Child(pGameObject);

	Desc.fX = -140;
	Desc.fY = 70;
	Desc.fZ = 1;
	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Gacha_FX"), &Desc));
	Add_Child(pGameObject);

	Desc.fX = -200;
	Desc.fY = 70;
	Desc.fZ = 2;
	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Gacha_FX"), &Desc));
	Add_Child(pGameObject);

	Desc.fX = -240;
	Desc.fY = 80;
	Desc.fZ = 0;
	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Gacha_FX"), &Desc));
	Add_Child(pGameObject);

	Desc.fX = -300;
	Desc.fY = 45;
	Desc.fZ = 2;
	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Gacha_FX"), &Desc));
	Add_Child(pGameObject);

	Desc.fX = -320;
	Desc.fY = 30;
	Desc.fZ = 4;
	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Gacha_FX"), &Desc));
	Add_Child(pGameObject);

	Desc.fX = -340;
	Desc.fY = 40;
	Desc.fZ = 1;
	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Gacha_FX"), &Desc));
	Add_Child(pGameObject);

	Desc.fX = -440;
	Desc.fY = 80;
	Desc.fZ = 4;
	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Gacha_FX"), &Desc));
	Add_Child(pGameObject);

	Desc.fX = -460;
	Desc.fY = 80;
	Desc.fZ = 2;
	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Gacha_FX"), &Desc));
	Add_Child(pGameObject);

	Desc.fX = -490;
	Desc.fY = 60;
	Desc.fZ = 0;
	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Gacha_FX"), &Desc));
	Add_Child(pGameObject);

	Desc.fX = -520;
	Desc.fY = 30;
	Desc.fZ = 2;
	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Gacha_FX"), &Desc));
	Add_Child(pGameObject);

	Desc.fX = -530;
	Desc.fY = 30;
	Desc.fZ = 1;
	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Gacha_FX"), &Desc));
	Add_Child(pGameObject);
}

void CGacha::Rand_Itme(GACHA_TYPE eType)
{
	CItem_Base* pItem = nullptr;
	vector<_int> vecIndex;
	_int iTemp{};
	
	switch (eType)
	{
	case Client::CGacha::GACHA_TYPE::ALL:
		for (_int i = 0; i < g_ItemDataBase.size(); ++i)
		{
			if(g_ItemDataBase[i].m_eType == ITEM_TYPE::ARTEFACT || g_ItemDataBase[i].m_eType == ITEM_TYPE::SKILLBOOK, g_ItemDataBase[i].m_eType == ITEM_TYPE::STONE)
				vecIndex.push_back(g_ItemDataBase[i].m_iItemID);
		}
		break;

	case Client::CGacha::GACHA_TYPE::ARTEFACT:
		for (_int i = 0; i < g_ItemDataBase.size(); ++i)
		{
			if (g_ItemDataBase[i].m_eType == ITEM_TYPE::ARTEFACT || g_ItemDataBase[i].m_eType == ITEM_TYPE::SKILLBOOK)
				vecIndex.push_back(g_ItemDataBase[i].m_iItemID);
		}
		break;

	case Client::CGacha::GACHA_TYPE::STONE:
		for (_int i = 0; i < g_ItemDataBase.size(); ++i)
		{
			if (g_ItemDataBase[i].m_eType == ITEM_TYPE::STONE)
				vecIndex.push_back(g_ItemDataBase[i].m_iItemID);
		}
		break;
	}
	
	for (_int i = 0; i < 100; ++i)
	{
		_int iIndex1 = m_pGameInstance->Rand(0, vecIndex.size());
		_int iIndex2 = m_pGameInstance->Rand(0, vecIndex.size());
		iTemp = vecIndex[iIndex1];
		vecIndex[iIndex1] = vecIndex[iIndex2];
		vecIndex[iIndex2] = iTemp;
	}

	for (_int i = 0; i < 5; ++i)
	{
		pItem = static_cast<CItem_Base*>(m_pGameInstance->Get_ItemObject(vecIndex[i]));
		static_cast<CGacha_Slot*>(m_vecChildren[i])->Push_Item_ReRoll(pItem);
	}
	m_bIsReandom = true;
}

CGacha* CGacha::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
	CGacha* pInstance = new CGacha(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eLevel)))
	{
		MSG_BOX(TEXT("Failed to Created : CGacha"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CGacha::Clone(void* pArg)
{
	CGacha* pInstance = new CGacha(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CGacha"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CGacha::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
}

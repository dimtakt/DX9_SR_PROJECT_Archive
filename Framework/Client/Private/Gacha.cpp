#include "Gacha.h"
#include "GameInstance.h"
#include "Gacha_Slot.h"
#include "Inventory.h"
#include "Gacha_Pass.h"
#include "Gacha_Reroll.h"
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
		UI_Switch();
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

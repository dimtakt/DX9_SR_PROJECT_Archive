#include "Hud_Button.h"
#include "GameInstance.h"
#include "Inven_Button.h"
#include "Stats_Button.h"
#include "Mirror_Button.h"

CHud_Button::CHud_Button(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CHud_Button::CHud_Button(const CHud_Button& Prototype) : CUIObject(Prototype), m_eLevel{ Prototype.m_eLevel }
{
}

HRESULT CHud_Button::Initialize_Prototype(LEVEL eLevel)
{
	m_eLevel = eLevel;

	if (FAILED(Ready_ChildPrototype(eLevel)))
		return E_FAIL;

	return S_OK;
}

HRESULT CHud_Button::Initialize(void* pArg)
{
	UIOBJECT_DESC Desc{};

	Desc.fSizeX = 0;
	Desc.fSizeY = 0;
	Desc.fX = 5;
	Desc.fY = g_iWinSizeY - 40.f;
	Desc.fZ = 0.f;
	Desc.iWinSizeX = g_iWinSizeX;
	Desc.iWinSizeY = g_iWinSizeY;

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
	__super::Update_Position();

	if (FAILED(Ready_Children()))
		return E_FAIL;

	return S_OK;
}

void CHud_Button::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CHud_Button::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CHud_Button::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CHud_Button::Render()
{
	return S_OK;
}

HRESULT CHud_Button::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CHud_Button::Ready_ChildPrototype(LEVEL eLevel)
{
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_Button_Inven"),
		CInven_Button::Create(m_pGraphic_Device, eLevel))))
		return E_FAIL;

 	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_Button_Stats"),
		CStats_Button::Create(m_pGraphic_Device, eLevel))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_Button_Mirror"),
		CMirror_Button::Create(m_pGraphic_Device, eLevel))))
		return E_FAIL;

	return S_OK;
}

HRESULT CHud_Button::Ready_Children()
{
	CUIObject* pGameObject = nullptr;

	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Button_Inven")));
	if (nullptr == pGameObject)
		return E_FAIL;
	Add_Child(pGameObject);

	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Button_Stats")));
	if (nullptr == pGameObject)
		return E_FAIL;
	Add_Child(pGameObject);

	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Button_Mirror")));
	if (nullptr == pGameObject)
		return E_FAIL;
	Add_Child(pGameObject);

	return S_OK;
}

CHud_Button* CHud_Button::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
	CHud_Button* pInstance = new CHud_Button(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eLevel)))
	{
		MSG_BOX(TEXT("Failed to Create : CHud_Button"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CHud_Button::Clone(void* pArg)
{
	CHud_Button* pInstance = new CHud_Button(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CHud_Button"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CHud_Button::Free()
{
	__super::Free();
}

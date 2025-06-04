#include "Status_Window.h"
#include "GameInstance.h"
#include "Status_Frame_UP.h"
#include "Status_Frame_Mid.h"
#include "Status_Frame_Down.h"
#include "Hud_States_Frame.h"
#include "ChapMap.h"
CStatus_Window::CStatus_Window(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CStatus_Window::CStatus_Window(const CStatus_Window& Prototype) : CUIObject(Prototype), m_eLevel{ Prototype.m_eLevel }
{
}

HRESULT CStatus_Window::Initialize_Prototype(LEVEL eLevel)
{
	m_eLevel = eLevel;

	if (FAILED(Ready_ChildPrototype(eLevel)))
		return E_FAIL;

	return S_OK;
}

HRESULT CStatus_Window::Initialize(void* pArg)
{
	m_fSizeX = 0;
	m_fSizeY = 0;
	m_fX = 180;
	m_fY = g_iWinSizeY * 0.5;
	m_fZ = UI_DEPTH::PLAYER_STAUTS;
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

	m_pGameInstance->Add_UIObject(ENUM_CLASS(m_eLevel), TEXT("UI_Status"), this);

	return S_OK;
}

void CStatus_Window::UI_Switch()
{
	if (m_bIsOpen)
	{
		m_bIsOpen = false;
		static_cast<CHud_States_Frame*>(m_pGameInstance->Get_GameObject(m_pGameInstance->Get_CurrentLevel(), TEXT("Layer_Hud_States")))->FontRender_Switch();
	}
	else
	{
		static_cast<CHud_States_Frame*>(m_pGameInstance->Get_GameObject(m_pGameInstance->Get_CurrentLevel(), TEXT("Layer_Hud_States")))->FontRender_Switch();
		m_bIsOpen = true;
	}
}

void CStatus_Window::Priority_Update(_float fTimeDelta)
{
	if (m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_LOADING) || m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_LOGO) || m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_MAPEDIT))
		return;
	
	if (!m_bIsUpdate)
		return;

	if (!m_bIsOpen)
		return;

	__super::Priority_Update(fTimeDelta);
	
}

void CStatus_Window::Update(_float fTimeDelta)
{
	if (m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_LOADING) || m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_LOGO) || m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_MAPEDIT))
		return;
	
	if (!m_bIsUpdate)
		return;

	if (!m_bIsOpen)
		return;

	__super::Update(fTimeDelta);
}

void CStatus_Window::Late_Update(_float fTimeDelta)
{
	if (m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_LOADING) || m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_LOGO) || m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_MAPEDIT))
		return;

	if (!m_bIsUpdate)
		return;

	if (!m_bIsOpen)
		return;

	__super::Late_Update(fTimeDelta);
}

HRESULT CStatus_Window::Render()
{
	return S_OK;
}

HRESULT CStatus_Window::Ready_Components()
{

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CStatus_Window::Ready_ChildPrototype(LEVEL eLevel)
{
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_Status_Frame_UP"),
		CStatus_Frame_UP::Create(m_pGraphic_Device, eLevel))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_Status_Frame_Mid"),
		CStatus_Frame_Mid::Create(m_pGraphic_Device, eLevel))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_Status_Frame_Down"),
		CStatus_Frame_Down::Create(m_pGraphic_Device, eLevel))))
		return E_FAIL;
	return S_OK;
}

HRESULT CStatus_Window::Ready_Children()
{
	CUIObject* pGameObject = nullptr;

	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Status_Frame_UP")));
	if (nullptr == pGameObject)
		return E_FAIL;
	Add_Child(pGameObject);

	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Status_Frame_Mid")));
	if (nullptr == pGameObject)
		return E_FAIL;
	Add_Child(pGameObject);

	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Status_Frame_Down")));
	if (nullptr == pGameObject)
		return E_FAIL;
	Add_Child(pGameObject);
	return S_OK;
}

CStatus_Window* CStatus_Window::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
	CStatus_Window* pInstance = new CStatus_Window(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eLevel)))
	{
		MSG_BOX(TEXT("Failed to Create : CStatus_Window"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CStatus_Window::Clone(void* pArg)
{
	CStatus_Window* pInstance = new CStatus_Window(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CStatus_Window"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CStatus_Window::Free()
{
	__super::Free();
}

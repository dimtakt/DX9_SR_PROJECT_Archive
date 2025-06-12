#include "Event_ClashPattern.h"
#include "GameInstance.h"
#include "Event_Rect.h"

CEvent_ClashPattern::CEvent_ClashPattern(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CEvent_ClashPattern::CEvent_ClashPattern(const CEvent_ClashPattern& Prototype) : CUIObject(Prototype), m_eLevel{ Prototype.m_eLevel }
{
}

HRESULT CEvent_ClashPattern::Initialize_Prototype(LEVEL eLevel)
{
	m_eLevel = eLevel;

	if (FAILED(Ready_ChildPrototype(eLevel)))
		return E_FAIL;

	return S_OK;
}

HRESULT CEvent_ClashPattern::Initialize(void* pArg)
{
	UIOBJECT_DESC* Desc = static_cast<UIOBJECT_DESC*>(pArg);

	m_fSizeX = 0;
	m_fSizeY = 0;
	m_fX = g_iWinSizeX * 0.5;
	m_fY = g_iWinSizeY * 0.5;
	m_fZ = UI_DEPTH::EVENT_AZ;
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

	//m_pGameInstance->Add_UIObject(Desc->m_iLevel, TEXT("EVENT_Clash"), this);

	return S_OK;
}

void CEvent_ClashPattern::Priority_Update(_float fTimeDelta)
{
	if (m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_LOADING) || m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_LOGO) || m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_MAPEDIT))
		return;

	if (m_pGameInstance->IsKeyDown(VK_DOWN))
		m_bIsOpen = true;

	if (!m_bIsUpdate)
		return;

	if (!m_bIsOpen)
		return;
	__super::Priority_Update(fTimeDelta);
}

void CEvent_ClashPattern::Update(_float fTimeDelta)
{
	if (m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_LOADING) || m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_LOGO) || m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_MAPEDIT))
		return;

	if (!m_bIsUpdate)
		return;

	if (!m_bIsOpen)
		return;

	__super::Update(fTimeDelta);
}

void CEvent_ClashPattern::Late_Update(_float fTimeDelta)
{
	if (m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_LOADING) || m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_LOGO) || m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_MAPEDIT))
		return;

	if (!m_bIsUpdate)
		return;

	if (!m_bIsOpen)
		return;

	__super::Late_Update(fTimeDelta);
}

HRESULT CEvent_ClashPattern::Render()
{
	return S_OK;
}

HRESULT CEvent_ClashPattern::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CEvent_ClashPattern::Ready_ChildPrototype(LEVEL eLevel)
{
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_Clash_Rect"),
		CEvent_Rect::Create(m_pGraphic_Device, m_eLevel))))
		return E_FAIL;

	return S_OK;
}

HRESULT CEvent_ClashPattern::Ready_Children()
{
	CUIObject* pGameObject = nullptr;

	UIOBJECT_DESC Desc{};

	for (_int i = 0; i < 2; ++i)
	{
		Desc.fZ = i;

		pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Clash_Rect"), &Desc));
		if (nullptr == pGameObject)
			return E_FAIL;
		Add_Child(pGameObject);
	}
	return S_OK;
}

CEvent_ClashPattern* CEvent_ClashPattern::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
	CEvent_ClashPattern* pInstance = new CEvent_ClashPattern(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eLevel)))
	{
		MSG_BOX(TEXT("Failed to Create : CEvent_ClashPattern"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CEvent_ClashPattern::Clone(void* pArg)
{
	CEvent_ClashPattern* pInstance = new CEvent_ClashPattern(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CEvent_ClashPattern"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEvent_ClashPattern::Free()
{
	__super::Free();
}

#include "Event_AZPattern.h"
#include "GameInstance.h"
#include "EventKey.h"
CEvent_AZPattern::CEvent_AZPattern(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CEvent_AZPattern::CEvent_AZPattern(const CEvent_AZPattern& Prototype) : CUIObject(Prototype), m_eLevel{ Prototype.m_eLevel }
{
}

HRESULT CEvent_AZPattern::Initialize_Prototype(LEVEL eLevel)
{
	m_eLevel = eLevel;

	if (FAILED(Ready_ChildPrototype(eLevel)))
		return E_FAIL;

	return S_OK;
}

HRESULT CEvent_AZPattern::Initialize(void* pArg)
{
	UIOBJECT_DESC* Desc = static_cast<UIOBJECT_DESC*>(pArg);

	m_fSizeX = 0;
	m_fSizeY = 0;
	m_fX = g_iWinSizeX * 0.5;
	m_fY = g_iWinSizeY * 0.5 + 200;
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

	//m_pGameInstance->Add_UIObject(Desc->m_iLevel, TEXT("EVENT_AZ"), this);

	return S_OK;
}

void CEvent_AZPattern::Priority_Update(_float fTimeDelta)
{
	if (m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_LOADING) || m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_LOGO) || m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_MAPEDIT))
		return;

	if (!m_bIsUpdate)
		return;

	__super::Priority_Update(fTimeDelta);
}

void CEvent_AZPattern::Update(_float fTimeDelta)
{
	if (m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_LOADING) || m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_LOGO) || m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_MAPEDIT))
		return;
	if (!m_bIsUpdate)
		return;

	__super::Update(fTimeDelta);
}

void CEvent_AZPattern::Late_Update(_float fTimeDelta)
{
	if (m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_LOADING) || m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_LOGO) || m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_MAPEDIT))
		return;

	if (!m_bIsUpdate)
		return;

	__super::Late_Update(fTimeDelta);
}

HRESULT CEvent_AZPattern::Render()
{
	return S_OK;
}

HRESULT CEvent_AZPattern::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CEvent_AZPattern::Ready_ChildPrototype(LEVEL eLevel)
{
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_Event_Key"),
		CEventKey::Create(m_pGraphic_Device))))
		return E_FAIL;

	return S_OK;
}

HRESULT CEvent_AZPattern::Ready_Children()
{
	CUIObject* pGameObject = nullptr;
	CEventKey::SLOT_KEYGUIDE_DESC Desc{};

	Desc.fZ = 8;

	for (_int i = 0; i < 8; ++i)
	{
		Desc.fX = i;
		pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_Event_Key"), &Desc));
		if (nullptr == pGameObject)
			return E_FAIL;
		Add_Child(pGameObject);
	}
	//pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Wallet_Dice")));
	//if (nullptr == pGameObject)
	//	return E_FAIL;
	//Add_Child(pGameObject);

	return S_OK;
}

CEvent_AZPattern* CEvent_AZPattern::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
	CEvent_AZPattern* pInstance = new CEvent_AZPattern(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eLevel)))
	{
		MSG_BOX(TEXT("Failed to Create : CEvent_AZPattern"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CEvent_AZPattern::Clone(void* pArg)
{
	CEvent_AZPattern* pInstance = new CEvent_AZPattern(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CEvent_AZPattern"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEvent_AZPattern::Free()
{
	__super::Free();
}

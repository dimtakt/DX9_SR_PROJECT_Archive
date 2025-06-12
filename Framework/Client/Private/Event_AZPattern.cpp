#include "Event_AZPattern.h"
#include "GameInstance.h"
#include "EventKey.h"
#include "Event_Rect.h"
#include "Event_Result.h"
#include "Event_Timer.h"
#include "Room_Manager.h"
CEvent_AZPattern::CEvent_AZPattern(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CEvent_AZPattern::CEvent_AZPattern(const CEvent_AZPattern& Prototype) : CUIObject(Prototype), m_eLevel{ Prototype.m_eLevel }
{
}

void CEvent_AZPattern::Start_Event()
{

	if (m_vecInputKey_Boss.size() == 0)
	{
		m_fEventTime = 7.f;
		m_fEventTimeOver = m_fEventTime;

		m_bIsEvent = true;
		m_pGameInstance->All_Update_Off();
		m_bIsUpdate = true;

		for (_int i = 0; i < m_pBoss->Get_AZPatternDesc().vecRequiredInputs.size(); ++i)
		{
			m_vecInputKey_Boss.push_back(m_pBoss->Get_AZPatternDesc().vecRequiredInputs[i]);
		}
		Setting_Event();
	}
}

void CEvent_AZPattern::End_Event()
{
	m_bIsEvent = false;
	m_pGameInstance->All_Update_On();
	Reset_Player();
	Reset_Boss();

	for (auto& pEventKey : m_vecEventKey)
		Safe_Release(pEventKey);
	m_vecEventKey.clear();
	
	for (_int i = 4; i < m_vecChildren.size(); ++i)
	{
		Safe_Release(m_vecChildren[i]);
	}

	m_vecChildren.erase(m_vecChildren.begin() + 4, m_vecChildren.end());
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
	m_fY = g_iWinSizeY * 0.5 + 150;
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

	m_pGameInstance->Add_UIObject(Desc->m_iLevel, TEXT("EVENT_AZ"), this);
	return S_OK;
}

void CEvent_AZPattern::Priority_Update(_float fTimeDelta)
{
	if (m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_LOADING) || m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_LOGO) || m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_MAPEDIT))
		return;
	if (!m_bIsUpdate)
		return;
	m_pBoss = static_cast<CErma*>(CRoom_Manager::GetInstance()->Find_CurrentRoom_Monster(MONSTER_TYPE::ERMA));
	
	if (m_pBoss == nullptr)
		return;


	if (m_pBoss->Get_AZPatternDesc().isPatternStart)
		Start_Event();

	if (!m_bIsEvent)
		return;
	__super::Priority_Update(fTimeDelta);
}

void CEvent_AZPattern::Update(_float fTimeDelta)
{
	if (m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_LOADING) || m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_LOGO) || m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_MAPEDIT))
		return;
	if (!m_bIsUpdate)
		return;
	if (!m_bIsEvent)
		return;

	
	if (m_fEventTimeOver <= 0)
	{
		m_bIsOver = true;
		m_bIsResult = true;
		static_cast<CEvent_Result*>(m_vecChildren[3])->Fail_Setting();
	}

	//if(!m_bIsResult)
	//	KeyDown_Player();

	for (int i = 0; i < m_pBoss->Get_AZPatternDesc().vecInputs.size(); ++i)
	{
		if (m_vecInputKey_Boss[m_vecInputKey_Boss.size() - 1] == m_pBoss->Get_AZPatternDesc().vecInputs[m_pBoss->Get_AZPatternDesc().vecInputs.size() - 1])
		{
			m_bIsResult = true;
			m_bIsClear = true;
			static_cast<CEvent_Result*>(m_vecChildren[3])->Succes_Setting();
			break;
		}
		if (m_vecInputKey_Boss[i] == m_pBoss->Get_AZPatternDesc().vecInputs[i])
		{
			m_vecEventKey[i]->Clear_KeySetting();
		}
		else
		{
			Reset_Player();
			m_bIsResult = true;
			static_cast<CEvent_Result*>(m_vecChildren[3])->Fail_Setting();
		}
	}

	if (m_vecInputKey_Boss.size() > m_pBoss->Get_AZPatternDesc().vecInputs.size())
	{
		m_vecEventKey[m_pBoss->Get_AZPatternDesc().vecInputs.size()]->Input_KeySetting();
	}

	//m_fEventTimeOver -= fTimeDelta;
	static_cast<CEvent_Timer*>(m_vecChildren[2])->Progerss_Set(m_pBoss->Get_AZPatternDesc().fLeftTime, m_fEventTime);
	
	__super::Update(fTimeDelta);
}

void CEvent_AZPattern::Late_Update(_float fTimeDelta)
{
	if (m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_LOADING) || m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_LOGO) || m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_MAPEDIT))
		return;

	if (!m_bIsUpdate)
		return;

	if (!m_bIsEvent)
		return;

	for (_int i = 0; i < 3; ++i)
	{
		m_vecChildren[i]->Late_Update(fTimeDelta);
	}

	if (m_bIsResult)
	{
		m_fAcctime += fTimeDelta;

		if (m_fAcctime > 0.5)
		{
			m_bIsResult = false;
			m_fAcctime = 0.f;

			if (m_pBoss->Get_AZPatternDesc().isSuccess)
				End_Event();
			else if (m_bIsOver)
				End_Event();
		}
		m_vecChildren[3]->Late_Update(fTimeDelta);
	}
	else
	{
		for (_int i = 0; i < m_vecEventKey.size(); ++i)
		{
			m_vecEventKey[i]->Late_Update(fTimeDelta);
		}
	}

}

HRESULT CEvent_AZPattern::Render()
{
	return S_OK;
}

HRESULT CEvent_AZPattern::Setting_Event()
{
	CUIObject* pGameObject = nullptr;
	CEventKey::SLOT_KEYGUIDE_DESC Desc{};

	Desc.fZ = m_vecInputKey_Boss.size();
	for (_int i = 0; i < m_vecInputKey_Boss.size(); ++i)
	{
		Desc.fX = i;
		Desc.strKey = m_vecInputKey_Boss[i];
		pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_Event_Key"), &Desc));
		if (nullptr == pGameObject)
		{
			MSG_BOX(TEXT("Failed to Event : AZPattern"));
			return E_FAIL;
		}
		Add_Child(pGameObject);
		m_vecEventKey.push_back(static_cast<CEventKey*>(pGameObject));
		Safe_AddRef(pGameObject);
	}
}

void CEvent_AZPattern::KeyDown_Player()
{

	//if (m_pGameInstance->IsKeyDown('Q'))
	//	(*m_vecInputKey_Player).push_back('Q');
	//else if (m_pGameInstance->IsKeyDown('W'))
	//	(*m_vecInputKey_Player).push_back('W');
	//else if (m_pGameInstance->IsKeyDown('E'))
	//	(*m_vecInputKey_Player).push_back('E');
	//else if (m_pGameInstance->IsKeyDown('R'))
	//	(*m_vecInputKey_Player).push_back('R');
	//else if (m_pGameInstance->IsKeyDown('A'))
	//	(*m_vecInputKey_Player).push_back('A');
	//else if (m_pGameInstance->IsKeyDown('S'))
	//	(*m_vecInputKey_Player).push_back('S');
	//else if (m_pGameInstance->IsKeyDown('D'))
	//	(*m_vecInputKey_Player).push_back('D');
	//else if (m_pGameInstance->IsKeyDown('E'))
	//	(*m_vecInputKey_Player).push_back('E');
	//else if (m_pGameInstance->IsKeyDown('F'))
	//	(*m_vecInputKey_Player).push_back('F');
}

void CEvent_AZPattern::Reset_Player()
{
	for (_int i = 0; i < m_vecEventKey.size(); ++i)
	{
		m_vecEventKey[i]->Reset_KeySetting();
	}
}

void CEvent_AZPattern::Reset_Boss()
{
	m_vecInputKey_Boss.clear();
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

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_Event_Rect"),
		CEvent_Rect::Create(m_pGraphic_Device, m_eLevel))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_Event_Result"),
		CEvent_Result::Create(m_pGraphic_Device))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_Event_Timer"),
		CEvent_Timer::Create(m_pGraphic_Device))))
		return E_FAIL;
	return S_OK;
}

HRESULT CEvent_AZPattern::Ready_Children()
{
	CUIObject* pGameObject = nullptr;
	CEventKey::SLOT_KEYGUIDE_DESC Desc{};
	
	for (_int i = 0; i < 2; ++i)
	{
		Desc.fZ = i;
		pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_Event_Rect"), &Desc));
		if (nullptr == pGameObject)
			return E_FAIL;
		Add_Child(pGameObject);
	}

	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_Event_Timer")));
	if (nullptr == pGameObject)
		return E_FAIL;
	Add_Child(pGameObject);

	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_Event_Result")));
	if (nullptr == pGameObject)
		return E_FAIL;
	Add_Child(pGameObject);

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
	for (auto& pEventKey : m_vecEventKey)
		Safe_Release(pEventKey);
	m_vecEventKey.clear();

	__super::Free();
}

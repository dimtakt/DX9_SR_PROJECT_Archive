#include "Event_ClashPattern.h"
#include "GameInstance.h"
#include "Event_Rect.h"
#include "Stat_Manager.h"
#include "Event_Circle.h"
CEvent_ClashPattern::CEvent_ClashPattern(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CEvent_ClashPattern::CEvent_ClashPattern(const CEvent_ClashPattern& Prototype) : CUIObject(Prototype), m_eLevel{ Prototype.m_eLevel }
{
}

void CEvent_ClashPattern::Start_Event()
{
	m_pGameInstance->All_Update_Off();
	m_bIsUpdate = true;
	m_bIsPlaying = true;
	CStat_Manager::GetInstance()->Set_UIOpen(true);
	m_eCLASH_RESULT = CLASH_RESULT::CLASH_PLAYING;
	m_iSpawnerIndex = m_pGameInstance->Rand(0, m_vecSpawner.size());

}

void CEvent_ClashPattern::End_Event()
{
	Result_Check();
	Reset_Event();
	m_pGameInstance->All_Update_On();
	CStat_Manager::GetInstance()->Set_UIOpen(false);
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

	Patten_Set();
	m_iInputKey_Index = 0;
	m_iSpawnerIndex = m_pGameInstance->Rand(0, m_vecSpawner.size());
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

	m_pGameInstance->Add_UIObject(Desc->m_iLevel, TEXT("EVENT_Clash"), this);

	return S_OK;
}

void CEvent_ClashPattern::Priority_Update(_float fTimeDelta)
{
	if (m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_LOADING) || m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_LOGO) || m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_MAPEDIT))
		return;

	if (!m_bIsUpdate)
		return;

	if (!m_bIsPlaying)
		return;
	__super::Priority_Update(fTimeDelta);
}

void CEvent_ClashPattern::Update(_float fTimeDelta)
{
	if (m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_LOADING) || m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_LOGO) || m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_MAPEDIT))
		return;

	if (!m_bIsUpdate)
		return;

	if (!m_bIsPlaying)
		return;

	Circle_Spawn(fTimeDelta);
	if(m_bIsSpawn)
		Next_KeyInput();

	__super::Update(fTimeDelta);
}

void CEvent_ClashPattern::Late_Update(_float fTimeDelta)
{
	if (m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_LOADING) || m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_LOGO) || m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_MAPEDIT))
		return;

	if (!m_bIsUpdate)
		return;

	if (!m_bIsPlaying)
		return;

	if (m_bIsGameEnd)
	{
		m_fEndAccTime += fTimeDelta;
		if(m_fEndAccTime > 0.5)
			End_Event();
	}
		
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

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_Clash_Circle"),
		CEvent_Circle::Create(m_pGraphic_Device, eLevel))))
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
	m_iInputKey_Index = 2;
	return S_OK;
}

void CEvent_ClashPattern::Circle_Spawn(_float fTimeDelta)
{
	if (m_vecSpawner[m_iSpawnerIndex].size() <= m_iSpawnCount)
		return;

	CUIObject* pGameObject = nullptr;
	UIOBJECT_DESC Desc{};

	m_fAccTime += fTimeDelta;

	if (m_fAccTime > m_vecSpawner[m_iSpawnerIndex][m_iSpawnCount].m_fDelayTime)
	{
		Desc.fX = m_vecSpawner[m_iSpawnerIndex][m_iSpawnCount].m_iSpawnX;
		Desc.fY = m_vecSpawner[m_iSpawnerIndex][m_iSpawnCount].m_iSpawnY;

		pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Clash_Circle"), &Desc));
		if (nullptr == pGameObject)
			return;
		Add_Child(pGameObject);
		if (m_iSpawnCount == 0)
		{
			static_cast<CEvent_Circle*>(pGameObject)->KeyInput_On();
		}
			
		++m_iSpawnCount;
		m_fAccTime = 0;
		m_bIsSpawn = true;
		m_bIsKeyInput = true;
	}
}

void CEvent_ClashPattern::Patten_Set()
{
	m_vecPattern_1.push_back({ 0.7f, -150, 0 });
	m_vecPattern_1.push_back({ 0.4f, 150, 0 });

	m_vecPattern_2.push_back({ 0.5f, -200, -10 });
	m_vecPattern_2.push_back({ 0.5f, -50, -30 });
	m_vecPattern_2.push_back({ 0.5f, 150, -50 });

	m_vecPattern_3.push_back({ 0.5f, -250, 0 });
	m_vecPattern_3.push_back({ 0.5f, 0, 0 });
	m_vecPattern_3.push_back({ 0.5f, 250, 0 });

	m_vecSpawner.push_back(m_vecPattern_1);
	m_vecSpawner.push_back(m_vecPattern_2);
	m_vecSpawner.push_back(m_vecPattern_3);

}

void CEvent_ClashPattern::Next_KeyInput()
{
	if (m_vecSpawner[m_iSpawnerIndex].size() + 2 == m_iInputKey_Index)
	{
		m_bIsKeyInput = false;
		m_bIsGameEnd = true;
		return;
	}
	if (m_bIsKeyInput && static_cast<CEvent_Circle*>(m_vecChildren[m_iInputKey_Index])->Get_CircleState() != CEvent_Circle::CIRCLE_STATE::CIRCLE_PLAYING)
	{
		if (m_vecSpawner[m_iSpawnerIndex].size() + 2 >= m_iInputKey_Index)
		{
			++m_iInputKey_Index;
			if (m_vecSpawner[m_iSpawnerIndex].size() + 2 == m_iInputKey_Index)
				return;
			static_cast<CEvent_Circle*>(m_vecChildren[m_iInputKey_Index])->KeyInput_On();
			//if(m_vecSpawner[m_iSpawnerIndex].size() + 2 == m_iInputKey_Index)
		
		}
	}
}

void CEvent_ClashPattern::Result_Check()
{
	_int iCount = 0;

	for (_int i = 2; i < m_vecChildren.size(); ++i)
	{
		if (static_cast<CEvent_Circle*>(m_vecChildren[i])->Get_CircleState() == CEvent_Circle::CIRCLE_STATE::CIRCLE_PERFECT)
			++iCount;
	}

	_float fCircleWinPoint = m_vecSpawner[m_iSpawnerIndex].size() / 2;
	
	if (fCircleWinPoint < iCount)
		m_eCLASH_RESULT = CLASH_RESULT::CLASH_CLEAR;
	else
		m_eCLASH_RESULT = CLASH_RESULT::CLASH_FAIL;

}

void CEvent_ClashPattern::Reset_Event()
{
	m_bIsPlaying = false;
	m_bIsSpawn = false;
	m_bIsKeyInput = false;
	m_bIsGameEnd = false;

	m_iSpawnCount = 0;
	m_fEndAccTime = 0;
	m_fAccTime = 0;
	m_iInputKey_Index = 2;

	for (_int i = 2; i < m_vecChildren.size(); ++i)
	{
		Safe_Release(m_vecChildren[i]);
	}

	m_vecChildren.erase(m_vecChildren.begin() + 2, m_vecChildren.end());

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

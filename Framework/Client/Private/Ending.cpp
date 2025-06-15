#include "Ending.h"
#include "GameInstance.h"
#include "Ending_Mole.h"
#include "Stat_Manager.h"
#include "Ending_List.h"
#include "Ending_Light.h"
CEnding::CEnding(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CEnding::CEnding(const CEnding& Prototype) : CUIObject(Prototype), m_eLevel(Prototype.m_eLevel)
{
}

HRESULT CEnding::Initialize_Prototype(LEVEL eLevel)
{
	m_eLevel = eLevel;

	if (FAILED(Ready_ChildPrototype(eLevel)))
		return E_FAIL;

	return S_OK;
}

HRESULT CEnding::Initialize(void* pArg)
{
	m_bisOpen = false;
	m_fSizeX = g_iWinSizeX;
	m_fSizeY = g_iWinSizeY;
	m_fX = m_fSizeX * 0.5f;
	m_fY = m_fSizeY * 0.5f;
	m_fZ = 0.33f;
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

	m_pGameInstance->Add_UIObject(ENUM_CLASS(LEVEL::LEVEL_BOSS2), TEXT("UI_Ending"), this);

	return S_OK;
}

void CEnding::Priority_Update(_float fTimeDelta)
{
	if (m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_LOADING) || m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_LOGO) || m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_MAPEDIT))
		return;
	if (!m_bIsUpdate)
		return;
	if (!m_bisOpen)
		return;

	__super::Priority_Update(fTimeDelta);
}

void CEnding::Update(_float fTimeDelta)
{
	if (m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_LOADING) || m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_LOGO) || m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_MAPEDIT))
		return;

	if (!m_bIsUpdate)
		return;

	if (!m_bisOpen)
		return;

	__super::Update(fTimeDelta);

}

void CEnding::Late_Update(_float fTimeDelta)
{
	if (m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_LOADING) || m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_LOGO) || m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_MAPEDIT))
		return;

	if (!m_bIsUpdate)
		return;

	if (!m_bisOpen)
		return;

	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
	__super::Late_Update(fTimeDelta);

}

HRESULT CEnding::Render()
{
	m_pGraphic_Device->SetTexture(0, NULL);
	m_pVIBufferCom->Bind_Buffers();
	__super::Begin();
	m_pVIBufferCom->Render();
	Font_Render();
	__super::End();
	return S_OK;
}

void CEnding::UI_Switch()
{
	if (m_bisOpen)
	{
		m_pGameInstance->All_Update_On();
		m_bisOpen = false;
		CStat_Manager::GetInstance()->Set_UIOpen(false);
	}
	else
	{
		m_pGameInstance->All_Update_On();
		m_pGameInstance->All_Update_Off();
		m_pGameInstance->Update_On(ENUM_CLASS(LEVEL::LEVEL_BOSS2), TEXT("Ending_Chat"));
		m_bIsUpdate = true;
		m_bisOpen = true;
		CStat_Manager::GetInstance()->Set_UIOpen(true);
	}
}

void CEnding::Start_Ending()
{
	if(!m_bisOpen)
		UI_Switch();
}

HRESULT CEnding::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Rect_UI_BalckRect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CEnding::Ready_ChildPrototype(LEVEL eLevel)
{
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_Ending_Light"),
		CEnding_Light::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_Ending_Mole"),
		CEnding_Mole::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_Ending_List"),
		CEnding_List::Create(m_pGraphic_Device))))
		return E_FAIL;

	return S_OK;
}

HRESULT CEnding::Ready_Children()
{
	CUIObject* pGameObject = nullptr;

	UIOBJECT_DESC Desc{};

	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Ending_Light")));
	if (nullptr == pGameObject)
		return E_FAIL;
	Add_Child(pGameObject);

	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Ending_List")));
	if (nullptr == pGameObject)
		return E_FAIL;
	Add_Child(pGameObject);

	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Ending_Mole")));
	if (nullptr == pGameObject)
		return E_FAIL;
	Add_Child(pGameObject);
	return S_OK;
}

void CEnding::Font_Render()
{
	TCHAR szText[MAX_PATH];

	//_stprintf_s(szText, TEXT("Àç´É"));
	//m_pGameInstance->Render_Font(TEXT("UI_Font_40"), szText, m_vTexRect, D3DXCOLOR(1.000f, 0.886f, 0.608f, 1.0f), DT_CENTER | DT_TOP);


}

CEnding* CEnding::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
	CEnding* pInstance = new CEnding(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eLevel)))
	{
		MSG_BOX(TEXT("Failed to Created : CEnding"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CEnding::Clone(void* pArg)
{
	CEnding* pInstance = new CEnding(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CEnding"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEnding::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
}

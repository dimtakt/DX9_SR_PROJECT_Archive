#include "Talent.h"
#include "GameInstance.h"
#include "Talent_Reset.h"
#include "Talent_Point.h"
#include "Talent_Slot.h"
CTalent::CTalent(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CTalent::CTalent(const CTalent& Prototype) : CUIObject(Prototype), m_eLevel(Prototype.m_eLevel)
{
}

HRESULT CTalent::Initialize_Prototype(LEVEL eLevel)
{
	m_eLevel = eLevel;

	if (FAILED(Ready_ChildPrototype(eLevel)))
		return E_FAIL;

	return S_OK;
}

HRESULT CTalent::Initialize(void* pArg)
{
	m_bisOpen = false;
	m_fSizeX = g_iWinSizeX;
	m_fSizeY = g_iWinSizeY;
	m_fX = m_fSizeX * 0.5f;
	m_fY = m_fSizeY * 0.5f;
	m_fZ = UI_DEPTH::TALENT;
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

	m_pGameInstance->Add_UIObject(ENUM_CLASS(m_eLevel), TEXT("UI_Talent"), this);

	return S_OK;
}

void CTalent::Priority_Update(_float fTimeDelta)
{
	if (m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_LOADING) || m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_LOGO) || m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_MAPEDIT))
		return;
	if (!m_bIsUpdate)
		return;
	if (!m_bisOpen)
		return;
	
	__super::Priority_Update(fTimeDelta);
}

void CTalent::Update(_float fTimeDelta)
{
	if (m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_LOADING) || m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_LOGO) || m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_MAPEDIT))
		return;

	if (!m_bIsUpdate)
		return;

	if (m_pGameInstance->IsKeyDown('P'))
		UI_Switch();

	if (!m_bisOpen)
		return;
	if (m_pGameInstance->IsKeyDown(VK_ESCAPE))
	{
		UI_Switch();
		return;
	}
	__super::Update(fTimeDelta);
	
}

void CTalent::Late_Update(_float fTimeDelta)
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

HRESULT CTalent::Render()
{
	m_pGraphic_Device->SetTexture(0, NULL);
	m_pVIBufferCom->Bind_Buffers();
	__super::Begin();
	m_pVIBufferCom->Render();
	Font_Render();
	__super::End();
	return S_OK;
}

void CTalent::UI_Switch()
{
	if (m_bisOpen)
	{
		m_pGameInstance->All_Update_On();
		m_bisOpen = false;
	}
	else
	{
		m_pGameInstance->All_Update_Off();
		m_bIsUpdate = true;
		m_bisOpen = true;
	}
}

void CTalent::Slot_Reset()
{
	for (_int i = 0; i < 4; ++i)
		static_cast<CTalent_Slot*>(m_vecChildren[i])->Reset_Value();
}

HRESULT CTalent::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Rect_UI_BalckRect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CTalent::Ready_ChildPrototype(LEVEL eLevel)
{
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_Talent_Reset"),
		CTalent_Reset::Create(m_pGraphic_Device, eLevel))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_Talent_Point"),
		CTalent_Point::Create(m_pGraphic_Device))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_Talent_Slot"),
		CTalent_Slot::Create(m_pGraphic_Device, m_eLevel))))
		return E_FAIL;

	return S_OK;
}

HRESULT CTalent::Ready_Children()
{
	CUIObject* pGameObject = nullptr;

	UIOBJECT_DESC Desc{};

	for (_int i = 0; i < 4; ++i)
	{
		Desc.fX = -460 + i * 310;
		Desc.fZ = i;
		pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Talent_Slot"), &Desc));
		if (nullptr == pGameObject)
			return E_FAIL;
		Add_Child(pGameObject);
	}

	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Talent_Reset")));
	if (nullptr == pGameObject)
		return E_FAIL;
	Add_Child(pGameObject);

	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Talent_Point")));
	if (nullptr == pGameObject)
		return E_FAIL;
	Add_Child(pGameObject);


	return S_OK;
}

void CTalent::Font_Render()
{
	TCHAR szText[64];
	m_vTexRect.left = 0;
	m_vTexRect.top =  50;
	m_vTexRect.right = m_fSizeX;
	m_vTexRect.bottom = m_fSizeY;

	_stprintf_s(szText, TEXT("재능"));
	m_pGameInstance->Render_Font(TEXT("UI_Font_40"), szText, m_vTexRect, D3DXCOLOR(1.000f, 0.886f, 0.608f, 1.0f), DT_CENTER | DT_TOP);
	
	m_vTexRect.top = 100;
	_stprintf_s(szText, TEXT("어릴 적부터 타고났던 특별한 능력입니다."));
	m_pGameInstance->Render_Font(TEXT("UI_Font_22_Normal"), szText, m_vTexRect, D3DXCOLOR(0.8, 0.8, 0.8, 1.0f), DT_CENTER | DT_TOP);

	//m_pGameInstance->Render_Font(TEXT("UI_Font_22"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), DT_CENTER | DT_VCENTER | DT_SINGLELINE);

}

CTalent* CTalent::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
	CTalent* pInstance = new CTalent(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eLevel)))
	{
		MSG_BOX(TEXT("Failed to Created : CTalent"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CTalent::Clone(void* pArg)
{
	CTalent* pInstance = new CTalent(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CTalent"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTalent::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
}

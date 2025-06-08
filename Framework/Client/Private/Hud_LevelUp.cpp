#include "Hud_LevelUp.h"
#include "GameInstance.h"
#include "Stat_Manager.h"
#include "UI_KeyGuide.h"
#include "Gacha.h"
CHud_LevelUp::CHud_LevelUp(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CHud_LevelUp::CHud_LevelUp(const CHud_LevelUp& Prototype) : CUIObject(Prototype), m_eLevel(Prototype.m_eLevel)
{
}

HRESULT CHud_LevelUp::Initialize_Prototype(LEVEL eLevel)
{
	m_eLevel = eLevel;

	if (FAILED(Ready_ChildPrototype(eLevel)))
		return E_FAIL;

	return S_OK;
}

HRESULT CHud_LevelUp::Initialize(void* pArg)
{
	m_fSpeed = 17;
	m_iTexIndex = 0;
	m_fSizeX = 128;
	m_fSizeY = 128;
	m_fX = g_iWinSizeX * 0.5;
	m_fY = 200 + g_iWinSizeY * 0.5;
	m_fZ = UI_DEPTH::HUD_EXP;
	m_iWinSizeX = g_iWinSizeX;
	m_iWinSizeY = g_iWinSizeY;

	if (FAILED(CUIObject::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
	__super::Update_Position();

	if (FAILED(Ready_Children()))
		return E_FAIL;

	m_pGameInstance->Add_UIObject(ENUM_CLASS(m_eLevel), TEXT("Hud_LevelUp"), this);
	return S_OK;
}

void CHud_LevelUp::Priority_Update(_float fTimeDelta)
{
	if (m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_LOADING) || m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_LOGO) || m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_MAPEDIT))
		return;
	if (!m_bIsUpdate)
		return;

	CUIObject::Priority_Update(fTimeDelta);
}

void CHud_LevelUp::Update(_float fTimeDelta)
{
	if (m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_LOADING) || m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_LOGO) || m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_MAPEDIT))
		return;
	if (!m_bIsUpdate)
		return;
	if (CStat_Manager::GetInstance()->Get_CurStats()[ENUM_CLASS(STAT_INFO::LEVELUPPOINT)] <= 0)
		return;
	
	if(m_pGameInstance->IsKeyDown('R'))
		static_cast<CGacha*>(m_pGameInstance->Find_UIObj(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("UI_Gacha")))->UI_Open(CGacha::GACHA_TYPE::ALL);


	m_fFrame += fTimeDelta * m_fSpeed;

	if (m_fFrame > 1)
	{
		m_fFrame = 0;
		++m_iTexIndex;

		if (m_iTexIndex > 12)
			m_iTexIndex = 0;
	}
	CUIObject::Update(fTimeDelta);
}

void CHud_LevelUp::Late_Update(_float fTimeDelta)
{
	if (m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_LOADING) || m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_LOGO) || m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_MAPEDIT))
		return;
	if (!m_bIsUpdate)
		return;
	if (CStat_Manager::GetInstance()->Get_CurStats()[ENUM_CLASS(STAT_INFO::LEVELUPPOINT)] <= 0)
		return;

	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
	CUIObject::Late_Update(fTimeDelta);
}

HRESULT CHud_LevelUp::Render()
{
	if (FAILED(m_pTextureCom->Bind_Texture(m_iTexIndex)))
		return E_FAIL;
	m_pVIBufferCom->Bind_Buffers();

	__super::Begin();
	m_pVIBufferCom->Render();

	Render_Font();
	__super::End();

}

HRESULT CHud_LevelUp::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Rect_LevelUp"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CHud_LevelUp::Ready_ChildPrototype(LEVEL eLevel)
{
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_LevelUp_Guide"),
		CUI_KeyGuide::Create(m_pGraphic_Device, TEXT("R")))))
		return E_FAIL;

	return S_OK;
}

HRESULT CHud_LevelUp::Ready_Children()
{
	CUIObject* pGameObject = nullptr;
	CUIObject::UIOBJECT_DESC Desc = {};

	Desc.fX = -35;
	Desc.fY = 60;

	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_LevelUp_Guide"), &Desc));
	if (nullptr == pGameObject)
		return E_FAIL;
	Add_Child(pGameObject);

	return S_OK;
}

void CHud_LevelUp::Render_Font()
{
	Font_Rect_Update();
	TCHAR szText[64];

	m_vTexRect.top += 60;
	m_vTexRect.bottom += 60;
	m_vTexRect.right += 15;
	m_vTexRect.left += 15;
	_stprintf_s(szText, TEXT("·¹º§¾÷"));
	m_pGameInstance->Render_Font(TEXT("UI_Font_22"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

CHud_LevelUp* CHud_LevelUp::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
	CHud_LevelUp* pInstance = new CHud_LevelUp(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eLevel)))
	{
		MSG_BOX(TEXT("Failed to Created : CHud_LevelUp"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CHud_LevelUp::Clone(void* pArg)
{
	CHud_LevelUp* pInstance = new CHud_LevelUp(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CHud_LevelUp"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CHud_LevelUp::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}

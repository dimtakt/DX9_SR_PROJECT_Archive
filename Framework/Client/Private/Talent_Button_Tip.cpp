#include "Talent_Button_Tip.h"
#include "GameInstance.h"

CTalent_Button_Tip::CTalent_Button_Tip(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CTalent_Button_Tip::CTalent_Button_Tip(const CTalent_Button_Tip& Prototype) : CUIObject(Prototype)
{
}

HRESULT CTalent_Button_Tip::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTalent_Button_Tip::Initialize(void* pArg)
{
	UIOBJECT_DESC* Desc = static_cast<UIOBJECT_DESC*>(pArg);

	m_iIndex = Desc->fZ * 2 + Desc->fX;
	Tooltip_SizePosSet();
		
	m_fZ = UI_DEPTH::TOOLTIP;
	m_iWinSizeX = g_iWinSizeX;
	m_iWinSizeY = g_iWinSizeY;

	if (FAILED(CUIObject::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
	__super::Update_Position();

	return S_OK;
}

void CTalent_Button_Tip::Priority_Update(_float fTimeDelta)
{
}

void CTalent_Button_Tip::Update(_float fTimeDelta)
{
}

void CTalent_Button_Tip::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI_BLEND, this);
}

HRESULT CTalent_Button_Tip::Render()
{
	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;
	m_pVIBufferCom->Bind_Buffers();
	__super::Begin();
	m_pVIBufferCom->Render();
	__super::End();
	Font_Render();
	return S_OK;
}

HRESULT CTalent_Button_Tip::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Rect_Tooltip"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

void CTalent_Button_Tip::Font_Render()
{
	TCHAR szText[MAX_PATH];
	CUIObject::Font_Rect_Update();
	m_vTexRect.left += 20;
	m_vTexRect.right -= 20;

	switch (m_iIndex)
	{
	case 0:
		_stprintf_s(szText, TEXT("치명타 피해가 20%% 증가합니다."));
		m_pGameInstance->Render_Font(TEXT("UI_Font_22"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.0f), DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		break;
	case 1:
		_stprintf_s(szText, TEXT("치명타 확률이 20%% 증가합니다."));
		m_pGameInstance->Render_Font(TEXT("UI_Font_22"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.0f), DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		break;
	case 2:
		_stprintf_s(szText, TEXT("대시 회복 속도가 10%% 증가합니다."));
		m_pGameInstance->Render_Font(TEXT("UI_Font_22"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.0f), DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		break;
	case 3:
		_stprintf_s(szText, TEXT("대시 횟수가 1 증가합니다."));
		m_pGameInstance->Render_Font(TEXT("UI_Font_22"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.0f), DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		break;
	case 4:
		_stprintf_s(szText, TEXT("최대 HP +10 증갑합니다."));
		m_pGameInstance->Render_Font(TEXT("UI_Font_22"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.0f), DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		break;
	case 5:
		_stprintf_s(szText, TEXT("방어력 +10 증가합니다."));
		m_pGameInstance->Render_Font(TEXT("UI_Font_22"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.0f), DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		break;
	case 6:
		_stprintf_s(szText, TEXT("공격력 +10 증갑합니다."));
		m_pGameInstance->Render_Font(TEXT("UI_Font_22"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.0f), DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
		break;
	case 7:
		_stprintf_s(szText, TEXT("최대 HP +10, 최대 MP +20 증가합니다."));
		m_pGameInstance->Render_Font(TEXT("UI_Font_22"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.0f), DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
		break;
	}

}
void CTalent_Button_Tip::Tooltip_SizePosSet()
{
	switch (m_iIndex)
	{
	case 0:
		m_fX = 210;
		m_fY = 11;
		m_fSizeX = 350;
		m_fSizeY = 50;
		break;
	case 1:
		m_fX = 210;
		m_fY = 11;
		m_fSizeX = 350;
		m_fSizeY = 50;
		break;
	case 2:
		m_fX = 210;
		m_fY = 11;
		m_fSizeX = 350;
		m_fSizeY = 50;
		break;
	case 3:
		m_fX = 210;
		m_fY = 11;
		m_fSizeX = 260;
		m_fSizeY = 50;
		break;
	case 4:
		m_fX = 210;
		m_fY = 11;
		m_fSizeX = 260;
		m_fSizeY = 50;
		break;
	case 5:
		m_fX = 210;
		m_fY = 11;
		m_fSizeX = 260;
		m_fSizeY = 50;
		break;
	case 6:
		m_fX = -210;
		m_fY = 11;
		m_fSizeX = 260;
		m_fSizeY = 50;
		break;
	case 7:
		m_fX = -210;
		m_fY = 11;
		m_fSizeX = 380;
		m_fSizeY = 50;
		break;
	}
}
CTalent_Button_Tip* CTalent_Button_Tip::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTalent_Button_Tip* pInstance = new CTalent_Button_Tip(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CTalent_Button_Tip"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CTalent_Button_Tip::Clone(void* pArg)
{
	CTalent_Button_Tip* pInstance = new CTalent_Button_Tip(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CTalent_Button_Tip"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTalent_Button_Tip::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}

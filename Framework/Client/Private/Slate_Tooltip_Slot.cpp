#include "Slate_Tooltip_Slot.h"
#include "GameInstance.h"
CSlate_Tooltip_Slot::CSlate_Tooltip_Slot(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CSlate_Tooltip_Slot::CSlate_Tooltip_Slot(const CSlate_Tooltip_Slot& Prototype) : CUIObject(Prototype), m_eLevel(Prototype.m_eLevel)
{
}

HRESULT CSlate_Tooltip_Slot::Initialize_Prototype(LEVEL eLevel)
{
	m_eLevel = eLevel;

	return S_OK;
}

HRESULT CSlate_Tooltip_Slot::Initialize(void* pArg)
{
	UIOBJECT_DESC* Desc = static_cast<UIOBJECT_DESC*>(pArg);

	
	m_fSizeX = 20;
	m_fSizeY = 20;
	m_fX = -13 + 25 * Desc->fX;
	m_fY = 135 + 25 * Desc->fY;


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

void CSlate_Tooltip_Slot::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CSlate_Tooltip_Slot::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CSlate_Tooltip_Slot::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI_BLEND, this);
	__super::Late_Update(fTimeDelta);

	if (m_pItemObject != nullptr)
		static_cast<CItem_Base*>(m_pItemObject)->IsTooltip_Slate(m_pTransformCom->Get_State(STATE::POSITION));
}

HRESULT CSlate_Tooltip_Slot::Render()
{
	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;
	m_pVIBufferCom->Bind_Buffers();

	__super::Begin();
	m_pVIBufferCom->Render();

	if (m_iValue != 0)
		Render_Font();
	return S_OK;
}

HRESULT CSlate_Tooltip_Slot::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Rect_Item_Tooltip_Stone"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

void CSlate_Tooltip_Slot::Render_Font()
{
	TCHAR szText[MAX_PATH];
	D3DXCOLOR TexColor{};

	Font_Rect_Update();
	if (m_iValue > 0)
	{
		_stprintf_s(szText, TEXT("+%d"), m_iValue);
		TexColor = D3DXCOLOR(0.2f, 1.f, 0.f, 1.f);
		m_pGameInstance->Render_Font(TEXT("UI_Font_14"), szText, m_vTexRect, TexColor, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
	else
	{
		_stprintf_s(szText, TEXT("%d"), m_iValue);
		TexColor = D3DXCOLOR(1.f, 0.f, 0.5f, 1.f);
		m_pGameInstance->Render_Font(TEXT("UI_Font_14"), szText, m_vTexRect, TexColor, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	}

}

CSlate_Tooltip_Slot* CSlate_Tooltip_Slot::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
	CSlate_Tooltip_Slot* pInstance = new CSlate_Tooltip_Slot(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eLevel)))
	{
		MSG_BOX(TEXT("Failed to Created : CSlate_Tooltip_Slot"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CSlate_Tooltip_Slot::Clone(void* pArg)
{
	CSlate_Tooltip_Slot* pInstance = new CSlate_Tooltip_Slot(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CSlate_Tooltip_Slot"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSlate_Tooltip_Slot::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}

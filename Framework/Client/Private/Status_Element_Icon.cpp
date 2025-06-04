#include "Status_Element_Icon.h"
#include "GameInstance.h"
#include "Stat_Manager.h"
CStatus_Element_Icon::CStatus_Element_Icon(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CStatus_Element_Icon::CStatus_Element_Icon(const CStatus_Element_Icon& Prototype) : CUIObject(Prototype), m_eLevel{ Prototype.m_eLevel }
{
}

HRESULT CStatus_Element_Icon::Initialize_Prototype(LEVEL eLevel)
{
	m_eLevel = eLevel;
	return S_OK;
}

HRESULT CStatus_Element_Icon::Initialize(void* pArg)
{
	UIOBJECT_DESC* Desc = static_cast<UIOBJECT_DESC*>(pArg);

	m_iIndex = Desc->fZ;
	m_fSizeX = 50;
	m_fSizeY = 50;
	m_fX = Desc->fX;
	m_fY = 50;
	m_fZ = UI_DEPTH::PLAYER_STAUTS;
	m_iWinSizeX = g_iWinSizeX;
	m_iWinSizeY = g_iWinSizeY;

	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
	__super::Update_Position();

	return S_OK;
}

void CStatus_Element_Icon::Priority_Update(_float fTimeDelta)
{

}

void CStatus_Element_Icon::Update(_float fTimeDelta)
{
}

void CStatus_Element_Icon::Late_Update(_float fTimeDelta)
{
	Update_Value();
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CStatus_Element_Icon::Render()
{
	if (FAILED(m_pTextureCom->Bind_Texture(m_iIndex)))
		return E_FAIL;
	m_pVIBufferCom->Bind_Buffers();

	__super::Begin();
	m_pVIBufferCom->Render();
	__super::End();
	Font_Render();
	return S_OK;
}

HRESULT CStatus_Element_Icon::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Rect_Status_Icon"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

void CStatus_Element_Icon::Update_Value()
{
	switch (m_iIndex)
	{
	case 8:
		m_iCulValue = (_int)CStat_Manager::GetInstance()->Get_CurStats()[ENUM_CLASS(STAT_INFO::CULDAMAGE)];
		break;
	case 9:
		m_iCulValue = 20;
		break;
	case 10:
		m_iCulValue = 20;
		break;
	case 11:
		m_iCulValue = 20;
		break;
	}
}

void CStatus_Element_Icon::Font_Render()
{
	TCHAR szText[64];

	m_vTexRect.left = 2 + (m_pParent->Get_WorldPos().x + m_fX) - m_fSizeX * 0.5;
	m_vTexRect.top = 2 + 28 + (m_pParent->Get_WorldPos().y + m_fY) - m_fSizeX * 0.5;
	m_vTexRect.right = 2 + (m_pParent->Get_WorldPos().x + m_fX) + m_fSizeX * 0.5;
	m_vTexRect.bottom = 2 + 28 + (m_pParent->Get_WorldPos().y + m_fY) + m_fSizeX * 0.5;

	switch (m_iIndex)
	{
	case 8:
		_stprintf_s(szText, TEXT("%d"), m_iCulValue);
		m_pGameInstance->Render_Font(TEXT("UI_Font_25"), szText, m_vTexRect, D3DXCOLOR(0.f, 0.f, 0.f, 1.f), DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		break;
	case 9:
		_stprintf_s(szText, TEXT("%d"), m_iCulValue);
		m_pGameInstance->Render_Font(TEXT("UI_Font_25"), szText, m_vTexRect, D3DXCOLOR(0.f, 0.f, 0.f, 1.f), DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		break;
	case 10:
		_stprintf_s(szText, TEXT("%d"), m_iCulValue);
		m_pGameInstance->Render_Font(TEXT("UI_Font_25"), szText, m_vTexRect, D3DXCOLOR(0.f, 0.f, 0.f, 1.f), DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		break;
	case 11:
		_stprintf_s(szText, TEXT("%d"), m_iCulValue);
		m_pGameInstance->Render_Font(TEXT("UI_Font_25"), szText, m_vTexRect, D3DXCOLOR(0.f, 0.f, 0.f, 1.f), DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		break;
	}

	m_vTexRect.left -= 2;
	m_vTexRect.top -= 2;
	m_vTexRect.right -= 2;
	m_vTexRect.bottom -= 2;

	switch (m_iIndex)
	{
	case 8:
		m_pGameInstance->Render_Font(TEXT("UI_Font_25"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		break;
	case 9:
		m_pGameInstance->Render_Font(TEXT("UI_Font_25"), szText, m_vTexRect, D3DXCOLOR(0.929f, 0.694f, 0.180f, 1.0f), DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		break;
	case 10:
		m_pGameInstance->Render_Font(TEXT("UI_Font_25"), szText, m_vTexRect, D3DXCOLOR(0.392f, 0.600f, 0.910f, 1.0f), DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		break;	
	case 11:
		m_pGameInstance->Render_Font(TEXT("UI_Font_25"), szText, m_vTexRect, D3DXCOLOR(0.506f, 1.000f, 1.000f, 1.0f), DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		break;
	}

}

CStatus_Element_Icon* CStatus_Element_Icon::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
	CStatus_Element_Icon* pInstance = new CStatus_Element_Icon(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eLevel)))
	{
		MSG_BOX(TEXT("Failed to Create : CStatus_Element_Icon"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CStatus_Element_Icon::Clone(void* pArg)
{
	CStatus_Element_Icon* pInstance = new CStatus_Element_Icon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CStatus_Element_Icon"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CStatus_Element_Icon::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}

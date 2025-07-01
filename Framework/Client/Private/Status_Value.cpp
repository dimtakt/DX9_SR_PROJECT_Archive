#include "Status_Value.h"
#include "GameInstance.h"
#include "Stat_Manager.h"
CStatus_Value::CStatus_Value(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CStatus_Value::CStatus_Value(const CStatus_Value& Prototype) : CUIObject(Prototype), m_eLevel{ Prototype.m_eLevel }
{
}

HRESULT CStatus_Value::Initialize_Prototype(LEVEL eLevel)
{
	m_eLevel = eLevel;

	return S_OK;
}

HRESULT CStatus_Value::Initialize(void* pArg)
{
	UIOBJECT_DESC* Desc = static_cast<UIOBJECT_DESC*>(pArg);

	m_iIndex = Desc->fZ;
	m_fSizeX = 22;
	m_fSizeY = 22 ;
	m_fX = - 110;
	m_fY = Desc->fY;
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

void CStatus_Value::Priority_Update(_float fTimeDelta)
{
}

void CStatus_Value::Update(_float fTimeDelta)
{
}

void CStatus_Value::Late_Update(_float fTimeDelta)
{
	Update_Value();
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CStatus_Value::Render()
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

HRESULT CStatus_Value::Ready_Components()
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

void CStatus_Value::Update_Value()
{
	switch (m_iIndex)
	{
	case 0:
		m_iMaxValue = (_int)CStat_Manager::GetInstance()->Get_CurStats()[ENUM_CLASS(STAT_INFO::MAXHP)];
		m_iCulValue = (_int)CStat_Manager::GetInstance()->Get_CurStats()[ENUM_CLASS(STAT_INFO::CULHP)];
		break;
	case 1:
		m_iMaxValue = (_int)CStat_Manager::GetInstance()->Get_CurStats()[ENUM_CLASS(STAT_INFO::MAXMP)];
		m_iCulValue = (_int)CStat_Manager::GetInstance()->Get_CurStats()[ENUM_CLASS(STAT_INFO::CULMP)];
		break;
	case 2:
		m_iCulValue = (_int)CStat_Manager::GetInstance()->Get_CurStats()[ENUM_CLASS(STAT_INFO::CULDEF)];
		break;
	case 3:
		m_iCulValue = (_int)CStat_Manager::GetInstance()->Get_CurStats()[ENUM_CLASS(STAT_INFO::CULCRITICAL)];
		break;
	case 4:
		m_iCulValue = (_int)CStat_Manager::GetInstance()->Get_CurStats()[ENUM_CLASS(STAT_INFO::CRITICALDAMAGE)];
		break;
	case 5:
		m_iCulValue = 100;
		break;
	case 6:
		m_iCulValue = 100;
		break;
	case 7:
		m_iCulValue = (_int)CStat_Manager::GetInstance()->Get_CurStats()[ENUM_CLASS(STAT_INFO::REGENDASH)];
		break;
	}
}

void CStatus_Value::Font_Render()
{
	TCHAR szText[64];
	m_vTexRect.left = 25 + (m_pParent->Get_WorldPos().x + m_fX) - m_fSizeX * 0.5;
	m_vTexRect.top = (m_pParent->Get_WorldPos().y + m_fY) - m_fSizeX * 0.5;
	m_vTexRect.right = 215 + (m_pParent->Get_WorldPos().x + m_fX) + m_fSizeX * 0.5;
	m_vTexRect.bottom = (m_pParent->Get_WorldPos().y + m_fY) + m_fSizeX * 0.5;

	switch (m_iIndex)
	{
	case 0:
		_stprintf_s(szText, TEXT("HP"));
		m_pGameInstance->Render_Font(TEXT("UI_Font_13_Stat"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		_stprintf_s(szText, TEXT("%d / %d"), static_cast<int>(m_iCulValue), static_cast<int>(m_iMaxValue));
		m_pGameInstance->Render_Font(TEXT("UI_Font_13_Stat"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), DT_RIGHT | DT_VCENTER | DT_SINGLELINE);

		break;
	case 1:
		_stprintf_s(szText, TEXT("MP"));
		m_pGameInstance->Render_Font(TEXT("UI_Font_13_Stat"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		_stprintf_s(szText, TEXT("%d / %d"), m_iCulValue, m_iMaxValue);
		m_pGameInstance->Render_Font(TEXT("UI_Font_13_Stat"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
		break;
	case 2:
		_stprintf_s(szText, TEXT("방어력"));
		m_pGameInstance->Render_Font(TEXT("UI_Font_13_Stat"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		_stprintf_s(szText, TEXT("%d"), m_iCulValue);
		m_pGameInstance->Render_Font(TEXT("UI_Font_13_Stat"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
		break;
	case 3:
		_stprintf_s(szText, TEXT("치명타 확률"));
		m_pGameInstance->Render_Font(TEXT("UI_Font_13_Stat"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		_stprintf_s(szText, TEXT("%d%%"), m_iCulValue);
		m_pGameInstance->Render_Font(TEXT("UI_Font_13_Stat"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
		break;
	case 4:
		_stprintf_s(szText, TEXT("치명타 피해"));
		m_pGameInstance->Render_Font(TEXT("UI_Font_13_Stat"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		_stprintf_s(szText, TEXT("%d%%"), m_iCulValue);
		m_pGameInstance->Render_Font(TEXT("UI_Font_13_Stat"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
		break;
	case 5:
		_stprintf_s(szText, TEXT("공격 속도"));
		m_pGameInstance->Render_Font(TEXT("UI_Font_13_Stat"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		_stprintf_s(szText, TEXT("%d%%"), m_iCulValue);
		m_pGameInstance->Render_Font(TEXT("UI_Font_13_Stat"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
		break;
	case 6:
		_stprintf_s(szText, TEXT("이동 속도"));
		m_pGameInstance->Render_Font(TEXT("UI_Font_13_Stat"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		_stprintf_s(szText, TEXT("%d%%"), m_iCulValue);
		m_pGameInstance->Render_Font(TEXT("UI_Font_13_Stat"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
		break;
	case 7:
		_stprintf_s(szText, TEXT("대쉬 회복 속도"));
		m_pGameInstance->Render_Font(TEXT("UI_Font_13_Stat"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		_stprintf_s(szText, TEXT("%d%%"), m_iCulValue);
		m_pGameInstance->Render_Font(TEXT("UI_Font_13_Stat"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
		break;
	}
}

CStatus_Value* CStatus_Value::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
	CStatus_Value* pInstance = new CStatus_Value(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eLevel)))
	{
		MSG_BOX(TEXT("Failed to Create : CStatus_Value"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CStatus_Value::Clone(void* pArg)
{
	CStatus_Value* pInstance = new CStatus_Value(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CStatus_Value"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CStatus_Value::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}

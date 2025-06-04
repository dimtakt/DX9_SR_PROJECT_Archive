#include "Talent_Slot.h"
#include "GameInstance.h"
#include "Stat_Manager.h"
#include "Talent_Slot_Frame.h"
#include "Talent_Slot_Icon.h"
#include "Talent_Slot_Button.h"
CTalent_Slot::CTalent_Slot(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CTalent_Slot::CTalent_Slot(const CTalent_Slot& Prototype) : CUIObject(Prototype), m_iCulValue(Prototype.m_iCulValue), m_eLevel(Prototype.m_eLevel)
{
}

HRESULT CTalent_Slot::Initialize_Prototype(LEVEL eLevel)
{
	m_eLevel = eLevel;

	if (FAILED(Ready_ChildPrototype(eLevel)))
		return E_FAIL;

	return S_OK;
}

HRESULT CTalent_Slot::Initialize(void* pArg)
{
	UIOBJECT_DESC* Desc = static_cast<UIOBJECT_DESC*>(pArg);

	m_iIndex = Desc->fZ;

	m_fSizeX = 470;
	m_fSizeY = 470;
	m_fX = Desc->fX ;
	m_fY = 40;
	m_fZ = UI_DEPTH::TALENT;
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

	return S_OK;
}

void CTalent_Slot::Priority_Update(_float fTimeDelta)
{
	Player_State_Update();
	__super::Priority_Update(fTimeDelta);
}

void CTalent_Slot::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CTalent_Slot::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
	__super::Late_Update(fTimeDelta);
}

HRESULT CTalent_Slot::Render()
{
	SetUp_RenderState();

	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;
	m_pVIBufferCom->Bind_Buffers();

	__super::Begin();
	m_pVIBufferCom->Render();
	Font_Render();
	Reset_RenderState();
	return S_OK;
}

void CTalent_Slot::Add_Value(_int iValue)
{
	m_iCulValue += iValue;
	if (m_iCulValue <= 0)
		m_iCulValue = 0;
}

void CTalent_Slot::Reset_Value()
{
	m_iCulValue = 0;
}

HRESULT CTalent_Slot::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Rect_Talent_Slot"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

void CTalent_Slot::SetUp_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
}

void CTalent_Slot::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	m_pGraphic_Device->SetTexture(0, NULL);
}

HRESULT CTalent_Slot::Ready_ChildPrototype(LEVEL eLevel)
{
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_Talent_Slot_Frame"),
		CTalent_Slot_Frame::Create(m_pGraphic_Device))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_Talent_Slot_Icon"),
		CTalent_Slot_Icon::Create(m_pGraphic_Device, eLevel))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_Talent_Slot_Button"),
		CTalent_Slot_Button::Create(m_pGraphic_Device, eLevel))))
		return E_FAIL;

	return S_OK;
}

HRESULT CTalent_Slot::Ready_Children()
{
	CUIObject* pGameObject = nullptr;

	UIOBJECT_DESC Desc{};

	Desc.fZ = m_iIndex;
	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Talent_Slot_Frame"), &Desc));
	if (nullptr == pGameObject)
		return E_FAIL;
	Add_Child(pGameObject);

	for (_int i = 0; i < 2; ++i)
	{
		Desc.fZ = m_iIndex;
		Desc.fX = i;
		pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Talent_Slot_Icon"), &Desc));
		if (nullptr == pGameObject)
			return E_FAIL;
		Add_Child(pGameObject);
	}

	for (_int i = 0; i < 4; ++i)
	{
		Desc.fZ = m_iIndex;
		Desc.fX = i;
		pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Talent_Slot_Button"), &Desc));
		if (nullptr == pGameObject)
			return E_FAIL;
		Add_Child(pGameObject);
	}
	return S_OK;
}

void CTalent_Slot::Font_Render()
{
	//_int iPoint = CStat_Manager::GetInstance()->Get_CurStats()[ENUM_CLASS(STAT_INFO::CULSTATPOINT)];

	TCHAR szText[64];
	CUIObject::Font_Rect_Update();
	m_vTexRect.top += 30;

	switch (m_iIndex)
	{
	case 0:
		_stprintf_s(szText, TEXT("분노"));
		m_pGameInstance->Render_Font(TEXT("UI_Font_30"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.0f), DT_CENTER | DT_TOP);
		break;
	case 1:
		_stprintf_s(szText, TEXT("생존"));
		m_pGameInstance->Render_Font(TEXT("UI_Font_30"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.0f), DT_CENTER | DT_TOP);
		break;
	case 2:
		_stprintf_s(szText, TEXT("인내"));
		m_pGameInstance->Render_Font(TEXT("UI_Font_30"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.0f), DT_CENTER | DT_TOP);
		break;
	case 3:
		_stprintf_s(szText, TEXT("지혜"));
		m_pGameInstance->Render_Font(TEXT("UI_Font_30"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.0f), DT_CENTER | DT_TOP);
		break;
	}
	m_vTexRect.top += 70;

	switch (m_iIndex)
	{
	case 0:
		_stprintf_s(szText, TEXT("%d"), m_iCulValue);
		m_pGameInstance->Render_Font(TEXT("UI_Font_60"), szText, m_vTexRect, D3DXCOLOR(0.745f, 0.173f, 0.200f, 1.0f), DT_CENTER | DT_TOP);
		break;
	case 1:
		_stprintf_s(szText, TEXT("%d"), m_iCulValue);
		m_pGameInstance->Render_Font(TEXT("UI_Font_60"), szText, m_vTexRect, D3DXCOLOR(0.675f, 0.851f, 0.765f, 1.0f), DT_CENTER | DT_TOP);
		break;
	case 2:
		_stprintf_s(szText, TEXT("%d"), m_iCulValue);
		m_pGameInstance->Render_Font(TEXT("UI_Font_60"), szText, m_vTexRect, D3DXCOLOR(0.831f, 0.843f, 0.867f, 1.0f), DT_CENTER | DT_TOP);
		break;
	case 3:
		_stprintf_s(szText, TEXT("%d"), m_iCulValue);
		m_pGameInstance->Render_Font(TEXT("UI_Font_60"), szText, m_vTexRect, D3DXCOLOR(0.549f, 0.765f, 0.792f, 1.0f), DT_CENTER | DT_TOP);
		break;
	}

	m_vTexRect.top += 120;
	switch (m_iIndex)
	{
	case 0:
		m_vTexRect.left -= 55;
		m_vTexRect.right -= 55;
		_stprintf_s(szText, TEXT("+%d%%"), m_iCulValue);
		m_pGameInstance->Render_Font(TEXT("UI_Font_25_Normal"), szText, m_vTexRect, D3DXCOLOR(0.f, 1.f, 0.f, 1.0f), DT_CENTER | DT_TOP);
		m_vTexRect.left += 90;
		m_vTexRect.right += 90;
		_stprintf_s(szText, TEXT("치명타 확률"));
		m_pGameInstance->Render_Font(TEXT("UI_Font_25_Normal"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.0f), DT_CENTER | DT_TOP);
		break;
	case 1:
		m_vTexRect.left -= 45;
		m_vTexRect.right -= 45;
		_stprintf_s(szText, TEXT("+%d"), m_iCulValue * 5);
		m_pGameInstance->Render_Font(TEXT("UI_Font_25_Normal"), szText, m_vTexRect, D3DXCOLOR(0.f, 1.f, 0.f, 1.0f), DT_CENTER | DT_TOP);
		m_vTexRect.left += 65;
		m_vTexRect.right += 65;
		_stprintf_s(szText, TEXT("최대 HP"));
		m_pGameInstance->Render_Font(TEXT("UI_Font_25_Normal"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.0f), DT_CENTER | DT_TOP);		break;
	case 2:
		m_vTexRect.left -= 45;
		m_vTexRect.right -= 45;
		_stprintf_s(szText, TEXT("+%d"), m_iCulValue);
		m_pGameInstance->Render_Font(TEXT("UI_Font_25_Normal"), szText, m_vTexRect, D3DXCOLOR(0.f, 1.f, 0.f, 1.0f), DT_CENTER | DT_TOP);
		m_vTexRect.left += 60;
		m_vTexRect.right += 60;
		_stprintf_s(szText, TEXT("방어력"));
		m_pGameInstance->Render_Font(TEXT("UI_Font_25_Normal"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.0f), DT_CENTER | DT_TOP);	break;
	case 3:
		m_vTexRect.left -= 45;
		m_vTexRect.right -= 45;
		_stprintf_s(szText, TEXT("+%d"), m_iCulValue * 5);
		m_pGameInstance->Render_Font(TEXT("UI_Font_25_Normal"), szText, m_vTexRect, D3DXCOLOR(0.f, 1.f, 0.f, 1.0f), DT_CENTER | DT_TOP);
		m_vTexRect.left += 65;
		m_vTexRect.right += 65;
		_stprintf_s(szText, TEXT("최대 MP"));
		m_pGameInstance->Render_Font(TEXT("UI_Font_25_Normal"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.0f), DT_CENTER | DT_TOP);	break;
	}

	CUIObject::Font_Rect_Update();

	m_vTexRect.top += 290;
	m_vTexRect.left -= 48;
	m_vTexRect.right -= 48;
	_stprintf_s(szText, TEXT("%d"), 10);
	
	if (m_iCulValue >= 10)
		m_pGameInstance->Render_Font(TEXT("UI_Font_18_Normal"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.0f), DT_CENTER | DT_TOP);
	else
		m_pGameInstance->Render_Font(TEXT("UI_Font_18_Normal"), szText, m_vTexRect, D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f), DT_CENTER | DT_TOP);
	
	m_vTexRect.left += 96;
	m_vTexRect.right += 96;
	_stprintf_s(szText, TEXT("%d"), 20);
	if (m_iCulValue >= 20)
		m_pGameInstance->Render_Font(TEXT("UI_Font_18_Normal"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.0f), DT_CENTER | DT_TOP);
	else
		m_pGameInstance->Render_Font(TEXT("UI_Font_18_Normal"), szText, m_vTexRect, D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f), DT_CENTER | DT_TOP);

}

void CTalent_Slot::Player_State_Update()
{
	if (m_iPreValue != m_iCulValue)
	{
		switch (m_iIndex)
		{
		case 0:
			CStat_Manager::GetInstance()->Cal_Stats(STAT_INFO::CULCRITICAL, -m_iPreValue);
			CStat_Manager::GetInstance()->Cal_Stats(STAT_INFO::CULCRITICAL, m_iCulValue);
			break;
		case 1:
			CStat_Manager::GetInstance()->Cal_Stats(STAT_INFO::MAXHP, -m_iPreValue * 5);
			CStat_Manager::GetInstance()->Cal_Stats(STAT_INFO::MAXHP, m_iCulValue * 5) ;
			break;
		case 2:
			CStat_Manager::GetInstance()->Cal_Stats(STAT_INFO::CULDEF, -m_iPreValue);
			CStat_Manager::GetInstance()->Cal_Stats(STAT_INFO::CULDEF, m_iCulValue);
			break;
		case 3:
			CStat_Manager::GetInstance()->Cal_Stats(STAT_INFO::MAXMP, -m_iPreValue * 5);
			CStat_Manager::GetInstance()->Cal_Stats(STAT_INFO::MAXMP, m_iCulValue * 5);

			break;
		}
		m_iPreValue = m_iCulValue;
	}
}

CTalent_Slot* CTalent_Slot::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
	CTalent_Slot* pInstance = new CTalent_Slot(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eLevel)))
	{
		MSG_BOX(TEXT("Failed to Created : CTalent_Slot"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CTalent_Slot::Clone(void* pArg)
{
	CTalent_Slot* pInstance = new CTalent_Slot(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CTalent_Slot"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTalent_Slot::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}

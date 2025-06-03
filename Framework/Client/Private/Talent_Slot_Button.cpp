#include "Talent_Slot_Button.h"
#include "GameInstance.h"
#include "Talent_Button_Selete.h"
#include "Talent_Slot.h"
#include "Stat_Manager.h"
CTalent_Slot_Button::CTalent_Slot_Button(LPDIRECT3DDEVICE9 pGraphic_Device) : CButton(pGraphic_Device)
{
}

CTalent_Slot_Button::CTalent_Slot_Button(const CTalent_Slot_Button& Prototype) : CButton(Prototype), m_eLevel(Prototype.m_eLevel)
{
}

HRESULT CTalent_Slot_Button::Initialize_Prototype(LEVEL eLevel)
{
	m_eLevel = eLevel;

	if (FAILED(Ready_ChildPrototype(eLevel)))
		return E_FAIL;

	return S_OK;
}

HRESULT CTalent_Slot_Button::Initialize(void* pArg)
{
	UIOBJECT_DESC* Desc = static_cast<UIOBJECT_DESC*>(pArg);

	m_iTexIdex = Desc->fX;
	m_iIndex = Desc->fZ;

	if (Desc->fX == 0)
	{
		m_fSizeX = 64;
		m_fSizeY = 64;
		m_fX = -85;
	}
	else if (Desc->fX == 1)
	{
		m_fSizeX = 55;
		m_fSizeY = 55;
		m_fX = -16;
	}
	else if (Desc->fX == 2)
	{
		m_fSizeX = 55;
		m_fSizeY = 55;
		m_fX = 16;
	}
	else if (Desc->fX == 3)
	{
		m_fSizeX = 64;
		m_fSizeY = 64;
		m_fX = 85;
	}
	
	m_fY = 192;
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

	Ready_ButtonSet();
	return S_OK;
}

void CTalent_Slot_Button::Priority_Update(_float fTimeDelta)
{
	CUIObject::Priority_Update(fTimeDelta);
}

void CTalent_Slot_Button::Update(_float fTimeDelta)
{
	_int iTemp = static_cast<CTalent_Slot*>(m_pParent)->Get_Value();
	_int iSrc = CStat_Manager::GetInstance()->Get_CurStats()[ENUM_CLASS(STAT_INFO::CULSTATPOINT)];

	if (m_iIndex > 1 && iSrc > 0)
	{
		if (Is_Button_Pick() && m_pGameInstance->IsKeyDown(VK_LBUTTON))
		{
			static_cast<CTalent_Slot*>(m_pParent)->Add_Value(m_iAddValue);
			CStat_Manager::GetInstance()->Cal_Stats(STAT_INFO::CULSTATPOINT, -m_iAddValue);
		}
	}
	CUIObject::Update(fTimeDelta);
}

void CTalent_Slot_Button::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
	
	if(Is_Button_Pick())
		CUIObject::Late_Update(fTimeDelta);
}

HRESULT CTalent_Slot_Button::Render()
{
	SetUp_RenderState();

	if (FAILED(CButton::Bind_ButtonTex_Single(g_hWnd, m_iTexIdex)))
		return E_FAIL;

	Reset_RenderState();
	return S_OK;
}

HRESULT CTalent_Slot_Button::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	_tchar	szTextureFilePath[MAX_PATH];
	wsprintf(szTextureFilePath, TEXT("Prototype_Component_Texture_Rect_Talent_Slot_Button_%d"), m_iIndex);

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), szTextureFilePath,
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;


	return S_OK;
}

void CTalent_Slot_Button::SetUp_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
}

void CTalent_Slot_Button::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	m_pGraphic_Device->SetTexture(0, NULL);
}

HRESULT CTalent_Slot_Button::Ready_ChildPrototype(LEVEL eLevel)
{
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_Talent_Button_Selete"),
		CTalent_Button_Selete::Create(m_pGraphic_Device))))
		return E_FAIL;


	return S_OK;
}

HRESULT CTalent_Slot_Button::Ready_Children()
{
	CUIObject* pGameObject = nullptr;

	UIOBJECT_DESC Desc{};

	Desc.fZ = m_iTexIdex;
	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Talent_Button_Selete"), &Desc));
	if (nullptr == pGameObject)
		return E_FAIL;
	Add_Child(pGameObject);



	return S_OK;
}

void CTalent_Slot_Button::Ready_ButtonSet()
{
	switch (m_iTexIdex)
	{
	case 0:
		m_iAddValue -= 5;
		break;
	case 1:
		m_iAddValue -= 1;
		break;
	case 2:
		m_iAddValue += 1;
		break;
	case 3:
		m_iAddValue += 5;
		break;
	}
}

_bool CTalent_Slot_Button::Is_Button_Pick()
{
	POINT			ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	RECT			rcUI = { m_vWorldPos.x - m_fSizeX * 0.5f, m_vWorldPos.y - m_fSizeY * 0.5f, m_vWorldPos.x + m_fSizeX * 0.5f, +m_vWorldPos.y + m_fSizeY * 0.5f + 60 };

	return PtInRect(&rcUI, ptMouse);
}

CTalent_Slot_Button* CTalent_Slot_Button::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
	CTalent_Slot_Button* pInstance = new CTalent_Slot_Button(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eLevel)))
	{
		MSG_BOX(TEXT("Failed to Created : CTalent_Slot_Button"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CTalent_Slot_Button::Clone(void* pArg)
{
	CTalent_Slot_Button* pInstance = new CTalent_Slot_Button(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CTalent_Slot_Button"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTalent_Slot_Button::Free()
{
	__super::Free();
}

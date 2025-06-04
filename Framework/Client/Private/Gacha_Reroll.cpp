#include "Gacha_Reroll.h"
#include "GameInstance.h"

CGacha_Reroll::CGacha_Reroll(LPDIRECT3DDEVICE9 pGraphic_Device) : CButton(pGraphic_Device)
{
}

CGacha_Reroll::CGacha_Reroll(const CGacha_Reroll& Prototype) : CButton(Prototype), m_eLevel(Prototype.m_eLevel)
{
}

HRESULT CGacha_Reroll::Initialize_Prototype(LEVEL eLevel)
{
	m_eLevel = eLevel;

	if (FAILED(Ready_ChildPrototype(eLevel)))
		return E_FAIL;

	return S_OK;
}

HRESULT CGacha_Reroll::Initialize(void* pArg)
{
	m_fSizeX = 200;
	m_fSizeY = 200;
	m_fX = -320;
	m_fY = 180;
	m_fZ = UI_DEPTH::GACHA;
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

void CGacha_Reroll::Priority_Update(_float fTimeDelta)
{
	CUIObject::Priority_Update(fTimeDelta);
}

void CGacha_Reroll::Update(_float fTimeDelta)
{
	CUIObject::Update(fTimeDelta);
}

void CGacha_Reroll::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
	CUIObject::Late_Update(fTimeDelta);
}

HRESULT CGacha_Reroll::Render()
{
	SetUp_RenderState();

	if (Button_Pick())
		__super::Render_Button(1);
	else
		__super::Render_Button(0);
	Font_Render();
	Reset_RenderState();
	return S_OK;
}

HRESULT CGacha_Reroll::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Rect_GachaButton"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;


	return S_OK;
}

void CGacha_Reroll::SetUp_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
}

void CGacha_Reroll::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	m_pGraphic_Device->SetTexture(0, NULL);
}

HRESULT CGacha_Reroll::Ready_ChildPrototype(LEVEL eLevel)
{
//	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_Talent_Progress"),
//		CTalent_Progress::Create(m_pGraphic_Device))))
//		return E_FAIL;

	return S_OK;
}

HRESULT CGacha_Reroll::Ready_Children()
{
	CUIObject* pGameObject = nullptr;

	//pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Talent_Progress")));
	//if (nullptr == pGameObject)
	//	return E_FAIL;
	//Add_Child(pGameObject);


	return S_OK;
}

void CGacha_Reroll::Font_Render()
{
	//TCHAR szText[64];
	//m_vTexRect.left = 660 + m_fX - m_fSizeX * 0.5;
	//m_vTexRect.top = 442 + m_fY - m_fSizeY * 0.5;
	//m_vTexRect.right = 660 + m_fX + m_fSizeX * 0.5;
	//m_vTexRect.bottom = 441 + m_fY + m_fSizeY * 0.5;

	//_stprintf_s(szText, TEXT("ÃÊ±âÈ­"));
	//m_pGameInstance->Render_Font(TEXT("UI_Font_30"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.0f), DT_CENTER | DT_TOP);

}

_bool CGacha_Reroll::Button_Pick()
{
	POINT			ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	RECT			rcUI = { m_vWorldPos.x - m_fSizeX * 0.5f, m_vWorldPos.y - m_fSizeY * 0.5f + 80, m_vWorldPos.x + m_fSizeX * 0.5f, +m_vWorldPos.y + m_fSizeY * 0.5f - 50 };

	return PtInRect(&rcUI, ptMouse);
}

CGacha_Reroll* CGacha_Reroll::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
	CGacha_Reroll* pInstance = new CGacha_Reroll(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eLevel)))
	{
		MSG_BOX(TEXT("Failed to Created : CGacha_Reroll"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CGacha_Reroll::Clone(void* pArg)
{
	CGacha_Reroll* pInstance = new CGacha_Reroll(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CGacha_Reroll"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CGacha_Reroll::Free()
{
	__super::Free();
}

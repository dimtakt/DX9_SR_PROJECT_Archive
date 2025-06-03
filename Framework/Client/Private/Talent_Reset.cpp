#include "Talent_Reset.h"
#include "GameInstance.h"
#include "Talent_Button_Icon.h"
#include "Talent_Progress.h"
CTalent_Reset::CTalent_Reset(LPDIRECT3DDEVICE9 pGraphic_Device) : CButton(pGraphic_Device)
{
}

CTalent_Reset::CTalent_Reset(const CTalent_Reset& Prototype) : CButton(Prototype), m_eLevel(Prototype.m_eLevel)
{
}

HRESULT CTalent_Reset::Initialize_Prototype(LEVEL eLevel)
{
	m_eLevel = eLevel;

	if (FAILED(Ready_ChildPrototype(eLevel)))
		return E_FAIL;

	return S_OK;
}

HRESULT CTalent_Reset::Initialize(void* pArg)
{
	m_fSizeX = 200;
	m_fSizeY = 200;
	m_fX = 110 - g_iWinSizeX * 0.5;
	m_fY = -40 + g_iWinSizeY * 0.5;
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

void CTalent_Reset::Priority_Update(_float fTimeDelta)
{
	CUIObject::Priority_Update(fTimeDelta);
}

void CTalent_Reset::Update(_float fTimeDelta)
{
	if (Check_Key_Down(g_hWnd, VK_LBUTTON))
		m_bHold = true;
	else if (Check_Key_UP(g_hWnd, VK_LBUTTON))
		m_bHold = false;
	

	if (m_bHold)
		m_iClickValue += 100 * fTimeDelta;
	else
		m_iClickValue = 0;

	static_cast<CTalent_Progress*>(m_vecChildren[0])->Progress_Update(m_iClickValue);
	CUIObject::Update(fTimeDelta);
}

void CTalent_Reset::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
	CUIObject::Late_Update(fTimeDelta);
}

HRESULT CTalent_Reset::Render()
{
	SetUp_RenderState();

	if (m_bHold)
	{
		if (FAILED(CButton::Bind_ButtonTex_Single(g_hWnd, 0)))
			return E_FAIL;
	}
	else
	{
		if (FAILED(CButton::Bind_ButtonTex_Double(g_hWnd, 0, 1)))
			return E_FAIL;
	}
	Font_Render();
	Reset_RenderState();
	return S_OK;
}

HRESULT CTalent_Reset::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Rect_Talent_Button"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;


	return S_OK;
}

void CTalent_Reset::SetUp_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
}

void CTalent_Reset::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	m_pGraphic_Device->SetTexture(0, NULL);
}

HRESULT CTalent_Reset::Ready_ChildPrototype(LEVEL eLevel)
{
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_Talent_Progress"),
		CTalent_Progress::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_Talent_Button_Icon"),
		CTalent_Button_Icon::Create(m_pGraphic_Device))))
		return E_FAIL;

	return S_OK;
}

HRESULT CTalent_Reset::Ready_Children()
{
	CUIObject* pGameObject = nullptr;

	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Talent_Progress")));
	if (nullptr == pGameObject)
		return E_FAIL;
	Add_Child(pGameObject);

	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Talent_Button_Icon")));
	if (nullptr == pGameObject)
		return E_FAIL;
	Add_Child(pGameObject);

	return S_OK;
}

void CTalent_Reset::Font_Render()
{
	TCHAR szText[64];
	m_vTexRect.left = 660 + m_fX - m_fSizeX * 0.5;
	m_vTexRect.top = 442 + m_fY - m_fSizeY * 0.5;
	m_vTexRect.right = 660 + m_fX + m_fSizeX * 0.5;
	m_vTexRect.bottom = 441 + m_fY + m_fSizeY * 0.5;

	_stprintf_s(szText, TEXT("ÃÊ±âÈ­"));
	m_pGameInstance->Render_Font(TEXT("UI_Font_30"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.0f), DT_CENTER | DT_TOP);

}

CTalent_Reset* CTalent_Reset::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
	CTalent_Reset* pInstance = new CTalent_Reset(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eLevel)))
	{
		MSG_BOX(TEXT("Failed to Created : CTalent_Reset"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CTalent_Reset::Clone(void* pArg)
{
	CTalent_Reset* pInstance = new CTalent_Reset(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CTalent_Reset"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTalent_Reset::Free()
{
	__super::Free();
}

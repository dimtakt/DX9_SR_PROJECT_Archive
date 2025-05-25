#include "Mirror_Button.h"
#include "GameInstance.h"
#include "UI_KeyGuide.h"

CMirror_Button::CMirror_Button(LPDIRECT3DDEVICE9 pGraphic_Device) : CButton(pGraphic_Device)
{
}

CMirror_Button::CMirror_Button(const CMirror_Button& Prototype) : CButton(Prototype), m_eLevel(Prototype.m_eLevel)
{
}

HRESULT CMirror_Button::Initialize_Prototype(LEVEL eLevel)
{
	m_eLevel = eLevel;

	if (FAILED(Ready_ChildPrototype(eLevel)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMirror_Button::Initialize(void* pArg)
{
	UIOBJECT_DESC Desc{};
	Desc.fSizeX = 64;
	Desc.fSizeY = 64;
	Desc.fX = 128 + Desc.fSizeX * 0.5;;
	Desc.fY = 0;
	Desc.iWinSizeX = g_iWinSizeX;
	Desc.iWinSizeY = g_iWinSizeY;

	if (FAILED(CUIObject::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
	__super::Update_Position();

	if (FAILED(Ready_Children()))
		return E_FAIL;

	return S_OK;
}

void CMirror_Button::Priority_Update(_float fTimeDelta)
{
	CUIObject::Priority_Update(fTimeDelta);
}

void CMirror_Button::Update(_float fTimeDelta)
{
	CUIObject::Update(fTimeDelta);
}

void CMirror_Button::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
	CUIObject::Late_Update(fTimeDelta);
}

HRESULT CMirror_Button::Render()
{
	SetUp_RenderState();
	if (FAILED(CButton::Bind_ButtonTex_Double(g_hWnd, 0, 1)))
		return E_FAIL;

	Reset_RenderState();
	return S_OK;
}

HRESULT CMirror_Button::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Rect_UI_Hud_Mirror_Symbol"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

void CMirror_Button::SetUp_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
}

void CMirror_Button::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	m_pGraphic_Device->SetTexture(0, NULL);
}

HRESULT CMirror_Button::Ready_ChildPrototype(LEVEL eLevel)
{
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_MirrorButton_Guide"),
		CUI_KeyGuide::Create(m_pGraphic_Device, TEXT("C")))))

	return E_NOTIMPL;
}

HRESULT CMirror_Button::Ready_Children()
{
	CUIObject* pGameObject = nullptr;

	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_MirrorButton_Guide")));
	if (nullptr == pGameObject)
		return E_FAIL;
	Add_Child(pGameObject);

	return S_OK;
}

CMirror_Button* CMirror_Button::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
	CMirror_Button* pInstance = new CMirror_Button(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eLevel)))
	{
		MSG_BOX(TEXT("Failed to Created : CMirror_Button"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CMirror_Button::Clone(void* pArg)
{
	CMirror_Button* pInstance = new CMirror_Button(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CMirror_Button"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMirror_Button::Free()
{
	__super::Free();
}

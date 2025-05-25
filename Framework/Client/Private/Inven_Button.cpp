#include "Inven_Button.h"
#include "GameInstance.h"
#include "UI_KeyGuide.h"
#include "Inventory.h"
CInven_Button::CInven_Button(LPDIRECT3DDEVICE9 pGraphic_Device) : CButton{ pGraphic_Device }
{
}

CInven_Button::CInven_Button(const CInven_Button& Prototype) : CButton(Prototype), m_eLevel(Prototype.m_eLevel)
{
}

HRESULT CInven_Button::Initialize_Prototype(LEVEL eLevel)
{
	m_eLevel = eLevel;

	if (FAILED(Ready_ChildPrototype(eLevel)))
		return E_FAIL;

	return S_OK;
}

HRESULT CInven_Button::Initialize(void* pArg)
{
	UIOBJECT_DESC Desc{};
	Desc.fSizeX = 64 ;
	Desc.fSizeY = 64 ;
	Desc.fX = Desc.fSizeX * 0.5;
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

	if (FAILED(Setting_Target()))
		return E_FAIL;

	return S_OK;
}

void CInven_Button::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CInven_Button::Update(_float fTimeDelta)
{
	if (Click_Check(g_hWnd))
	{
		static_cast<CInventory*>(m_pTargetUI)->UI_Switch();
	}
	
	__super::Update(fTimeDelta);
}

void CInven_Button::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
	__super::Late_Update(fTimeDelta);
}

HRESULT CInven_Button::Render()
{
	SetUp_RenderState();
	if (FAILED(CButton::Bind_ButtonTex_Double(g_hWnd, 0, 1)))
		return E_FAIL;
	
	Reset_RenderState();
	return S_OK;
}

HRESULT CInven_Button::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Rect_UI_Hud_Inven_Symbol"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

void CInven_Button::SetUp_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
}

void CInven_Button::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	m_pGraphic_Device->SetTexture(0, NULL);
}

HRESULT CInven_Button::Ready_ChildPrototype(LEVEL eLevel)
{
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_InvenButton_Guide"),
		CUI_KeyGuide::Create(m_pGraphic_Device, TEXT("Z")))))
		return E_FAIL;

	return S_OK;
}

HRESULT CInven_Button::Ready_Children()
{
	CUIObject* pGameObject = nullptr;

	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_InvenButton_Guide")));
	if (nullptr == pGameObject)
		return E_FAIL;
	Add_Child(pGameObject);

	return S_OK;
}

HRESULT CInven_Button::Setting_Target()
{
	m_pTargetUI = m_pGameInstance->Get_GameObject(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Layer_UI"), 0);

	if (m_pTargetUI == nullptr)
	{
		MSG_BOX(TEXT("Failed to Setting : CInven_Button"));
		return E_FAIL;
	}
	Safe_AddRef(m_pTargetUI);
	return S_OK;
}

CInven_Button* CInven_Button::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
	CInven_Button* pInstance = new CInven_Button(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eLevel)))
	{
		MSG_BOX(TEXT("Failed to Created : CInven_Button"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CInven_Button::Clone(void* pArg)
{
	CInven_Button* pInstance = new CInven_Button(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CInven_Button"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CInven_Button::Free()
{
	__super::Free();

	Safe_Release(m_pTargetUI);
}

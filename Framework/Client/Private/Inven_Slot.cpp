#include "Inven_Slot.h"
#include "GameInstance.h"
#include "Inven_Slot_Selete.h"
CInven_Slot::CInven_Slot(LPDIRECT3DDEVICE9 pGraphic_Device) : CButton{ pGraphic_Device }
{
}

CInven_Slot::CInven_Slot(const CInven_Slot& Prototype) : CButton(Prototype), m_eLevel(Prototype.m_eLevel), m_bIsOver(false)
{
}

HRESULT CInven_Slot::Initialize_Prototype(LEVEL eLevel)
{
	m_eLevel = eLevel;

	if (FAILED(Ready_ChildPrototype(eLevel)))
		return E_FAIL;

	return S_OK;
}

HRESULT CInven_Slot::Initialize(void* pArg)
{
	CUIObject::UIOBJECT_DESC* Desc = static_cast<UIOBJECT_DESC*>(pArg);

	m_fSizeX = 76;
	m_fSizeY = 76;
	m_fX = -203 + Desc->fX * (m_fSizeX + 5);
	m_fY = -151 + Desc->fY * (m_fSizeY + 5);
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

void CInven_Slot::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CInven_Slot::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CInven_Slot::Late_Update(_float fTimeDelta)
{
	if (CUIObject::isPick(g_hWnd))
		m_bIsOver = true;
	else
		m_bIsOver = false;

	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);


	if(m_bIsOver)
		__super::Late_Update(fTimeDelta);
}

HRESULT CInven_Slot::Render()
{
	SetUp_RenderState();
	if (FAILED(CButton::Bind_ButtonTex_Single(g_hWnd, 0)))
		return E_FAIL;
	Reset_RenderState();
	return S_OK;
}

HRESULT CInven_Slot::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_Texture_Rect_Slot_Inven"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

void CInven_Slot::SetUp_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
}

void CInven_Slot::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	m_pGraphic_Device->SetTexture(0, NULL);
}

HRESULT CInven_Slot::Ready_ChildPrototype(LEVEL eLevel)
{
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_Inven_Slot_Selete"),
		CInven_Slot_Selete::Create(m_pGraphic_Device, eLevel))))
		return E_FAIL;

	return S_OK;
}

HRESULT CInven_Slot::Ready_Children()
{
	CUIObject* pGameObject = nullptr;

	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Inven_Slot_Selete")));
	if (nullptr == pGameObject)
		return E_FAIL;
	Add_Child(pGameObject);

	return S_OK;
}


CInven_Slot* CInven_Slot::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
	CInven_Slot* pInstance = new CInven_Slot(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eLevel)))
	{
		MSG_BOX(TEXT("Failed to Created : CInven_Slot"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CInven_Slot::Clone(void* pArg)
{
	CInven_Slot* pInstance = new CInven_Slot(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CInven_Slot"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CInven_Slot::Free()
{
	__super::Free();
}

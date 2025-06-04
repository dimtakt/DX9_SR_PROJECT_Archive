#include "Gacha_Slot.h"
#include "GameInstance.h"
#include "Inven_Slot_Selete.h"
#include "Client_Defines_Item.h"

CGacha_Slot::CGacha_Slot(LPDIRECT3DDEVICE9 pGraphic_Device) : CButton{ pGraphic_Device }
{
}

CGacha_Slot::CGacha_Slot(const CGacha_Slot& Prototype) : CButton(Prototype), m_eLevel(Prototype.m_eLevel), m_bIsOver(false)
{
}

HRESULT CGacha_Slot::Initialize_Prototype(LEVEL eLevel)
{
	m_eLevel = eLevel;

	if (FAILED(Ready_ChildPrototype(eLevel)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CGacha_Slot::Initialize(void* pArg)
{
	UIOBJECT_DESC* Desc = static_cast<UIOBJECT_DESC*>(pArg);

	m_iSlotIndex = Desc->fZ;
	m_fSizeX = 96;
	m_fSizeY = 96;
	m_fX = -320 + Desc->fX;
	m_fY = Desc->fY;
	m_fZ = UI_DEPTH::GACHA_SLOT;
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

void CGacha_Slot::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CGacha_Slot::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CGacha_Slot::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
	__super::Late_Update(fTimeDelta);

}

HRESULT CGacha_Slot::Render()
{
	SetUp_RenderState();
	if (FAILED(CButton::Bind_ButtonTex_Single(g_hWnd, m_iSlotItem_Tex)))
		return E_FAIL;
	Reset_RenderState();

	return S_OK;
}



HRESULT CGacha_Slot::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Rect_GachaSlot"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

void CGacha_Slot::SetUp_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
}

void CGacha_Slot::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	m_pGraphic_Device->SetTexture(0, NULL);
}

HRESULT CGacha_Slot::Ready_ChildPrototype(LEVEL eLevel)
{
	//if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_Inven_Slot_Selete"),
	//	CInven_Slot_Selete::Create(m_pGraphic_Device, eLevel))))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CGacha_Slot::Ready_Children()
{
	CUIObject* pGameObject = nullptr;

	//pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Inven_Slot_Selete")));
	//if (nullptr == pGameObject)
	//	return E_FAIL;
	//Add_Child(pGameObject);

	return S_OK;
}

CGacha_Slot* CGacha_Slot::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
	CGacha_Slot* pInstance = new CGacha_Slot(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eLevel)))
	{
		MSG_BOX(TEXT("Failed to Created : CGacha_Slot"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CGacha_Slot::Clone(void* pArg)
{
	CGacha_Slot* pInstance = new CGacha_Slot(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CGacha_Slot"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CGacha_Slot::Free()
{
	__super::Free();
}

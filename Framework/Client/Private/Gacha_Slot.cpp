#include "Gacha_Slot.h"
#include "GameInstance.h"
#include "Gacha_Slot_Selete.h"
#include "Client_Defines_Item.h"
#include "Gacha.h"
CGacha_Slot::CGacha_Slot(LPDIRECT3DDEVICE9 pGraphic_Device) : CButton{ pGraphic_Device }
{
}

CGacha_Slot::CGacha_Slot(const CGacha_Slot& Prototype) : CButton(Prototype), m_eLevel(Prototype.m_eLevel)
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
	m_fY = -50 + Desc->fY;
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

	if (m_pSlotItem != nullptr)
		m_iSlotItem_Tex = ENUM_CLASS(m_pSlotItem->Item_Info()->iRarity);
	else
		m_iSlotItem_Tex = 0;
}

void CGacha_Slot::Update(_float fTimeDelta)
{
	Item_Selete();
	__super::Update(fTimeDelta);
}

void CGacha_Slot::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
	
	if(CUIObject::isPick(g_hWnd))
		__super::Late_Update(fTimeDelta);

	if (m_pSlotItem != nullptr)
		m_pSlotItem->Late_Update(fTimeDelta, _float3{ m_pTransformCom->Get_State(STATE::POSITION).x, m_pTransformCom->Get_State(STATE::POSITION).y - 4, m_pTransformCom->Get_State(STATE::POSITION).z });
	
	if (m_bIsPick)
		m_pSlotItem->IsSelete();

}

HRESULT CGacha_Slot::Render()
{
	if (FAILED(CButton::Bind_ButtonTex_Single(g_hWnd, m_iSlotItem_Tex)))
		return E_FAIL;

	return S_OK;
}

void CGacha_Slot::Push_Item(CItemObject* pItem)
{
	m_pSlotItem = static_cast<CItem_Base*>(pItem);
	
	//static_cast<CGacha*>(m_pParent)->UI_Switch();
}

void CGacha_Slot::Push_Item_ReRoll(CItemObject* pItem)
{
	if(pItem != nullptr)
		Safe_Release(m_pSlotItem);

	m_pSlotItem = static_cast<CItem_Base*>(pItem);
}

void CGacha_Slot::Release_Item()
{
	if(m_pSlotItem != nullptr)
		Safe_Release(m_pSlotItem);
}

void CGacha_Slot::Release_Pop()
{
	m_pSlotItem = nullptr;
}

void CGacha_Slot::Item_Selete()
{
	if(Check_Key_Down(g_hWnd, VK_LBUTTON) && m_pSlotItem != nullptr)
	{
		m_pGameInstance->Pick_ItemSlot(m_pSlotItem, this, 1, 2);
		m_bIsPick = true;
	}
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

HRESULT CGacha_Slot::Ready_ChildPrototype(LEVEL eLevel)
{
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_Gacha_Slot_Selete"),
		CGacha_Slot_Selete::Create(m_pGraphic_Device, eLevel))))
		return E_FAIL;

	return S_OK;
}

HRESULT CGacha_Slot::Ready_Children()
{
	CUIObject* pGameObject = nullptr;

	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Gacha_Slot_Selete")));
	if (nullptr == pGameObject)
		return E_FAIL;
	Add_Child(pGameObject);

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
	Safe_Release(m_pSlotItem);

}

#include "Hud_Quick_Slot.h"
#include "GameInstance.h"
#include "Gacha_Slot_Selete.h"
#include "Client_Defines_Item.h"
#include "Gacha.h"
#include "Tooltip.h"
CHud_Quick_Slot::CHud_Quick_Slot(LPDIRECT3DDEVICE9 pGraphic_Device) : CButton{ pGraphic_Device }
{
}

CHud_Quick_Slot::CHud_Quick_Slot(const CHud_Quick_Slot& Prototype) : CButton(Prototype), m_eLevel(Prototype.m_eLevel)
{
}

HRESULT CHud_Quick_Slot::Initialize_Prototype(LEVEL eLevel)
{
	m_eLevel = eLevel;

	if (FAILED(Ready_ChildPrototype(eLevel)))
		return E_FAIL;

	return S_OK;
}

HRESULT CHud_Quick_Slot::Initialize(void* pArg)
{
	UIOBJECT_DESC* Desc = static_cast<UIOBJECT_DESC*>(pArg);

//	m_iSlotIndex = Desc->fZ;
	m_fSizeX = 96;
	m_fSizeY = 96;
	m_fX = g_iWinSizeX * 0.5;
	m_fY = g_iWinSizeY * 0.5;
	m_fZ = UI_DEPTH::HUD_Qyick_Slot;
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

void CHud_Quick_Slot::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);

	if (m_pSlotItem != nullptr)
		m_iSlotItem_Tex = ENUM_CLASS(m_pSlotItem->Item_Info()->iRarity);
	else
		m_iSlotItem_Tex = 0;
}

void CHud_Quick_Slot::Update(_float fTimeDelta)
{
	Item_Selete();

	if (m_pOldSlotItem != m_pSlotItem)
	{
		for (_int i = 0; i < 3; ++i)
		{
			static_cast<CTooltip*>(m_vecChildren[i])->Change_Item(m_pSlotItem);
		}
		m_pOldSlotItem = m_pSlotItem;

	}
	__super::Update(fTimeDelta);
}

void CHud_Quick_Slot::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);

	if (CUIObject::isPick(g_hWnd))
	{
		if (m_pSlotItem != nullptr)
		{

			m_vecChildren[3]->Late_Update(fTimeDelta);
			//ARTEFACT, STONE, POTION, SKILLBOOK
			switch (m_pSlotItem->Item_Info()->iItemType)
			{
			case 0:
				m_vecChildren[0]->Late_Update(fTimeDelta);
				break;
			case 1:
				m_vecChildren[1]->Late_Update(fTimeDelta);
				break;
			case 2:
				m_vecChildren[2]->Late_Update(fTimeDelta);
				break;
			case 3:
				m_vecChildren[0]->Late_Update(fTimeDelta);
				break;
			}
		}
		else
			m_vecChildren[3]->Late_Update(fTimeDelta);
	}


	if (m_pSlotItem != nullptr)
		m_pSlotItem->Late_Update(fTimeDelta, _float3{ m_pTransformCom->Get_State(STATE::POSITION).x, m_pTransformCom->Get_State(STATE::POSITION).y - 4, m_pTransformCom->Get_State(STATE::POSITION).z });

	if (m_bIsPick)
		m_pSlotItem->IsSelete();

}

HRESULT CHud_Quick_Slot::Render()
{
	if (FAILED(CButton::Bind_ButtonTex_Single(g_hWnd, m_iSlotItem_Tex)))
		return E_FAIL;

	return S_OK;
}

void CHud_Quick_Slot::Push_Item(CItemObject* pItem)
{
	m_pSlotItem = static_cast<CItem_Base*>(pItem);

	//static_cast<CGacha*>(m_pParent)->UI_Switch();
}

void CHud_Quick_Slot::Push_Item_ReRoll(CItemObject* pItem)
{
	if (pItem != nullptr)
		Safe_Release(m_pSlotItem);

	m_pSlotItem = static_cast<CItem_Base*>(pItem);
}

void CHud_Quick_Slot::Release_Item()
{
	if (m_pSlotItem != nullptr)
		Safe_Release(m_pSlotItem);
}

void CHud_Quick_Slot::Release_Pop()
{
	m_pSlotItem = nullptr;
}

void CHud_Quick_Slot::Item_Selete()
{
	if (Check_Key_Down(g_hWnd, VK_LBUTTON) && m_pSlotItem != nullptr)
	{
		m_pGameInstance->Pick_ItemSlot(m_pSlotItem, this, 1, 2);
		m_bIsPick = true;
	}
}

HRESULT CHud_Quick_Slot::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Rect_UI_Hud_Quick_Slot"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CHud_Quick_Slot::Ready_ChildPrototype(LEVEL eLevel)
{
	//if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_Gacha_Slot_Selete"),
	//	CGacha_Slot_Selete::Create(m_pGraphic_Device, eLevel))))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CHud_Quick_Slot::Ready_Children()
{
	CUIObject* pGameObject = nullptr;
//	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_Artefact_Tooltip")));
//	if (nullptr == pGameObject)
//		return E_FAIL;
//	Add_Child(pGameObject);


	return S_OK;
}

CHud_Quick_Slot* CHud_Quick_Slot::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
	CHud_Quick_Slot* pInstance = new CHud_Quick_Slot(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eLevel)))
	{
		MSG_BOX(TEXT("Failed to Created : CHud_Quick_Slot"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CHud_Quick_Slot::Clone(void* pArg)
{
	CHud_Quick_Slot* pInstance = new CHud_Quick_Slot(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CHud_Quick_Slot"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CHud_Quick_Slot::Free()
{
	__super::Free();
	Safe_Release(m_pSlotItem);
	Safe_Release(m_pOldSlotItem);
}

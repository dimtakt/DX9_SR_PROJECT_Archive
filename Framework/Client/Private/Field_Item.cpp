#include "Field_Item.h"
#include "GameInstance.h"
#include "Field_Item_Guide.h"
#include "Inventory.h"
#include "Stat_Manager.h"
CField_Item::CField_Item(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CField_Item::CField_Item(const CField_Item& Prototype) : CUIObject(Prototype), m_eLevel(Prototype.m_eLevel)
{
}

void CField_Item::Render_Field_Item(_float fTimeDelta)
{
	if (m_bDead)
		return;

	m_fAccTime += fTimeDelta;
	_float Offset = sinf(m_fAccTime * m_fSpeed) * m_fRange;
	m_vTargetPos.y = m_vWorldPos.y + Offset;
	m_pTransformCom->Set_State(STATE::POSITION, m_vTargetPos);
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);

}

void CField_Item::Buy_Item()
{
	if (CStat_Manager::GetInstance()->Get_CurStats()[ENUM_CLASS(STAT_INFO::GOLD)] >= m_iGold)
	{
		CStat_Manager::GetInstance()->Cal_Stats(STAT_INFO::GOLD, -m_iGold);
		static_cast<CInventory*>(m_pGameInstance->Find_UIObj(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("UI_Inven")))->Add_Item_Inven(m_iItemID);
		m_pGameInstance->StopSound(ENUM_CLASS(CHANNELID::SOUND_UI));
		m_pGameInstance->PlaySoundW(L"shopBuy.wav", ENUM_CLASS(CHANNELID::SOUND_UI), g_fUIVolume - 0.6f);

		m_bDead = true;
	}
}

HRESULT CField_Item::Initialize_Prototype(LEVEL eLevel)
{
	m_eLevel = eLevel;

	if (FAILED(Ready_ChildPrototype(eLevel)))
		return E_FAIL;

	return S_OK;
}

HRESULT CField_Item::Initialize(void* pArg)
{
	FIELD_ITEM_DESC* Desc = static_cast<FIELD_ITEM_DESC*>(pArg);

	m_iGold = Desc->m_iGold;
	m_fSizeX = 1.f;
	m_fSizeY = 1.f;
	m_fX = 0;
	m_fY = 2.f;
	m_fZ = UI_DEPTH::FILED_ITEM;
	m_iWinSizeX = g_iWinSizeX;
	m_iWinSizeY = g_iWinSizeY;
	
	m_fRange = 0.2f;
	m_fSpeed = m_pGameInstance->Rand(1.0, 1.5);

	m_iItemID = Desc->m_iItemID;
	m_iItemTex = g_ItemDataBase[m_iItemID].m_iItemTextureID;
	m_vTargetPos = Desc->m_vTargetPos;
	m_vTargetPos.y += m_fY; 

	m_eObjType = GAMEOBJ_TYPE::SHOP_ITEM;

	m_vWorldPos = m_vTargetPos;
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);

	if (FAILED(Ready_Children()))
		return E_FAIL;

	return S_OK;
}

void CField_Item::Priority_Update(_float fTimeDelta)
{

}

void CField_Item::Update(_float fTimeDelta)
{
	
}

void CField_Item::Late_Update(_float fTimeDelta)
{
}

HRESULT CField_Item::Render()
{
	m_pTransformCom->Bind_Matrix();
	if (FAILED(m_pTextureCom->Bind_Texture(m_iItemTex)))
		return E_FAIL;
	m_pVIBufferCom->Bind_Buffers();
	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CField_Item::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Item"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CField_Item::Ready_ChildPrototype(LEVEL eLevel)
{
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_Field_Item_Guide"),
		CField_Item_Guide::Create(m_pGraphic_Device, eLevel))))
		return E_FAIL;

	return S_OK;
}

HRESULT CField_Item::Ready_Children()
{
	CUIObject* pGameObject = nullptr;

	UIOBJECT_DESC Desc{};

	Desc.fZ = m_iItemID;
	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Field_Item_Guide"), &Desc));
	if (nullptr == pGameObject)
		return E_FAIL;
	Add_Child(pGameObject);

	return S_OK;
}

void CField_Item::OnCollision(CGameObject* pGameObject)
{
}

void CField_Item::RenderFont()
{
	__super::Late_Update(0.15f);
}

CField_Item* CField_Item::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
	CField_Item* pInstance = new CField_Item(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eLevel)))
	{
		MSG_BOX(TEXT("Failed to Created : CField_Item"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CField_Item::Clone(void* pArg)
{
	CField_Item* pInstance = new CField_Item(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CField_Item"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CField_Item::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}

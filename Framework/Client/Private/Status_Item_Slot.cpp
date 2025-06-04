#include "Status_Item_Slot.h"
#include "GameInstance.h"
#include "Status_Item_Icon.h"
CStatus_Item_Slot::CStatus_Item_Slot(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CStatus_Item_Slot::CStatus_Item_Slot(const CStatus_Item_Slot& Prototype) : CUIObject(Prototype), m_eLevel{ Prototype.m_eLevel }
{
}

HRESULT CStatus_Item_Slot::Initialize_Prototype(LEVEL eLevel)
{
	m_eLevel = eLevel;

	if (FAILED(Ready_ChildPrototype(eLevel)))
		return E_FAIL;

	return S_OK;
}

HRESULT CStatus_Item_Slot::Initialize(void* pArg)
{
	m_fSizeX = 64;
	m_fSizeY = 64;
	m_fX = -100;
	m_fY = 62;
	m_fZ = UI_DEPTH::PLAYER_STAUTS;
	m_iWinSizeX = g_iWinSizeX;
	m_iWinSizeY = g_iWinSizeY;

	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
	__super::Update_Position();

	if (FAILED(Ready_Children()))
		return E_FAIL;

	return S_OK;
}

void CStatus_Item_Slot::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CStatus_Item_Slot::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CStatus_Item_Slot::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
	__super::Late_Update(fTimeDelta);
}

HRESULT CStatus_Item_Slot::Render()
{
	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;
	m_pVIBufferCom->Bind_Buffers();

	__super::Begin();
	m_pVIBufferCom->Render();
	__super::End();
	return S_OK;
}

HRESULT CStatus_Item_Slot::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Rect_Status_Slot"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CStatus_Item_Slot::Ready_ChildPrototype(LEVEL eLevel)
{
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_Status_Item_Icon"),
		CStatus_Item_Icon::Create(m_pGraphic_Device, eLevel))))
		return E_FAIL;
}

HRESULT CStatus_Item_Slot::Ready_Children()
{
	CUIObject* pGameObject = nullptr;

	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Status_Item_Icon")));
	if (nullptr == pGameObject)
		return E_FAIL;
	Add_Child(pGameObject);

	return S_OK;
}

CStatus_Item_Slot* CStatus_Item_Slot::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
	CStatus_Item_Slot* pInstance = new CStatus_Item_Slot(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eLevel)))
	{
		MSG_BOX(TEXT("Failed to Create : CStatus_Item_Slot"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CStatus_Item_Slot::Clone(void* pArg)
{
	CStatus_Item_Slot* pInstance = new CStatus_Item_Slot(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CStatus_Item_Slot"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CStatus_Item_Slot::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}

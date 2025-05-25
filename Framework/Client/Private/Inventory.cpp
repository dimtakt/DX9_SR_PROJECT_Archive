#include "Inventory.h"
#include "GameInstance.h"

CInventory::CInventory(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CInventory::CInventory(const CInventory& Prototype) : CUIObject(Prototype), m_eLevel(Prototype.m_eLevel), m_bIsOpen(false)
{
}

HRESULT CInventory::Initialize_Prototype(LEVEL eLevel)
{
	m_eLevel = eLevel;

	if (FAILED(Ready_ChildPrototype(eLevel)))
		return E_FAIL;

	return S_OK;
}

HRESULT CInventory::Initialize(void* pArg)
{
	UIOBJECT_DESC Desc{};

	Desc.fSizeX = 200.f;
	Desc.fSizeY = 300.f;
	Desc.fX = g_iWinSizeX * 0.5;
	Desc.fY = g_iWinSizeY * 0.5;
	Desc.fZ = 0.f;
	Desc.iWinSizeX = g_iWinSizeX;
	Desc.iWinSizeY = g_iWinSizeY;

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
	__super::Update_Position();

	if (FAILED(Ready_Children()))
		return E_FAIL;

	return S_OK;
}

void CInventory::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CInventory::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CInventory::Late_Update(_float fTimeDelta)
{
	if (m_bIsOpen == true)
	{
		m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
		__super::Late_Update(fTimeDelta);
	}
}

HRESULT CInventory::Render()
{
	m_pTransformCom->Bind_Matrix();
	m_pVIBufferCom->Bind_Buffers();
	__super::Begin();
	m_pVIBufferCom->Render();
	__super::End();

	return S_OK;
}

void CInventory::UI_Switch()
{
	if (m_bIsOpen)
		m_bIsOpen = false;
	else
		m_bIsOpen = true;

}

HRESULT CInventory::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Rect_UI_BalckRect_Texture"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CInventory::Ready_ChildPrototype(LEVEL eLevel)
{
	return S_OK;
}

HRESULT CInventory::Ready_Children()
{
	return S_OK;
}

CInventory* CInventory::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
	CInventory* pInstance = new CInventory(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eLevel)))
	{
		MSG_BOX(TEXT("Failed to Created : CInventory"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CInventory::Clone(void* pArg)
{
	CInventory* pInstance = new CInventory(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CInventory"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CInventory::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
}

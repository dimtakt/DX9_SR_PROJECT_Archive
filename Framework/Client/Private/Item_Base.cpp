#include "Item_Base.h"
#include "GameInstance.h"
CItem_Base::CItem_Base(LPDIRECT3DDEVICE9 pGraphic_Device) : CItemObject(pGraphic_Device)
{
}

CItem_Base::CItem_Base(const CItem_Base& Prototype) : CItemObject(Prototype)
{
}

HRESULT CItem_Base::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CItem_Base::Initialize(void* pArg)
{
	CItemObject::ITEMOBJECT_DESC* Desc = static_cast<ITEMOBJECT_DESC*>(pArg);

	m_iItemID = Desc->iItemID;
	m_iItemType = Desc->iItemType;
	m_iRarity = Desc->iRarity;
	m_szName = Desc->szName;
	m_szDescription = Desc->szDescription;

	if (FAILED(Ready_Componet(Desc->iItemTextureID)))
		return E_FAIL;

	return S_OK;
}

void CItem_Base::Priority_Update(_float fTimeDelta)
{
}

void CItem_Base::Update(_float fTimeDelta)
{
}

void CItem_Base::Late_Update(_float fTimeDelta)
{
}

HRESULT CItem_Base::Render()
{
	SetUp_RenderState();

	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;
	m_pVIBufferCom->Bind_Buffers();

	__super::Begin();
	m_pVIBufferCom->Render();
	__super::End();

	Reset_RenderState();
	return S_OK;

}

HRESULT CItem_Base::Ready_Componet(_uint iTextureIndex)
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Item"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	return S_OK;
}

void CItem_Base::SetUp_RenderState()
{
}

void CItem_Base::Reset_RenderState()
{
}

CItem_Base* CItem_Base::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CItem_Base* pInstance = new CItem_Base(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CItem_Base"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CItem_Base::Clone(void* pArg)
{
	CItem_Base* pInstance = new CItem_Base(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CItem_Base"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CItem_Base::Free()
{
	__super::Free();
}

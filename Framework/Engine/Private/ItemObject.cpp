#include "ItemObject.h"
#include "GameInstance.h"

CItemObject::CItemObject(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CItemObject::CItemObject(const CItemObject& Prototype) : CUIObject(Prototype), m_iItemID{ Prototype.m_iItemID }, m_iItemType{ Prototype.m_iItemType }, 
m_iRarity{ Prototype.m_iRarity }, m_szName{ Prototype.m_szName }, m_szDescription{ Prototype.m_szDescription },
m_iItemTextureID(Prototype.m_iItemTextureID), m_iItemValue(Prototype.m_iItemValue), m_fAngle(Prototype.m_fAngle)
{
}
HRESULT CItemObject::Initialize(void* pArg)
{
	m_pDesc = new ITEMOBJECT_DESC;

	m_pDesc->iItemID = m_iItemID;
	m_pDesc->iItemTextureID = m_iItemTextureID;
	m_pDesc->iItemType = m_iItemType;
	m_pDesc->iRarity = m_iRarity;
	m_pDesc->szName = m_szName;
	m_pDesc->szDescription = m_szDescription;
	m_pDesc->iItemValue = m_iItemValue;
	m_pDesc->fAngle = m_fAngle;
	m_pDesc->iArtefact_Value = m_iArtefact_Value;

	return S_OK;
}

HRESULT CItemObject::Initialize_Prototype()
{
	return S_OK;
}

void CItemObject::Priority_Update(_float fTimeDelta)
{
}

void CItemObject::Update(_float fTimeDelta)
{
}

void CItemObject::Late_Update(_float fTimeDelta)
{
}

HRESULT CItemObject::Render()
{
	return S_OK;
}

CItemObject::tagItemObjectDesc*  CItemObject::Item_Info()
{
	delete m_pDesc;
	m_pDesc = nullptr;
	m_pDesc = new ITEMOBJECT_DESC;

	m_pDesc->iItemID = m_iItemID;
	m_pDesc->iItemTextureID = m_iItemTextureID;
	m_pDesc->iItemType = m_iItemType;
	m_pDesc->iRarity = m_iRarity;
	m_pDesc->szName = m_szName;
	m_pDesc->szDescription = m_szDescription;
	m_pDesc->iItemValue = m_iItemValue;
	m_pDesc->fAngle = m_fAngle;
	m_pDesc->iArtefact_Value = m_iArtefact_Value;
	return m_pDesc;
}

void CItemObject::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	delete m_pDesc;
	m_pDesc = nullptr;
}

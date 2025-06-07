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

CItemObject::tagItemObjectDesc* CItemObject::Item_Info()
{
	tagItemObjectDesc Desc{};

	Desc.iItemID = m_iItemID;
	Desc.iItemTextureID = m_iItemTextureID;
	Desc.iItemType = m_iItemType;
	Desc.iRarity = m_iRarity;
	Desc.szName = m_szName;
	Desc.szDescription = m_szDescription;
	Desc.iItemValue = m_iItemValue;
	Desc.fAngle = m_fAngle;
	Desc.iArtefact_Value = m_iArtefact_Value;
	return &Desc;
}

void CItemObject::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);

}

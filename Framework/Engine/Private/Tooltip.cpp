#include "Tooltip.h"
#include "GameInstance.h"

CTooltip::CTooltip(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CTooltip::CTooltip(const CTooltip& Prototype) : CUIObject(Prototype)
{
}

HRESULT CTooltip::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;
	return S_OK;
}

HRESULT CTooltip::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	return S_OK;
}

void CTooltip::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CTooltip::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CTooltip::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CTooltip::Render()
{
	return S_OK;
}

void CTooltip::Change_Item(CItemObject* pItem)
{
	if (pItem != nullptr)
		m_pItemObject = pItem;
	else
		m_pItemObject = nullptr;
}

void CTooltip::Free()
{
	__super::Free();
	m_pItemObject = nullptr;
}

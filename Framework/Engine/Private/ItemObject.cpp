#include "ItemObject.h"

CItemObject::CItemObject(LPDIRECT3DDEVICE9 pGraphic_Device) : CGameObject(pGraphic_Device)
{
}

CItemObject::CItemObject(const CItemObject& Prototype) : CGameObject(Prototype)
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

void CItemObject::Free()
{
	__super::Free();
}

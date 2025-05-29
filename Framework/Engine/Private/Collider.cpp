#include "Collider.h"

CCollider::CCollider(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent { pGraphic_Device }
{
}

CCollider::CCollider(const CCollider& Prototype)
	: CComponent { Prototype }
{
}

HRESULT CCollider::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCollider::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CCollider::Render()
{
	return S_OK;
}

void CCollider::Update_Collider(const CTransform* pTransform)
{
}

void CCollider::Free()
{
	__super::Free();
	m_pOwner = nullptr;
}

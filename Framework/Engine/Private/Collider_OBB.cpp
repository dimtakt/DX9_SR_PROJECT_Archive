// CCollider_OBB.cpp
#include "Collider_OBB.h"
#include "VIBuffer_Cube.h"
#include "GameInstance.h"

USING(Engine)

CCollider_OBB::CCollider_OBB(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCollider(pGraphic_Device)
{
}

CCollider_OBB::CCollider_OBB(const CCollider_OBB& Prototype)
	: CCollider(Prototype)
{
	// 복사 생성 시 VIBuffer도 클론 필요 (필요 시 구현)
}

HRESULT CCollider_OBB::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCollider_OBB::Initialize(void* pArg)
{
	OBB_DESC* desc = static_cast<OBB_DESC*>(pArg);
	if (desc == nullptr)
		return E_FAIL;

	m_pOwner = desc->pOwner;
	m_pTransformRef = desc->pTransform;
	m_vScale = desc->vScale;

	return S_OK;
}

void CCollider_OBB::Update_Collider()
{
	if (!m_pTransformRef)
		return;

	// 중심 좌표
	m_vWorldCenter = m_pTransformRef->Get_State(STATE::POSITION);

	// 축 방향 (스케일 반영된 상태)
	m_vAxis[0] = m_pTransformRef->Get_State(STATE::RIGHT); // X
	m_vAxis[1] = m_pTransformRef->Get_State(STATE::LOOK);  // Z
	m_vAxis[2] = m_pTransformRef->Get_State(STATE::UP);    // Y

	// 각 축의 반길이
	m_vWorldExtents.x = D3DXVec3Length(&m_vAxis[0]) * m_vScale.x * 0.5f;
	m_vWorldExtents.z = D3DXVec3Length(&m_vAxis[1]) * m_vScale.y * 0.5f;
	m_vWorldExtents.y = D3DXVec3Length(&m_vAxis[2]) * m_vScale.z * 0.5f;

	// 방향 벡터 정규화 (SAT 충돌 판정 시 사용)
	D3DXVec3Normalize(&m_vAxis[0], &m_vAxis[0]);
	D3DXVec3Normalize(&m_vAxis[1], &m_vAxis[1]);
	D3DXVec3Normalize(&m_vAxis[2], &m_vAxis[2]);
}

//void CCollider_OBB::Get_MatrixData(_float3& vCenter, _float3& vExtent, _float3* vAxis)
//{
//	vCenter = m_vWorldCenter;
//	vExtent = m_vWorldExtents;
//	memcpy(vAxis, m_vAxis, sizeof(_float3) * 3);
//}

HRESULT CCollider_OBB::Render()
{
	return S_OK;
}

CCollider_OBB* CCollider_OBB::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCollider_OBB* pInstance = new CCollider_OBB(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CCollider_OBB"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CCollider_OBB::Clone(void* pArg)
{
	CCollider_OBB* pInstance = new CCollider_OBB(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("CCollider_OBB Clone Failed"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCollider_OBB::Free()
{
	__super::Free();
	m_pTransformRef = nullptr;
}

// CollisionManager.cpp
#include "Collision_Manager.h"
#include "Collider.h"
#include "Collider_OBB.h"
#include "GameObject.h"

USING(Engine)

HRESULT CCollision_Manager::Add_OBB_Collider(CCollider_OBB* pCollider)
{
	if (pCollider == nullptr)
		return E_FAIL;
    m_vColliders.push_back(pCollider);
    Safe_AddRef(pCollider);

    return S_OK;
}

void CCollision_Manager::Clear_Colliders()
{
    for (size_t i = 0; i < m_vColliders.size(); ++i)
    {
        if (m_vColliders[i]->Get_Owner()->Get_ObjType() != GAMEOBJ_TYPE::PLAYER)
        {
            Safe_Release(m_vColliders[i]);
        }
        
    }
}

void CCollision_Manager::Check_RoomCollisions()
{
    for (size_t i = 0; i < m_vColliders.size(); ++i)
    {
        for (size_t j = i + 1; j < m_vColliders.size(); ++j)
        {
            if (Check_OBBtoOBB(m_vColliders[i], m_vColliders[j]))
            {
                m_vColliders[i]->Get_Owner()->OnCollision(m_vColliders[j]->Get_Owner());
                m_vColliders[j]->Get_Owner()->OnCollision(m_vColliders[i]->Get_Owner());
            }
        }
    }
}

bool CCollision_Manager::Check_OBBtoOBB(CCollider_OBB* pColliderA, CCollider_OBB* pColliderB)
{
    _float3 vCenterA, vExtentA, vAxisA[3];
    pColliderA->Get_MatrixData(vCenterA, vExtentA, vAxisA);

    _float3 vCenterB, vExtentB, vAxisB[3];
    pColliderB->Get_MatrixData(vCenterB, vExtentB, vAxisB);

    _float3 vToCenter = vCenterB - vCenterA;
    const _float fEpsilon = 1e-5f;

    // A√‡
    for (int k = 0; k < 3; ++k)
    {
        _float rA =
            vExtentA.x * fabsf(D3DXVec3Dot(&vAxisA[k], &vAxisA[0])) +
            vExtentA.y * fabsf(D3DXVec3Dot(&vAxisA[k], &vAxisA[1])) +
            vExtentA.z * fabsf(D3DXVec3Dot(&vAxisA[k], &vAxisA[2]));

        _float rB =
            vExtentB.x * fabsf(D3DXVec3Dot(&vAxisA[k], &vAxisB[0])) +
            vExtentB.y * fabsf(D3DXVec3Dot(&vAxisA[k], &vAxisB[1])) +
            vExtentB.z * fabsf(D3DXVec3Dot(&vAxisA[k], &vAxisB[2]));

        _float dist = fabsf(D3DXVec3Dot(&vToCenter, &vAxisA[k]));
        if (dist > rA + rB + fEpsilon)
            return false;
    }

    // B√‡
    for (int k = 0; k < 3; ++k)
    {
        _float rA =
            vExtentA.x * fabsf(D3DXVec3Dot(&vAxisB[k], &vAxisA[0])) +
            vExtentA.y * fabsf(D3DXVec3Dot(&vAxisB[k], &vAxisA[1])) +
            vExtentA.z * fabsf(D3DXVec3Dot(&vAxisB[k], &vAxisA[2]));

        _float rB =
            vExtentB.x * fabsf(D3DXVec3Dot(&vAxisB[k], &vAxisB[0])) +
            vExtentB.y * fabsf(D3DXVec3Dot(&vAxisB[k], &vAxisB[1])) +
            vExtentB.z * fabsf(D3DXVec3Dot(&vAxisB[k], &vAxisB[2]));

        _float dist = fabsf(D3DXVec3Dot(&vToCenter, &vAxisB[k]));
        if (dist > rA + rB + fEpsilon)
            return false;
    }

    return true;
}

CCollision_Manager* CCollision_Manager::Create()
{
	return new CCollision_Manager();
}

void CCollision_Manager::Free()
{
	__super::Free();
    for (size_t i = 0; i < m_vColliders.size(); ++i)
    {
        Safe_Release(m_vColliders[i]);
    }
    m_vColliders.clear();
}

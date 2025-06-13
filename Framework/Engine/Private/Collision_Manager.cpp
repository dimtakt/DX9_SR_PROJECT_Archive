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

    return S_OK;
}

void CCollision_Manager::Clear_Colliders()
{
    for (size_t i = 0; i < m_vColliders.size(); ++i)
    {
        if (m_vColliders[i] == nullptr)
            Safe_Release(m_vColliders[i]);
        if (m_vColliders[i]->Get_Owner() == nullptr)
            Safe_Release(m_vColliders[i]);
        else if (m_vColliders[i]->Get_Owner()->Get_IsDead())
            Safe_Release(m_vColliders[i]);
        else if (!m_vColliders[i]->Get_Owner()->Get_IsActive())
            Safe_Release(m_vColliders[i]);
    }
}

void CCollision_Manager::Check_RoomCollisions()
{

    for (auto it = m_vColliders.begin(); it != m_vColliders.end();) {
        if (*it != nullptr) {
            if ((*it)->Get_Owner() == nullptr || (*it)->Get_IsDead() || (*it)->Get_Owner()->Get_IsDead())
            {
                Safe_Release(*it);
                it = m_vColliders.erase(it);
            }
            else {
                it++;
            }
        }
        else {
            Safe_Release(*it);
            it = m_vColliders.erase(it);
        }
    }

    for (size_t i = 0; i < m_vColliders.size(); ++i)
    {   
        for (size_t j = i + 1; j < m_vColliders.size(); ++j)
        {  
            if (m_vColliders[i] == nullptr || 
                m_vColliders[j] == nullptr ||
                m_vColliders[i]->Get_Owner() == nullptr ||
                m_vColliders[j]->Get_Owner() == nullptr)
                continue;

            if (m_vColliders[i]->Get_Owner()->Get_ObjType() == m_vColliders[j]->Get_Owner()->Get_ObjType())
                continue;
            if (m_vColliders[i]->Get_Owner()->Get_ObjType() == GAMEOBJ_TYPE::PLAYER)
            {
                if (m_vColliders[j]->Get_Owner()->Get_ObjType() == GAMEOBJ_TYPE::PLAYER_EFFECT)
                    continue;
            }

            if (m_vColliders[j]->Get_Owner()->Get_ObjType() == GAMEOBJ_TYPE::PLAYER_EFFECT)
            {
                if (m_vColliders[i]->Get_Owner()->Get_ObjType() == GAMEOBJ_TYPE::PLAYER)
                    continue;
            }

            if (m_vColliders[i]->Get_Owner()->Get_ObjType() == GAMEOBJ_TYPE::MONSTER)
            {
                if (m_vColliders[j]->Get_Owner()->Get_ObjType() == GAMEOBJ_TYPE::MONSTER_EFFECT)
                    continue;
            }

            if (m_vColliders[j]->Get_Owner()->Get_ObjType() == GAMEOBJ_TYPE::MONSTER_EFFECT)
            {
                if (m_vColliders[i]->Get_Owner()->Get_ObjType() == GAMEOBJ_TYPE::MONSTER)
                    continue;
            }

           
            if (m_vColliders[i]->Get_Owner()->Get_ObjType() == GAMEOBJ_TYPE::PLAYER)
            {
                if (m_vColliders[j]->Get_Owner()->Get_ObjType() == GAMEOBJ_TYPE::MONSTER_EFFECT)
                    int a = 1;
            }

            /*if (Check_OBBtoOBB(m_vColliders[i], m_vColliders[j]) && Check_Y_Overlap(m_vColliders[i], m_vColliders[j]))
            {
                    m_vColliders[i]->Get_Owner()->OnCollision(m_vColliders[j]->Get_Owner());
                    m_vColliders[j]->Get_Owner()->OnCollision(m_vColliders[i]->Get_Owner());
            }*/

            if (Check_3DOBBto3DOBB(m_vColliders[i], m_vColliders[j]) && Check_Y_Overlap(m_vColliders[i], m_vColliders[j]))
            {                
                m_vColliders[i]->Get_Owner()->OnCollision(m_vColliders[j]->Get_Owner());
                m_vColliders[j]->Get_Owner()->OnCollision(m_vColliders[i]->Get_Owner());
            }
            else
            {
                m_vColliders[i]->Get_Owner()->OffCollision(m_vColliders[j]->Get_Owner());
                m_vColliders[j]->Get_Owner()->OffCollision(m_vColliders[i]->Get_Owner());
            }
        }
    }
}

bool CCollision_Manager::Check_OBBtoOBB(CCollider_OBB* pA, CCollider_OBB* pB)
{
    const _float3& vCenterA = pA->Get_Center();
    const _float3* pAxisA = pA->Get_Axis();
    const _float3& vExtentA = pA->Get_Extents();

    const _float3& vCenterB = pB->Get_Center();
    const _float3* pAxisB = pB->Get_Axis();
    const _float3& vExtentB = pB->Get_Extents();

    _float3 vToCenter = vCenterB - vCenterA;

    _float3 vAxisTest[4] = {
        pAxisA[0], pAxisA[2],
        pAxisB[0], pAxisB[2]
    };

    for (size_t i = 0; i < 4; ++i)
    {
        const _float3& axis = vAxisTest[i];

        float projA = fabsf(D3DXVec3Dot(&axis, &pAxisA[0]) * vExtentA.x) + fabsf(D3DXVec3Dot(&axis, &pAxisA[2]) * vExtentA.z);
        float projB = fabsf(D3DXVec3Dot(&axis, &pAxisB[0]) * vExtentB.x) + fabsf(D3DXVec3Dot(&axis, &pAxisB[2]) * vExtentB.z);

        float dist = fabsf(D3DXVec3Dot(&axis, &vToCenter));

        if (dist > projA + projB)
            return false;
    }

    

    return true;
}

_bool CCollision_Manager::Check_3DOBBto3DOBB(CCollider_OBB* pA, CCollider_OBB* pB)
{
    const _float3& vCenterA = pA->Get_Center();
    const _float3* pAxisA = pA->Get_Axis();   // RIGHT, UP, LOOK
    const _float3& vExtentA = pA->Get_Extents();

    const _float3& vCenterB = pB->Get_Center();
    const _float3* pAxisB = pB->Get_Axis();
    const _float3& vExtentB = pB->Get_Extents();

    _float3 vToCenter = vCenterB - vCenterA;

    _float3 vAxisTest[15];
    int iTest = 0;

    // A의 축 3개
    vAxisTest[iTest++] = pAxisA[0];
    vAxisTest[iTest++] = pAxisA[1];
    vAxisTest[iTest++] = pAxisA[2];

    // B의 축 3개
    vAxisTest[iTest++] = pAxisB[0];
    vAxisTest[iTest++] = pAxisB[1];
    vAxisTest[iTest++] = pAxisB[2];

    // A × B 조합
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            _float3 crossAxis;
            D3DXVec3Cross(&crossAxis, &pAxisA[i], &pAxisB[j]);
            if (D3DXVec3LengthSq(&crossAxis) > 0.0001f)
            {
                D3DXVec3Normalize(&vAxisTest[iTest], &crossAxis);
                ++iTest;
            }
        }
    }

    for (int i = 0; i < iTest; ++i)
    {
        const _float3& axis = vAxisTest[i];

        float projA =
            fabsf(D3DXVec3Dot(&axis, &pAxisA[0]) * vExtentA.x) +
            fabsf(D3DXVec3Dot(&axis, &pAxisA[1]) * vExtentA.y) +
            fabsf(D3DXVec3Dot(&axis, &pAxisA[2]) * vExtentA.z);

        float projB =
            fabsf(D3DXVec3Dot(&axis, &pAxisB[0]) * vExtentB.x) +
            fabsf(D3DXVec3Dot(&axis, &pAxisB[1]) * vExtentB.y) +
            fabsf(D3DXVec3Dot(&axis, &pAxisB[2]) * vExtentB.z);

        float dist = fabsf(D3DXVec3Dot(&axis, &vToCenter));

        if (dist > projA + projB)
            return false;
    }

    return true;
}

_bool CCollision_Manager::Check_Y_Overlap(CCollider_OBB* pA, CCollider_OBB* pB)
{
    const _float3& vCenterA = pA->Get_Center();
    const _float3& vExtentA = pA->Get_Extents();
    const _float3& vCenterB = pB->Get_Center();
    const _float3& vExtentB = pB->Get_Extents();

    float minA = vCenterA.y - vExtentA.y;
    float maxA = vCenterA.y + vExtentA.y;
    float minB = vCenterB.y - vExtentB.y;
    float maxB = vCenterB.y + vExtentB.y;

    return !(maxA < minB || maxB < minA);
}


void CCollision_Manager::Render()
{
    for (auto& Col : m_vColliders)
    {
        if(nullptr != Col)
            Col->Render();
    }
}

void CCollision_Manager::Update()
{
    for (auto it = m_vColliders.begin(); it != m_vColliders.end();) {
        if (*it != nullptr) {
            if ((*it)->Get_IsDead() || (*it)->Get_Owner()->Get_IsDead())
            {
                Safe_Release(*it);
                it = m_vColliders.erase(it);
            }
            else
            {
                (*it)->Update_Collider();
                ++it;
            }
        }
        else {
            Safe_Release(*it);
            it = m_vColliders.erase(it);
        }
    }
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

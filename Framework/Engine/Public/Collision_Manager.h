#pragma once

#include "Base.h"
#include "Collider_OBB.h"
BEGIN(Engine)

class CCollision_Manager final : public CBase
{
private:
	CCollision_Manager() = default;
	virtual ~CCollision_Manager() = default;

public:
	HRESULT Add_OBB_Collider(CCollider_OBB* pCollider);
	void Clear_Colliders();
	void Check_RoomCollisions();

private:
	_bool Check_OBBtoOBB(CCollider_OBB* pA, CCollider_OBB* pB);
	_bool Check_Y_Overlap(CCollider_OBB* pA, CCollider_OBB* pB);

private:
	std::vector<CCollider_OBB*> m_vColliders;
public:
	static CCollision_Manager* Create();
	virtual void Free() override;
};

END

#pragma once

#include "Base.h"
BEGIN(Engine)

class CCollision_Manager final : public CBase
{
private:
	CCollision_Manager() = default;
	virtual ~CCollision_Manager() = default;

public:
	HRESULT Add_Collider(class CCollider* pCollider);
	void Clear_Colliders();
	void Check_AllCollisions();

private:
	bool Check_OBBtoOBB(class CCollider_OBB* pA, class CCollider_OBB* pB);

private:
	vector<CCollider*> m_Colliders[ENUM_CLASS(COLLIDER_OBJ_TYPE::COL_END)];

public:
	static CCollision_Manager* Create();
	virtual void Free() override;
};

END

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
	_bool Check_3DOBBto3DOBB(CCollider_OBB* pA, CCollider_OBB* pB);
	_bool Check_Y_Overlap(CCollider_OBB* pA, CCollider_OBB* pB);

public:
	void Remove_Collider(CCollider_OBB* pTarget);

public:
	void Render();
	void Update();

public:
	_bool	Get_IsRender() { return m_bIsRender; }
	void	Set_IsRender() { m_bIsRender = !m_bIsRender; }
private:
	vector<CCollider_OBB*>	m_vColliders = {};
	_bool					m_bIsRender = { false };
public:
	static CCollision_Manager* Create();
	virtual void Free() override;
};

END

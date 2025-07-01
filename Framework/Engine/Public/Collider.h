#pragma once

#include "Component.h"
#include "Transform.h"
#include "GameObject.h"
BEGIN(Engine)

class ENGINE_DLL CCollider abstract : public CComponent
{
protected:
	CCollider(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCollider(const CCollider& Prototype);
	virtual ~CCollider() = default;

public:
	HRESULT Initialize_Prototype();
	HRESULT Initialize(void* pArg);
	virtual HRESULT Render();
public:
	virtual void Update_Collider();
	void Set_Owner(CGameObject* pOwner) {
		m_pOwner = pOwner;
	}
	CGameObject* Get_Owner() {
		return m_pOwner;
	}
protected:
	CGameObject* m_pOwner = { nullptr };
public:
	virtual CComponent* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END
#pragma once
#include "Monster.h"
#include "Client_Defines.h"

BEGIN(Client)

class CMole_A final : public CMonster
{
private:
	CMole_A(LPDIRECT3DDEVICE9 pGraphic_Device);
	CMole_A(const CMole_A& Prototype);
	virtual ~CMole_A() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Components(void* pArg);

public:
	virtual void OnCollision(CGameObject* pGameObject) override;

private:
	CTexture*	m_pTextureCom_Idle			= { nullptr }; 
	CTexture*	m_pTextureCom_Move			= { nullptr }; 
	CTexture*	m_pTextureCom_Down			= { nullptr }; 
	CTexture*	m_pTextureCom_Attack		= { nullptr }; 
	CTexture*	m_pTextureCom_Airborne		= { nullptr }; 

	CAnimator*	m_pAnimatorCom				= { nullptr };

	_bool		m_isFlippedX				= false;

public:
	static CMole_A* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
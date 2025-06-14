#pragma once
#include "Monster.h"
#include "Client_Defines.h"
#include "Field_Hp.h"
#include "AttackFx.h"
BEGIN(Client)

class COink_A final : public CMonster
{
private:
	COink_A(LPDIRECT3DDEVICE9 pGraphic_Device);
	COink_A(const COink_A& Prototype);
	virtual ~COink_A() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Components(void* pArg);
	HRESULT Ready_Object();

public:
	virtual void OnCollision(CGameObject* pGameObject) override;

private:
	CTexture* m_pTextureCom_Idle				= { nullptr };
	CTexture* m_pTextureCom_Move				= { nullptr };
	CTexture* m_pTextureCom_Attack				= { nullptr };
	CTexture* m_pTextureCom_ChargeReady			= { nullptr };
	CTexture* m_pTextureCom_ChargeReady_Cycle	= { nullptr };
	CTexture* m_pTextureCom_Charge_Cycle		= { nullptr };
	CTexture* m_pTextureCom_Charge_Airborne		= { nullptr };
	CTexture* m_pTextureCom_Charge_Down			= { nullptr };
	CTexture* m_pTextureCom_Charge_End			= { nullptr };

	CAnimator* m_pAnimatorCom					= { nullptr };


	_bool		m_isFlippedX					= false;
	_int		m_iAtkCooldownFrames			= 0;

	_int		AttackDaley = {};
	CAttackFx*	m_pAttackFx = { nullptr };

public:
	static COink_A* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
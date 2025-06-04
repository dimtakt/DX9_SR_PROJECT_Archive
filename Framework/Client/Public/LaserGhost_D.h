#pragma once
#include "Monster.h"
#include "Client_Defines.h"

BEGIN(Client)

class CLaserGhost_D final : public CMonster
{
private:
	CLaserGhost_D(LPDIRECT3DDEVICE9 pGraphic_Device);
	CLaserGhost_D(const CLaserGhost_D& Prototype);
	virtual ~CLaserGhost_D() = default;

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
	CTexture* m_pTextureCom_Idle			= { nullptr };
	CTexture* m_pTextureCom_Move			= { nullptr };
	CTexture* m_pTextureCom_Down			= { nullptr };
	CTexture* m_pTextureCom_AttackReady		= { nullptr };
	CTexture* m_pTextureCom_Attack_Start	= { nullptr };
	CTexture* m_pTextureCom_Attack_Cycle	= { nullptr };
	CTexture* m_pTextureCom_Attack_End		= { nullptr };
	CTexture* m_pTextureCom_Airborne		= { nullptr };

	CAnimator* m_pAnimatorCom = { nullptr };

	_bool		m_isFlippedX = false;
	_bool		m_isRunOut = false;
	_float3		m_vLockedOnPos = {};

	//_int		m_iAtkCooldownFrames = 0;

public:
	static CLaserGhost_D* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
#pragma once
#include "Monster.h"
#include "Client_Defines.h"


BEGIN(Client)

class CErma_Hand_R final : public CMonster
{
private:
	CErma_Hand_R(LPDIRECT3DDEVICE9 pGraphic_Device);
	CErma_Hand_R(const CErma_Hand_R& Prototype);
	virtual ~CErma_Hand_R() = default;

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
	// 각종 컴포넌트들
	CTexture* m_pTextureCom_Normal					= { nullptr };
	CTexture* m_pTextureCom_LaserStart				= { nullptr };
	CTexture* m_pTextureCom_LaserProgress_Start		= { nullptr };
	CTexture* m_pTextureCom_LaserProgress_Cycle		= { nullptr };
	CTexture* m_pTextureCom_Laser_End				= { nullptr };



	CAnimator* m_pAnimatorCom = { nullptr };


	// 로컬 변수들

	_int		iPhase = 0;
	_int		iPattern = 0;


public:
	static CErma_Hand_R* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END
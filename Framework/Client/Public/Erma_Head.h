#pragma once
#include "Monster.h"
#include "Client_Defines.h"


BEGIN(Client)

class CErma_Head final : public CMonster
{
private:
	CErma_Head(LPDIRECT3DDEVICE9 pGraphic_Device);
	CErma_Head(const CErma_Head& Prototype);
	virtual ~CErma_Head() = default;

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
	CTexture* m_pTextureCom_Standby					= { nullptr };
	CTexture* m_pTextureCom_Normal					= { nullptr };
	CTexture* m_pTextureCom_AttackStart				= { nullptr };
	CTexture* m_pTextureCom_AttackProgress			= { nullptr };
	CTexture* m_pTextureCom_AttackEnd				= { nullptr };
	CTexture* m_pTextureCom_DramaticDying			= { nullptr };
	CTexture* m_pTextureCom_Broken					= { nullptr };




	CAnimator* m_pAnimatorCom						= { nullptr };


	// 로컬 변수들

	_int		iPhase = 0;
	_int		iPattern = 0;


public:
	static CErma_Head* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END
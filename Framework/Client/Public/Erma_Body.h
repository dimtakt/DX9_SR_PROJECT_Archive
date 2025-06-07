#pragma once
#include "Monster.h"
#include "Client_Defines.h"


BEGIN(Client)

class CErma_Body final : public CMonster
{
private:
	CErma_Body(LPDIRECT3DDEVICE9 pGraphic_Device);
	CErma_Body(const CErma_Body& Prototype);
	virtual ~CErma_Body() = default;

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
	CTexture* m_pTextureCom_Body			= { nullptr };
	CTexture* m_pTextureCom_Body_Broken		= { nullptr };



	CAnimator* m_pAnimatorCom = { nullptr };


	// 로컬 변수들

	_int		iPhase = 0;
	_int		iPattern = 0;


public:
	static CErma_Body* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END
#pragma once
#include "Monster.h"
#include "Client_Defines.h"


BEGIN(Client)

class CErma_Hand_R final : public CMonster
{
public:
	enum class PATTERN_HAND_R
	{
		PT_IDLE,
		PT_STRIKE,	// 주먹으로 내려찍는 패턴
		PT_LASER,	// 주먹을 펴고 레이저를 쏘는 패턴
		PT_END
	};

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

public:
	void PlayPattern(PATTERN_HAND_R ePattern);

private:
	// 각종 컴포넌트들
	CTexture* m_pTextureCom_Normal = { nullptr };
	CTexture* m_pTextureCom_LaserStart = { nullptr };
	CTexture* m_pTextureCom_LaserProgress_Start = { nullptr };
	CTexture* m_pTextureCom_LaserProgress_Cycle = { nullptr };
	CTexture* m_pTextureCom_Laser_End = { nullptr };

	CTransform* m_pTerrainTransformCom = { nullptr };

	CAnimator* m_pAnimatorCom = { nullptr };
	CAnimator* m_pAnimatorPatternCom = { nullptr };


	// 로컬 변수들

	_int			m_iPhase = 0;
	PATTERN_HAND_R	m_ePattern = PATTERN_HAND_R::PT_IDLE;

	_bool			m_isPatternPlaying = false;



public:
	static CErma_Hand_R* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END
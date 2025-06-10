#pragma once
#include "Monster.h"
#include "Client_Defines.h"


BEGIN(Client)

class CErma_Body final : public CMonster
{
public:
	enum class PATTERN_BODY
	{
		PT_IDLE,
		PT_MISSILE_L,	// 미사일 발사
		PT_MISSILE_R,	// 미사일 발사

		PT_BROKEN,
		PT_END
	};

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

public:
	void PlayPattern(PATTERN_BODY ePattern, _bool isForced = false);
	void Set_Stop(_bool isStop = true) { m_isAllStop = isStop; }

private:
	// 각종 컴포넌트들
	CTexture* m_pTextureCom_Body			= { nullptr };
	CTexture* m_pTextureCom_Body_Broken		= { nullptr };

	CTransform* m_pTerrainTransformCom = { nullptr };

	CAnimator* m_pAnimatorCom			= { nullptr };
	CAnimator* m_pAnimatorPatternCom	= { nullptr };


	// 로컬 변수들

	_int			m_iPhase		= 0;
	PATTERN_BODY	m_ePattern		= PATTERN_BODY::PT_IDLE;

	_bool			m_isPatternPlaying = false;
	_bool			m_isAllStop = false;


public:
	static CErma_Body* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END
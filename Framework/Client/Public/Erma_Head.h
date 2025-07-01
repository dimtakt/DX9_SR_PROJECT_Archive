#pragma once
#include "Monster.h"
#include "Client_Defines.h"


BEGIN(Client)

class CErma_Head final : public CMonster
{
public:
	enum class PATTERN_HEAD
	{
		PT_IDLE,		// 평시
		PT_AWAKEN,		// 에르마 들어가서 켜지는거
		PT_BULLETFIRE,	// 탄막 패턴?
		PT_BROKEN,		// 망가짐

		PT_END
	};

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

public:
	void PlayPattern(PATTERN_HEAD ePattern, _bool isForced = false);
	void Set_Stop(_bool isStop = true) { m_isAllStop = isStop; }

private:
	// 각종 컴포넌트들
	CTexture* m_pTextureCom_Standby					= { nullptr };
	CTexture* m_pTextureCom_Normal					= { nullptr };
	CTexture* m_pTextureCom_AttackStart				= { nullptr };
	CTexture* m_pTextureCom_AttackProgress			= { nullptr };
	CTexture* m_pTextureCom_AttackEnd				= { nullptr };
	CTexture* m_pTextureCom_DramaticDying			= { nullptr };
	CTexture* m_pTextureCom_Broken					= { nullptr };

	CTransform* m_pTerrainTransformCom = { nullptr };



	CAnimator* m_pAnimatorCom						= { nullptr };
	CAnimator* m_pAnimatorPatternCom				= { nullptr };


	// 로컬 변수들

	_int			m_iPhase		= 0;
	PATTERN_HEAD	m_ePattern		= PATTERN_HEAD::PT_IDLE;

	_bool			m_isPatternPlaying = false;
	_bool			m_isAllStop = false;

	//std::list<_float3> m_effectVecList = {};

public:
	static CErma_Head* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END
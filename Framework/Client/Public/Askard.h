#pragma once
#include "Monster.h"
#include "Client_Defines.h"

#include "BossHp_Askard.h"

#include "Askard_Tentacle.h"
#include "Askard_Eye.h"


BEGIN(Client)

//class CAskard_Tentacle;

class CAskard :	public CMonster
{
public:
	// 패턴 정보 : https://puu.sh/Kv6IE/a6172d4916.png
	// 영상 참고 : https://www.youtube.com/watch?v=xSxo8JE2dKo
	enum class PATTERN_ASKARD
	{
		PT_IDLE,
	
		// Phase 1
		PT_SPAWN_WIDTH,		// (3반복) 가로줄로 촉수 소환
		PT_FOLLOWING_EYES,	// 눈달린 유도탄 5개 소환, 수명 다하면 폭발
		PT_CORNER_LASER,	// 구석에서 90도 돌아가는 레이저
		PT_SPAWN_CROSS,		// 플레이어에게 다가와서 4방향 촉수 소환
		PT_SPAWN_LINE,		// 플레이어로부터 떨어져서 2줄 촉수 소환
		PT_SPARK,			// 점프도약 후 충격파

		// Phase 2		(Phase 1 패턴도 섞어씀, _ADV 붙은 건 1페 강화패턴)
		PT_DARK_TENTACLE,		// 소등 후 화면을 덮는 거대한 촉수 피하기 패턴
		PT_SPAWN_WIDTH_ADV,		// (강화패턴) 공격 빈도 증가 및 세로줄로도 공격
		PT_FOLLOWING_EYES_ADV,	// (강화패턴) 유도탄 폭발 시 해당 위치에 장판 남김
		PT_CORNER_LASER_ADV,	// (강화패턴) 레이저를 3방향에서 순차적으로 사용
		PT_SPAWN_CROSS_ADV,		// (강화패턴) 촉수 생성위치의 랜덤성 증가?
		PT_SPAWN_LINE_ADV,		// (강화패턴) 2줄에서 3줄로 증가
		PT_SPARK_ADV,			// (강화패턴) 분신 소환으로 충격파 3배

		PT_END
	};


private:
	CAskard(LPDIRECT3DDEVICE9 pGraphic_Device);
	CAskard(const CAskard& Prototype);
	virtual ~CAskard() = default;

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

	void Summon_Tentacle(_float3 vPosition, CAskard_Tentacle::TYPE_TENTACLE eType);
	void Summon_FollowingEye(_float3 vPosition);

public:
	virtual void OnCollision(CGameObject* pGameObject) override;

private:
	// 각종 컴포넌트들
	CTexture*		m_pTextureCom_P1_Attack					= { nullptr };
	CTexture*		m_pTextureCom_P1_Attack_End				= { nullptr };
	CTexture*		m_pTextureCom_P1_Attack_Ready			= { nullptr };
	CTexture*		m_pTextureCom_P1_Die					= { nullptr };
	CTexture*		m_pTextureCom_P1_GroundIdle				= { nullptr };
	CTexture*		m_pTextureCom_P1_Idle					= { nullptr };
	CTexture*		m_pTextureCom_P1_Laser					= { nullptr };
	CTexture*		m_pTextureCom_P1_PhaseChange			= { nullptr };
	CTexture*		m_pTextureCom_P1_PhaseStart				= { nullptr };
	CTexture*		m_pTextureCom_P1_RangeAttack			= { nullptr };
	CTexture*		m_pTextureCom_P1_StaffIdle				= { nullptr };
	CTexture*		m_pTextureCom_P1_SummonStaff			= { nullptr };
	CTexture*		m_pTextureCom_P1_Wave					= { nullptr };

	CTexture*		m_pTextureCom_P2_Attack					= { nullptr };
	CTexture*		m_pTextureCom_P2_Attack_End				= { nullptr };
	CTexture*		m_pTextureCom_P2_Attack_Ready			= { nullptr };
	CTexture*		m_pTextureCom_P2_Idle					= { nullptr };
	CTexture*		m_pTextureCom_P2_Laser					= { nullptr };
	CTexture*		m_pTextureCom_P2_RangeAttack			= { nullptr };
	CTexture*		m_pTextureCom_P2_Tentacle				= { nullptr };
	CTexture*		m_pTextureCom_P2_Tentacle_End			= { nullptr };
	//CTexture*		m_pTextureCom_P2_Tentacle_Unlit			= { nullptr };
	CTexture*		m_pTextureCom_P2_Wave					= { nullptr };

	CTransform*		m_pTerrainTransformCom					= { nullptr };

	CAnimator*		m_pAnimatorCom							= { nullptr };





	CBossHp_Askard*	m_pBossHp = { nullptr };


	// 로컬 변수들

	_int			m_iPhase						= 0;
	PATTERN_ASKARD	m_ePattern;

	_int			m_iElapsedFrame_Pattern			= 0;
	_int			m_iElapsedFrame_Update			= 0;
	_float3			m_vTargettedPos					= {};
	_float3			m_vMovePos						= {};
	_float3			m_vLockedOnPos					= {};

	_bool			m_isAllStop						= false;

private:
	void			Adjust_Scale();					// 임시, 리소스 크기 조정하면 삭제

	// Patterns
	void			Play_Spawn_Width(_float fTimeDelta);
	void			Play_Spawn_Cross(_float fTimeDelta);
	void			Play_Spawn_Line(_float fTimeDelta);
	void			Play_Corner_Laser(_float fTimeDelta);
	//void			Play_Following_Eyes(_float fTimeDelta);
	void			Play_Spark(_float fTimeDelta);

public:
	static CAskard* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};


END
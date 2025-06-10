#pragma once
#include "Monster.h"
#include "Client_Defines.h"

BEGIN(Client)


class CAskard :	public CMonster
{
public:
	// 패턴 정보 : https://puu.sh/Kv6IE/a6172d4916.png
	// 영상 참고 : https://www.youtube.com/watch?v=xSxo8JE2dKo
	enum class PATTERN_ASKARD
	{
		PT_IDLE,
	
		// Phase 1
		PT_SPAWN_WIDTH,		// (3반복) 가로줄로 소환
		PT_FOLLOWING_EYES,	// 눈달린 유도탄 5개 소환
		PT_CORNER_LASER,	// 구석에서 90도 돌아가는 레이저
		PT_SPAWN_CROSS,		// 다가와서 4방향 소환
		PT_SPAWN_LINE,		// 떨어져서 2줄 소환
		PT_SPARK,			// 점프도약 후 충격파

		// Phase 2
		PT_DARK_TENTACLE,
		PT_SPAWN_WIDTH_ADV,
		PT_FOLLOWING_EYES_ADV,
		PT_CORNER_LASER_ADV,
		PT_SPAWN_CROSS_ADV,
		PT_SPAWN_LINE_ADV,
		PT_SPARK_ADV,

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
	CTexture*		m_pTextureCom_P2_Tentacle_Unlit			= { nullptr };
	CTexture*		m_pTextureCom_P2_Wave					= { nullptr };






	//..

	CTransform* m_pTerrainTransformCom			= { nullptr };

	CAnimator*	m_pAnimatorCom					= { nullptr };



	// 로컬 변수들

	_int		m_iPhase						= 0;
	_int		m_iPattern						= 0;

public:
	static CAskard* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};


END
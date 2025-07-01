#pragma once
#include "Monster.h"

BEGIN(Client)

class CAskard_Tentacle : public CMonster
{
public:
	enum class PATTERN_TENTACLE
	{
		PT_IDLE,

		// 단순 등장 및 사라짐
		//PT_UP,
		PT_DOWNWAIT,
		PT_DOWN,	

		// 별종
		//PT_EYEOPEN,
		PT_WAITTARGET,
		PT_ATTACK,

		PT_END
	};

	enum class TYPE_TENTACLE
	{
		TYPE_NORMAL_1,
		TYPE_NORMAL_2,
		TYPE_NORMAL_3,

		TYPE_ODD_1,

		TYPE_END
	};

	typedef struct tagTentacleDesc
	{
		_float3		vPosition;	// 생성 위치

		TYPE_TENTACLE	eType;	// 생성될 타입

		CTerrainBox* pTerrainBox;

	}TENTACLE_DESC;

private:
	CAskard_Tentacle(LPDIRECT3DDEVICE9 pGraphic_Device);
	CAskard_Tentacle(const CAskard_Tentacle& Prototype);
	virtual ~CAskard_Tentacle() = default;

public:
	virtual HRESULT Initialize_Prototype()				override;
	virtual HRESULT Initialize(void* pArg)				override;
	virtual void Priority_Update(_float fTimeDelta)		override;
	virtual void Update(_float fTimeDelta)				override;
	virtual void Late_Update(_float fTimeDelta)			override;
	virtual HRESULT Render()							override;

private:
	HRESULT Ready_Components(void* pArg);

public:
	virtual void OnCollision(CGameObject* pGameObject) override;

private:
	// 컴포넌트들

	CTexture*	m_pTextureCom_Root_Standby				= { nullptr };

	CTexture*	m_pTextureCom_Root_Up_1					= { nullptr };
	CTexture*	m_pTextureCom_Root_Up_2					= { nullptr };
	CTexture*	m_pTextureCom_Root_Up_3					= { nullptr };

	CTexture*	m_pTextureCom_Root_DownWait_1			= { nullptr };
	CTexture*	m_pTextureCom_Root_DownWait_2			= { nullptr };
	CTexture*	m_pTextureCom_Root_DownWait_3			= { nullptr };

	CTexture*	m_pTextureCom_Root_Down_1				= { nullptr };
	CTexture*	m_pTextureCom_Root_Down_2				= { nullptr };
	CTexture*	m_pTextureCom_Root_Down_3				= { nullptr };

	CTexture*	m_pTextureCom_Root_EyeOpen				= { nullptr };
	CTexture*	m_pTextureCom_Root_EyeOpened			= { nullptr };
	CTexture*	m_pTextureCom_Root_AttackReady			= { nullptr };
	CTexture*	m_pTextureCom_Root_Attack				= { nullptr };

	CTexture*	m_pTextureCom_Root_ExplosionReady		= { nullptr };


	CAnimator*	m_pAnimatorCom							= { nullptr };
	CAnimator*	m_pAnimatorPatternCom					= { nullptr };


	CTransform*	m_pTargetTransformCom					= { nullptr };
	CTransform* m_pTerrainTransformCom					= { nullptr };

	// 로컬 변수들

	TYPE_TENTACLE		m_eType		= TYPE_TENTACLE::TYPE_END;
	PATTERN_TENTACLE	m_ePattern	= PATTERN_TENTACLE::PT_IDLE;

	_int				m_iStackedFrames = 0;	// Update 문 마지막에서 매 프레임마다 ++

public:
	static CAskard_Tentacle* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};


END
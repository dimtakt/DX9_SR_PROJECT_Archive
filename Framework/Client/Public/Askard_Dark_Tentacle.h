#pragma once
#include "Monster.h"


BEGIN(Client)


class CAskard_Dark_Tentacle : public CMonster
{
public:
	typedef enum class eDarkTentacleDir
	{
		DIR_TO_XNEG,	// x축 음방향
		DIR_TO_XPOS,	// x축 양방향
		DIR_TO_ZNEG,	// z축 음방향
		DIR_TO_ZPOS,	// z축 양방향

		DIR_END
	}DARKTENTACLE_DIR;

	typedef struct tagDarkTentacleDesc
	{
		_float3				vPosition;	// 타겟 위치 (생성 위치에 관여)
		DARKTENTACLE_DIR	eDir;		// 어느 방향으로 공격하는 촉수인지

		CTerrainBox*		pTerrainBox;

	}DARKTENTACLE_DESC;


private:
	CAskard_Dark_Tentacle(LPDIRECT3DDEVICE9 pGraphic_Device);
	CAskard_Dark_Tentacle(const CAskard_Dark_Tentacle& Prototype);
	virtual ~CAskard_Dark_Tentacle() = default;

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
	CTransform*			m_pTargetTransformCom			= { nullptr };
	CTransform*			m_pTerrainTransformCom			= { nullptr };

	//CAnimator*			m_pAnimatorCom					= { nullptr };


	DARKTENTACLE_DIR	m_eDir							= {};
	_float3				m_vTargetPos					= {};
	_float3				m_vLockedOnPos					= {};

	_int				m_iElapsedFrame					= 0;

public:
	static CAskard_Dark_Tentacle* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;


};

END
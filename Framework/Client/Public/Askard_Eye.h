#pragma once
#include "Monster.h"

BEGIN(Client)

class CAskard_Eye : public CMonster
{
public:
	typedef struct tagEyeDesc
	{
		_float3		vPosition;	// 생성 위치

		CTerrainBox* pTerrainBox;

	}EYE_DESC;

private:
	CAskard_Eye(LPDIRECT3DDEVICE9 pGraphic_Device);
	CAskard_Eye(const CAskard_Eye& Prototype);
	virtual ~CAskard_Eye() = default;

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

	//CTexture*	m_pTextureCom_Root_Standby				= { nullptr };

	CAnimator* m_pAnimatorCom = { nullptr };
	//CAnimator*	m_pAnimatorPatternCom					= { nullptr };

	CTransform* m_pTargetTransformCom = { nullptr };
	CTransform* m_pTerrainTransformCom = { nullptr };



	// 로컬 변수들


	_int				m_iStackedFrames = 0;	// Update 문 마지막에서 매 프레임마다 ++

public:
	static CAskard_Eye* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
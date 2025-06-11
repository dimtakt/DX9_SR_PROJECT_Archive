#pragma once
#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CParticle abstract : public CGameObject
{
public:
	typedef struct tagParticleDesc
	{
		_float3 vPos = {};
		_float3 vScale = {};
		_bool bUseOrtho = false;
		_float fSpwanTimer = {};
		_float fSpeed = {};
		_float fLifeTime = {};
	}PARTICLE_DESC;

protected:
	CParticle(LPDIRECT3DDEVICE9 pGraphic_Device);
	CParticle(const CParticle& Prototype);
	virtual ~CParticle() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();
	

	void Set_StartPos(_float3 fPos) { m_vStartPos = fPos; }
	_bool Get_Active() { return m_bIsActive; }
	void Set_Active(_bool bActive) { m_bIsActive = bActive; }

protected:
	_float3                  m_vStartPos;     // 처음 위치 기준
	_float3					 m_vPos;		  // 처음 위치 기준으로 pos 변경해서 사용하고자 함. 이후 일정 시간 지나면 다시 초기값으로 되돌림
	_bool m_bIsActive = false;				// 트루값만 랜더 , 업데이트 돌림

	_float                  m_fSpawnTimer = 0.f;	
	_float					m_fSpeed = 0.f;			// 떨어지는 속도
	_float					m_fLifeTime = 0.f;		// 수명시간
	_float					m_fElapsedTime = 0.f;	// 누적시간


public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free();
};

END
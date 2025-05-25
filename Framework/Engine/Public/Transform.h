#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTransform final : public CComponent
{
public:
	typedef struct tagTransformDesc
	{
		_float		fSpeedPerSec;
		_float		fRotationPerSec;
	}TRANSFORM_DESC;

private:
	CTransform(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTransform(const CTransform& Prototype);
	virtual ~CTransform() = default;

public:
	_float3 Get_State(STATE eState) const {
 		return *reinterpret_cast<const _float3*>(&m_WorldMatrix.m[ENUM_CLASS(eState)][0]);
	}

	_float3 Get_Scaled() const {
		_float3		vRight = Get_State(STATE::RIGHT);
		_float3		vUp = Get_State(STATE::UP);
		_float3		vLook = Get_State(STATE::LOOK);

		return _float3(D3DXVec3Length(&vRight), D3DXVec3Length(&vUp), D3DXVec3Length(&vLook));
	}

	const _float4x4* Get_WorldMatrix_Inverse() {
		return D3DXMatrixInverse(&m_WorldMatrixInverse, nullptr, &m_WorldMatrix);
	}

	void Set_State(STATE eState, const _float3& vState) {
		memcpy(&m_WorldMatrix.m[ENUM_CLASS(eState)][0], &vState, sizeof(_float3));
	}

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);

public:
	void Go_Straight(_float fTimeDelta);
	void Go_Backward(_float fTimeDelta);
	void Go_Right(_float fTimeDelta);
	void Go_Left(_float fTimeDelta);

	void Look_At(const _float3& vTarget);
	void Move_To(const _float3& vTarget, _float fTimeDelta, _float fLimitRange);
	
	void Rotation(const _float3& vAxis, _float fRadian);
	void RotationAccumulate(const _float3& vAxis, _float fRadian);
	void Turn(const _float3& vAxis, _float fTimeDelta);
	void Scaling(_float fScaleX, _float fScaleY, _float fScaleZ);

	//void StartShake(_float fDuration, _float fStrength, _float fTime);


public:
	void Bind_Matrix();

private:
	_float4x4					m_WorldMatrix = { };
	_float4x4					m_WorldMatrixInverse = { };
	_float						m_fSpeedPerSec = { };
	_float						m_fRotationPerSec = { };

	// 카메라 쉐이킹용 멤버변수
	/*_bool						m_bIsShaking = { };
	_float						m_fShakeDuration = { };
	_float						m_fShakeTime = { };
	_float						m_fShakeStrength = { };*/

public:
	static CTransform* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

};

END
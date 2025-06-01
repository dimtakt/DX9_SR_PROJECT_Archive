#include "Transform.h"

CTransform::CTransform(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent{ pGraphic_Device }
{
}

CTransform::CTransform(const CTransform& Prototype)
	: CComponent{ Prototype }
	, m_WorldMatrix { Prototype.m_WorldMatrix }
{
}

HRESULT CTransform::Initialize_Prototype()
{
	D3DXMatrixIdentity(&m_WorldMatrix);

	return S_OK;
}

HRESULT CTransform::Initialize(void* pArg)
{
	TRANSFORM_DESC* pDesc = static_cast<TRANSFORM_DESC*>(pArg);

	if (pDesc == nullptr)
		return S_OK;

	m_fSpeedPerSec = pDesc->fSpeedPerSec;
	m_fRotationPerSec = pDesc->fRotationPerSec;

	return S_OK;
}

void CTransform::Go_Straight(_float fTimeDelta)
{
	_float3		vPosition = Get_State(STATE::POSITION);
	_float3		vLook = Get_State(STATE::LOOK);

	vPosition += *D3DXVec3Normalize(&vLook, &vLook) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE::POSITION, vPosition);
}

void CTransform::Go_Backward(_float fTimeDelta)
{
	_float3		vPosition = Get_State(STATE::POSITION);
	_float3		vLook = Get_State(STATE::LOOK);

	vPosition -= *D3DXVec3Normalize(&vLook, &vLook) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE::POSITION, vPosition);
}

void CTransform::Go_Right(_float fTimeDelta)
{
	_float3		vPosition = Get_State(STATE::POSITION);
	_float3		vRight = Get_State(STATE::RIGHT);

	vPosition += *D3DXVec3Normalize(&vRight, &vRight) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE::POSITION, vPosition);
}

void CTransform::Go_Left(_float fTimeDelta)
{
	_float3		vPosition = Get_State(STATE::POSITION);
	_float3		vRight = Get_State(STATE::RIGHT);

	vPosition -= *D3DXVec3Normalize(&vRight, &vRight) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE::POSITION, vPosition);
}

void CTransform::Look_At(const _float3& vTarget)
{
	_float3		vScaled = Get_Scaled();

	_float3		vRight{}, vUp{}, vLook{};

	vLook = vTarget - Get_State(STATE::POSITION);

	_float3		vUpDir = _float3(0.f, 1.f, 0.f);

	D3DXVec3Cross(&vRight, &vUpDir, &vLook);
	D3DXVec3Cross(&vUp, &vLook, &vRight);

	Set_State(STATE::RIGHT, *D3DXVec3Normalize(&vRight, &vRight) * vScaled.x);
	Set_State(STATE::UP,	*D3DXVec3Normalize(&vUp, &vUp) * vScaled.y);
	Set_State(STATE::LOOK,	*D3DXVec3Normalize(&vLook, &vLook) * vScaled.z);
}

void CTransform::Move_To(const _float3& vTarget, _float fTimeDelta, _float fLimitRange)
{
	_float3		vPosition = Get_State(STATE::POSITION);
	_float3		vMoveDir = vTarget - vPosition;

	if (D3DXVec3Length(&vMoveDir) >= fLimitRange)
	{
		vPosition += *D3DXVec3Normalize(&vMoveDir, &vMoveDir) * m_fSpeedPerSec * fTimeDelta;
		Set_State(STATE::POSITION, vPosition);
	}
}

void CTransform::Rotation(const _float3& vAxis, _float fRadian)
{
	_float3			vScaled = Get_Scaled();

	_float3			vRight = _float3(1.f, 0.f, 0.f) * vScaled.x;
	_float3			vUp = _float3(0.f, 1.f, 0.f) * vScaled.y;
	_float3			vLook = _float3(0.f, 0.f, 1.f) * vScaled.z;

	_float4x4		RotationMatrix = {};

	D3DXMatrixRotationAxis(&RotationMatrix, &vAxis, fRadian);

	D3DXVec3TransformNormal(&vRight, &vRight, &RotationMatrix);
	D3DXVec3TransformNormal(&vUp, &vUp, &RotationMatrix);
	D3DXVec3TransformNormal(&vLook, &vLook, &RotationMatrix);

	Set_State(STATE::RIGHT, vRight);
	Set_State(STATE::UP, vUp);
	Set_State(STATE::LOOK, vLook);
}

void CTransform::RotationAccumulate(const _float3& vAxis, _float fRadian)
{
	_float3			vRight = Get_State(STATE::RIGHT);
	_float3			vUp = Get_State(STATE::UP);
	_float3			vLook = Get_State(STATE::LOOK);

	_float4x4		RotationMatrix = {};

	D3DXMatrixRotationAxis(&RotationMatrix, &vAxis, fRadian);

	D3DXVec3TransformNormal(&vRight, &vRight, &RotationMatrix);
	D3DXVec3TransformNormal(&vUp, &vUp, &RotationMatrix);
	D3DXVec3TransformNormal(&vLook, &vLook, &RotationMatrix);

	Set_State(STATE::RIGHT, vRight);
	Set_State(STATE::UP, vUp);
	Set_State(STATE::LOOK, vLook);
}

void CTransform::Turn(const _float3& vAxis, _float fTimeDelta)
{
	_float3			vRight = Get_State(STATE::RIGHT);
	_float3			vUp = Get_State(STATE::UP);
	_float3			vLook = Get_State(STATE::LOOK);

	_float4x4		RotationMatrix = {};

	D3DXMatrixRotationAxis(&RotationMatrix, &vAxis, m_fRotationPerSec * fTimeDelta);

	D3DXVec3TransformNormal(&vRight, &vRight, &RotationMatrix);
	D3DXVec3TransformNormal(&vUp, &vUp, &RotationMatrix);
	D3DXVec3TransformNormal(&vLook, &vLook, &RotationMatrix);

	Set_State(STATE::RIGHT, vRight);
	Set_State(STATE::UP, vUp);
	Set_State(STATE::LOOK, vLook);

}

void CTransform::Scaling(_float fScaleX, _float fScaleY, _float fScaleZ)
{
	_float3		vRight = Get_State(STATE::RIGHT);
	_float3		vUp = Get_State(STATE::UP);
	_float3		vLook = Get_State(STATE::LOOK);

	Set_State(STATE::RIGHT, *D3DXVec3Normalize(&vRight, &vRight) * fScaleX);
	Set_State(STATE::UP, *D3DXVec3Normalize(&vUp, &vUp) * fScaleY);
	Set_State(STATE::LOOK, *D3DXVec3Normalize(&vLook, &vLook) * fScaleZ);

}

// 특정 부모좌표와 특정 단위벡터를 이용하여 내 좌표를 평면상 회전시키는 함수
void CTransform::RotationByParent(const _float3 axis, CTransform* parent, _float fRadian)
{
	// 객체가 돌아갈 때의 기준점 행렬 설정, 없으면 원점 기준.
	_float4x4 matTrackTarget;
	if (parent != nullptr)
		matTrackTarget = *parent->Get_WorldMatrix();
	else
		D3DXMatrixIdentity(&matTrackTarget);

	// 변환..
	
	// 원점으로 중심축 이동
	_float4x4 matToOrigin;
	D3DXMatrixTranslation(&matToOrigin,
		-matTrackTarget._41,
		-matTrackTarget._42,
		-matTrackTarget._43);

	// 축 기준 회전행렬
	_float4x4 matRot;
	D3DXMatrixRotationAxis(&matRot, &axis, fRadian);

	// 다시 제자리로
	_float4x4 matFromOrigin;
	D3DXMatrixTranslation(&matFromOrigin,
		matTrackTarget._41,
		matTrackTarget._42,
		matTrackTarget._43);

	// 다 합치기
	_float4x4 matRotationTotal = matToOrigin * matRot * matFromOrigin;

	// 반영..

	_float4x4 matResult = m_WorldMatrix * matRotationTotal;
	m_WorldMatrix = matResult;

	return;
}

void CTransform::ApplyEulerRotation(const _float3& vEuler)
{
	_float3			vScaled = Get_Scaled();

	D3DXMATRIX matX, matY, matZ, matFinal;

	D3DXMatrixRotationX(&matX, D3DXToRadian(vEuler.x));
	D3DXMatrixRotationY(&matY, D3DXToRadian(vEuler.y));
	D3DXMatrixRotationZ(&matZ, D3DXToRadian(vEuler.z));

	matFinal = matZ * matY * matX;

	_float3 vRight = _float3(1.f, 0.f, 0.f) * vScaled.x;
	_float3 vUp = _float3(0.f, 1.f, 0.f) * vScaled.y;
	_float3	vLook = _float3(0.f, 0.f, 1.f) * vScaled.z;

	D3DXVec3TransformNormal(&vRight, &vRight, &matFinal);
	D3DXVec3TransformNormal(&vUp, &vUp, &matFinal);
	D3DXVec3TransformNormal(&vLook, &vLook, &matFinal);

	Set_State(STATE::RIGHT, vRight);
	Set_State(STATE::UP, vUp);
	Set_State(STATE::LOOK, vLook);
}

void CTransform::Bind_Matrix()
{
	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_WorldMatrix);
}

CTransform* CTransform::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTransform* pInstance = new CTransform(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CTransform"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CTransform::Clone(void* pArg)
{
	CTransform* pInstance = new CTransform(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to cloned : CTransform"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTransform::Free()
{
	__super::Free();
}

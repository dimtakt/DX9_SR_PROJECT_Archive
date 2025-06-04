#include "Camera_Follow.h"
#include "GameInstance.h"

inline _float Lerp(_float a, _float b, _float t)
{
	return a + (b - a) * t;
}

inline _float3 Lerp(const _float3& a, const _float3& b, _float t)
{
	return {
		a.x + (b.x - a.x) * t,
		a.y + (b.y - a.y) * t,
		a.z + (b.z - a.z) * t
	};
}

CCamera_Follow::CCamera_Follow(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCamera{ pGraphic_Device }
{
}

CCamera_Follow::CCamera_Follow(const CCamera_Follow& Prototype)
	: CCamera{ Prototype }
{
}

HRESULT CCamera_Follow::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCamera_Follow::Initialize(void* pArg)
{
	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Target(pArg)))
		return E_FAIL;
	
	CCamera::CAMERA_DESC CameraDesc{};

	CameraDesc.vEye = _float3(0.f, 10.f, -10.f);
	CameraDesc.vAt = m_pTargetPlayerTransformCom->Get_State(STATE::POSITION);
	CameraDesc.fFov = D3DXToRadian(60.f);
	CameraDesc.fNear = 0.1f;
	CameraDesc.fFar = 1000.f;
	CameraDesc.fSpeedPerSec = 40.f;
	CameraDesc.fRotationPerSec = D3DXToRadian(90.f);

	m_vOffset = _float3(0.f, 8.f, -8.f);
	m_fCurrentAngle = 0.f;

	if (FAILED(__super::Initialize(&CameraDesc)))
		return E_FAIL;

	Safe_AddRef(m_pTargetPlayerTransformCom);

	m_vRotateObjectsTransformCom.push_back(dynamic_cast<CTransform*>(
		m_pGameInstance->Get_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Layer_Weapon"), TEXT("Com_Transform"))));

	m_pGameInstance->Subscribe(ENUM_CLASS(EVENT_TYPE::ROOMCHANGE), this);

	m_vTerrainPos = _float3{ 0.f, 0.f, 0.f };
	m_fMaxXRange = 5.f;
	m_fZMin = -13.f;
	m_fZMax = 13.f;
	m_fDistance = 30.f;
	m_bCameraTransition = false;
	m_fCamTransitionDuration = 0.5f;
	m_fXTransitionDuration = 0.5f;

	return S_OK;
}

void CCamera_Follow::Priority_Update(_float fTimeDelta)
{
	Move_Angle(90.f, fTimeDelta);
	Follow_Target(fTimeDelta);
	__super::Update_VP_Matrices();
}

void CCamera_Follow::Update(_float fTimeDelta)
{
	
	if (m_bIgnoreXOffset)
	{
		m_fXIgnoreTimer -= fTimeDelta;
		if (m_fXIgnoreTimer <= 0.f)
			m_bIgnoreXOffset = false;
	}

}

void CCamera_Follow::Late_Update(_float fTimeDelta)
{
}

HRESULT CCamera_Follow::Render()
{
	return S_OK;
}

void CCamera_Follow::OnEvent(_uint iTypeindex, const EVENTDATA* pData)
{
	if (static_cast<EVENT_TYPE>(iTypeindex) == EVENT_TYPE::ROOMCHANGE) {
		auto pRoom = static_cast<const ROOMCHANGE*>(pData);
		m_vTerrainPos = pRoom->vPosition;
		//// 기존 코드 삭제!
		//// m_pTransformCom->Set_State(STATE::POSITION, m_vTerrainPos); 팍 튐 원인

		//// 대신 카메라를 플레이어 기준 오프셋 위치에 순간적으로 놓자
		//_float3 vPlayerPos = m_pTargetPlayerTransformCom->Get_State(STATE::POSITION);

		//// 카메라 위치 바로 계산 (ZRatio 적용 없이 기본 오프셋만)
		//_float3 vNewCamPos = {
		//	vPlayerPos.x,
		//	vPlayerPos.y + 6.0f,  // 적당히 위
		//	vPlayerPos.z - 9.0f   // 적당히 뒤
		//};

		//m_pTransformCom->Set_State(STATE::POSITION, vNewCamPos);

		//// Look 방향도 정면으로 즉시 맞춰주기
		//m_pTransformCom->Look_At(vPlayerPos);

		//// 스냅 모드 활성화
		//m_bForceSnapCamera = true;
		//m_bIgnoreXOffset = true;
		//m_fXIgnoreTimer = 0.5f; // 너무 길면 부자연스러우니 줄이자

		/*auto pRoom = static_cast<const ROOMCHANGE*>(pData);
		m_vTerrainPos = pRoom->vPosition;

		m_vCamTransitionStartPos = m_pTransformCom->Get_State(STATE::POSITION);
		_float3 vPlayerPos = m_pTargetPlayerTransformCom->Get_State(STATE::POSITION);
		_float3 vTargetPos = {
			vPlayerPos.x,
			vPlayerPos.y + 6.f,
			vPlayerPos.z - 9.f
		};
		m_vCamTransitionTargetPos = vTargetPos;

		m_bCameraTransition = true;
		m_fCamTransitionTimer = 5.f;

		m_bIgnoreXOffset = true;
		m_fXIgnoreTimer = 0.5f;*/
		/*auto pRoom = static_cast<const ROOMCHANGE*>(pData);
		m_vTerrainPos = pRoom->vPosition;

		_float3 vPlayerPos = m_pTargetPlayerTransformCom->Get_State(STATE::POSITION);
		_float fZRatio = (vPlayerPos.z - m_fZMin) / (m_fZMax - m_fZMin);
		fZRatio = max(0.f, min(fZRatio, 1.f));

		_float fYOffset = Lerp(4.5f, 8.5f, fZRatio);
		_float fZTargetOffset = Lerp(8.0f, 10.5f, fZRatio);

		m_vCamTransitionStartPos = m_pTransformCom->Get_State(STATE::POSITION);
		m_vCamTransitionTargetPos = {
			vPlayerPos.x,
			vPlayerPos.y + fYOffset,
			vPlayerPos.z - fZTargetOffset
		};
		m_bCameraTransition = true;
		m_fCamTransitionTimer = 0.f;*/
	}
}


HRESULT CCamera_Follow::Ready_Target(void* pArg)
{
	CAMERAFOLDESC* pDesc = static_cast<CAMERAFOLDESC*>(pArg);

	m_pTargetPlayerTransformCom = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(pDesc->iLayerIndex, TEXT("Layer_Player"), TEXT("Com_Transform")));
	if (m_pTargetPlayerTransformCom == nullptr)
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Follow::Ready_Components(void* pArg)
{
	/* For,Com_Transform*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), pArg)))
		return E_FAIL;


	return S_OK;
}

void CCamera_Follow::Move_Angle(_float fAngle, _float fTimeDelta)
{
	if (m_pGameInstance->IsKeyDown('Q'))
	{
		m_fCurrentAngle += fAngle;
		if (m_fCurrentAngle >= 360.f)
			m_fCurrentAngle -= 360.f;

		m_pTargetPlayerTransformCom->RotationAccumulate(_float3{ 0.f, 1.f, 0.f }, D3DXToRadian(fAngle));
		//for (auto* component : m_vRotateObjectsTransformCom) {
		//	component->RotationAccumulate(_float3{ 0.f, 1.f, 0.f }, D3DXToRadian(fAngle));
		//}
	}

	if (m_pGameInstance->IsKeyDown('E'))
	{
		m_fCurrentAngle -= fAngle;
		if (m_fCurrentAngle < 0.f)
			m_fCurrentAngle += 360.f;

		m_pTargetPlayerTransformCom->RotationAccumulate(_float3{ 0.f, 1.f, 0.f }, D3DXToRadian(fAngle) * -1.f);
		//for (auto* component : m_vRotateObjectsTransformCom) {
		//	component->RotationAccumulate(_float3{ 0.f, 1.f, 0.f }, D3DXToRadian(fAngle) * -1.f);
		//}
	}
}

void CCamera_Follow::Follow_Target(_float fTimeDelta)
{
	_float3		vPlayerPosition = m_pTargetPlayerTransformCom->Get_State(STATE::POSITION);
	_float3		vPosition = m_pTransformCom->Get_State(STATE::POSITION);

	_float4x4	matRotY;
	D3DXMatrixRotationY(&matRotY, D3DXToRadian(m_fCurrentAngle));

	_float3 vRotatedOffset;
	D3DXVec3TransformCoord(&vRotatedOffset, &m_vOffset, &matRotY);

	_float3 vCameraPos = vPlayerPosition + vRotatedOffset;
	m_pTransformCom->Set_State(STATE::POSITION, vCameraPos);
	m_pTransformCom->Look_At(vPlayerPosition);

	//_float3 vPlayerPos = m_pTargetPlayerTransformCom->Get_State(STATE::POSITION);
	//_float3 vCameraRight = m_pTransformCom->Get_State(STATE::RIGHT);
	//_float3 vCameraUp = m_pTransformCom->Get_State(STATE::UP);
	//_float3 vCameraLook = m_pTransformCom->Get_State(STATE::LOOK);
	//_float3 vCameraPos = m_pTransformCom->Get_State(STATE::POSITION);

	//// 2. Z 위치 비율
	//_float fZRatio = (vPlayerPos.z - m_fZMin) / (m_fZMax - m_fZMin);
	//fZRatio = max(0.f, min(fZRatio, 1.f));

	//// 3. X 제한
	//_float fX = vPlayerPos.x;
	//if (fabsf(fX - m_vTerrainPos.x) > m_fMaxXRange)
	//	fX = vCameraPos.x;

	//// 4. 조정된 Y/Z 보간
	//// 더 위에서 내려다보게 하고, Z 거리는 살짝 뒤에서 따라오게 함
	//_float fYOffset = Lerp(4.5f, 8.5f, fZRatio);   // 더 자연스러운 Y 시점
	//_float fZTargetOffset = Lerp(8.0f, 10.5f, fZRatio); // 앞에서는 살짝 뒤로, 뒤에서는 살짝 멀게

	//_float fFinalZ = 0.f;
	//if (m_bForceSnapCamera || m_bIgnoreXOffset)
	//{
	//	// 포탈 이동 후, 즉시 위치 보정
	//	 // 즉시 스냅 적용
	//	fFinalZ = vPlayerPos.z - fZTargetOffset;
	//	fX = vPlayerPos.x;

	//	m_bForceSnapCamera = false;
	//	m_bIgnoreXOffset = false;
	//}
	//else
	//{
	//	// 부드러운 Z 추적
	//	_float fTargetZ = vPlayerPos.z - fZTargetOffset;
	//	_float fZDeadZone = 1.5f;

	//	if (fabsf(vCameraPos.z - fTargetZ) > fZDeadZone)
	//		fFinalZ = Lerp(vCameraPos.z, fTargetZ, fTimeDelta * 2.f);
	//	else
	//		fFinalZ = vCameraPos.z;

	//	if (fabsf(vPlayerPos.x - m_vTerrainPos.x) > m_fMaxXRange)
	//		fX = vCameraPos.x;
	//	else
	//		fX = vPlayerPos.x;
	//}
	//// --- Step 3: 최종 카메라 위치 계산 ---
	//_float3 vNewCameraPos = {
	//	fX,
	//	vPlayerPos.y + fYOffset,
	//	fFinalZ
	//};

	//// 7. LookAt 대상 위치
	//_float3 vLookAtPos = vPlayerPos;
	//if (fabsf(vPlayerPos.x - m_vTerrainPos.x) > m_fMaxXRange)
	//	vLookAtPos.x = vNewCameraPos.x;

	//// 8. 방향 벡터 계산
	//_float3 vLook = vLookAtPos - vNewCameraPos;
	//D3DXVec3Normalize(&vLook, &vLook);

	//_float3 vRight, vUp, vTemp{0.f, 1.f, 0.f};
	//D3DXVec3Cross(&vRight, &vTemp, &vLook);
	//D3DXVec3Normalize(&vRight, &vRight);
	//D3DXVec3Cross(&vUp, &vLook, &vRight);
	//D3DXVec3Normalize(&vUp, &vUp);

	//// 9. Transform 반영
	//m_pTransformCom->Set_State(STATE::RIGHT, vRight);
	//m_pTransformCom->Set_State(STATE::UP, vUp);
	//m_pTransformCom->Set_State(STATE::LOOK, vLook);
	//m_pTransformCom->Set_State(STATE::POSITION, vNewCameraPos);

	/*if (m_bCameraTransition)
	{
		m_fCamTransitionTimer += fTimeDelta;
		_float fT = m_fCamTransitionTimer / m_fCamTransitionDuration;
		if (fT >= 1.f)
		{
			fT = 1.f;
			m_bCameraTransition = false;
		}

		_float3 vNewCamPos = Lerp(m_vCamTransitionStartPos, m_vCamTransitionTargetPos, fT);
		_float3 vLookAtPos = m_pTargetPlayerTransformCom->Get_State(STATE::POSITION);
		_float3 vLook = vLookAtPos - vNewCamPos;
		D3DXVec3Normalize(&vLook, &vLook);

		_float3 vRight, vUp, vTemp{ 0.f, 1.f, 0.f };
		D3DXVec3Cross(&vRight, &vTemp, &vLook);
		D3DXVec3Normalize(&vRight, &vRight);
		D3DXVec3Cross(&vUp, &vLook, &vRight);
		D3DXVec3Normalize(&vUp, &vUp);

		m_pTransformCom->Set_State(STATE::RIGHT, vRight);
		m_pTransformCom->Set_State(STATE::UP, vUp);
		m_pTransformCom->Set_State(STATE::LOOK, vLook);
		m_pTransformCom->Set_State(STATE::POSITION, vNewCamPos);
		return;
	}

	_float3 vPlayerPos = m_pTargetPlayerTransformCom->Get_State(STATE::POSITION);
	_float3 vCameraPos = m_pTransformCom->Get_State(STATE::POSITION);

	_float fZRatio = (vPlayerPos.z - m_fZMin) / (m_fZMax - m_fZMin);
	fZRatio = max(0.f, min(fZRatio, 1.f));

	_float fYOffset = Lerp(4.5f, 8.5f, fZRatio);
	_float fZTargetOffset = Lerp(8.0f, 10.5f, fZRatio);

	_float fX = vPlayerPos.x;

	_float fTargetZ = vPlayerPos.z - fZTargetOffset;
	_float fZDeadZone = 1.5f;
	_float fFinalZ = (fabsf(vCameraPos.z - fTargetZ) > fZDeadZone)
		? Lerp(vCameraPos.z, fTargetZ, fTimeDelta * 2.f)
		: vCameraPos.z;

	_float3 vNewCameraPos = { fX, vPlayerPos.y + fYOffset, fFinalZ };
	_float3 vLookAtPos = vPlayerPos;
	_float3 vLook = vLookAtPos - vNewCameraPos;
	D3DXVec3Normalize(&vLook, &vLook);

	_float3 vRight, vUp, vTemp{ 0.f, 1.f, 0.f };
	D3DXVec3Cross(&vRight, &vTemp, &vLook);
	D3DXVec3Normalize(&vRight, &vRight);
	D3DXVec3Cross(&vUp, &vLook, &vRight);
	D3DXVec3Normalize(&vUp, &vUp);

	m_pTransformCom->Set_State(STATE::RIGHT, vRight);
	m_pTransformCom->Set_State(STATE::UP, vUp);
	m_pTransformCom->Set_State(STATE::LOOK, vLook);
	m_pTransformCom->Set_State(STATE::POSITION, vNewCameraPos);*/

}

CCamera_Follow* CCamera_Follow::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCamera_Follow* pInstance = new CCamera_Follow(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CCamera_Follow"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCamera_Follow::Clone(void* pArg)
{
	CCamera_Follow* pInstance = new CCamera_Follow(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CCamera_Follow"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Follow::Free()
{
	m_pGameInstance->Unsubscribe(ENUM_CLASS(EVENT_TYPE::ROOMCHANGE), this);

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTargetPlayerTransformCom);

	for (auto& pObj : m_vRotateObjectsTransformCom)
		Safe_Release(pObj);

	m_vRotateObjectsTransformCom.clear();

	__super::Free();
} 
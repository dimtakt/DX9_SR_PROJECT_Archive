#include "Camera_Follow.h"
#include "GameInstance.h"

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
	CameraDesc.fSpeedPerSec = 50.f;
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
	/*m_fMaxXRange = 5.f;
	m_fZMin = -13.f;
	m_fZMax = 13.f;
	m_fDistance = 30.f;
	m_bCameraTransition = false;
	m_fCamTransitionDuration = 0.5f;
	m_fXTransitionDuration = 0.5f;*/

	m_vCamTransitionStartPos = {};
	m_vCamTransitionTargetPos = {};
	m_fCamTransitionDuration = 0.4f;
	m_fCamTransitionTimer = 0.f;
	m_bCameraTransition = false;

	return S_OK;
}

void CCamera_Follow::Priority_Update(_float fTimeDelta)
{

	if (m_pGameInstance->IsKeyDown(VK_F2)) {
		Start_Shake(1.f, 0.2f);
	}

	Move_Angle(90.f, fTimeDelta);
	_float3 vNewCameraPos = Follow_Target(fTimeDelta);
	_float3 vShakedCamPos = Apply_Shake(vNewCameraPos, fTimeDelta);
	m_pTransformCom->Set_State(STATE::POSITION, vShakedCamPos);
	__super::Update_VP_Matrices();
}

void CCamera_Follow::Update(_float fTimeDelta)
{
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

		m_vCamTransitionStartPos = m_pTransformCom->Get_State(STATE::POSITION);
		m_vCamTransitionTargetPos = m_vCamTransitionStartPos + _float3(0.f, 5.f, -10.f); // 줌아웃

		m_fCamTransitionTimer = 0.f;
		m_fCamTransitionDuration = 0.4f;
		m_bCameraTransition = true;
		m_eCamTransitionState = CAM_TRANS_STATE::ZOOM_OUT;
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
	//if (m_pGameInstance->IsKeyDown('Q'))
	//{
	//	m_fCurrentAngle += fAngle;
	//	if (m_fCurrentAngle >= 360.f)
	//		m_fCurrentAngle -= 360.f;

	//	m_pTargetPlayerTransformCom->RotationAccumulate(_float3{ 0.f, 1.f, 0.f }, D3DXToRadian(fAngle));
	//	//for (auto* component : m_vRotateObjectsTransformCom) {
	//	//	component->RotationAccumulate(_float3{ 0.f, 1.f, 0.f }, D3DXToRadian(fAngle));
	//	//}
	//}

	//if (m_pGameInstance->IsKeyDown('E'))
	//{
	//	m_fCurrentAngle -= fAngle;
	//	if (m_fCurrentAngle < 0.f)
	//		m_fCurrentAngle += 360.f;

	//	m_pTargetPlayerTransformCom->RotationAccumulate(_float3{ 0.f, 1.f, 0.f }, D3DXToRadian(fAngle) * -1.f);
	//	//for (auto* component : m_vRotateObjectsTransformCom) {
	//	//	component->RotationAccumulate(_float3{ 0.f, 1.f, 0.f }, D3DXToRadian(fAngle) * -1.f);
	//	//}
	//}
}

_float3 CCamera_Follow::Follow_Target(_float fTimeDelta)
{
	if (m_bFirstFrame)
	{
		m_bFirstFrame = false;

		_float3 vPlayerPos = m_pTargetPlayerTransformCom->Get_State(STATE::POSITION);

		// 초기 ZRatio 계산 및 고정
		m_fCurrentZRatio = (vPlayerPos.z - m_fZMin) / (m_fZMax - m_fZMin);
		m_fCurrentZRatio = max(0.f, min(m_fCurrentZRatio, 1.f));

		_float fYOffset = Lerp(6.5f, 10.5f, m_fCurrentZRatio);
		_float fZTargetOffset = Lerp(10.0f, 12.5f, m_fCurrentZRatio);

		_float3 vStartPos = {
			vPlayerPos.x,
			vPlayerPos.y + fYOffset,
			vPlayerPos.z - fZTargetOffset
		};
		m_pTransformCom->Set_State(STATE::POSITION, vStartPos);

		// 초기 Look 방향 고정
		_float3 vLook = vPlayerPos - vStartPos;
		D3DXVec3Normalize(&vLook, &vLook);

		_float3 vUp = { 0.f, 1.f, 0.f };
		_float3 vRight;
		D3DXVec3Cross(&vRight, &vUp, &vLook);
		D3DXVec3Normalize(&vRight, &vRight);
		D3DXVec3Cross(&vUp, &vLook, &vRight);
		D3DXVec3Normalize(&vUp, &vUp);

		m_pTransformCom->Set_State(STATE::RIGHT, vRight);
		m_pTransformCom->Set_State(STATE::UP, vUp);
		m_pTransformCom->Set_State(STATE::LOOK, vLook);

		// 바로 return하여 보간 스킵 (첫 프레임만)
		return vStartPos;
	}

	if (m_bCameraTransition)
	{
		m_fCamTransitionTimer += fTimeDelta;
		_float fT = m_fCamTransitionTimer / m_fCamTransitionDuration;
		fT = min(fT, 1.f);

		_float3 vNewCamPos;

		switch (m_eCamTransitionState)
		{
		case CAM_TRANS_STATE::ZOOM_OUT:
			vNewCamPos = Lerp(m_vCamTransitionStartPos, m_vCamTransitionTargetPos, EaseInOutSine(fT));

			if (fT >= 1.f)
			{
				// 실제 포탈 위치로 순간이동 + 줌인 준비
				_float3 vPlayerPos = m_pTargetPlayerTransformCom->Get_State(STATE::POSITION);
				_float fZRatio = (vPlayerPos.z - m_fZMin) / (m_fZMax - m_fZMin);
				fZRatio = max(0.f, min(fZRatio, 1.f));

				_float fYOffset = Lerp(4.5f, 8.5f, fZRatio);
				_float fZTargetOffset = Lerp(8.0f, 10.5f, fZRatio);

				m_vCamTransitionStartPos = vNewCamPos;
				m_vCamTransitionTargetPos = {
					vPlayerPos.x,
					vPlayerPos.y + fYOffset,
					vPlayerPos.z - fZTargetOffset
				};

				m_fCamTransitionTimer = 0.f;
				m_fCamTransitionDuration = 0.4f;
				m_eCamTransitionState = CAM_TRANS_STATE::ZOOM_IN_AFTER_TELEPORT;
			}
			break;

		case CAM_TRANS_STATE::ZOOM_IN_AFTER_TELEPORT:
			vNewCamPos = Lerp(m_vCamTransitionStartPos, m_vCamTransitionTargetPos, EaseInOutSine(fT));
			if (fT >= 1.f)
			{
				m_bCameraTransition = false;
				m_eCamTransitionState = CAM_TRANS_STATE::NONE;
			}
			break;

		default:
			break;
		}

		// 회전 완화
		_float3 vPlayerPos = m_pTargetPlayerTransformCom->Get_State(STATE::POSITION);
		_float3 vTargetLook = vPlayerPos - vNewCamPos;
		D3DXVec3Normalize(&vTargetLook, &vTargetLook);

		_float3 vCurLook = m_pTransformCom->Get_State(STATE::LOOK);
		D3DXVec3Normalize(&vCurLook, &vCurLook);

		_float fTurnSpeed = 5.f;
		_float3 vNewLook = Lerp(vCurLook, vTargetLook, fTimeDelta * fTurnSpeed);
		D3DXVec3Normalize(&vNewLook, &vNewLook);

		_float3 vUp = { 0.f, 1.f, 0.f };
		_float3 vRight;
		D3DXVec3Cross(&vRight, &vUp, &vNewLook);
		D3DXVec3Normalize(&vRight, &vRight);
		D3DXVec3Cross(&vUp, &vNewLook, &vRight);
		D3DXVec3Normalize(&vUp, &vUp);

		m_pTransformCom->Set_State(STATE::RIGHT, vRight);
		m_pTransformCom->Set_State(STATE::UP, vUp);
		m_pTransformCom->Set_State(STATE::LOOK, vNewLook);
		m_pTransformCom->Set_State(STATE::POSITION, vNewCamPos);
		return vNewCamPos;
	}

	// 일반 추적
	_float3 vPlayerPos = m_pTargetPlayerTransformCom->Get_State(STATE::POSITION);
	_float3 vCameraPos = m_pTransformCom->Get_State(STATE::POSITION);

	// Z 비율
	_float fTargetZRatio = (vPlayerPos.z - m_fZMin) / (m_fZMax - m_fZMin);
	fTargetZRatio = max(0.f, min(fTargetZRatio, 1.f));
	m_fCurrentZRatio = Lerp(m_fCurrentZRatio, fTargetZRatio, fTimeDelta * 2.f);

	// 보간 위치 계산
	_float fYOffset = Lerp(4.5f, 8.5f, m_fCurrentZRatio);
	_float fZTargetOffset = Lerp(8.0f, 10.5f, m_fCurrentZRatio);

	_float fTargetZ = vPlayerPos.z - fZTargetOffset;
	_float fZDeadZone = 1.5f;
	_float fFinalZ = (fabsf(vCameraPos.z - fTargetZ) > fZDeadZone)
		? Lerp(vCameraPos.z, fTargetZ, fTimeDelta * 2.f)
		: vCameraPos.z;

	_float fTargetX = vPlayerPos.x;
	_float fXDeadZone = 1.5f;
	_float fFinalX = (fabsf(vCameraPos.x - fTargetX) > fXDeadZone)
		? Lerp(vCameraPos.x, fTargetX, fTimeDelta * 2.f)
		: vCameraPos.x;

	_float3 vNewCameraPos = { fFinalX, vPlayerPos.y + fYOffset, fFinalZ };

	// 회전 보간
	_float3 vTargetLook = vPlayerPos - vNewCameraPos;
	D3DXVec3Normalize(&vTargetLook, &vTargetLook);

	_float3 vCurLook = m_pTransformCom->Get_State(STATE::LOOK);
	D3DXVec3Normalize(&vCurLook, &vCurLook);

	_float fTurnSpeed = 5.f;
	_float3 vNewLook = Lerp(vCurLook, vTargetLook, fTimeDelta * fTurnSpeed);
	D3DXVec3Normalize(&vNewLook, &vNewLook);

	_float3 vUp = { 0.f, 1.f, 0.f };
	_float3 vRight;
	D3DXVec3Cross(&vRight, &vUp, &vNewLook);
	D3DXVec3Normalize(&vRight, &vRight);
	D3DXVec3Cross(&vUp, &vNewLook, &vRight);
	D3DXVec3Normalize(&vUp, &vUp);

	m_pTransformCom->Set_State(STATE::RIGHT, vRight);
	m_pTransformCom->Set_State(STATE::UP, vUp);
	m_pTransformCom->Set_State(STATE::LOOK, vNewLook);
	m_pTransformCom->Set_State(STATE::POSITION, vNewCameraPos);

	return vNewCameraPos;
}

void CCamera_Follow::Start_Shake(_float fDuration, _float fIntensity)
{
	m_bShake = true;
	m_fShakeDuration = fDuration;
	m_fShakeElapsed = 0.f;
	m_fShakeIntensity = fIntensity;
}

_float3 CCamera_Follow::Apply_Shake(_float3 vBasePos, _float fTimeDelta)
{
	if (!m_bShake)
		return vBasePos;

	m_fShakeElapsed += fTimeDelta;

	if (m_fShakeElapsed >= m_fShakeDuration)
	{
		m_bShake = false;
		return vBasePos;
	}

	// 진동 감쇠
	_float fProgress = m_fShakeElapsed / m_fShakeDuration;
	_float fDampenedIntensity = m_fShakeIntensity * (1.f - fProgress);

	_float fOffsetX = (rand() % 2000 / 1000.f - 1.f) * fDampenedIntensity;
	_float fOffsetY = (rand() % 2000 / 1000.f - 1.f) * fDampenedIntensity;
	_float fOffsetZ = (rand() % 2000 / 1000.f - 1.f) * fDampenedIntensity;

	return vBasePos + _float3(fOffsetX, fOffsetY, fOffsetZ);
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
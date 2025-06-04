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
	m_fCamTransitionDuration = 1.0f;
	m_fCamTransitionTimer = 0.f;
	m_bCameraTransition = false;

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
		//auto pRoom = static_cast<const ROOMCHANGE*>(pData);
		//m_vTerrainPos = pRoom->vPosition;
		////// 기존 코드 삭제!
		// m_pTransformCom->Set_State(STATE::POSITION, m_vTerrainPos);

		/*auto pRoom = static_cast<const ROOMCHANGE*>(pData);
		m_vTerrainPos = pRoom->vPosition;

		m_vCamTransitionStartPos = m_pTransformCom->Get_State(STATE::POSITION);

		_float3 vPlayerPos = m_pTargetPlayerTransformCom->Get_State(STATE::POSITION);
		_float fZRatio = (vPlayerPos.z - m_fZMin) / (m_fZMax - m_fZMin);
		fZRatio = max(0.f, min(fZRatio, 1.f));

		_float fYOffset = Lerp(4.5f, 8.5f, fZRatio);
		_float fZTargetOffset = Lerp(8.0f, 10.5f, fZRatio);

		m_vCamTransitionTargetPos = {
			vPlayerPos.x,
			vPlayerPos.y + fYOffset,
			vPlayerPos.z - fZTargetOffset
		};

		m_bCameraTransition = true;
		m_fCamTransitionTimer = 0.f;*/

		auto pRoom = static_cast<const ROOMCHANGE*>(pData);
		m_vTerrainPos = pRoom->vPosition;

		m_vCamTransitionStartPos = m_pTransformCom->Get_State(STATE::POSITION);

		_float3 vPlayerPos = m_pTargetPlayerTransformCom->Get_State(STATE::POSITION);
		_float fZRatio = (vPlayerPos.z - m_fZMin) / (m_fZMax - m_fZMin);
		fZRatio = max(0.f, min(fZRatio, 1.f));

		_float fYOffset = Lerp(4.5f, 8.5f, fZRatio);
		_float fZTargetOffset = Lerp(8.0f, 10.5f, fZRatio);

		m_vCamTransitionTargetPos = {
			vPlayerPos.x,
			vPlayerPos.y + fYOffset,
			vPlayerPos.z - fZTargetOffset
		};

		m_bCameraTransition = true;
		m_fCamTransitionTimer = 1.f;
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
	/*_float3		vPlayerPosition = m_pTargetPlayerTransformCom->Get_State(STATE::POSITION);
	_float3		vPosition = m_pTransformCom->Get_State(STATE::POSITION);

	_float4x4	matRotY;
	D3DXMatrixRotationY(&matRotY, D3DXToRadian(m_fCurrentAngle));

	_float3 vRotatedOffset;
	D3DXVec3TransformCoord(&vRotatedOffset, &m_vOffset, &matRotY);

	_float3 vCameraPos = vPlayerPosition + vRotatedOffset;
	m_pTransformCom->Set_State(STATE::POSITION, vCameraPos);
	m_pTransformCom->Look_At(vPlayerPosition);*/

	//_float3 vPlayerPos = m_pTargetPlayerTransformCom->Get_State(STATE::POSITION);
	//_float3 vCameraRight = m_pTransformCom->Get_State(STATE::RIGHT);
	//_float3 vCameraUp = m_pTransformCom->Get_State(STATE::UP);
	//_float3 vCameraLook = m_pTransformCom->Get_State(STATE::LOOK);
	//_float3 vCameraPos = m_pTransformCom->Get_State(STATE::POSITION);

	//if (m_bCameraTransition)
	//{
	//	m_fCamTransitionTimer += fTimeDelta;
	//	_float fT = m_fCamTransitionTimer / m_fCamTransitionDuration;
	//	if (fT >= 1.f)
	//	{
	//		fT = 1.f;
	//		m_bCameraTransition = false;
	//	}

	//	_float3 vNewCamPos = Lerp(m_vCamTransitionStartPos, m_vCamTransitionTargetPos, fT);
	//	_float3 vLookAtPos = m_pTargetPlayerTransformCom->Get_State(STATE::POSITION);
	//	_float3 vLook = vLookAtPos - vNewCamPos;
	//	D3DXVec3Normalize(&vLook, &vLook);

	//	_float3 vRight, vUp, vTemp{ 0.f, 1.f, 0.f };
	//	D3DXVec3Cross(&vRight, &vTemp, &vLook);
	//	D3DXVec3Normalize(&vRight, &vRight);
	//	D3DXVec3Cross(&vUp, &vLook, &vRight);
	//	D3DXVec3Normalize(&vUp, &vUp);

	//	m_pTransformCom->Set_State(STATE::RIGHT, vRight);
	//	m_pTransformCom->Set_State(STATE::UP, vUp);
	//	m_pTransformCom->Set_State(STATE::LOOK, vLook);
	//	m_pTransformCom->Set_State(STATE::POSITION, vNewCamPos);
	//	return;
	//}

	//// 일반 추적 로직
	//_float3 vPlayerPos = m_pTargetPlayerTransformCom->Get_State(STATE::POSITION);
	//_float3 vCameraPos = m_pTransformCom->Get_State(STATE::POSITION);

	//_float fZRatio = (vPlayerPos.z - m_fZMin) / (m_fZMax - m_fZMin);
	//fZRatio = max(0.f, min(fZRatio, 1.f));

	//_float fYOffset = Lerp(4.5f, 8.5f, fZRatio);
	//_float fZTargetOffset = Lerp(8.0f, 10.5f, fZRatio);

	//_float fTargetZ = vPlayerPos.z - fZTargetOffset;
	//_float fZDeadZone = 1.5f;
	//_float fFinalZ = (fabsf(vCameraPos.z - fTargetZ) > fZDeadZone)
	//	? Lerp(vCameraPos.z, fTargetZ, fTimeDelta * 2.f)
	//	: vCameraPos.z;

	//_float fTargetX = vPlayerPos.x;
	//_float fXDeadZone = 1.5f;
	//_float fFinalX = (fabsf(vCameraPos.x - fTargetX) > fXDeadZone)
	//	? Lerp(vCameraPos.x, fTargetX, fTimeDelta * 2.f)
	//	: vCameraPos.x;

	//_float3 vNewCameraPos = { fFinalX, vPlayerPos.y + fYOffset, fFinalZ };
	//_float3 vLook = vPlayerPos - vNewCameraPos;
	//D3DXVec3Normalize(&vLook, &vLook);

	//_float3 vRight, vUp, vTemp{ 0.f, 1.f, 0.f };
	//D3DXVec3Cross(&vRight, &vTemp, &vLook);
	//D3DXVec3Normalize(&vRight, &vRight);
	//D3DXVec3Cross(&vUp, &vLook, &vRight);
	//D3DXVec3Normalize(&vUp, &vUp);

	//m_pTransformCom->Set_State(STATE::RIGHT, vRight);
	//m_pTransformCom->Set_State(STATE::UP, vUp);
	//m_pTransformCom->Set_State(STATE::LOOK, vLook);
	//m_pTransformCom->Set_State(STATE::POSITION, vNewCameraPos);
	if (m_bCameraTransition)
	{
		m_fCamTransitionTimer += fTimeDelta;
		_float fT = m_fCamTransitionTimer / m_fCamTransitionDuration;
		if (fT >= 1.f)
		{
			fT = 1.f;
			m_bCameraTransition = false;
		}

		// 가속 보간 적용
		_float fEaseT = EaseInOutSine(fT);
		_float3 vNewCamPos = Lerp(m_vCamTransitionStartPos, m_vCamTransitionTargetPos, fEaseT);

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

	// 일반 추적
	_float3 vPlayerPos = m_pTargetPlayerTransformCom->Get_State(STATE::POSITION);
	_float3 vCameraPos = m_pTransformCom->Get_State(STATE::POSITION);

	// 실제 ZRatio 계산
	_float fTargetZRatio = (vPlayerPos.z - m_fZMin) / (m_fZMax - m_fZMin);
	fTargetZRatio = max(0.f, min(fTargetZRatio, 1.f));

	// 부드럽게 따라가기
	m_fCurrentZRatio = Lerp(m_fCurrentZRatio, fTargetZRatio, fTimeDelta * 2.f);

	// 이걸로 보간값 계산
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
	_float3 vLook = vPlayerPos - vNewCameraPos;
	D3DXVec3Normalize(&vLook, &vLook);

	_float3 vRight, vUp, vTemp{ 0.f, 1.f, 0.f };
	D3DXVec3Cross(&vRight, &vTemp, &vLook);
	D3DXVec3Normalize(&vRight, &vRight);
	D3DXVec3Cross(&vUp, &vLook, &vRight);
	D3DXVec3Normalize(&vUp, &vUp);

	m_pTransformCom->Set_State(STATE::RIGHT, vRight);
	m_pTransformCom->Set_State(STATE::UP, vUp);
	m_pTransformCom->Set_State(STATE::LOOK, vLook);
	m_pTransformCom->Set_State(STATE::POSITION, vNewCameraPos);


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
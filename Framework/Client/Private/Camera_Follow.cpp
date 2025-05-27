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

	if (FAILED(Ready_Target()))
		return E_FAIL;
	
	CCamera::CAMERA_DESC CameraDesc{};

	CameraDesc.vEye = _float3(0.f, 10.f, -10.f);
	CameraDesc.vAt = m_pTargetTransformCom->Get_State(STATE::POSITION);
	CameraDesc.fFov = D3DXToRadian(60.f);
	CameraDesc.fNear = 0.1f;
	CameraDesc.fFar = 1000.f;
	CameraDesc.fSpeedPerSec = 10.f;
	CameraDesc.fRotationPerSec = D3DXToRadian(90.f);

	m_vOffset = _float3(0.f, 8.f, -8.f);
	m_fCurrentAngle = 0.f;

	if (FAILED(__super::Initialize(&CameraDesc)))
		return E_FAIL;

	Safe_AddRef(m_pTargetTransformCom);



 	m_vRotateObjectsTransformCom.push_back(dynamic_cast<CTransform*>(
		m_pGameInstance->Get_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Layer_Weapon"), TEXT("Com_Transform"))));


	return S_OK;
}

void CCamera_Follow::Priority_Update(_float fTimeDelta)
{
	Move_Angle(90.f, fTimeDelta);
	Follow_Target();
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

HRESULT CCamera_Follow::Ready_Target()
{
	m_pTargetTransformCom = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Layer_Player"), TEXT("Com_Transform")));
	if (m_pTargetTransformCom == nullptr)
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

		m_pTargetTransformCom->RotationAccumulate(_float3{ 0.f, 1.f, 0.f }, D3DXToRadian(fAngle));
		for (auto* component : m_vRotateObjectsTransformCom) {
			component->RotationAccumulate(_float3{ 0.f, 1.f, 0.f }, D3DXToRadian(fAngle));
		}
	}

	if (m_pGameInstance->IsKeyDown('E'))
	{
		m_fCurrentAngle -= fAngle;
		if (m_fCurrentAngle < 0.f)
			m_fCurrentAngle += 360.f;

		m_pTargetTransformCom->RotationAccumulate(_float3{ 0.f, 1.f, 0.f }, D3DXToRadian(fAngle) * -1.f);
		for (auto* component : m_vRotateObjectsTransformCom) {
			component->RotationAccumulate(_float3{ 0.f, 1.f, 0.f }, D3DXToRadian(fAngle) * -1.f);
		}
	}
}

void CCamera_Follow::Follow_Target()
{
	_float3		vPlayerPosition = m_pTargetTransformCom->Get_State(STATE::POSITION);
	_float3		vPosition = m_pTransformCom->Get_State(STATE::POSITION);

	_float4x4	matRotY;
	D3DXMatrixRotationY(&matRotY, D3DXToRadian(m_fCurrentAngle));

	_float3 vRotatedOffset;
	D3DXVec3TransformCoord(&vRotatedOffset, &m_vOffset, &matRotY);

	_float3 vCameraPos = vPlayerPosition + vRotatedOffset;
	m_pTransformCom->Set_State(STATE::POSITION, vCameraPos);
	m_pTransformCom->Look_At(vPlayerPosition);
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
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTargetTransformCom);
}
 
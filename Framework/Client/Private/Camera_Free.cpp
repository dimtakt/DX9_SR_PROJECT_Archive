#include "Camera_Free.h"
#include "GameInstance.h"

CCamera_Free::CCamera_Free(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCamera{ pGraphic_Device }
{
}

CCamera_Free::CCamera_Free(const CCamera_Free& Prototype)
	: CCamera{ Prototype }
{
}

HRESULT CCamera_Free::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCamera_Free::Initialize(void* pArg)
{
	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	CCamera_Free::CAMERA_DESC CameraDesc{};

	CameraDesc.vEye = _float3(0.f, 10.f, -10.f);
	CameraDesc.vAt = _float3(10.f, 0.f, 10.f);
	CameraDesc.fFov = D3DXToRadian(60.f);
	CameraDesc.fNear = 0.1f;
	CameraDesc.fFar = 1000.f;
	CameraDesc.fSpeedPerSec = 10.f;
	CameraDesc.fRotationPerSec = D3DXToRadian(90.f);
	CameraDesc.fMouseSensor = 0.3f;

	//m_fMouseSensor = pDesc->fMouseSensor;

	if (FAILED(__super::Initialize(&CameraDesc)))
		return E_FAIL;

	m_pTargetTransformCom = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Layer_Player"), TEXT("Com_Transform")));
	if (m_pTargetTransformCom == nullptr)
		return E_FAIL;
	Safe_AddRef(m_pTargetTransformCom);

	m_OldPoint.x = g_iWinSizeX >> 1;
	m_OldPoint.y = g_iWinSizeY >> 1;

	POINT	ptMouse = m_OldPoint;
	ClientToScreen(g_hWnd, &ptMouse);

	SetCursorPos(ptMouse.x, ptMouse.y);

	m_vOffset = _float3(0.f, 8.f, -8.f);
	m_fCurrentAngle = 0.f;

	return S_OK;
}

void CCamera_Free::Priority_Update(_float fTimeDelta)
{
	Move_Angle(90.f, fTimeDelta);
	Follow_Target();
	__super::Update_VP_Matrices();
}

void CCamera_Free::Update(_float fTimeDelta)
{

}

void CCamera_Free::Late_Update(_float fTimeDelta)
{
}

HRESULT CCamera_Free::Render()
{
	return S_OK;
}

HRESULT CCamera_Free::Ready_Components(void* pArg)
{
	/* For,Com_Transform*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), pArg)))
		return E_FAIL;


	return S_OK;
}

void CCamera_Free::Mouse_Move(_float fTimeDelta)
{
	POINT			ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	_int		iMouseMove = {};

	if (iMouseMove = ptMouse.x - m_OldPoint.x)
	{
		m_pTransformCom->Turn(_float3(0.f, 1.f, 0.f), iMouseMove * fTimeDelta * m_fMouseSensor);
	}

	if (iMouseMove = ptMouse.y - m_OldPoint.y)
	{
		m_pTransformCom->Turn(m_pTransformCom->Get_State(STATE::RIGHT), iMouseMove * fTimeDelta * m_fMouseSensor);
	}
	m_OldPoint = ptMouse;
}

void CCamera_Free::Move_Angle(_float fAngle, _float fTimeDelta)
{
	if (m_pGameInstance->IsKeyDown('Q'))
	{
		m_fCurrentAngle += fAngle;
		if (m_fCurrentAngle >= 360.f)
			m_fCurrentAngle -= 360.f;

		m_pTargetTransformCom->RotationAccumulate(_float3{ 0.f, 1.f, 0.f }, D3DXToRadian(fAngle));
	}

	if (m_pGameInstance->IsKeyDown('E'))
	{
		m_fCurrentAngle -= fAngle;
		if (m_fCurrentAngle < 0.f)
			m_fCurrentAngle += 360.f;

		m_pTargetTransformCom->RotationAccumulate(_float3{ 0.f, 1.f, 0.f }, D3DXToRadian(fAngle) * -1.f);
	}
}

void CCamera_Free::Follow_Target()
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

CCamera_Free* CCamera_Free::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCamera_Free* pInstance = new CCamera_Free(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CCamera_Free"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCamera_Free::Clone(void* pArg)
{
	CCamera_Free* pInstance = new CCamera_Free(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CCamera_Free"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Free::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTargetTransformCom);
}
 
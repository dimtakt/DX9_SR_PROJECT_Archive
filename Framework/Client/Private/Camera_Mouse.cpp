#include "Camera_Mouse.h"
#include "GameInstance.h"

CCamera_Mouse::CCamera_Mouse(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCamera{ pGraphic_Device }
{
}

CCamera_Mouse::CCamera_Mouse(const CCamera_Mouse& Prototype)
	: CCamera{ Prototype }
{
}

HRESULT CCamera_Mouse::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCamera_Mouse::Initialize(void* pArg)
{
	CCamera::CAMERA_DESC CameraDesc{};

	CameraDesc.vEye = _float3(0.f, 10.f, -10.f);
	CameraDesc.vAt = _float3(0.f, 0.f, 0.f);
	CameraDesc.fFov = D3DXToRadian(60.f);
	CameraDesc.fNear = 0.1f;
	CameraDesc.fFar = 1000.f;
	CameraDesc.fSpeedPerSec = 10.f;
	CameraDesc.fRotationPerSec = D3DXToRadian(90.f);

	m_fMouseSensor = 0.2f;

	if (FAILED(Ready_Components(&CameraDesc)))
		return E_FAIL;

	if (FAILED(__super::Initialize(&CameraDesc)))
		return E_FAIL;

	Ready_Mouse();

	return S_OK;
}

void CCamera_Mouse::Priority_Update(_float fTimeDelta)
{
	Key_Input(fTimeDelta);
	Mouse_Move(fTimeDelta);
}

void CCamera_Mouse::Update(_float fTimeDelta)
{

}

void CCamera_Mouse::Late_Update(_float fTimeDelta)
{
}

HRESULT CCamera_Mouse::Render()
{
	return S_OK;
}

void CCamera_Mouse::Ready_Mouse()
{
	m_OldPoint.x = g_iWinSizeX >> 1;
	m_OldPoint.y = g_iWinSizeY >> 1;

	POINT	ptMouse = m_OldPoint;
	ClientToScreen(g_hWnd, &ptMouse);

	SetCursorPos(ptMouse.x, ptMouse.y);
}

HRESULT CCamera_Mouse::Ready_Components(void* pArg)
{
	/* For,Com_Transform*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), pArg)))
		return E_FAIL;


	return S_OK;
}

void CCamera_Mouse::Mouse_Move(_float fTimeDelta)
{
	POINT			ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	_int		iMouseMove = {};
	
	if (m_bMouseMove)
	{
		if (iMouseMove = ptMouse.x - m_OldPoint.x)
		{
			m_pTransformCom->Turn(_float3(0.f, 1.f, 0.f), iMouseMove * fTimeDelta * m_fMouseSensor);
		}

		if (iMouseMove = ptMouse.y - m_OldPoint.y)
		{
			m_pTransformCom->Turn(m_pTransformCom->Get_State(STATE::RIGHT), iMouseMove * fTimeDelta * m_fMouseSensor);
		}
	}
	__super::Update_VP_Matrices();
	m_OldPoint = ptMouse;
}

void CCamera_Mouse::Key_Input(_float fTimeDelta)
{
	if (m_pGameInstance->IsKeyHold(VK_UP))
	{
		m_pTransformCom->Go_Straight(fTimeDelta);
	}

	if (m_pGameInstance->IsKeyHold(VK_DOWN))
	{
		m_pTransformCom->Go_Backward(fTimeDelta);
	}

	if (m_pGameInstance->IsKeyHold(VK_LEFT))
	{
		m_pTransformCom->Go_Left(fTimeDelta);
	}

	if (m_pGameInstance->IsKeyHold(VK_RIGHT))
	{
		m_pTransformCom->Go_Right(fTimeDelta);
	}

	if (m_pGameInstance->IsKeyDown('Q'))
	{
		if (m_bMouseMove)
			m_bMouseMove = false;
		else
			m_bMouseMove = true;
	}
}

CCamera_Mouse* CCamera_Mouse::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCamera_Mouse* pInstance = new CCamera_Mouse(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CCamera_Mouse"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCamera_Mouse::Clone(void* pArg)
{
	CCamera_Mouse* pInstance = new CCamera_Mouse(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CCamera_Mouse"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Mouse::Free()
{
	Safe_Release(m_pTransformCom);

	__super::Free();
}
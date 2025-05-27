#include "Dagger.h"
#include "GameInstance.h"

CDagger::CDagger(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CDagger::CDagger(const CDagger& Prototype)
	: CGameObject{ Prototype }
{
}

HRESULT CDagger::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CDagger::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CDagger::Priority_Update(_float fTimeDelta)
{
}

void CDagger::Update(_float fTimeDelta)
{
	// ksta : test
	
	// Q, E 시 따라서 돌아가지 않음..
	// 이건 나중에 Transform 쪽이나 Object Manager에 따로 함수 만들어서
	// 거기서 루프 돌리면서 2d 오브젝트 전부 다 돌려줘야 할 듯?
	

	// 기준점 옮기는 건 정점 위치를 수정하는 편이 더 쉬울듯

	// 플레이어에 붙어서 이동 및 커서방향에 맞게 돌도록 처리
	Follow_Player();
	


	// ksta : 임시 (좌표확인용)
	if (m_pGameInstance->IsKeyHold(VK_UP))
		m_pTransformCom->Go_Straight(fTimeDelta);

	if (m_pGameInstance->IsKeyHold(VK_DOWN))
		m_pTransformCom->Go_Backward(fTimeDelta);

	if (m_pGameInstance->IsKeyHold(VK_LEFT))
		m_pTransformCom->Go_Left(fTimeDelta);

	if (m_pGameInstance->IsKeyHold(VK_RIGHT))
		m_pTransformCom->Go_Right(fTimeDelta);

	if (m_pGameInstance->IsKeyHold('J'))
		m_pTransformCom->Turn({0, 1, 0}, fTimeDelta);

	if (m_pGameInstance->IsKeyHold('K'))
		m_pTransformCom->Turn({0, -1, 0}, fTimeDelta);
	
	_float3 vPos = {}, vLook = {};    // 단검 좌표
	vPos  = m_pTransformCom->Get_State(STATE::POSITION);
	vLook = m_pTransformCom->Get_State(STATE::LOOK);
	std::cout << "[Dagger::Update] Pos  : {" << vPos.x << ", " << vPos.y << ", " << vPos.z << "}" << std::endl;
	std::cout << "[Dagger::Update] Look : {" << vLook.x << ", " << vLook.y << ", " << vLook.z << "}" << std::endl;


}

void CDagger::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_BLEND, this);
}

HRESULT CDagger::Render()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//SetUp_RenderState();

	m_pTransformCom->Bind_Matrix();

	m_pAnimatorCom->Update_State();

	m_pVIBufferCom->Bind_Buffers();
	m_pVIBufferCom->Render();

	if (m_isFlippedX)
	{
		m_pVIBufferCom->ResetUV_FlipX();
		m_isFlippedX = false;
	}

	//Reset_RenderState();
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	return S_OK;
}

HRESULT	CDagger::Ready_Components()
{
	/* For Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;


	/* For Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Weapon_Dagger"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;


	/* For Com_Transform */
	CTransform::TRANSFORM_DESC TransformDesc{};
	TransformDesc.fSpeedPerSec = 1.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(45.f);

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	m_pTransformCom->Scaling(float(7) / 18, float(15) / 19, 1);

	/* For Com_Animator */
	CAnimator::ANIMSTATE_DESC StartAnimStateDesc{};
	StartAnimStateDesc.strTimerTag = L"Animator_Dagger_Main";   // 해당 애니메이터가 타이머에서 사용할 태그 key값

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Animator"),
		TEXT("Com_Animator"), reinterpret_cast<CComponent**>(&m_pAnimatorCom), &StartAnimStateDesc)))
		return E_FAIL;

	m_pAnimatorCom->Add_State(L"Idle", { m_pTextureCom, 4, true });



	/* Get Player Transform to m_pTargetTransformCom */
	m_pTargetTransformCom = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Layer_Player"), TEXT("Com_Transform")));



	return S_OK;
}

void CDagger::SetUp_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
}

void CDagger::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	m_pGraphic_Device->SetTexture(0, NULL);
}

void CDagger::Follow_Player()
{
	_float3 vPlayerPos = {};    // 플레이어 좌표
	_float3 vDaggerPos = {};	// 단검 좌표
	_float3 vPlayerLook = {};	// 플레이어 바라보는방향
	vPlayerPos = m_pTargetTransformCom->Get_State(STATE::POSITION);
	vDaggerPos = m_pTransformCom->Get_State(STATE::POSITION);
	vPlayerLook = m_pTargetTransformCom->Get_State(STATE::LOOK);

	_float3 vRotatePoint = {};	// 회전의 기준이 될 좌표


	_float fPointY = vPlayerPos.y;       // 교차 평면의 기준이 될 Y값
	_float3 vRayPoint = {};     // fPointY 값 기준 마우스 Ray와 교차하는 좌표
	m_pGameInstance->Get_IntersectAtY(fPointY, vRayPoint);

	_float3 vPlayerScale = {};
	vPlayerScale = m_pTransformCom->Get_Scaled();
	if (vPlayerScale.x < 0)		vPlayerScale.x *= -1;





	// 상하에 따른 Rotate 할 기준좌표 변경
	if (vRayPoint.z > vPlayerPos.z)
		vRotatePoint.z = 0.1f;
	else
		vRotatePoint.z = -0.1f;

	// 좌우에 따른 Rotate 할 기준좌표 변경
	if (vRayPoint.x < vPlayerPos.x)	// 좌측
		vRotatePoint.x = -0.2f;		// 기준좌표변경
	else							// 우측
		vRotatePoint.x = 0.2f;		// 기준좌표변경

	vRotatePoint.y = -0.01f;
	_float angle = atan2f(vRayPoint.x - vPlayerPos.x, vRayPoint.z - vPlayerPos.z);

	m_pTransformCom->Set_State(STATE::POSITION, vPlayerPos + vRotatePoint);
	m_pTransformCom->Set_State(STATE::LOOK, vPlayerLook);

	if (vRayPoint.x < vPlayerPos.x)			// 좌측
	{
		m_pTransformCom->Rotation({ 0, 0, -1 }, angle + D3DXToRadian(45));
		m_pTransformCom->Scaling(-vPlayerScale.x, vPlayerScale.y, vPlayerScale.z);
	}
	else									// 우측
	{
		m_pTransformCom->Rotation({ 0, 0, 1 }, -angle + D3DXToRadian(45));
		m_pTransformCom->Scaling(vPlayerScale.x, vPlayerScale.y, vPlayerScale.z);
	}



	return;
}

CDagger* CDagger::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CDagger* pInstance = new CDagger(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CDagger"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CDagger::Clone(void* pArg)
{
	CDagger* pInstance = new CDagger(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CDagger"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDagger::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pAnimatorCom);
}
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
	if (m_pTargetTransformCom == nullptr)
		m_pTargetTransformCom = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Layer_Player"), TEXT("Com_Transform")));

}

void CDagger::Update(_float fTimeDelta)
{



	// ksta : test

	_float fAngle = 90.f;

	if (m_pGameInstance->IsKeyDown('Q'))
	{
		m_fCurrentAngle += fAngle;
		if (m_fCurrentAngle >= 360.f)
			m_fCurrentAngle -= 360.f;
	}

	if (m_pGameInstance->IsKeyDown('E'))
	{
		m_fCurrentAngle -= fAngle;
		if (m_fCurrentAngle < 0.f)
			m_fCurrentAngle += 360.f;
	}
	
	// ----

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
	
	_float3 vPos = {}, vRight = {}, vUp = {}, vLook = {};    // 단검 좌표
	vPos  = m_pTransformCom->Get_State(STATE::POSITION);

	vRight = m_pTransformCom->Get_State(STATE::RIGHT);
	vUp = m_pTransformCom->Get_State(STATE::UP);
	vLook = m_pTransformCom->Get_State(STATE::LOOK);
	std::cout << "[Dagger::Update] Pos   : {" << vPos.x << ", " << vPos.y << ", " << vPos.z << "}" << std::endl;
	std::cout << "[Dagger::Update] Right : {" << vRight.x << ", " << vRight.y << ", " << vRight.z << "}" << std::endl;
	std::cout << "[Dagger::Update] Up    : {" << vUp.x << ", " << vUp.y << ", " << vUp.z << "}" << std::endl;
	std::cout << "[Dagger::Update] Look  : {" << vLook.x << ", " << vLook.y << ", " << vLook.z << "}" << std::endl;
	std::cout << "[Dagger::Update] ================================================================" << std::endl;



}

void CDagger::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_BLEND, this);
}

HRESULT CDagger::Render()
{
	SetUp_RenderState();

	m_pTransformCom->Bind_Matrix();

	m_pAnimatorCom->Update_State();

	m_pVIBufferCom->Bind_Buffers();
	m_pVIBufferCom->Render();

	if (m_isFlippedX)
	{
		m_pVIBufferCom->ResetUV_FlipX();
		m_isFlippedX = false;
	}

	Reset_RenderState();
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
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
}

void CDagger::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	m_pGraphic_Device->SetTexture(0, NULL);
}

void CDagger::Follow_Player()
{
	_float3 vPlayerPos = {};    // 플레이어 좌표
	_float3 vPlayerLook = {};	// 플레이어 바라보는방향
	
	_float3 vPlayerScale = {};	// 플레이어 스케일

	vPlayerPos = m_pTargetTransformCom->Get_State(STATE::POSITION);
	vPlayerLook = m_pTargetTransformCom->Get_State(STATE::LOOK);
	vPlayerScale = m_pTargetTransformCom->Get_Scaled();

	_float3 vDaggerPos = {};	// 단검 좌표
	vDaggerPos = m_pTransformCom->Get_State(STATE::POSITION);

	_float3 vRotatePoint = {};	// 회전의 기준이 될 좌표

	_float fPointY = vPlayerPos.y;       // 교차 평면의 기준이 될 Y값
	_float3 vRayPoint = {};     // fPointY 값 기준 마우스 Ray와 교차하는 좌표

	m_pGameInstance->Get_IntersectAtY(fPointY, vRayPoint);

	if (vPlayerScale.x < 0)		vPlayerScale.x *= -1;
	
	// 마지막에 사용할 변환용 행렬 선언
	_float4x4 matLocalScale, matLocalRot, matLocalTrans;
	_float4x4 matLocal;

	// 회전의 기준점이 될 좌표 설정
	_float3 vOffset = {
		vRayPoint.x < vPlayerPos.x ? -0.2f : 0.2f,
		vRayPoint.z > vPlayerPos.z ? 0.1f : -0.1f,
		-0.01f
	};


	// 변환용 행렬 (이동)
	D3DXMatrixTranslation(&matLocalTrans, vOffset.x, vOffset.y, vOffset.z);

	// 커서위치에 따른 회전 방향 및 회전 정도 설정
	// 변환용 행렬 (회전, 크기)
	_float angle = atan2f(vRayPoint.x - vPlayerPos.x, vRayPoint.z - vPlayerPos.z);
	angle -= D3DXToRadian(m_fCurrentAngle);

	if (vRayPoint.x < vPlayerPos.x)			// 좌측
	{
		D3DXMatrixScaling(&matLocalScale, -vPlayerScale.x * (7 / 18.f), vPlayerScale.y * (15 / 19.f), vPlayerScale.z);
		D3DXMatrixRotationZ(&matLocalRot, -angle + D3DXToRadian(-45));
	}
	else									// 우측
	{
		D3DXMatrixScaling(&matLocalScale, vPlayerScale.x * (7 / 18.f), vPlayerScale.y * (15 / 19.f), vPlayerScale.z);
		D3DXMatrixRotationZ(&matLocalRot, -angle + D3DXToRadian(45));
	}
	
	D3DXMatrixIdentity(&matLocal);
	matLocal = matLocalScale * matLocalRot * matLocalTrans;


	//	1. 월드 행렬 선언
	//	2. 우선 원점 기준으로 로컬상의 변경사항 선반영하여 Transform 반영
	//	3. 반영된 행렬에 플레이어 행렬을 곱하여 최종 Transform 완성
	
	_float4x4 matPlayerWorld = *m_pTargetTransformCom->Get_WorldMatrix();

	_float4x4 matDaggerWorld;
	D3DXMatrixMultiply(&matDaggerWorld, &matLocal, &matPlayerWorld);

	// 최종 변환 행렬을 단검에
	for (int i = 0; i < 3; i++)
		m_pTransformCom->Set_State(STATE(i), *reinterpret_cast<_float3*>(&matDaggerWorld.m[i]));
	
	m_pTransformCom->Set_State(STATE::POSITION, *reinterpret_cast<_float3*>(&matDaggerWorld.m[3]));

	m_isFlippedX = vRayPoint.x < vPlayerPos.x;

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
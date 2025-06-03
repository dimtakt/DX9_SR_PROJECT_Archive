#include "PlayerEffect.h"
#include "GameInstance.h"

CPlayerEffect::CPlayerEffect(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CEffect(pGraphic_Device)
{

}

CPlayerEffect::CPlayerEffect(const CPlayerEffect& Prototype)
	: CEffect(Prototype)
{
}

HRESULT CPlayerEffect::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlayerEffect::Initialize(void* pArg)
{
	// 이펙트 태그 반영

	EFFECT_DESC* pDesc = reinterpret_cast<EFFECT_DESC*>(pArg);
	m_strEffectTag = pDesc->strEffectTag;
	m_isFlippedX = pDesc->isFlippedX;

	if (FAILED(Ready_Components()))
		return E_FAIL;


	// 위치 반영

	_float4x4 matTransform;

	if (pDesc->isMatWorld)			// 월드 행렬이 주어진 경우
		matTransform = pDesc->matWorld;
	else							// 월드 행렬이 주어지지 않은 경우
	{
		_float4x4 matScale, matRot, matTrans;

		_float3 vPos = pDesc->vPos;
		D3DXQUATERNION qRot = pDesc->qRot;
		_float3 vScale = pDesc->vScale;

		D3DXMatrixScaling(&matScale, vScale.x, vScale.y, vScale.z);
		D3DXMatrixRotationQuaternion(&matRot, &qRot);
		D3DXMatrixTranslation(&matTrans, vPos.x, vPos.y, vPos.z);
		
		matTransform = matScale * matRot * matTrans;
	}
	//std::cout << "[PlayerEffect::Initialize] EffectPos : " << matTransform.m[3][0] << ", " << matTransform.m[3][1] << ", " << matTransform.m[3][2] << std::endl;


	for (int i = 0; i < 3; i++)
		m_pTransformCom->Set_State(STATE(i), *reinterpret_cast<_float3*>(&matTransform.m[i]));
	m_pTransformCom->Set_State(STATE::POSITION, *reinterpret_cast<_float3*>(&matTransform.m[3]));

	m_pFollowTransformCom = nullptr;
	if (pDesc->pFollowTransformCom != nullptr)
	{
		m_pFollowTransformCom = pDesc->pFollowTransformCom;
		_float3 vTargetPos = pDesc->pFollowTransformCom->Get_State(STATE::POSITION);
		m_vFollowOffset = vTargetPos - m_pTransformCom->Get_State(STATE::POSITION);
	}
	m_vThrownDir = pDesc->vThrownDir;
	m_fThrownPower = pDesc->fThrownPower;
	m_fLifeTimeSec = pDesc->fLifeTimeSec;

	m_iStackedFrame = 0;

	return S_OK;
}

void CPlayerEffect::Priority_Update(_float fTimeDelta)
{

}

void CPlayerEffect::Update(_float fTimeDelta)
{
	if (m_pAnimatorCom->Get_IsLastFrame() &&
		m_iStackedFrame >= m_fLifeTimeSec * 60.f)
		m_bDead = true;

	// 생성 위치 기준 해당 객체 따라가도록
	if (m_pFollowTransformCom != nullptr)
	{
		// Initialize 시 타겟과 이펙트 생성 위치의 오프셋을 저장 후
		// 매 업데이트마다 오프셋을 적용해주는 식
		_float3 vTargetPos = m_pFollowTransformCom->Get_State(STATE::POSITION);
		// 타겟에서 m_vFollowOffset 을 뺄셈하면 의도한 이펙트의 좌표가 됨
		_float3 vResultPos = vTargetPos - m_vFollowOffset;
		std::cout << "vResultPos = " << vResultPos.x << ", " << vResultPos.y << ", " << vResultPos.z << std::endl;
		m_pTransformCom->Set_State(STATE::POSITION, vResultPos);
	}

	// 해당 방향으로 날아가도록
	if (m_fThrownPower != 0)
	{
		_float3 vEffectPos = m_pTransformCom->Get_State(STATE::POSITION);
		_float3 vThrownDir = m_vThrownDir * m_fThrownPower * fTimeDelta;
		vEffectPos += vThrownDir;
		m_pTransformCom->Set_State(STATE::POSITION, vEffectPos);
	}


	m_iStackedFrame++;
}

void CPlayerEffect::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_BLEND, this);
}

HRESULT CPlayerEffect::Render()
{
	SetUp_RenderState();


	_float fPointY = 0.f;       // 교차 평면의 기준이 될 Y값
	_float3 vRayPoint = {};     // fPointY 값 기준 마우스 Ray와 교차하는 좌표
	m_pGameInstance->Get_IntersectAtY(fPointY, vRayPoint);

	_float3 vPlayerPos = {};    // 플레이어 좌표
	vPlayerPos = m_pTransformCom->Get_State(STATE::POSITION);


	m_pTransformCom->Bind_Matrix();

	m_pAnimatorCom->Update_State();

	m_pVIBufferCom->Bind_Buffers();
	m_pVIBufferCom->Render();



	Reset_RenderState();
	return S_OK;
}

HRESULT CPlayerEffect::Ready_Components()
{
	/* For.Com_VIBuffer_Rect */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), m_strEffectTag,
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For Com_Animator */
	CAnimator::ANIMSTATE_DESC StartAnimStateDesc{};
	StartAnimStateDesc.strTimerTag = L"Animator_Effect_Main";   // 해당 애니메이터가 타이머에서 사용할 태그 key값

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Animator"),
		TEXT("Com_Animator"), reinterpret_cast<CComponent**>(&m_pAnimatorCom), &StartAnimStateDesc)))
		return E_FAIL; 

	m_pAnimatorCom->Add_State(L"Effect", { m_pTextureCom, 5, false });

	return S_OK;
}

void CPlayerEffect::SetUp_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	//m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	//m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
}

void CPlayerEffect::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	//m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	//m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	//m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	//m_pGraphic_Device->SetTexture(0, NULL);
}


CGameObject* CPlayerEffect::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPlayerEffect* pInstance = new CPlayerEffect(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CPlayerEffect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPlayerEffect::Clone(void* pArg)
{
	CPlayerEffect* pInstance = new CPlayerEffect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CPlayerEffect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayerEffect::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pAnimatorCom);
}

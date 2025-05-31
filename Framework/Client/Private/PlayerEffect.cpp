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


	for (int i = 0; i < 3; i++)
		m_pTransformCom->Set_State(STATE(i), *reinterpret_cast<_float3*>(&matTransform.m[i]));
	m_pTransformCom->Set_State(STATE::POSITION, *reinterpret_cast<_float3*>(&matTransform.m[3]));

	return S_OK;
}

void CPlayerEffect::Priority_Update(_float fTimeDelta)
{

}

void CPlayerEffect::Update(_float fTimeDelta)
{
	if (m_pAnimatorCom->Get_IsLastFrame())
		m_bDead = true;
}

void CPlayerEffect::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_BLEND, this);
}

HRESULT CPlayerEffect::Render()
{
	m_pTransformCom->Bind_Matrix();

	m_pAnimatorCom->Update_State();

	m_pVIBufferCom->Bind_Buffers();
	m_pVIBufferCom->Render();

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

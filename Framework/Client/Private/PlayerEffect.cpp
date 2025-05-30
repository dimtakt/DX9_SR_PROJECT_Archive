#include "PlayerEffect.h"
#include "GameInstance.h"

BEGIN(Client)

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
	if (FAILED(Ready_Components()))
		return E_FAIL;


	// 이펙트 태그 반영

	EFFECT_DESC* pDesc = reinterpret_cast<EFFECT_DESC*>(pArg);
	m_strEffectTag = pDesc->strEffectTag;


	// 위치 반영

	_float4x4 matScale, matRot, matTrans;

	_float3 vPos = pDesc->vPos;
	D3DXQUATERNION qRot = pDesc->qRot;
	_float3 vScale = pDesc->vScale;

	D3DXMatrixScaling(&matScale, vScale.x, vScale.y, vScale.z);
	D3DXMatrixRotationQuaternion(&matRot, &qRot);
	D3DXMatrixTranslation(&matTrans, vPos.x, vPos.y, vPos.z);

	_float4x4 matTransform = matScale * matRot * matTrans;

	for (int i = 0; i < 3; i++)
		m_pTransformCom->Set_State(STATE(i), *reinterpret_cast<_float3*>(&matTransform.m[i]));
	m_pTransformCom->Set_State(STATE::POSITION, *reinterpret_cast<_float3*>(&matTransform.m[3]));


	// 장당 프레임 수 지정
	//m_iImagePerFrame = pDesc->iFramePerImage;


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
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture"),
		m_strEffectTag, reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For Com_Animator */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Effect"),
		TEXT("Com_Animator"), reinterpret_cast<CComponent**>(&m_pAnimatorCom))))
		return E_FAIL;

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

END

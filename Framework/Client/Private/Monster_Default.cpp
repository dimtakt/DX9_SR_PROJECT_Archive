#include "Monster_Default.h"
#include "GameInstance.h"

CMonster_Default::CMonster_Default(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CMonster{ pGraphic_Device }
{
}

CMonster_Default::CMonster_Default(const CMonster_Default& Prototype)
	: CMonster{ Prototype }
{
}

HRESULT CMonster_Default::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMonster_Default::Initialize(void* pArg)
{
	/*MONSTER_DESC* pDesc = static_cast<MONSTER_DESC*>(pArg);

	m_iData = pDesc->iData;*/

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Set_State(STATE::POSITION, _float3(
		m_pGameInstance->Compute_Random(0.f, 50.f),
		0.f,
		m_pGameInstance->Compute_Random(0.f, 50.f)));

	return S_OK;
}

void CMonster_Default::Priority_Update(_float fTimeDelta)
{


	int a = 10;
}

void CMonster_Default::Update(_float fTimeDelta)
{
	
}

void CMonster_Default::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CMonster_Default::Render()
{
	m_pTransformCom->Bind_Matrix();

	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;

	/* 그리기위해 이용할 자원과 설정들을 장치에 바인딩한다. */
	m_pVIBufferCom->Bind_Buffers();

	SetUp_RenderState();

	m_pVIBufferCom->Render();

	Reset_RenderState();

	return S_OK;
}

HRESULT CMonster_Default::Ready_Components()
{
	/* For.Com_VIBuffer*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_Component_Texture_Monster"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC		TransformDesc{};
	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

void CMonster_Default::SetUp_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	/* 알파 테스트를 수행한다. */
	/* 현재 픽셀을 그릴때 픽셀에 알파값을 비교한다. 통과한 픽셀만 그린다. */
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

	/* 현재 그리려고하는 픽셀과 0의 값을 비교한다,ㅡ /.*/
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 125);

	/* 위에서 비교한 결과, 내가 그릴려고하는 픽셀의 알파가 0보다 크면 통과. */
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
}

void CMonster_Default::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}

CMonster_Default* CMonster_Default::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMonster_Default* pInstance = new CMonster_Default(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CTerrain"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CMonster_Default::Clone(void* pArg)
{
	CMonster_Default* pInstance = new CMonster_Default(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CTerrain"));
		Safe_Release(pInstance);
	}

	return pInstance;
}
void CMonster_Default::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);

}

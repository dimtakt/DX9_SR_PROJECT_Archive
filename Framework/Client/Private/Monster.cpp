#include "Monster.h"
#include "GameInstance.h"

CMonster::CMonster(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CMonster::CMonster(const CMonster& Prototype)
	: CGameObject{ Prototype }
{
}

HRESULT CMonster::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMonster::Initialize(void* pArg)
{

	if (FAILED(Ready_Components()))
		return E_FAIL;

	MONSTERDESC* desc = static_cast<MONSTERDESC*>(pArg);

	CTransform* pTerrainTransform = dynamic_cast<CTransform*>(desc->pTerrainBox->Find_Component(TEXT("Com_Transform_TerrainBox")));
	_float3 fTerrainPos = pTerrainTransform->Get_State(STATE::POSITION);
	_float3 fTerrainScale = pTerrainTransform->Get_Scaled();
	m_pTransformCom->Set_State(STATE::POSITION, _float3(
		m_pGameInstance->Compute_Random(fTerrainPos.x - fTerrainScale.x/2.f, fTerrainPos.x + fTerrainScale.x / 2.f),
		0.f,
		m_pGameInstance->Compute_Random(fTerrainPos.z - fTerrainScale.z / 2.f, fTerrainPos.z + fTerrainScale.z / 2.f)));
	//m_pTransformCom->Set_State(STATE::POSITION, desc->vPosition);
	m_pTerrainBox = desc->pTerrainBox;
	Safe_AddRef(m_pTerrainBox);
	m_eObjType = GAMEOBJ_TYPE::MONSTER;

	return S_OK;
}

void CMonster::Priority_Update(_float fTimeDelta)
{
}

void CMonster::Update(_float fTimeDelta)
{

}

void CMonster::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CMonster::Render()
{
	m_pTransformCom->Bind_Matrix();

	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;

	/* 그리기위해 이용할 자원과 설정들을 장치에 바인딩한다. */
	m_pVIBufferCom->Bind_Buffers();

	SetUp_RenderState();

	m_pVIBufferCom->Render();

	//m_pTerrainBox->Render();
	m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, TRUE);
	Reset_RenderState();

	return S_OK;
}

HRESULT CMonster::Ready_Components()
{
	
	/* For.Com_VIBuffer*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer_Monster"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_Component_Texture_Monster"),
		TEXT("Com_Texture_Monster"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC		TransformDesc{};
	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform_Monster"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;
	return S_OK;
}

void CMonster::SetUp_RenderState()
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

void CMonster::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}

void CMonster::OnCollision(CGameObject* pGameObject)
{
	//CTransform* pTransform = dynamic_cast<CTransform*>(pGameObject->Find_Component(TEXT("Com_Transform")));
	//switch (pGameObject->Get_ObjType())
	//{
	//case GAMEOBJ_TYPE::PLAYER:
	//	{
	//		/*_float3 vDir = m_pTransformCom->Get_State(STATE::POSITION) - pTransform->Get_State(STATE::POSITION);*/
	//		//m_pTransformCom->Look_At(pTransform->Get_State(STATE::POSITION));
	//		m_pTransformCom->Move_To(pTransform->Get_State(STATE::POSITION), 0.01f, 0.05f);
	//		break;
	//	}
	//}
}

CMonster* CMonster::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMonster* pInstance = new CMonster(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CMonster"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMonster::Clone(void* pArg)
{
	CMonster* pInstance = new CMonster(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CMonster"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMonster::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTerrainBox);
}

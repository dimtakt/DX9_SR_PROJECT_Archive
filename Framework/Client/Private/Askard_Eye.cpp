#include "Askard_Eye.h"
#include "Effect_Factory.h"

#include "Room_Manager.h"

CAskard_Eye::CAskard_Eye(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CMonster{ pGraphic_Device }
{
}

CAskard_Eye::CAskard_Eye(const CAskard_Eye& Prototype)
	: CMonster{ Prototype }
{
}

HRESULT CAskard_Eye::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CAskard_Eye::Initialize(void* pArg)
{
	//if (FAILED(__super::Initialize(pArg)))
	//	return E_FAIL;

	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	if (FAILED(this->Ready_Components(pArg)))
		return E_FAIL;



	//desc..

	EYE_DESC* desc = static_cast<EYE_DESC*>(pArg);

	CTransform* pTerrainTransform = dynamic_cast<CTransform*>(desc->pTerrainBox->Find_Component(TEXT("Com_Transform_TerrainBox")));
	m_pTerrainTransformCom = pTerrainTransform;
	_float3 fTerrainPos = pTerrainTransform->Get_State(STATE::POSITION);
	_float3 fTerrainScale = pTerrainTransform->Get_Scaled();
	m_pTransformCom->Scaling(3.f, 3.f, 3.f);

	m_pTransformCom->Set_State(STATE::POSITION, desc->vPosition);


	m_pTerrainBox = desc->pTerrainBox;
	Safe_AddRef(m_pTerrainBox);
	m_eObjType = GAMEOBJ_TYPE::MONSTER;

	m_dwHitTime = 0.f;





	// 플레이어 Transform 할당
	LEVEL eCurLevel = static_cast<LEVEL>(m_pGameInstance->Get_CurrentLevel());
	m_pTargetTransformCom = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(ENUM_CLASS(eCurLevel), TEXT("Layer_Player"), TEXT("Com_Transform")));
	if (m_pTargetTransformCom == nullptr)
		return E_FAIL;

	return S_OK;
}

void CAskard_Eye::Priority_Update(_float fTimeDelta)
{

}

void CAskard_Eye::Update(_float fTimeDelta)
{

}

void CAskard_Eye::Late_Update(_float fTimeDelta)
{

}

HRESULT CAskard_Eye::Render()
{

	return S_OK;
}

HRESULT CAskard_Eye::Ready_Components(void* pArg)
{

	return S_OK;
}

void CAskard_Eye::OnCollision(CGameObject* pGameObject)
{
	if (pGameObject == nullptr)
		return;
	__super::OnCollision(pGameObject);
}


CAskard_Eye* CAskard_Eye::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CAskard_Eye* pInstance = new CAskard_Eye(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CAskard_Eye"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CAskard_Eye::Clone(void* pArg)
{
	CAskard_Eye* pInstance = new CAskard_Eye(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CAskard_Eye"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CAskard_Eye::Free()
{
	m_pGameInstance->Remove_Collider_ByOwner(this);
	__super::Free();

	Safe_Release(m_pAnimatorCom);
}
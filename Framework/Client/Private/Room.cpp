#include "Room.h"
#include "GameInstance.h"

CRoom::CRoom(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject { pGraphic_Device }
{
}

CRoom::CRoom(const CRoom& Prototype)
	: CGameObject{ Prototype }
	, m_bIsActive{ Prototype.m_bIsActive }
	, m_bIsVisited{ Prototype.m_bIsVisited }
{
}

HRESULT CRoom::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CRoom::Initialize(void* pArg)
{

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

void CRoom::Priority_Update(_float fTimeDelta)
{
	if (m_bIsActive)
	{
		if (m_pTerrainBox != nullptr)
			m_pTerrainBox->Priority_Update(fTimeDelta);
		// 추후 지형 오브젝트 처리
		/*for (auto& obj : m_vObject)
		{
			obj->
		}*/

		for (auto it = m_vMonster.begin(); it != m_vMonster.end(); ) {
			if ((*it)->Get_IsDead()) {
				Safe_Release(*it);
				it = m_vMonster.erase(it); 
			}
			else {
				(*it)->Priority_Update(fTimeDelta);
				++it;
			}
		}

	}
}

void CRoom::Update(_float fTimeDelta)
{
	if (m_bIsActive)
	{
		if (m_pTerrainBox != nullptr)
			m_pTerrainBox->Update(fTimeDelta);
		// 추후 지형 오브젝트 처리
		/*for (auto& obj : m_vObject)
		{
			obj->
		}*/

		for (auto& obj : m_vMonster)
		{
			if(nullptr != obj)
				obj->Update(fTimeDelta);
		}
	}

}

void CRoom::Late_Update(_float fTimeDelta)
{
	if (m_bIsActive)
	{
		if (m_pTerrainBox != nullptr)
			m_pTerrainBox->Late_Update(fTimeDelta);
		// 추후 지형 오브젝트 처리
		/*for (auto& obj : m_vObject)
		{
			obj->
		}*/

		for (auto& obj : m_vMonster)
		{
			if (nullptr != obj)
				obj->Late_Update(fTimeDelta);
		}
	}
}

HRESULT CRoom::Render()
{
	if (m_bIsActive)
	{
		if (m_pTerrainBox != nullptr)
			m_pTerrainBox->Render();
		// 추후 지형 오브젝트 처리
		/*for (auto& obj : m_vObject)
		{
			obj->
		}*/

		for (auto& obj : m_vMonster)
		{
			obj->Render();
		}
	}
	return S_OK;
}

HRESULT CRoom::Ready_Components(void* pArg)
{
	return S_OK;
}
HRESULT CRoom::Ready_Objects(void* pArg)
{

	return S_OK;
}

void CRoom::Enter()
{
	m_bIsActive = true;
	m_bIsVisited = true;

	for (auto& pMonster : m_vMonster)
	{
		// collider
		CCollider_OBB::OBB_DESC tColliderDesc;
		tColliderDesc.vScale = _float3(1.f, 4.f, 1.f);
		tColliderDesc.pOwner = pMonster;
		tColliderDesc.pTransform = pMonster->Get_Transform();
		CCollider_OBB* pCol = dynamic_cast<CCollider_OBB*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::COMPONENT, ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Collider_OBB"), &tColliderDesc));
		m_pGameInstance->Add_Collider(pCol);
	}
}

void CRoom::Exit()
{
	m_bIsActive = false;
}

void CRoom::Activate()
{
	m_bIsActive = true;
}

void CRoom::Deactivate()
{
	m_bIsActive = false;
}

CRoom* CRoom::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CRoom* pInstance = new CRoom(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CRoom"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CRoom::Clone(void* pArg)
{
	CRoom* pInstance = new CRoom(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CRoom"));
		Safe_Release(pInstance);
	}

	return pInstance;
}
void CRoom::Free()
{
	__super::Free();

	Safe_Release(m_pTerrainBox);
	for (auto& obj : m_vObject)
	{
		Safe_Release(obj);
	}
	m_vObject.clear();
	for (auto& obj : m_vMonster)
	{
		Safe_Release(obj);
	}
	m_vMonster.clear();
}

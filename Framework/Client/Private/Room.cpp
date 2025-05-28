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

		for (auto& obj : m_vMonster)
		{
			obj->Priority_Update(fTimeDelta);
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

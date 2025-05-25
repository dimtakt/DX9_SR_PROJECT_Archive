#include "Room_Manager.h"
#include "Room.h"
#include "GameInstance.h"
_uint CRoom_Manager::iRoomIndex = 0;

CRoom_Manager::CRoom_Manager()
	: m_pGameInstance{ CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CRoom_Manager::Initialize()
{
	return S_OK;
}

void CRoom_Manager::Priority_Update(_float fTimeDelta)
{
	for (auto& iter : m_mRooms)
	{
		iter.second->Priority_Update(fTimeDelta);
	}
}

void CRoom_Manager::Update(_float fTimeDelta)
{
	for (auto& iter : m_mRooms)
	{
		iter.second->Update(fTimeDelta);
	}
}

void CRoom_Manager::Late_Update(_float fTimeDelta)
{
	for (auto& iter : m_mRooms)
	{
		iter.second->Late_Update(fTimeDelta);
		m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, iter.second);
	}
}

HRESULT CRoom_Manager::Render()
{
	return S_OK;
}

HRESULT CRoom_Manager::Add_Room(CRoom* pRoom)
{
	auto iter = m_mRooms.find(CRoom_Manager::iRoomIndex);
	if (iter != m_mRooms.end())
		m_mRooms.insert(pair<int, CRoom*>(CRoom_Manager::iRoomIndex, pRoom));
	else
		return E_FAIL;

	Safe_AddRef(pRoom);
	return S_OK;
}

HRESULT CRoom_Manager::Enter_Room(_int iRoomID)
{
	auto iter = m_mRooms.find(iRoomID);
	if (iter != m_mRooms.end())
		iter->second->Enter();

	return S_OK;
}

CRoom* CRoom_Manager::Get_CurrentRoom()
{
	for (auto& iter : m_mRooms)
	{
		if (iter.second->GetIsActive())
			return iter.second;
	}

	return nullptr;
}

CRoom* CRoom_Manager::Get_RoomByID(_int iRoomID)
{
	auto iter = m_mRooms.find(iRoomID);

	if (iter == m_mRooms.end())
	{
		return nullptr;
	}

	return iter->second;
}

void CRoom_Manager::Clear()
{
	for (auto& iter : m_mRooms)
	{
		Safe_Release(iter.second);
	}
	m_mRooms.clear();
	CRoom_Manager::RoomIndexReset();
}

CRoom_Manager* CRoom_Manager::Create()
{
	CRoom_Manager* pInstance = new CRoom_Manager();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CRoom_Manager"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRoom_Manager::Free()
{
	__super::Free();
	Clear();
	Safe_Release(m_pGameInstance);

}

void CRoom_Manager::RoomIndexAdd()
{
	iRoomIndex++;
}

void CRoom_Manager::RoomIndexReset()
{
	iRoomIndex = 0;
}

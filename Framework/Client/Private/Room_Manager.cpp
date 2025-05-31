
#include "Room_Manager.h"
#include "Room.h"

IMPLEMENT_SINGLETON(CRoom_Manager)

_uint CRoom_Manager::iRoomIndex = 0;

CRoom_Manager::CRoom_Manager()
	: m_pGameInstance { CGameInstance::GetInstance()}
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CRoom_Manager::Initialize()
{
	return S_OK;
}

HRESULT CRoom_Manager::Add_Room(CRoom* pRoom, _uint iLayerLevelIndex, const _wstring& strLayerTag)
{
	pRoom->Insert_ID
	(CRoom_Manager::iRoomIndex);
	m_mRooms[iLayerLevelIndex].push_back(pRoom);
	if (FAILED(m_pGameInstance->Add_Direct_GameObject_ToLayer(iLayerLevelIndex, strLayerTag, pRoom)))
		return E_FAIL;
	if (iRoomIndex == 0) {
		pRoom->Enter();
		m_iCurrentLevelID = iLayerLevelIndex;
		m_iCurrentRoomID = 0;
	}
	Safe_AddRef(pRoom);
	CRoom_Manager::RoomIndexAdd();
	return S_OK;
}

HRESULT CRoom_Manager::Enter_Room(_int iRoomID)
{
	auto pair = m_mRooms.find(m_iCurrentLevelID);
	if (pair != m_mRooms.end())
	{
		for (auto& pRoom : pair->second)
		{
			if (pRoom->GetID() == iRoomID)
			{
				pRoom->Enter();
				m_iCurrentRoomID = iRoomID;
			}
			else {
				pRoom->Exit();
			}
		}
	}

	return S_OK;
}

vector<pair<_int, _int>> CRoom_Manager::Create_RandomRooms(_int iRoomMax)
{
	_int x{ 0 }, z{ 0 };

	m_RoomIndex.push_back({ x, z });
	
	for (_int i = 0; i < iRoomMax; ++i)
	{

		_int iRandom = static_cast<_int>(m_pGameInstance->Compute_Random(0.f, 8.f));

		_int nX = x;
		_int nZ = z;

		if (iRandom == 0 || iRandom == 8)
			nZ += 1;

		else if (iRandom == 1 || iRandom == 7)
			nX -= 1;
		else if (iRandom == 2 || iRandom == 6)
			nZ -= 1;
		else 
			nX += 1;

		pair<_int, _int> CheckIndex = { nX, nZ };

		if (0 != count(m_RoomIndex.begin(), m_RoomIndex.end(), CheckIndex))
			continue;

		m_RoomIndex.push_back({ nX, nZ });

		x = nX;
		z = nZ;

	}

	return vector<pair<_int, _int>>(m_RoomIndex);
}


CRoom* CRoom_Manager::Get_CurrentRoom()
{
	auto pair = m_mRooms.find(m_iCurrentLevelID);
	if (pair != m_mRooms.end())
	{
		for (auto& pRoom : pair->second)
		{
			if (pRoom->GetID() == m_iCurrentRoomID)
			{
				return pRoom;
			}
		}
	}

	return nullptr;
}

CRoom* CRoom_Manager::Get_RoomByID(_int iRoomID)
{
	auto pair = m_mRooms.find(m_iCurrentLevelID);

	if (pair != m_mRooms.end())
	{
		for (auto& pRoom : pair->second)
		{
			if (pRoom->GetID() == iRoomID)
			{
				return pRoom;
			}
		}
	}

	return nullptr;
}

void CRoom_Manager::Clear(_uint iLevelIndex)
{
	auto pair = m_mRooms.find(iLevelIndex);

	if (pair != m_mRooms.end())
	{
		for (auto& pRoom : pair->second)
		{
			Safe_Release(pRoom);
		}
	}
	m_mRooms[iLevelIndex].clear();
	CRoom_Manager::RoomIndexReset();
}

void CRoom_Manager::Free()
{
	for (auto& pair : m_mRooms)
	{
		_int iKey = pair.first;
		vector<CRoom*>& vRooms = pair.second;

		for (CRoom* pRoom : vRooms)
		{
			Safe_Release(pRoom);
		}
		vRooms.clear();
	}
	m_mRooms.clear();
	Safe_Release(m_pGameInstance);
	__super::Free();

	DestroyInstance();
}

void CRoom_Manager::RoomIndexAdd()
{
	iRoomIndex++;
}

void CRoom_Manager::RoomIndexReset()
{
	iRoomIndex = 0;
}



#include "Room_Manager.h"
#include "Room.h"
#include "Player.h"
#include "Client_Defines.h"
#include "Client_Defines_Event.h"

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
		CTransform* pPlayerTransform = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(iLayerLevelIndex, TEXT("Layer_Player"), TEXT("Com_Transform")));
		pPlayerTransform->Set_State(STATE::POSITION, _float3(0.f, 10.f, 0.f));
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
				pRoom->On_Fire();
			}
			else {
				pRoom->Exit();
				pRoom->Off_Fire();
			}
		}
	}

	return S_OK;
}

vector<pair<_int, _int>> CRoom_Manager::Create_RandomRooms(_int iRoomMax)
{

	_int x{ 0 }, z{ 0 };

	m_RoomIndex.push_back({ x, z });

	_int iCreated = 1;

	while(iCreated <= iRoomMax)
	{

		_int iRandom = static_cast<_int>(m_pGameInstance->Compute_Random(0.f, 7.f));

		_int nX = x;
		_int nZ = z;

		if (iRandom == 0 || iRandom == 7)
			nZ += 1;

		else if (iRandom == 1 || iRandom == 6)
			nX -= 1;
		else if (iRandom == 2 || iRandom == 5)
			nZ -= 1;
		else 
			nX += 1;

		pair<_int, _int> CheckIndex = { nX, nZ };

		if (0 != count(m_RoomIndex.begin(), m_RoomIndex.end(), CheckIndex))
			continue;

		m_RoomIndex.push_back({ nX, nZ });

		x = nX;
		z = nZ;
		++iCreated;
	}

	return vector<pair<_int, _int>>(m_RoomIndex);
}

void CRoom_Manager::Create_SpecialRoom(LEVEL eLevel)
{
	if (eLevel == LEVEL::LEVEL_BOSS1 || eLevel == LEVEL::LEVEL_BOSS2)
	{
		m_RoomIndex.push_back({ 0, 0 });
		m_RoomIndex.push_back({ 0, 1 });		//보스룸은 룸인덱스 고정 
	}
	else if (eLevel == LEVEL::LEVEL_SHELTER || eLevel == LEVEL::LEVEL_TOWN)
	{
		m_RoomIndex.push_back({ 0,0 });
	}
}

HRESULT CRoom_Manager::Check_Room(_uint iLayerLevelIndex, const _wstring& strLayerTag, _int iRoomID)
{
	CRoom* pRoom = Get_RoomByID(iRoomID);

	_int X = pRoom->Get_RoomX();
	_int Z = pRoom->Get_RoomZ();

	//x - 1, x + 1 , z - 1 , z + 1 을 체크해줘야 함.
	for (auto& Room : m_RoomIndex) 
	{
		_int iNeighborRoomX = Room.first;
		_int iNeighborRoomZ = Room.second;

		if(iNeighborRoomX == X - 1 && iNeighborRoomZ == Z)
		{
			//현재 찾은 ID의 룸에 포탈 생성 함수 자동 호출, _float3는 오프셋값 그 룸이 가지고 있는 지형을 중심 기줌으로 포탈의 위치를 - 준것
			pRoom->Ready_Potal(iLayerLevelIndex, strLayerTag, _float3(-10.f, 2.f, 0.f), POTAL_TYPE::LEFT);
		}

		if(iNeighborRoomX == X + 1 && iNeighborRoomZ == Z)
		{
			pRoom->Ready_Potal(iLayerLevelIndex, strLayerTag, _float3(10.f, 2.f, 0.f), POTAL_TYPE::RIGHT);
		}

		if(iNeighborRoomZ == Z - 1 && iNeighborRoomX == X)
		{
			pRoom->Ready_Potal(iLayerLevelIndex, strLayerTag, _float3(0.f, 2.f, -10.f), POTAL_TYPE::DOWN);
		}

		if(iNeighborRoomZ == Z + 1 && iNeighborRoomX == X)
		{
			pRoom->Ready_Potal(iLayerLevelIndex, strLayerTag, _float3(0.f, 2.f, 10.f), POTAL_TYPE::UP);
		}
	}

	return S_OK;
}

HRESULT CRoom_Manager::Check_SpecialRoom(LEVEL eLevel, const _wstring& strLayerTag, _int iRoomID)
{
	if (eLevel == LEVEL::LEVEL_SHELTER || eLevel == LEVEL::LEVEL_TOWN)
	{
		CRoom* pRoom = Get_RoomByID(iRoomID);

		pRoom->Ready_Stage_Potal(ENUM_CLASS(eLevel), strLayerTag, _float3(0.f, 2.f, 8.f), POTAL_TYPE::STAGE_POTAL);
	}
	else if (eLevel == LEVEL::LEVEL_BOSS1 || eLevel == LEVEL::LEVEL_BOSS2)
	{
		CRoom* pRoom = Get_RoomByID(iRoomID);

		pRoom->Ready_Stage_Potal(ENUM_CLASS(eLevel), strLayerTag, _float3(0.f, 2.f, 0.f), POTAL_TYPE::STAGE_POTAL);
	}
	
	return S_OK;
}

HRESULT CRoom_Manager::Check_Potal_Coll(POTAL_TYPE ePotalType, _float3& vNextPos)
{
	CRoom* pRoom = Get_CurrentRoom();

	_int X = pRoom->Get_RoomX();
	_int Z = pRoom->Get_RoomZ();

	POTAL_TYPE ePotal = {};
	_float3 vOffset = {0.f, 0.f, 0.f};

	if (ePotalType == POTAL_TYPE::LEFT) // X - 1 , Z 그대로
	{
		X -= 1;
		ePotal = POTAL_TYPE::RIGHT;
		vOffset.x = -2.f;
		
	}
	else if (ePotalType == POTAL_TYPE::RIGHT)
	{
		X += 1;
		ePotal = POTAL_TYPE::LEFT;
		vOffset.x = 2.f;
	}
	else if (ePotalType == POTAL_TYPE::UP)
	{
		Z += 1;
		ePotal = POTAL_TYPE::DOWN;
		vOffset.z = 2.f;
	}
	else if (ePotalType == POTAL_TYPE::DOWN)
	{
		Z -= 1;
		ePotal = POTAL_TYPE::UP;
		vOffset.z = -2.f;
	}
	else if (ePotalType == POTAL_TYPE::BOSS_POTAL)
	{
		CRoom* pNeighborRoom = Find_Room(0, 1);
		Enter_Room(pNeighborRoom->GetID());
		pRoom->Exit();
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_GameObject(m_iCurrentLevelID, TEXT("Layer_Player")));
		pPlayer->Change_TerrainBox(pNeighborRoom->Get_TerrainBox(), 99);

		ROOMCHANGE EventDesc;
		EventDesc.vPosition = dynamic_cast<CTransform*>(pNeighborRoom->Get_TerrainBox()->Find_Component(TEXT("Com_Transform_TerrainBox")))->Get_State(STATE::POSITION);
		m_pGameInstance->Broadcast(ENUM_CLASS(EVENT_TYPE::ROOMCHANGE), &EventDesc);
		vNextPos = EventDesc.vPosition - _float3(0.f, 0.f, 12.f);
		return S_OK;

	}
	for (auto& Room : m_RoomIndex)
	{
		_int iNeighborRoomX = Room.first;
		_int iNeighborRoomZ = Room.second;

		if (iNeighborRoomX == X && iNeighborRoomZ == Z)
		{
			CRoom* pNeighborRoom = Find_Room(iNeighborRoomX, iNeighborRoomZ);
			CPotal* pPotal = pNeighborRoom->Find_Potal(ePotal);
			CTransform* pPotalTransform = static_cast<CTransform*>(pPotal->Find_Component(TEXT("Com_Transform")));
			Enter_Room(pNeighborRoom->GetID());
			pRoom->Exit();
			CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_GameObject(m_iCurrentLevelID, TEXT("Layer_Player")));
			pPlayer->Change_TerrainBox(pNeighborRoom->Get_TerrainBox(), 99);

			ROOMCHANGE EventDesc;
			EventDesc.vPosition = dynamic_cast<CTransform*>(pNeighborRoom->Get_TerrainBox()->Find_Component(TEXT("Com_Transform_TerrainBox")))->Get_State(STATE::POSITION);
			m_pGameInstance->Broadcast(ENUM_CLASS(EVENT_TYPE::ROOMCHANGE), &EventDesc);
			vNextPos = (pPotalTransform->Get_State(STATE::POSITION)) + vOffset;
			if (pPotal == nullptr || pPotalTransform == nullptr)
				return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT CRoom_Manager::Check_END_Potal(_uint iLayerLevelIndex, const _wstring& strLayerTag, _int iIndex)
{
	//CRoom* pRoom = Get_RoomByID(iRoomID);
	//_int iRandom = static_cast<_int>(m_pGameInstance->Compute_Random(0.f, 3.f));			

	_bool CheckEndPotal = false;

	while (!CheckEndPotal)
	{
	_int iEndPotalIndexRoom = static_cast<_int>(m_pGameInstance->Compute_Random((_float)iIndex - 4.f, (_float)iIndex - 1.f));

	CRoom * pRoom = Get_RoomByID(iEndPotalIndexRoom);

	if (pRoom->Get_RoomType() == ROOM_INFO::EVENT_NORMAL)
	{
		CheckEndPotal = true;
		pRoom->Ready_Potal(iLayerLevelIndex, strLayerTag, _float3(0.f, 2.f, 0.f), POTAL_TYPE::END_POTAL);
		m_bCheckEnd = true;
		pRoom->Set_RoomType(ROOM_INFO::EVENT_ENDPOTAL);
	}

	}

	//	
	//if(!m_bCheckEnd)
	//{
	//	if (pRoom->GetID() >= 4 && iRandom == 2)	// 4번방 이후 부터 앤드포탈 생기게 설정
	//	{
	//		pRoom->Ready_Potal(iLayerLevelIndex, strLayerTag, _float3(0.f, 2.f, 0.f), POTAL_TYPE::END_POTAL);
	//		m_bCheckEnd = true;    //스테이지 변경시 그부분에서 false로 초기화 해줘야 함.
	//		pRoom->Set_RoomType(ROOM_INFO::EVENT_ENDPOTAL);
	//	}
	//}
	//
	//if (!m_bCheckEnd && iRoomIndex == pRoom->GetID() + 1)  // 룸인덱스 8 == 7 , + 1해줘야 됨
	//{
	//	pRoom->Ready_Potal(iLayerLevelIndex, strLayerTag, _float3(0.f, 2.f, 0.f), POTAL_TYPE::END_POTAL);
	//	m_bCheckEnd = true;
	//	pRoom->Set_RoomType(ROOM_INFO::EVENT_ENDPOTAL);
	//}

	return S_OK;
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

CRoom* CRoom_Manager::Find_Room(_int iRoomIndexX, _int iRoomIndexZ)
{
	auto pair = m_mRooms.find(m_iCurrentLevelID);
	
	if (pair != m_mRooms.end())
	{
		for (auto& pRoom : pair->second)
		{
			if (pRoom->Get_RoomX() == iRoomIndexX && pRoom->Get_RoomZ() == iRoomIndexZ)
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
			pRoom->Set_IsDead(true);
			Safe_Release(pRoom);
		}
	}
	m_mRooms[iLevelIndex].clear();
	m_RoomIndex.clear();
	CRoom_Manager::RoomIndexReset();
	m_bCheckEnd = false;

}

CMonster* CRoom_Manager::Find_CurrentRoom_Monster(MONSTER_TYPE eType)
{
	CRoom* pRoom = Get_CurrentRoom();
	return pRoom->Find_Monster(eType);
}

void CRoom_Manager::Free()
{
	for (auto& pair : m_mRooms)
	{
		vector<CRoom*>& vRooms = pair.second;

		for (CRoom* pRoom : vRooms)
		{
			Safe_Release(pRoom);
		}
		vRooms.clear();
	}
	m_mRooms.clear();

	m_RoomIndex.clear();

	Safe_Release(m_pGameInstance);

	DestroyInstance();

	__super::Free();
}

void CRoom_Manager::RoomIndexAdd()
{
	iRoomIndex++;
}

void CRoom_Manager::RoomIndexReset()
{
	iRoomIndex = 0;
}


#pragma once
#include "GameInstance.h"

BEGIN(Client)

class CRoom_Manager final : public CBase
{
	DECLARE_SINGLETON(CRoom_Manager)
public:
	CRoom_Manager();
	~CRoom_Manager() = default;
public:
	HRESULT Initialize();

public:
	HRESULT Add_Room(class CRoom* pRoom, _uint iLayerLevelIndex, const _wstring& strLayerTag);
	HRESULT Enter_Room(_int iRoomID);
	vector<pair<_int, _int>> Create_RandomRooms(_int iRoomMax);
	HRESULT Check_Room(_uint iLayerLevelIndex, const _wstring& strLayerTag, _int iRoomID);

	CRoom* Get_CurrentRoom();
	CRoom* Get_RoomByID(_int iRoomID);

	void Clear(_uint iLevelIndex);

private:
	CGameInstance* m_pGameInstance = { nullptr };
	map<_int, vector<CRoom*>> m_mRooms = {};
	_uint m_iCurrentRoomID = { };			//현재 레벨에 있는 룸 ID
	_uint m_iCurrentLevelID = {};			//현재 레벨(스테이지) ID

	vector<pair<_int, _int>> m_RoomIndex = {};


public:
	virtual void Free() override;

private:
	static _uint iRoomIndex;

private:
	static void RoomIndexAdd();
	static void RoomIndexReset();

};

END
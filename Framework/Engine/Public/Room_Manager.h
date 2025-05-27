#pragma once
#include "Base.h"

BEGIN(Engine)

class CRoom_Manager final : public CBase
{
public:
	CRoom_Manager();
	~CRoom_Manager() = default;

public:
	HRESULT Initialize();

public:
	HRESULT Add_Room(class CRoom* pRoom, _uint iLayerLevelIndex, const _wstring& strLayerTag);
	HRESULT Enter_Room(_int iRoomID);

	CRoom* Get_CurrentRoom();
	CRoom* Get_RoomByID(_int iRoomID);

	void Clear(_uint iLevelIndex);

private:
	class CGameInstance* m_pGameInstance = { nullptr };
	map<_int, vector<CRoom*>> m_mRooms = {};
	_uint m_iCurrentRoomID = { };
	_uint m_iCurrentLevelID = {};

public:
	static CRoom_Manager* Create();
	virtual void Free() override;

private:
	static _uint iRoomIndex;

private:
	static void RoomIndexAdd();
	static void RoomIndexReset();

};

END
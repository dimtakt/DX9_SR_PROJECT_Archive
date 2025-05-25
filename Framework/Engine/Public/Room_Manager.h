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
	void Priority_Update(_float fTimeDelta);
	void Update(_float fTimeDelta);
	void Late_Update(_float fTimeDelta);
	HRESULT Render();

public:
	HRESULT Add_Room(class CRoom* pRoom);
	HRESULT Enter_Room(_int iRoomID);

	CRoom* Get_CurrentRoom();
	CRoom* Get_RoomByID(_int iRoomID);

	void Clear();

private:
	class CGameInstance* m_pGameInstance = { nullptr };
	map<_int, CRoom*> m_mRooms = { };
	_int m_iCurrentRoomID = { };

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
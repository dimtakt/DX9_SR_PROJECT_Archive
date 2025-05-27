#pragma once
#include "Base.h"

BEGIN(Engine)

class CRoom_Factory final : public CBase
{
public:
	CRoom_Factory();
	~CRoom_Factory() = default;
public:
	void SetUp_MonsterRoom(class CRoom* pRoom,
		_uint iLayerLevelIndex,
		const _wstring& strLayerTag,
		_uint iPrototypeLevelIndex,
		const _wstring strPrototypeTag,
		void* pArg = nullptr);

private:
	class CGameInstance* m_pGameInstance = { nullptr };
	vector<class CMonster*> m_vMonster = {};



public:
	static CRoom_Factory* Create();
	virtual void Free() override;
};

END
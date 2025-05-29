#pragma once
#include "Base.h"
#include "Monster.h"
BEGIN(Engine)

class ENGINE_DLL CMonster_Factory final : public CBase
{
public:
	CMonster_Factory();
	~CMonster_Factory() = default;

public:
	HRESULT Initialize();

public:
	HRESULT Add_Monsters(class CRoom* pRoom, list<CMonster::MONSTERDESC> tMonsterDescList);

private:
	class CGameInstance* m_pGameInstance = { nullptr };

public:
	static CMonster_Factory* Create();
	virtual void Free() override;
};

END
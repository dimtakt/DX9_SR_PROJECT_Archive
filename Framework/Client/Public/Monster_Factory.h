#pragma once
#include "Base.h"
#include "Monster.h"
BEGIN(Client)

class CMonster_Factory final : public CBase
{
	DECLARE_SINGLETON(CMonster_Factory)
public:
	CMonster_Factory();
	~CMonster_Factory() = default;

public:
	HRESULT Initialize();

public:
	HRESULT Add_Monsters(class CRoom* pRoom, list<CMonster::MONSTERDESC> ObjectDescList);

private:
	class CGameInstance* m_pGameInstance = { nullptr };

public:
	static CMonster_Factory* Create();
	virtual void Free() override;
};

END
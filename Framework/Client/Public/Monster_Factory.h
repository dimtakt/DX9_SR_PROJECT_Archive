#pragma once
#include "Base.h"
#include "Monster.h"
BEGIN(Client)

class CMonster_Factory final : public CBase
{
	DECLARE_SINGLETON(CMonster_Factory)
public:
	enum class MONSTER_TYPE
	{
		MONSTER_DEFAULT,

		MONSTER_MOLE_A,
		MONSTER_OINK_A,
		MONSTER_LASERGHOST_D,

		MONSTER_END
	};

public:
	CMonster_Factory();
	~CMonster_Factory() = default;

public:
	HRESULT Initialize();

public:
	HRESULT Add_Monsters(class CRoom* pRoom, list<CMonster::MONSTERDESC> ObjectDescList, MONSTER_TYPE eType = MONSTER_TYPE::MONSTER_DEFAULT);

private:
	class CGameInstance* m_pGameInstance = { nullptr };

public:
	static CMonster_Factory* Create();
	virtual void Free() override;
};

END
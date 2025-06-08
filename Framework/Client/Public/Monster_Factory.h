#pragma once
#include "Base.h"
#include "Monster.h"

BEGIN(Client)

class CMonster_Factory final : public CBase
{
	DECLARE_SINGLETON(CMonster_Factory)
public:
	//enum class MONSTER_TYPE
	//{
	//	MONSTER_DEFAULT,

	//	MONSTER_MOLE_A,
	//	MONSTER_OINK_A,
	//	MONSTER_LASERGHOST_D,

	//	/* Erma */
	//	MONSTER_BOSS_ERMA,
	//	MONSTER_BOSS_ERMA_BODY,
	//	MONSTER_BOSS_ERMA_HAND_L,
	//	MONSTER_BOSS_ERMA_HAND_R,
	//	MONSTER_BOSS_ERMA_HEAD,

	//	MONSTER_END
	//}; //클라이언트 디파인으로 옮김 

public:
	CMonster_Factory();
	~CMonster_Factory() = default;

public:
	HRESULT Initialize();

public:
	HRESULT Add_Monsters(class CRoom* pRoom, list<CMonster::MONSTERDESC> ObjectDescList, MONSTER_TYPE_A eType = MONSTER_TYPE_A::MONSTER_DEFAULT);

	HRESULT Add_MonstersV2(CRoom* pRoom, list<CMonster::MONSTERDESC> ObjectDescList);

private:
	class CGameInstance* m_pGameInstance = { nullptr };

public:
	static CMonster_Factory* Create();
	virtual void Free() override;
};

END
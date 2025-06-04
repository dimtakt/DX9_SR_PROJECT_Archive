#include "Monster_Factory.h"
#include "GameInstance.h"
#include "Monster.h"
#include "Room.h"
#include "GameObject.h"

#include "Mole_A.h"
#include "Oink_A.h"

IMPLEMENT_SINGLETON(CMonster_Factory)

CMonster_Factory::CMonster_Factory()
	: m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CMonster_Factory::Initialize()
{
	return S_OK;
}

HRESULT CMonster_Factory::Add_Monsters(CRoom* pRoom, list<CMonster::MONSTERDESC> ObjectDescList, MONSTER_TYPE eType)
{
	NULL_CHECK_RETURN(pRoom, E_FAIL);
	for (auto& desc : ObjectDescList)
	{
		CMonster* pMonster = nullptr;

		switch (eType)
		{
		case CMonster_Factory::MONSTER_TYPE::MONSTER_DEFAULT:
			pMonster = dynamic_cast<CMonster*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, desc.iPrototypeLevelIndex, desc.strPrototypeTag, &desc));
			break;
		case CMonster_Factory::MONSTER_TYPE::MONSTER_MOLE_A:
			pMonster = dynamic_cast<CMole_A*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, desc.iPrototypeLevelIndex, desc.strPrototypeTag, &desc));
			break;
		case CMonster_Factory::MONSTER_TYPE::MONSTER_OINK_A:
			pMonster = dynamic_cast<COink_A*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, desc.iPrototypeLevelIndex, desc.strPrototypeTag, &desc));
			break;
		default:
			break;
		}

		NULL_CHECK_RETURN(pMonster, E_FAIL);

		pRoom->Add_Monster(pMonster);
	}

	return S_OK;
}

CMonster_Factory* CMonster_Factory::Create()
{
	return new CMonster_Factory();
}

void CMonster_Factory::Free()
{
	Safe_Release(m_pGameInstance);
	DestroyInstance();

	__super::Free();
}

#include "Monster_Factory.h"
#include "GameInstance.h"
#include "Monster.h"
#include "Room.h"
#include "GameObject.h"

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

HRESULT CMonster_Factory::Add_Monsters(CRoom* pRoom, list<CMonster::MONSTERDESC> ObjectDescList)
{
	NULL_CHECK_RETURN(pRoom, E_FAIL);
	for (auto& desc : ObjectDescList)
	{
		CMonster* pMonster = nullptr;
		pMonster = dynamic_cast<CMonster*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, desc.iPrototypeLevelIndex, desc.strPrototypeTag, &desc));
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
	__super::Free();
	Safe_Release(m_pGameInstance);
	DestroyInstance();
}

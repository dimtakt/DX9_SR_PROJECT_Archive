#include "Monster_Factory.h"
#include "GameInstance.h"
#include "Room.h"
#include "GameObject.h"

#include "Mole_A.h"
#include "Oink_A.h"
#include "LaserGhost_D.h"
#include "Erma.h"
#include "Erma_Body.h"
#include "Erma_Hand_L.h"
#include "Erma_Hand_R.h"
#include "Erma_Head.h"
#include "Askard.h"
#include "Askard_Tentacle.h"

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

HRESULT CMonster_Factory::Add_Monsters(CRoom* pRoom, list<CMonster::MONSTERDESC> ObjectDescList, MONSTER_TYPE_A eType)
{
	NULL_CHECK_RETURN(pRoom, E_FAIL);
	for (auto& desc : ObjectDescList)
	{
		CMonster* pMonster = nullptr;

		switch (eType)
		{
		case MONSTER_TYPE_A::MONSTER_DEFAULT:
			pMonster = dynamic_cast<CMonster*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, desc.iPrototypeLevelIndex, desc.strPrototypeTag, &desc));
			break;
		case MONSTER_TYPE_A::MONSTER_MOLE_A:
			pMonster = dynamic_cast<CMole_A*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, desc.iPrototypeLevelIndex, desc.strPrototypeTag, &desc));
			break;
		case MONSTER_TYPE_A::MONSTER_OINK_A:
			pMonster = dynamic_cast<COink_A*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, desc.iPrototypeLevelIndex, desc.strPrototypeTag, &desc));
			break;
		case MONSTER_TYPE_A::MONSTER_LASERGHOST_D:
			pMonster = dynamic_cast<CLaserGhost_D*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, desc.iPrototypeLevelIndex, desc.strPrototypeTag, &desc));
			break;


		/* Erma */
		case MONSTER_TYPE_A::MONSTER_BOSS_ERMA:
			pMonster = dynamic_cast<CErma*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, desc.iPrototypeLevelIndex, desc.strPrototypeTag, &desc));
			break;
		case MONSTER_TYPE_A::MONSTER_BOSS_ERMA_BODY:
			pMonster = dynamic_cast<CErma_Body*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, desc.iPrototypeLevelIndex, desc.strPrototypeTag, &desc));
			break;
		case MONSTER_TYPE_A::MONSTER_BOSS_ERMA_HAND_L:
			pMonster = dynamic_cast<CErma_Hand_L*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, desc.iPrototypeLevelIndex, desc.strPrototypeTag, &desc));
			break;
		case MONSTER_TYPE_A::MONSTER_BOSS_ERMA_HAND_R:
			pMonster = dynamic_cast<CErma_Hand_R*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, desc.iPrototypeLevelIndex, desc.strPrototypeTag, &desc));
			break;
		case MONSTER_TYPE_A::MONSTER_BOSS_ERMA_HEAD:
			pMonster = dynamic_cast<CErma_Head*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, desc.iPrototypeLevelIndex, desc.strPrototypeTag, &desc));
			break;

		/* Askard */
		case MONSTER_TYPE_A::MONSTER_BOSS_ASKARD:
			pMonster = dynamic_cast<CAskard*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, desc.iPrototypeLevelIndex, desc.strPrototypeTag, &desc));
			break;
		/* Askard_Tentacle (temporary) */
		case MONSTER_TYPE_A::MONSTER_BOSS_ASKARD_TENTACLE:
			pMonster = dynamic_cast<CAskard_Tentacle*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, desc.iPrototypeLevelIndex, desc.strPrototypeTag, &desc));
			break;


		default:
			break;
		}

		NULL_CHECK_RETURN(pMonster, E_FAIL);

		pRoom->Add_Monster(pMonster);
	}

	return S_OK;
}

HRESULT CMonster_Factory::Add_MonstersV2(CRoom* pRoom, list<CMonster::MONSTERDESC> ObjectDescList)
{
   NULL_CHECK_RETURN(pRoom, E_FAIL);
   for (auto& desc : ObjectDescList)
   {
      CMonster* pMonster = nullptr;

      switch (desc.eType)
      {
      case MONSTER_TYPE_A::MONSTER_DEFAULT:
         pMonster = dynamic_cast<CMonster*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, desc.iPrototypeLevelIndex, desc.strPrototypeTag, &desc));
         break;
      case MONSTER_TYPE_A::MONSTER_MOLE_A:
         pMonster = dynamic_cast<CMole_A*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, desc.iPrototypeLevelIndex, desc.strPrototypeTag, &desc));
         break;
      case MONSTER_TYPE_A::MONSTER_OINK_A:
         pMonster = dynamic_cast<COink_A*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, desc.iPrototypeLevelIndex, desc.strPrototypeTag, &desc));
         break;
      case MONSTER_TYPE_A::MONSTER_LASERGHOST_D:
         pMonster = dynamic_cast<CLaserGhost_D*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, desc.iPrototypeLevelIndex, desc.strPrototypeTag, &desc));
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

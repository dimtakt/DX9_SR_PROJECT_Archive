#include "Room_Factory.h"
#include "GameInstance.h"
#include "Room.h"
CRoom_Factory::CRoom_Factory()
	: m_pGameInstance { CGameInstance::GetInstance()}
{
	Safe_AddRef(m_pGameInstance);
}

void CRoom_Factory::SetUp_MonsterRoom(CRoom* pRoom, _uint iLayerLevelIndex, const _wstring& strLayerTag, _uint iPrototypeLevelIndex, const _wstring strPrototypeTag, void* pArg)
{
	m_pGameInstance->Add_GameObject_ToLayer(iLayerLevelIndex, strLayerTag, iPrototypeLevelIndex, strPrototypeTag, pArg);
	
}

CRoom_Factory* CRoom_Factory::Create()
{
	return nullptr;
}

void CRoom_Factory::Free()
{
}

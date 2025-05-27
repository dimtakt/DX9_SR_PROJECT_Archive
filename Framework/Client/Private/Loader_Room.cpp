#include "Loader_Room.h"

#include "GameInstance.h"

#include "BackGround.h"
#include "Terrain.h"
#include "Camera_Follow.h"
#include "Player.h"
#include "Land.h"
#include "Monster_Default.h"
#include "Room_Default.h"

CLoader_Room::CLoader_Room(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device{ pGraphic_Device }
	, m_pGameInstance{ CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pGraphic_Device);
}

unsigned int APIENTRY LoadingRoom(void* pArg)
{
	CLoader_Room* pLoader = static_cast<CLoader_Room*>(pArg);

	if (FAILED(pLoader->Loading()))
		return 1;

	return 0;
}


HRESULT CLoader_Room::Initialize(LEVEL eNextLevelID)
{
	m_eNextLevelID = eNextLevelID;

	InitializeCriticalSection(&m_CriticalSection);

	/* 스레드를 생성하고 */
	/* 생성한 스레드가 로딩을 할 수 있도록 처리한다. */

	/* 스택 메모리를 제외한 기타 다른 메모리공간(힙, 데이터, 코드, ) 은 스레드간 서로 공유한다. */
	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, LoadingRoom, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader_Room::Loading()
{
	EnterCriticalSection(&m_CriticalSection);

	HRESULT			hr = {};

	switch (m_eNextLevelID)
	{
	case LEVEL::LEVEL_STAGE1:
		hr = Loading_For_Stage1_Room();
		break;
	}

	if (FAILED(hr))
		return E_FAIL;

	LeaveCriticalSection(&m_CriticalSection);

	return S_OK;
}

HRESULT CLoader_Room::Loading_For_Stage1_Room()
{
	// 텍스쳐를 로딩중입니다
	


	// 모델을 로딩중입니다

	// 쉐이더를 로딩중입니다

	// 게임오브젝트를 로딩중입니다.

	

	//// 룸 정보 셋팅
	//for (size_t i = 0; i < 5; i++)
	//{
	//	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Layer_Room"),
	//		ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_GameObject_Room"))))
	//		return E_FAIL;
	//}
	//if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Layer_Room"),
	//	ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_GameObject_Room"))))
	//	return E_FAIL;

	//if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Layer_Land"),
	//	ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_GameObject_Land"))))
	//	return E_FAIL;

	//for (size_t i = 0; i < 100; i++)
	//{
	//	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Layer_ShortMonster"),
	//		ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_GameObject_ShortMonster"))))
	//		return E_FAIL;
	//}


	
	//

	// 룸 매니저 세팅



	//로딩 완료

	m_isFinished = true;

	return S_OK;
}
CLoader_Room* CLoader_Room::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eNextLevelID)
{
	CLoader_Room* pInstance = new CLoader_Room(pGraphic_Device);

	if (FAILED(pInstance->Initialize(eNextLevelID)))
	{
		MSG_BOX(TEXT("Failed to Created : CLoader_Room"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoader_Room::Free()
{
	__super::Free();

	WaitForSingleObject(m_hThread, INFINITE);

	CloseHandle(m_hThread);

	DeleteCriticalSection(&m_CriticalSection);

	Safe_Release(m_pGameInstance);
	Safe_Release(m_pGraphic_Device);
}

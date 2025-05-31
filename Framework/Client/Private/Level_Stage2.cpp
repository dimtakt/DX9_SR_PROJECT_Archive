#include "Level_Stage2.h"
#include "Client_Struct.h"
#include "GameInstance.h"
#include "Camera_Free.h"
#include "Player.h"
#include "Hud_Buff.h"
#include "Room.h"
#include "Monster.h"
#include "Monster.h"
#include "TerrainBox.h"
#include "Room_Manager.h"
#include "Monster_Factory.h"

CLevel_Stage2::CLevel_Stage2(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel{ pGraphic_Device }
{
}

HRESULT CLevel_Stage2::Initialize()
{
	CRoom_Manager::GetInstance()->Clear(ENUM_CLASS(LEVEL::LEVEL_STAGE1));
	g_hCursor = LoadCursorFromFile(L"Resources/Sephiria/UI/Cursor/Cursor_Combat.cur");

	/*if (FAILED(Ready_Light()))
		return E_FAIL;*/

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Room(TEXT("Layer_Room"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	/*if (FAILED(Ready_Layer_UI(TEXT("Layer_UI"))))
		return E_FAIL;*/

	return S_OK;
}

void CLevel_Stage2::Update(_float fTimeDelta)
{
	//if (m_pGameInstance->IsKeyDown(VK_RETURN))		//테스트용
	//{
	//	CRoom_Manager::GetInstance()->Enter_Room(i);
	//	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_GameObject(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Layer_Player")));
	//	CRoom* pRoom = CRoom_Manager::GetInstance()->Get_RoomByID(i);
	//	pPlayer->Change_TerrainBox(pRoom->Get_TerrainBox());


	//	if(i < 5)
	//		i++;
	//}
}

HRESULT CLevel_Stage2::Render()
{
	SetWindowText(g_hWnd, TEXT("Level_Stage2 레벨입니다."));

	return S_OK;
}

HRESULT CLevel_Stage2::Ready_Light()
{
	D3DLIGHT9	LightDesc;
	ZeroMemory(&LightDesc, sizeof(D3DLIGHT9));

	LightDesc.Type = D3DLIGHT_DIRECTIONAL;
	//LightDesc.Position = _float3(10.f, 3.f, 5.f);
	//LightDesc.Direction = _float3(1.f, -1.f, 1.f);
	//LightDesc.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//LightDesc.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightDesc.Diffuse = { 1.f, 1.f, 1.f, 1.f };
	LightDesc.Specular = { 1.f, 1.f, 1.f, 1.f };
	LightDesc.Ambient = { 1.f, 1.f, 1.f, 1.f };

	LightDesc.Direction = { 1.f, -1.f, 1.f };


	if (FAILED(m_pGameInstance->Ready_Light(&LightDesc, 0)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Stage2::Ready_Layer_Camera(const _wstring& strLayerTag)
{
	/*if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE2), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STAGE2), TEXT("Prototype_GameObject_Camera_Follow"))))
		return E_FAIL;*/
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE2), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STAGE2), TEXT("Prototype_GameObject_Camera_Mouse"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Stage2::Ready_Layer_BackGround(const _wstring& strLayerTag)
{
  	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE2), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STAGE2), TEXT("Prototype_GameObject_Sky"))))
		return E_FAIL;
	return S_OK;
}

HRESULT CLevel_Stage2::Ready_Layer_Player(const _wstring& strLayerTag)
{
	/*if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STATIC), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Player"))))
		return E_FAIL;*/

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_GameObject(ENUM_CLASS(LEVEL::LEVEL_STATIC), strLayerTag));
	
	CRoom* pRoom = CRoom_Manager::GetInstance()->Get_CurrentRoom();
	
	pPlayer->Change_TerrainBox(dynamic_cast<CTerrainBox*>(pRoom->Get_TerrainBox()));

	return S_OK;
}

HRESULT CLevel_Stage2::Ready_Layer_UI(const _wstring& strLayerTag)
{
	
	return S_OK;
}

HRESULT CLevel_Stage2::Ready_Layer_Room(const _wstring& strLayerTag)
{
	CRoom* pRoom = nullptr;
	m_pGameInstance->Seed_Random();

	vector<pair<_int, _int>> RoomIndex = CRoom_Manager::GetInstance()->Create_RandomRooms(5);

	for (size_t num = 0; num < 5; num++)
	{

		pRoom = dynamic_cast<CRoom*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::LEVEL_STAGE2), TEXT("Prototype_GameObject_Room")));
		NULL_CHECK_RETURN(pRoom, E_FAIL);

		_int RoomX = RoomIndex[num].first;
		_int RoomZ = RoomIndex[num].second;

		//현재 정해진 ID값의 룸에 지형, 오브젝트 세팅 내부에서 지형 위치 자동 배치
		pRoom->Load_From_File(ENUM_CLASS(LEVEL::LEVEL_STAGE2), TEXT("Layer_Room"), TEXT("../../data/Stage2_Map%d.txt"), num, RoomX, RoomZ);

		//현재 정해진 ID값의 룸에 포탈 설치 , 내부에서 인접한 지형 체크 후 포탈 위치 자동 조정
	

		//// 지형 셋팅
		//CTerrainBox* pTerrainBox = nullptr;
		//

		//MAP_OBJECT_DESC tDesc{};
		//tDesc.iTextureIndex = 0;
		//tDesc.vPos = _float3(static_cast<_float>(num) * 20.f + 2.f, 0.f, 0.f);
		//pTerrainBox = dynamic_cast<CTerrainBox*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::LEVEL_STAGE2), TEXT("Prototype_GameObject_TerrainBox"), &tDesc));
		//NULL_CHECK_RETURN(pTerrainBox, E_FAIL);
		//pRoom->Add_TerrainBox(pTerrainBox);

		//// 몬스터 셋팅
		//CMonster* pMonster = nullptr;
		//list<CMonster::MONSTERDESC> DescList;
		//for (size_t i = 0; i < 20; i++)
		//{
		//	CMonster::MONSTERDESC tDesc = {};
		//	tDesc.iLayerLevelIndex = ENUM_CLASS(LEVEL::LEVEL_STAGE2);
		//	tDesc.iPrototypeLevelIndex = ENUM_CLASS(LEVEL::LEVEL_STAGE2);
		//	tDesc.strLayerTag = strLayerTag;
		//	tDesc.strPrototypeTag = TEXT("Prototype_GameObject_ShortMonster");
		//	tDesc.vPosition = _float3(10.f * i + 10.f, 0.f, 5.f * i + 5.f);
		//	tDesc.pTerrainBox = pTerrainBox;
		//	//pMonster = dynamic_cast<CMonster_Default*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_GameObject_ShortMonster")));
		//	//NULL_CHECK_RETURN(pMonster, E_FAIL);
		//	//pRoom->Add_Monster(pMonster);
		//	DescList.push_back(tDesc);
		//}
		//CMonster_Factory::GetInstance()->Add_Monsters(pRoom, DescList);
		

		// 오브젝트 셋팅

		// 룸매니저 투입
 	CRoom_Manager::GetInstance()->Add_Room(pRoom, ENUM_CLASS(LEVEL::LEVEL_STAGE2), TEXT("Layer_Room"));
	}

	for (size_t num = 0; num < 5; num++)
	{
		CRoom_Manager::GetInstance()->Check_Room(ENUM_CLASS(LEVEL::LEVEL_STAGE2), TEXT("Layer_Room"), num);
	}

	return S_OK;
}

CLevel_Stage2* CLevel_Stage2::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_Stage2* pInstance = new CLevel_Stage2(pGraphic_Device);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CLevel_Stage2"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Stage2::Free()
{
	__super::Free();
}

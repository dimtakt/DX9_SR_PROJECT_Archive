#include "Level_Stage1.h"
#include "Client_Struct.h"
#include "GameInstance.h"
#include "Camera_Free.h"
#include "Camera_Follow.h"
#include "Player.h"
#include "Hud_Buff.h"
#include "Room.h"
#include "Monster.h"
#include "TerrainBox.h"
#include "Room_Manager.h"
#include "Monster_Factory.h"
#include "Level_Loading.h"
#include "Field_Hp.h"
#include "Animation.h"
#include "Mole_A.h"
#include "Dagger.h"
#include "Client_Defines_Event.h"

CLevel_Stage1::CLevel_Stage1(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel{ pGraphic_Device }
{
}

HRESULT CLevel_Stage1::Initialize()
{
	g_hCursor = LoadCursorFromFile(L"Resources/Sephiria/UI/Cursor/Cursor_Combat.cur");

	if (FAILED(Ready_Light()))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Room(TEXT("Layer_Room"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI(TEXT("Layer_UI"))))
		return E_FAIL;

	//if (FAILED(Ready_Test(TEXT("Layer_Test"))))
	//	return E_FAIL;

	return S_OK;
}

void CLevel_Stage1::Update(_float fTimeDelta)
{
	if (m_pGameInstance->IsKeyDown(VK_RETURN))
	{
 		if(FAILED(m_pGameInstance->Open_Level(ENUM_CLASS(LEVEL::LEVEL_LOADING), CLevel_Loading::Create(m_pGraphic_Device, LEVEL::LEVEL_STAGE2))))
			return;
	}
}

HRESULT CLevel_Stage1::Render()
{
	SetWindowText(g_hWnd, TEXT("Level_Stage1 레벨입니다."));

	return S_OK;
}

HRESULT CLevel_Stage1::Ready_Light()
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

HRESULT CLevel_Stage1::Ready_Layer_Camera(const _wstring& strLayerTag)
{
	CCamera_Follow::CAMERAFOLDESC desc{};
	desc.iLayerIndex = ENUM_CLASS(LEVEL::LEVEL_STAGE1);

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE1), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_GameObject_Camera_Follow"), &desc)))
		return E_FAIL;

	//if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE1), strLayerTag,
	//	ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_GameObject_Camera_Mouse"))))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Stage1::Ready_Layer_BackGround(const _wstring& strLayerTag)
{
  	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE1), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_GameObject_Sky"))))
		return E_FAIL;
	return S_OK;
}

HRESULT CLevel_Stage1::Ready_Layer_Player(const _wstring& strLayerTag)
{
	CDagger::DAGGERDESC DaggerDesc{};
	DaggerDesc.iLayerIndex = ENUM_CLASS(LEVEL::LEVEL_STAGE1);

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Layer_Weapon"),
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Weapon_Dagger"), &DaggerDesc)))
		return E_FAIL;

	CPlayer::PLAYERDESC PlayerDesc{};
	PlayerDesc.iLayerIndex = ENUM_CLASS(LEVEL::LEVEL_STAGE1);

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Layer_Player"),
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Player"), &PlayerDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Stage1::Ready_Layer_Room(const _wstring& strLayerTag)
{
	CRoom* pRoom = nullptr;
	m_pGameInstance->Seed_Random();

	_int iIndex = 8;		//룸의 총 개수
	_int iCount = 0;		//생성되는 룸 인덱스
	_int iEventCheck = 0;	//이벤트 룸 생성된 개수 저장용
	_int iEventRoomIndex1 = static_cast<_int>(m_pGameInstance->Compute_Random((_float)iIndex - 4, (_float)iIndex));;	// 1,2 이벤트룸 생성될 룸인덱스 값
	_int iEventRoomIndex2 = iEventRoomIndex1;
	while (iEventRoomIndex2 == iEventRoomIndex1)
	{
		iEventRoomIndex2 = static_cast<_int>(m_pGameInstance->Compute_Random((_float)iIndex - 4, (_float)iIndex));;
	}

	vector<pair<_int, _int>> RoomIndex = CRoom_Manager::GetInstance()->Create_RandomRooms(iIndex-1); //지정하고 싶은 룸의 개수 - 1 ( 내부에서 0 0 디폴트로 저장함 )
	//스테이지 1 이벤트는 상인, 골드
	while(iCount < iIndex )
	{
		
		pRoom = dynamic_cast<CRoom*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_GameObject_Room")));
		NULL_CHECK_RETURN(pRoom, E_FAIL);

		_int RoomX = RoomIndex[iCount].first;
		_int RoomZ = RoomIndex[iCount].second;

		if (iCount != iEventRoomIndex1 && iCount != iEventRoomIndex2) //if(상호작용 방이 아닌경우!)
		{
			//현재 정해진 ID값의 룸에 지형, 오브젝트 세팅 내부에서 지형 위치 자동 배치
			pRoom->Load_From_File(ENUM_CLASS(LEVEL::LEVEL_STAGE1), strLayerTag, TEXT("../../data/Stage2_Map%d.txt"), iCount, RoomX, RoomZ);

			if (iCount == 0)
			{
				CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_GameObject(ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Layer_Player")));
				pPlayer->Change_TerrainBox(dynamic_cast<CTerrainBox*>(pRoom->Get_TerrainBox()), iCount);

				ROOMCHANGE EventDesc;
				EventDesc.vPosition = dynamic_cast<CTransform*>(pRoom->Get_TerrainBox()->Find_Component(TEXT("Com_Transform_TerrainBox")))->Get_State(STATE::POSITION);
				m_pGameInstance->Broadcast(ENUM_CLASS(EVENT_TYPE::ROOMCHANGE), &EventDesc);
			}


			//CMole_A* pMonster = nullptr;
			list<CMonster::MONSTERDESC> DescList;
			for (size_t i = 0; i < 2; i++)	// ksta : 패턴 파악 위해 임시로 1만 바꿈, 원래값 5
			{
				CMonster::MONSTERDESC tDesc = {};
				tDesc.iLayerLevelIndex = ENUM_CLASS(LEVEL::LEVEL_STAGE1);
				tDesc.iPrototypeLevelIndex = ENUM_CLASS(LEVEL::LEVEL_STAGE1);
				tDesc.strLayerTag = strLayerTag;
				//tDesc.strPrototypeTag = TEXT("Prototype_GameObject_ShortMonster");
				//tDesc.strPrototypeTag = TEXT("Prototype_GameObject_Monster_Mole_A");
				tDesc.strPrototypeTag = TEXT("Prototype_GameObject_Monster_Oink_A");
				//tDesc.strPrototypeTag = TEXT("Prototype_GameObject_Monster_LaserGhost_D");
				tDesc.pTerrainBox = pRoom->Get_TerrainBox();
				DescList.push_back(tDesc);
			}
			CMonster_Factory::GetInstance()->Add_Monsters(pRoom, DescList, CMonster_Factory::MONSTER_TYPE::MONSTER_OINK_A);
			DescList.clear();
			for (size_t i = 0; i < 2; i++)	// ksta : 패턴 파악 위해 임시로 1만 바꿈, 원래값 5
			{
				CMonster::MONSTERDESC tDesc = {};
				tDesc.iLayerLevelIndex = ENUM_CLASS(LEVEL::LEVEL_STAGE1);
				tDesc.iPrototypeLevelIndex = ENUM_CLASS(LEVEL::LEVEL_STAGE1);
				tDesc.strLayerTag = strLayerTag;
				//tDesc.strPrototypeTag = TEXT("Prototype_GameObject_ShortMonster");
				tDesc.strPrototypeTag = TEXT("Prototype_GameObject_Monster_Mole_A");
				//tDesc.strPrototypeTag = TEXT("Prototype_GameObject_Monster_Oink_A");
				//tDesc.strPrototypeTag = TEXT("Prototype_GameObject_Monster_LaserGhost_D");
				tDesc.pTerrainBox = pRoom->Get_TerrainBox();
				DescList.push_back(tDesc);
			}
			CMonster_Factory::GetInstance()->Add_Monsters(pRoom, DescList, CMonster_Factory::MONSTER_TYPE::MONSTER_MOLE_A);
			DescList.clear();
			//for (size_t i = 0; i < 2; i++)	// ksta : 패턴 파악 위해 임시로 1만 바꿈, 원래값 5
			//{
			//	CMonster::MONSTERDESC tDesc = {};
			//	tDesc.iLayerLevelIndex = ENUM_CLASS(LEVEL::LEVEL_STAGE1);
			//	tDesc.iPrototypeLevelIndex = ENUM_CLASS(LEVEL::LEVEL_STAGE1);
			//	tDesc.strLayerTag = strLayerTag;
			//	//tDesc.strPrototypeTag = TEXT("Prototype_GameObject_ShortMonster");
			//	//tDesc.strPrototypeTag = TEXT("Prototype_GameObject_Monster_Mole_A");
			//	//tDesc.strPrototypeTag = TEXT("Prototype_GameObject_Monster_Oink_A");
			//	tDesc.strPrototypeTag = TEXT("Prototype_GameObject_Monster_LaserGhost_D");
			//	tDesc.pTerrainBox = pRoom->Get_TerrainBox();
			//	DescList.push_back(tDesc);
			//}
			//CMonster_Factory::GetInstance()->Add_Monsters(pRoom, DescList, CMonster_Factory::MONSTER_TYPE::MONSTER_LASERGHOST_D);		
		}
		else  //상호작용 전용 룸일 경우 전용파일 읽어옴 , 몬스터배치 x
		{
			pRoom->Load_From_File(ENUM_CLASS(LEVEL::LEVEL_STAGE1), strLayerTag, TEXT("../../data/Stage2_Event%d.txt"), iEventCheck, RoomX, RoomZ);			
			iEventCheck++;
		}

		//// 룸매니저 투입
		CRoom_Manager::GetInstance()->Add_Room(pRoom, ENUM_CLASS(LEVEL::LEVEL_STAGE1), strLayerTag);
		iCount++; 
	}

	for (size_t num = 0; num < iIndex; num++)	//각룸 돌면서 포탈 생성
	{
		CRoom_Manager::GetInstance()->Check_Room(ENUM_CLASS(LEVEL::LEVEL_STAGE1), strLayerTag, num);
	}


	return S_OK;
}

HRESULT CLevel_Stage1::Ready_Test(const _wstring& strLayerTag)
{
	MAP_OBJECT_DESC ExpDesc{};
	ExpDesc.eType = GAMEOBJ_TYPE::EXP;
	ExpDesc.vPos = _float3(0.f, 3.f, 0.f);
	ExpDesc.vRotate = _float3(0.f, 0.f, 0.f);
	ExpDesc.vScale = _float3(4.f, 4.f, 4.f);
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE1), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Interaction_Normal"), &ExpDesc)))
		return E_FAIL;

	MAP_OBJECT_DESC StoneDesc{};
	StoneDesc.eType = GAMEOBJ_TYPE::STONE_TABLET;
	StoneDesc.vPos = _float3(3.f, 2.f, 3.f);
	StoneDesc.vRotate = _float3(0.f, 0.f, 0.f);
	StoneDesc.vScale = _float3(1.f, 1.f, 1.f);
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE1), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Interaction_Normal"), &StoneDesc)))
		return E_FAIL;

	MAP_OBJECT_DESC MerchantDesc{};
	MerchantDesc.eType = GAMEOBJ_TYPE::MERCAHNT;
	MerchantDesc.vPos = _float3(-3.f, 2.f, -3.f);
	MerchantDesc.vRotate = _float3(0.f, 0.f, 0.f);
	MerchantDesc.vScale = _float3(5.f, 5.f, 5.f);
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE1), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Interaction_Normal"), &MerchantDesc)))
		return E_FAIL;

	MAP_OBJECT_DESC ArtifactDesc{};
	ArtifactDesc.eType = GAMEOBJ_TYPE::ATIFACT;
	ArtifactDesc.vPos = _float3(3.f, 2.f, -3.f);
	ArtifactDesc.vRotate = _float3(0.f, 0.f, 0.f);
	ArtifactDesc.vScale = _float3(2.f, 2.f, 2.f);
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE1), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Interaction_Normal"), &ArtifactDesc)))
		return E_FAIL;

	MAP_OBJECT_DESC HPDesc{};
	HPDesc.eType = GAMEOBJ_TYPE::HP;
	HPDesc.vPos = _float3(-3.f, 2.f, 3.f);
	HPDesc.vRotate = _float3(0.f, 0.f, 0.f);
	HPDesc.vScale = _float3(2.f, 2.f, 2.f);
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE1), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Interaction_Normal"), &HPDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Stage1::Ready_Layer_UI(const _wstring& strLayerTag)
{
	CUIObject::UIOBJECT_DESC Desc{};

	Desc.m_iLevel = ENUM_CLASS(LEVEL::LEVEL_STAGE1);

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Layer_Hud_States"),
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_UI_Hud_States"), &Desc)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE1), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_UI_Hud_Exp"), &Desc)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE1), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_UI_Hud_Wallet"), &Desc)))
		return E_FAIL;

  	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE1), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_UI_Hud_Dash"), &Desc)))
		return E_FAIL;
	
	Desc.fX = 190.f;
	Desc.fY = 100.f;
	Desc.fZ = 0;
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE1), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_UI_Hud_Buff"), &Desc)))
		return E_FAIL;

	Desc.fX = 227.f;
	Desc.fY = 100.f;
	Desc.fZ = 1;
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE1), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_UI_Hud_Buff"), &Desc)))
		return E_FAIL;

	Desc.fX = 264.f;
	Desc.fY = 100.f;
	Desc.fZ = 2;
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE1), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_UI_Hud_Buff"), &Desc)))
		return E_FAIL;

	return S_OK;
}

CLevel_Stage1* CLevel_Stage1::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_Stage1* pInstance = new CLevel_Stage1(pGraphic_Device);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CLevel_Stage1"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Stage1::Free()
{
	__super::Free();
}

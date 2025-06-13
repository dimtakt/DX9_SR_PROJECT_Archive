#include "Level_Boss1.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "Room_Manager.h"
#include "Point.h"
#include "Camera_Follow.h"
#include "Dagger.h"
#include "Player.h"
#include "Monster_Factory.h"

CLevel_Boss1::CLevel_Boss1(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel{ pGraphic_Device }
{
}

HRESULT CLevel_Boss1::Initialize()
{
	CRoom_Manager::GetInstance()->Clear(ENUM_CLASS(LEVEL::LEVEL_STAGE4));
	m_pGameInstance->Clear(); //파티클 초기화

	g_hCursor = LoadCursorFromFile(L"Resources/Sephiria/UI/Cursor/Cursor_Combat.cur");

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	if (FAILED(Ready_Light(TEXT("Layer_Light"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Room(TEXT("Layer_Room"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI(TEXT("Layer_UI"))))
		return E_FAIL;

	m_pGameInstance->StopAll();
	m_pGameInstance->PlayBGM(L"placeDungeonLibrary_Boss(Loop).wav", g_fBGMVolume - 0.9f);

	return S_OK;
}

void CLevel_Boss1::Update(_float fTimeDelta)
{

	return;
}



HRESULT CLevel_Boss1::Render()
{
	SetWindowText(g_hWnd, TEXT("보스1 레벨입니다."));

	return S_OK;
}

HRESULT CLevel_Boss1::Ready_Light(const _wstring& strLayerTag)
{
	CPoint::POINTDESC desc{};
	desc.pTargetTransform = dynamic_cast<CTransform*>(m_pGameInstance->Get_GameObject(ENUM_CLASS(LEVEL::LEVEL_BOSS1), TEXT("Layer_Player"))->Find_Component(TEXT("Com_Transform")));
	desc.strLightID = TEXT("Player_Light");
	desc.desc.eType = LIGHT_TYPE::POINT;
	desc.desc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	desc.desc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);
	desc.desc.vAmbient = _float4(0.01f, 0.01f, 0.01f, 1.f); // 기본 어두움 유지
	desc.desc.fSpecPower = 64.f;
	desc.desc.fRange = 15.f;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_BOSS1), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Point"), &desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Boss1::Ready_Layer_Camera(const _wstring& strLayerTag)
{

	CCamera_Follow::CAMERAFOLDESC desc{};
	desc.iLayerIndex = ENUM_CLASS(LEVEL::LEVEL_BOSS1);

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_BOSS1), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_BOSS1), TEXT("Prototype_GameObject_Camera_Follow"), &desc)))
		return E_FAIL;

	//if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_BOSS1), strLayerTag,
	//	ENUM_CLASS(LEVEL::LEVEL_BOSS1), TEXT("Prototype_GameObject_Camera_Mouse"))))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Boss1::Ready_Layer_BackGround(const _wstring& strLayerTag)
{

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_BOSS1), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_BOSS1), TEXT("Prototype_GameObject_Sky"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Boss1::Ready_Layer_Player(const _wstring& strLayerTag)
{
	CDagger::DAGGERDESC DaggerDesc{};
	DaggerDesc.iLayerIndex = ENUM_CLASS(LEVEL::LEVEL_BOSS1);

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_BOSS1), TEXT("Layer_Weapon"),
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Weapon_Dagger"), &DaggerDesc)))
		return E_FAIL;

	CPlayer::PLAYERDESC PlayerDesc{};
	PlayerDesc.iLayerIndex = ENUM_CLASS(LEVEL::LEVEL_BOSS1);

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_BOSS1), TEXT("Layer_Player"),
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Player"), &PlayerDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Boss1::Ready_Layer_UI(const _wstring& strLayerTag)
{
	CUIObject::UIOBJECT_DESC Desc{};

	Desc.m_iLevel = ENUM_CLASS(LEVEL::LEVEL_BOSS1);

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_BOSS1), TEXT("Layer_Hud_States"),
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_UI_Hud_States"), &Desc)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_BOSS1), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_UI_Hud_Exp"), &Desc)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_BOSS1), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_UI_Hud_Wallet"), &Desc)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_BOSS1), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_UI_Hud_Dash"), &Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Boss1::Ready_Layer_Room(const _wstring& strLayerTag)
{

	CRoom* pRoom = nullptr;
	m_pGameInstance->Seed_Random();

	_int iIndex = 2;		// 보스룸은 룸 총 생성 개수 2
	_int iCount = 0;		// 룸 생성 for문 돌리는 용도 + 데이터파일 0번부터 순차적으로 읽어오는 용도
	_int iNumber = 0;
	_int iRoomX = 0;
	_int iRoomZ = 0;

	CRoom_Manager::GetInstance()->Create_SpecialRoom(LEVEL::LEVEL_BOSS1); // 보스룸의 인덱스는 무조건 고정적으로 넣어주고자 함. 랜덤X

	while (iCount < iIndex)
	{

		pRoom = dynamic_cast<CRoom*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::LEVEL_BOSS1), TEXT("Prototype_GameObject_Room")));
		NULL_CHECK_RETURN(pRoom, E_FAIL);

			pRoom->Load_From_File(ENUM_CLASS(LEVEL::LEVEL_BOSS1), strLayerTag, TEXT("../../data/StageBoss1_Map%d.txt"), iNumber, iRoomX, iRoomZ, ROOM_INFO::EVENT_BOSS);
			if (iNumber == 1)
			{
				pRoom->Load_Particle(PARTICLE_TYPE::DUST, TEXT("Prototype_GameObject_FireParticle"), ENUM_CLASS(LEVEL::LEVEL_BOSS1), _float3(0.5f, 0.5f, 1.f), 2);
				pRoom->Set_ParticleType(PARTICLE_TYPE::DUST);
				pRoom->Set_ParticleOn();
			}
			iNumber++;		
			iRoomZ++;		//보스룸은 무조건 0,0 일반룸1  0,1 보스룸 설정
			if (iCount == 0)
			{
				CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_GameObject(ENUM_CLASS(LEVEL::LEVEL_BOSS1), TEXT("Layer_Player")));
				pPlayer->Change_TerrainBox(dynamic_cast<CTerrainBox*>(pRoom->Get_TerrainBox()), iCount);

				ROOMCHANGE EventDesc;
				EventDesc.vPosition = dynamic_cast<CTransform*>(pRoom->Get_TerrainBox()->Find_Component(TEXT("Com_Transform_TerrainBox")))->Get_State(STATE::POSITION);
				m_pGameInstance->Broadcast(ENUM_CLASS(EVENT_TYPE::ROOMCHANGE), &EventDesc);

			}



			list<CMonster::MONSTERDESC> DescList;

			if (iCount == 1)
			{

				CMonster::MONSTERDESC tDesc = {};
				tDesc.iLayerLevelIndex = ENUM_CLASS(LEVEL::LEVEL_BOSS1);
				tDesc.iPrototypeLevelIndex = ENUM_CLASS(LEVEL::LEVEL_BOSS1);
				tDesc.strLayerTag = strLayerTag;
				tDesc.pTerrainBox = pRoom->Get_TerrainBox();

#pragma region Erma Spawn...
				
				tDesc.strPrototypeTag = TEXT("Prototype_GameObject_Boss_Erma_Body");
				DescList.push_back(tDesc);
				CMonster_Factory::GetInstance()->Add_Monsters(pRoom, DescList, MONSTER_TYPE_A::MONSTER_BOSS_ERMA_BODY);
				DescList.clear();

				tDesc.strPrototypeTag = TEXT("Prototype_GameObject_Boss_Erma_Hand_L");
				DescList.push_back(tDesc);
				CMonster_Factory::GetInstance()->Add_Monsters(pRoom, DescList, MONSTER_TYPE_A::MONSTER_BOSS_ERMA_HAND_L);
				DescList.clear();

				tDesc.strPrototypeTag = TEXT("Prototype_GameObject_Boss_Erma_Hand_R");
				DescList.push_back(tDesc);
				CMonster_Factory::GetInstance()->Add_Monsters(pRoom, DescList, MONSTER_TYPE_A::MONSTER_BOSS_ERMA_HAND_R);
				DescList.clear();

				tDesc.strPrototypeTag = TEXT("Prototype_GameObject_Boss_Erma_Head");
				DescList.push_back(tDesc);
				CMonster_Factory::GetInstance()->Add_Monsters(pRoom, DescList, MONSTER_TYPE_A::MONSTER_BOSS_ERMA_HEAD);
				DescList.clear();

				tDesc.strPrototypeTag = TEXT("Prototype_GameObject_Boss_Erma");
				DescList.push_back(tDesc);
				CMonster_Factory::GetInstance()->Add_Monsters(pRoom, DescList, MONSTER_TYPE_A::MONSTER_BOSS_ERMA);
				DescList.clear();
				
#pragma endregion


#pragma region Askard Spawn test...

				//tDesc.strPrototypeTag = TEXT("Prototype_GameObject_Boss_Askard");
				//DescList.push_back(tDesc);
				//CMonster_Factory::GetInstance()->Add_Monsters(pRoom, DescList, MONSTER_TYPE_A::MONSTER_BOSS_ASKARD);
				//DescList.clear();

#pragma endregion





			}

			//CMonster_Factory::GetInstance()->Add_Monsters(pRoom, DescList, CMonster_Factory::MONSTER_TYPE::MONSTER_MOLE_A);
			//DescList.clear();







			// 룸매니저 투입
			CRoom_Manager::GetInstance()->Add_Room(pRoom, ENUM_CLASS(LEVEL::LEVEL_BOSS1), strLayerTag);
			iCount++;
	}

		CRoom_Manager::GetInstance()->Check_Room(ENUM_CLASS(LEVEL::LEVEL_BOSS1), strLayerTag, 0);	//첫번째룸 포탈 설치 ( 보스방으로 이어짐 )
		CRoom_Manager::GetInstance()->Check_SpecialRoom(LEVEL::LEVEL_BOSS1, strLayerTag, 1);		//보스룸 포탈 설치, 보스 잡으면 중앙에 생기게끔
	

	return S_OK;
}

CLevel_Boss1* CLevel_Boss1::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_Boss1* pInstance = new CLevel_Boss1(pGraphic_Device);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CLevel_Boss1"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Boss1::Free()
{
	__super::Free();
}
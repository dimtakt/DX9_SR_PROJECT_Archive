#include "Level_Stage3.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "Room_Manager.h"
#include "Camera_Follow.h"
#include "Dagger.h"
#include "Player.h"
#include "ChapMap.h"
#include "Monster_Factory.h"
#include "Point.h"

CLevel_Stage3::CLevel_Stage3(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel{ pGraphic_Device }
{
}

HRESULT CLevel_Stage3::Initialize()
{
	CRoom_Manager::GetInstance()->Clear(ENUM_CLASS(LEVEL::LEVEL_STAGE2));
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


	return S_OK;
}

void CLevel_Stage3::Update(_float fTimeDelta)
{

	return;
}



HRESULT CLevel_Stage3::Render()
{
	SetWindowText(g_hWnd, TEXT("스테이지3 레벨입니다."));

	return S_OK;
}

HRESULT CLevel_Stage3::Ready_Layer_BackGround(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE3), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STAGE3), TEXT("Prototype_GameObject_Sky"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Stage3::Ready_Layer_UI(const _wstring& strLayerTag)
{
	CUIObject::UIOBJECT_DESC Desc{};

	Desc.m_iLevel = ENUM_CLASS(LEVEL::LEVEL_STAGE3);

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE3), TEXT("Layer_Hud_States"),
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_UI_Hud_States"), &Desc)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE3), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_UI_Hud_Exp"), &Desc)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE3), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_UI_Hud_Wallet"), &Desc)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE3), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_UI_Hud_Dash"), &Desc)))
		return E_FAIL;
	Desc.fX = 190.f;
	Desc.fY = 100.f;
	Desc.fZ = 0;
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE3), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_UI_Hud_Buff"), &Desc)))
		return E_FAIL;

	Desc.fX = 227.f;
	Desc.fY = 100.f;
	Desc.fZ = 1;
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE3), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_UI_Hud_Buff"), &Desc)))
		return E_FAIL;

	Desc.fX = 264.f;
	Desc.fY = 100.f;
	Desc.fZ = 2;
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE3), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_UI_Hud_Buff"), &Desc)))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE3), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_UI_Minimap"), &Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Stage3::Ready_Light(const _wstring& strLayerTag)
{
	CPoint::POINTDESC desc{};
	desc.pTargetTransform = dynamic_cast<CTransform*>(m_pGameInstance->Get_GameObject(ENUM_CLASS(LEVEL::LEVEL_STAGE3), TEXT("Layer_Player"))->Find_Component(TEXT("Com_Transform")));
	desc.strLightID = TEXT("Player_Light");
	desc.desc.eType = LIGHT_TYPE::POINT;
	desc.desc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	desc.desc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);
	desc.desc.vAmbient = _float4(0.01f, 0.01f, 0.01f, 1.f); // 기본 어두움 유지
	desc.desc.fSpecPower = 64.f;
	desc.desc.fRange = 15.f;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE3), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Point"), &desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Stage3::Ready_Layer_Camera(const _wstring& strLayerTag)
{

	CCamera_Follow::CAMERAFOLDESC desc{};
	desc.iLayerIndex = ENUM_CLASS(LEVEL::LEVEL_STAGE3);

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE3), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STAGE3), TEXT("Prototype_GameObject_Camera_Follow"), &desc)))
		return E_FAIL;
}

HRESULT CLevel_Stage3::Ready_Layer_Player(const _wstring& strLayerTag)
{
	CDagger::DAGGERDESC DaggerDesc{};
	DaggerDesc.iLayerIndex = ENUM_CLASS(LEVEL::LEVEL_STAGE3);

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE3), TEXT("Layer_Weapon"),
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Weapon_Dagger"), &DaggerDesc)))
		return E_FAIL;

	CPlayer::PLAYERDESC PlayerDesc{};
	PlayerDesc.iLayerIndex = ENUM_CLASS(LEVEL::LEVEL_STAGE3);

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE3), TEXT("Layer_Player"),
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Player"), &PlayerDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Stage3::Ready_Layer_Room(const _wstring& strLayerTag)
{
	CRoom* pRoom = nullptr;
	m_pGameInstance->Seed_Random();

	_int iIndex = 7;
	_int iCount = 0;
	_int iEventCheck = 0;
	_int iEventRoomEventID = dynamic_cast<CChapMap*>(m_pGameInstance->Get_GameObject(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Layer_ChapMap")))->Get_Click_ID();
	_int iEventRoomCreate = 2;
	_int iNumber = 0; //읽어올 파일 번호

	if (iEventRoomEventID != 0)
		iIndex = 8; //이벤트룸 1개면 전체 룸 7개, 2개면 전체룸 8개

	_int iEventRoomIndex1 = static_cast<_int>(m_pGameInstance->Compute_Random((_float)iIndex - 4, (_float)iIndex-1));;
	_int iEventRoomIndex2 = iEventRoomIndex1;

	while (iEventRoomIndex2 <= iEventRoomIndex1) // 혹여나 랜덤으로 같은 위치 뽑히면 다시 돌리게 설정.
	{
		iEventRoomIndex2 = static_cast<_int>(m_pGameInstance->Compute_Random((_float)iIndex - 4, (_float)iIndex-1));;
	}

	vector<pair<_int, _int>> RoomIndex = CRoom_Manager::GetInstance()->Create_RandomRooms(iIndex - 1); //지정하고 싶은 룸의 개수 - 1 ( 내부에서 0 0 디폴트로 저장함 )
	//while문 돌릴 인덱스값 정의 일반룸 5 + 상호룸 2 = 7  스테이지 1 이벤트는 상인, 골드
	while (iCount < iIndex)
	{

		pRoom = dynamic_cast<CRoom*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::LEVEL_STAGE3), TEXT("Prototype_GameObject_Room")));
		NULL_CHECK_RETURN(pRoom, E_FAIL);

		_int RoomX = RoomIndex[iCount].first;
		_int RoomZ = RoomIndex[iCount].second;

		if (iCount != iEventRoomIndex1 && iCount != iEventRoomIndex2)
		{
			//if(상호작용 방이 아닌경우!)
			//현재 정해진 ID값의 룸에 지형, 오브젝트 세팅 내부에서 지형 위치 자동 배치
			pRoom->Load_From_File(ENUM_CLASS(LEVEL::LEVEL_STAGE3), strLayerTag, TEXT("../../data/Stage3_Map%d.txt"), iNumber, RoomX, RoomZ, static_cast<ROOM_INFO>(0));
			iNumber++;

			if (iCount == 0)
			{
				CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_GameObject(ENUM_CLASS(LEVEL::LEVEL_STAGE3), TEXT("Layer_Player")));
				pPlayer->Change_TerrainBox(dynamic_cast<CTerrainBox*>(pRoom->Get_TerrainBox()), iCount);

				ROOMCHANGE EventDesc;
				EventDesc.vPosition = dynamic_cast<CTransform*>(pRoom->Get_TerrainBox()->Find_Component(TEXT("Com_Transform_TerrainBox")))->Get_State(STATE::POSITION);
				m_pGameInstance->Broadcast(ENUM_CLASS(EVENT_TYPE::ROOMCHANGE), &EventDesc);
			}

		}
		else if (iEventCheck < iEventRoomCreate)
		{
			if (iEventRoomEventID == 3 && iEventCheck == 0)
			{	//3스테이지_3_Event%d 파일명 이렇게 지어줄 예정 , 타입 따로 넘겨줘야해서..
				pRoom->Load_From_File(ENUM_CLASS(LEVEL::LEVEL_STAGE3), strLayerTag, TEXT("../../data/Stage3_1_Event%d.txt"), iEventCheck, RoomX, RoomZ, ROOM_INFO::EVENT_SHOP);
				iEventCheck++;
			}
			else if (iEventRoomEventID == 3 && iEventCheck == 1)
			{
				pRoom->Load_From_File(ENUM_CLASS(LEVEL::LEVEL_STAGE3), strLayerTag, TEXT("../../data/Stage3_1_Event%d.txt"), iEventCheck, RoomX, RoomZ, ROOM_INFO::EVENT_HP);
				iEventCheck++;
			}
			else if (iEventRoomEventID == 4 && iEventCheck == 0)
			{
				pRoom->Load_From_File(ENUM_CLASS(LEVEL::LEVEL_STAGE3), strLayerTag, TEXT("../../data/Stage3_2_Event%d.txt"), iEventCheck, RoomX, RoomZ, ROOM_INFO::EVENT_ARTEFACT);
				iEventCheck++;
			}
			else
			{
				pRoom->Load_From_File(ENUM_CLASS(LEVEL::LEVEL_STAGE3), strLayerTag, TEXT("../../data/Stage3_2_Event%d.txt"), iEventCheck, RoomX, RoomZ, ROOM_INFO::EVENT_STONE);
				iEventCheck++;
			}
		}

		// 룸매니저 투입
		CRoom_Manager::GetInstance()->Add_Room(pRoom, ENUM_CLASS(LEVEL::LEVEL_STAGE3), strLayerTag);
		iCount++;
	}		

	for (size_t num = 0; num < iIndex; num++)
		{
			CRoom_Manager::GetInstance()->Check_Room(ENUM_CLASS(LEVEL::LEVEL_STAGE3), strLayerTag, num);
		}

	//앤드포탈 생성 1회
	CRoom_Manager::GetInstance()->Check_END_Potal(ENUM_CLASS(LEVEL::LEVEL_STAGE3), strLayerTag, iIndex);

	return S_OK;
}

CLevel_Stage3* CLevel_Stage3::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_Stage3* pInstance = new CLevel_Stage3(pGraphic_Device);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CLevel_Stage3"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Stage3::Free()
{
	__super::Free();
}
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
#include "ChapMap.h"
#include "Sun.h"
#include "Point.h"

CLevel_Stage1::CLevel_Stage1(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel{ pGraphic_Device }
{
}

HRESULT CLevel_Stage1::Initialize()
{
	CRoom_Manager::GetInstance()->Clear(ENUM_CLASS(LEVEL::LEVEL_TOWN));
	m_pGameInstance->Clear();	//파티클 초기화

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

	//if (FAILED(Ready_Test(TEXT("Layer_Test"))))
	//	return E_FAIL;

	m_pGameInstance->StopAll();
	m_pGameInstance->PlayBGM(L"moleField_Start.wav", g_fBGMVolume - 0.8f);
	m_pGameInstance->PlayLoopSound(L"ambienceCave.wav", ENUM_CLASS(CHANNELID::SOUND_BACKGROUND), g_fBGMVolume - 0.9f);

	return S_OK;
}

void CLevel_Stage1::Update(_float fTimeDelta)
{
}

HRESULT CLevel_Stage1::Render()
{
	SetWindowText(g_hWnd, TEXT("Level_Stage1 레벨입니다."));

	return S_OK;
}

HRESULT CLevel_Stage1::Ready_Light(const _wstring& strLayerTag)
{	
	CPoint::POINTDESC desc{};
	desc.pTargetTransform = dynamic_cast<CTransform*>(m_pGameInstance->Get_GameObject(ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Layer_Player"))->Find_Component(TEXT("Com_Transform")));
	desc.strLightID = TEXT("Player_Light");
	desc.desc.eType = LIGHT_TYPE::POINT;
	desc.desc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	desc.desc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);
	desc.desc.vAmbient = _float4(0.01f, 0.01f, 0.01f, 1.f); // 기본 어두움 유지
	desc.desc.fSpecPower = 64.f;
	desc.desc.fRange = 8.f;


	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE1), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Point"), &desc)))
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

	_int iIndex = 7;		//룸의 총 개수
	_int iCount = 0;		//생성되는 룸 인덱스

	vector<pair<_int, _int>> RoomIndex = CRoom_Manager::GetInstance()->Create_RandomRooms(iIndex-1); //지정하고 싶은 룸의 개수 - 1 ( 내부에서 0 0 디폴트로 저장함 )
	//스테이지 1 이벤트는 상인, 골드
	while(iCount < iIndex )
	{
		
		pRoom = dynamic_cast<CRoom*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_GameObject_Room")));
		NULL_CHECK_RETURN(pRoom, E_FAIL);						

		_int RoomX = RoomIndex[iCount].first;
		_int RoomZ = RoomIndex[iCount].second;

	
			//현재 정해진 ID값의 룸에 지형, 오브젝트 세팅 내부에서 지형 위치 자동 배치
		pRoom->Load_From_File(ENUM_CLASS(LEVEL::LEVEL_STAGE1), strLayerTag, TEXT("../../data/Stage1_Map%d.txt"), iCount, RoomX, RoomZ,static_cast<ROOM_INFO>(0));
		pRoom->Set_ParticleOn();
		pRoom->Set_ParticleType(PARTICLE_TYPE::RAIN);			//파티클 쓰는거 스테이지로 통일한다함. 그러면 그스테이지 룸 파티클 설정값 켜줘야함.

			if (iCount == 0)
			{
				CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_GameObject(ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Layer_Player")));
				pPlayer->Change_TerrainBox(dynamic_cast<CTerrainBox*>(pRoom->Get_TerrainBox()), iCount);

				ROOMCHANGE EventDesc;
				EventDesc.vPosition = dynamic_cast<CTransform*>(pRoom->Get_TerrainBox()->Find_Component(TEXT("Com_Transform_TerrainBox")))->Get_State(STATE::POSITION);
				m_pGameInstance->Broadcast(ENUM_CLASS(EVENT_TYPE::ROOMCHANGE), &EventDesc);

				pRoom->Load_Particle(PARTICLE_TYPE::RAIN, TEXT("Prototype_GameObject_Rain"), ENUM_CLASS(LEVEL::LEVEL_STAGE1), _float3(0.5f, 1.f, 1.f), 1);
				pRoom->Set_ParticleOn();
				pRoom->Set_ParticleType(PARTICLE_TYPE::RAIN);
			}

		//// 룸매니저 투입
		CRoom_Manager::GetInstance()->Add_Room(pRoom, ENUM_CLASS(LEVEL::LEVEL_STAGE1), strLayerTag);
		iCount++; 
	}

	//각룸 돌면서 포탈 생성
	for (size_t num = 0; num < iIndex; num++)	
	{
		CRoom_Manager::GetInstance()->Check_Room(ENUM_CLASS(LEVEL::LEVEL_STAGE1), strLayerTag, num);
	}

	//앤드포탈 생성 1회
	CRoom_Manager::GetInstance()->Check_END_Potal(ENUM_CLASS(LEVEL::LEVEL_STAGE1), strLayerTag, iIndex);


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

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE1), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_UI_Minimap"), &Desc)))
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

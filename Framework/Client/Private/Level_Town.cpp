#include "Level_Town.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "Room_Manager.h"
#include "Point.h"
#include "Camera_Follow.h"
#include "Dagger.h"
#include "Player.h"
#include "Sun.h"

CLevel_Town::CLevel_Town(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel{ pGraphic_Device }
{
}

HRESULT CLevel_Town::Initialize()
{
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

void CLevel_Town::Update(_float fTimeDelta)
{

	return ;
}



HRESULT CLevel_Town::Render()
{
	SetWindowText(g_hWnd, TEXT("타운 레벨입니다."));

	return S_OK;
}

HRESULT CLevel_Town::Ready_Light(const _wstring& strLayerTag)
{
	CPoint::POINTDESC desc{};
	desc.pTargetTransform = dynamic_cast<CTransform*>(m_pGameInstance->Get_GameObject(ENUM_CLASS(LEVEL::LEVEL_TOWN), TEXT("Layer_Player"))->Find_Component(TEXT("Com_Transform")));
	desc.strLightID = TEXT("Player_Light");
	desc.desc.eType = LIGHT_TYPE::POINT;
	desc.desc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	desc.desc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);
	desc.desc.vAmbient = _float4(0.01f, 0.01f, 0.01f, 1.f); // 기본 어두움 유지
	desc.desc.fSpecPower = 64.f;
	desc.desc.fRange = 10.f;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_TOWN), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Point"), &desc)))
		return E_FAIL;

	CSun::SUNDESC SunDesc{};
	SunDesc.strLightID = TEXT("Sun");
	SunDesc.desc.eType = LIGHT_TYPE::DIRECTIONAL;
	SunDesc.desc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	SunDesc.desc.vSpecular = _float4(0.2f, 0.2f, 0.2f, 0.2f);
	SunDesc.desc.vAmbient = _float4(1.f, 1.f, 1.f, 1.f); // 기본 어두움 유지
	SunDesc.desc.fSpecPower = 128.f;
	SunDesc.vLightPos = _float3(0.f, 1000.f, 0.f);
	SunDesc.vLightDir = _float3(0.f, -1.f, 0.f);

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_TOWN), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Sun"), &desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Town::Ready_Layer_Camera(const _wstring& strLayerTag)
{
	CCamera_Follow::CAMERAFOLDESC desc{};
	desc.iLayerIndex = ENUM_CLASS(LEVEL::LEVEL_TOWN);

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_TOWN), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_TOWN), TEXT("Prototype_GameObject_Camera_Follow"), &desc)))
		return E_FAIL;
}

HRESULT CLevel_Town::Ready_Layer_BackGround(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_TOWN), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_TOWN), TEXT("Prototype_GameObject_Sky"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Town::Ready_Layer_Player(const _wstring& strLayerTag)
{
	CDagger::DAGGERDESC DaggerDesc{};
	DaggerDesc.iLayerIndex = ENUM_CLASS(LEVEL::LEVEL_TOWN);

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_TOWN), TEXT("Layer_Weapon"),
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Weapon_Dagger"), &DaggerDesc)))
		return E_FAIL;

	CPlayer::PLAYERDESC PlayerDesc{};
	PlayerDesc.iLayerIndex = ENUM_CLASS(LEVEL::LEVEL_TOWN);

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_TOWN), TEXT("Layer_Player"),
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Player"), &PlayerDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Town::Ready_Layer_UI(const _wstring& strLayerTag)
{
	CUIObject::UIOBJECT_DESC Desc{};

	Desc.m_iLevel = ENUM_CLASS(LEVEL::LEVEL_TOWN);

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_TOWN), TEXT("Layer_Hud_States"),
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_UI_Hud_States"), &Desc)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_TOWN), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_UI_Hud_Exp"), &Desc)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_TOWN), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_UI_Hud_Wallet"), &Desc)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_TOWN), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_UI_Hud_Dash"), &Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Town::Ready_Layer_Room(const _wstring& strLayerTag)
{
	CRoom* pRoom = nullptr;
	m_pGameInstance->Seed_Random();

	_int iNumber = 0;
	_int iRoomX = 0;
	_int iRoomZ = 0;

	CRoom_Manager::GetInstance()->Create_SpecialRoom(LEVEL::LEVEL_TOWN); // 마을 인덱스고정

		pRoom = dynamic_cast<CRoom*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::LEVEL_TOWN), TEXT("Prototype_GameObject_Room")));
		NULL_CHECK_RETURN(pRoom, E_FAIL);

		pRoom->Load_From_File(ENUM_CLASS(LEVEL::LEVEL_TOWN), strLayerTag, TEXT("../../data/Town.txt"), iNumber, iRoomX, iRoomZ, ROOM_INFO::EVENT_NORMAL);
		iNumber++;

		
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_GameObject(ENUM_CLASS(LEVEL::LEVEL_TOWN), TEXT("Layer_Player")));
		pPlayer->Change_TerrainBox(dynamic_cast<CTerrainBox*>(pRoom->Get_TerrainBox()), iNumber);

		ROOMCHANGE EventDesc;
		EventDesc.vPosition = dynamic_cast<CTransform*>(pRoom->Get_TerrainBox()->Find_Component(TEXT("Com_Transform_TerrainBox")))->Get_State(STATE::POSITION);
		m_pGameInstance->Broadcast(ENUM_CLASS(EVENT_TYPE::ROOMCHANGE), &EventDesc);
	

		// 룸매니저 투입
		CRoom_Manager::GetInstance()->Add_Room(pRoom, ENUM_CLASS(LEVEL::LEVEL_TOWN), strLayerTag);
	
		// 포탈 설치
		CRoom_Manager::GetInstance()->Check_SpecialRoom(LEVEL::LEVEL_TOWN, strLayerTag, 0); //마을은 룸인덱스 0 한개
	
		//CRoom_Manager::GetInstance()->Check_Room(ENUM_CLASS(LEVEL::LEVEL_TOWN), strLayerTag, iNumber);
		// 쉼터, 마을 전용 포탈 생성 함수 만들어줘야할거 같음.



	return S_OK;
}

CLevel_Town* CLevel_Town::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_Town* pInstance = new CLevel_Town(pGraphic_Device);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CLevel_Town"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Town::Free()
{
	__super::Free();
}
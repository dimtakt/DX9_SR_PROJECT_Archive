#include "Level_Shelter.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "Room_Manager.h"
#include "Point.h"
#include "Camera_Follow.h"
#include "Dagger.h"
#include "Player.h"

CLevel_Shelter::CLevel_Shelter(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel{ pGraphic_Device }
{
}

HRESULT CLevel_Shelter::Initialize()
{
	CRoom_Manager::GetInstance()->Clear(ENUM_CLASS(LEVEL::LEVEL_BOSS1));

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
	m_pGameInstance->PlayBGM(L"mainTheme02.wav", g_fBGMVolume - 0.9f);
	return S_OK;
}

void CLevel_Shelter::Update(_float fTimeDelta)
{

	return;
}



HRESULT CLevel_Shelter::Render()
{
	SetWindowText(g_hWnd, TEXT("쉘터 레벨입니다."));

	return S_OK;
}

HRESULT CLevel_Shelter::Ready_Light(const _wstring& strLayerTag)
{
	CPoint::POINTDESC desc{};
	desc.pTargetTransform = dynamic_cast<CTransform*>(m_pGameInstance->Get_GameObject(ENUM_CLASS(LEVEL::LEVEL_SHELTER), TEXT("Layer_Player"))->Find_Component(TEXT("Com_Transform")));
	desc.strLightID = TEXT("Player_Light");
	desc.desc.eType = LIGHT_TYPE::POINT;
	desc.desc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	desc.desc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);
	desc.desc.vAmbient = _float4(0.01f, 0.01f, 0.01f, 1.f); // 기본 어두움 유지
	desc.desc.fSpecPower = 64.f;
	desc.desc.fRange = 15.f;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_SHELTER), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Point"), &desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Shelter::Ready_Layer_Camera(const _wstring& strLayerTag)
{
	CCamera_Follow::CAMERAFOLDESC desc{};
	desc.iLayerIndex = ENUM_CLASS(LEVEL::LEVEL_SHELTER);

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_SHELTER), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_SHELTER), TEXT("Prototype_GameObject_Camera_Follow"), &desc)))
		return E_FAIL;
}

HRESULT CLevel_Shelter::Ready_Layer_BackGround(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_SHELTER), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_SHELTER), TEXT("Prototype_GameObject_Sky"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Shelter::Ready_Layer_Player(const _wstring& strLayerTag)
{
	CDagger::DAGGERDESC DaggerDesc{};
	DaggerDesc.iLayerIndex = ENUM_CLASS(LEVEL::LEVEL_SHELTER);

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_SHELTER), TEXT("Layer_Weapon"),
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Weapon_Dagger"), &DaggerDesc)))
		return E_FAIL;

	CPlayer::PLAYERDESC PlayerDesc{};
	PlayerDesc.iLayerIndex = ENUM_CLASS(LEVEL::LEVEL_SHELTER);

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_SHELTER), TEXT("Layer_Player"),
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Player"), &PlayerDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Shelter::Ready_Layer_UI(const _wstring& strLayerTag)
{
	CUIObject::UIOBJECT_DESC Desc{};

	Desc.m_iLevel = ENUM_CLASS(LEVEL::LEVEL_SHELTER);

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_SHELTER), TEXT("Layer_Hud_States"),
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_UI_Hud_States"), &Desc)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_SHELTER), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_UI_Hud_Exp"), &Desc)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_SHELTER), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_UI_Hud_Wallet"), &Desc)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_SHELTER), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_UI_Hud_Dash"), &Desc)))
		return E_FAIL;
	Desc.fX = 190.f;
	Desc.fY = 100.f;
	Desc.fZ = 0;
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_SHELTER), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_UI_Hud_Buff"), &Desc)))
		return E_FAIL;

	Desc.fX = 227.f;
	Desc.fY = 100.f;
	Desc.fZ = 1;
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_SHELTER), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_UI_Hud_Buff"), &Desc)))
		return E_FAIL;

	Desc.fX = 264.f;
	Desc.fY = 100.f;
	Desc.fZ = 2;
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_SHELTER), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_UI_Hud_Buff"), &Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Shelter::Ready_Layer_Room(const _wstring& strLayerTag)
{
	CRoom* pRoom = nullptr;
	m_pGameInstance->Seed_Random();

	_int iNumber = 0;
	_int iRoomX = 0;
	_int iRoomZ = 0;

	CRoom_Manager::GetInstance()->Create_SpecialRoom(LEVEL::LEVEL_SHELTER); // 마을 인덱스고정

	pRoom = dynamic_cast<CRoom*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::LEVEL_SHELTER), TEXT("Prototype_GameObject_Room")));
	NULL_CHECK_RETURN(pRoom, E_FAIL);

	pRoom->Load_From_File(ENUM_CLASS(LEVEL::LEVEL_SHELTER), strLayerTag, TEXT("../../data/Shelter.txt"), iNumber, iRoomX, iRoomZ, ROOM_INFO::EVENT_NORMAL);
	iNumber++;


	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_GameObject(ENUM_CLASS(LEVEL::LEVEL_SHELTER), TEXT("Layer_Player")));
	pPlayer->Change_TerrainBox(dynamic_cast<CTerrainBox*>(pRoom->Get_TerrainBox()), iNumber);

	ROOMCHANGE EventDesc;
	EventDesc.vPosition = dynamic_cast<CTransform*>(pRoom->Get_TerrainBox()->Find_Component(TEXT("Com_Transform_TerrainBox")))->Get_State(STATE::POSITION);
	m_pGameInstance->Broadcast(ENUM_CLASS(EVENT_TYPE::ROOMCHANGE), &EventDesc);


	// 룸매니저 투입
	CRoom_Manager::GetInstance()->Add_Room(pRoom, ENUM_CLASS(LEVEL::LEVEL_SHELTER), strLayerTag);

	// 포탈 설치
	CRoom_Manager::GetInstance()->Check_SpecialRoom(LEVEL::LEVEL_SHELTER, strLayerTag, 0); //마을은 룸인덱스 0 한개
}

CLevel_Shelter* CLevel_Shelter::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_Shelter* pInstance = new CLevel_Shelter(pGraphic_Device);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CLevel_Shelter"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Shelter::Free()
{
	__super::Free();
}
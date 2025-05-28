#include "Level_Stage1.h"
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

CLevel_Stage1::CLevel_Stage1(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel{ pGraphic_Device }
{
}

HRESULT CLevel_Stage1::Initialize()
{
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

void CLevel_Stage1::Update(_float fTimeDelta)
{
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
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE1), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_GameObject_Camera_Follow"))))
		return E_FAIL;

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
	/*if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STATIC), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Player"))))
		return E_FAIL;*/

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_GameObject(ENUM_CLASS(LEVEL::LEVEL_STATIC), strLayerTag));
	
	CRoom* pRoom = CRoom_Manager::GetInstance()->Get_CurrentRoom();
	
	pPlayer->Change_TerrainBox(dynamic_cast<CTerrainBox*>(pRoom->Get_TerrainBox()));

	return S_OK;
}

HRESULT CLevel_Stage1::Ready_Layer_UI(const _wstring& strLayerTag)
{
	
	return S_OK;
}

HRESULT CLevel_Stage1::Ready_Layer_Room(const _wstring& strLayerTag)
{
	CRoom* pRoom = nullptr;

	for (size_t num = 0; num < 5; num++)
	{

		pRoom = dynamic_cast<CRoom*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_GameObject_Room")));
		NULL_CHECK_RETURN(pRoom, E_FAIL);
		// 지형 셋팅
		CTerrainBox* pTerrainBox = nullptr;

		MAP_OBJECT_DESC tDesc{};
		tDesc.iTextureIndex = 0;
		tDesc.vPos = _float3(static_cast<_float>(num) * 20.f + 2.f, 0.f, 0.f);
		pTerrainBox = dynamic_cast<CTerrainBox*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_GameObject_TerrainBox"), &tDesc));
		NULL_CHECK_RETURN(pTerrainBox, E_FAIL);
		pRoom->Add_TerrainBox(pTerrainBox);

		// 몬스터 셋팅
		CMonster* pMonster = nullptr;
		list<CMonster::MONSTERDESC> DescList;
		for (size_t i = 0; i < 20; i++)
		{
			CMonster::MONSTERDESC tDesc = {};
			tDesc.iLayerLevelIndex = ENUM_CLASS(LEVEL::LEVEL_STAGE1);
			tDesc.iPrototypeLevelIndex = ENUM_CLASS(LEVEL::LEVEL_STAGE1);
			tDesc.strLayerTag = strLayerTag;
			tDesc.strPrototypeTag = TEXT("Prototype_GameObject_ShortMonster");
			tDesc.vPosition = _float3(10.f * i + 10.f, 0.f, 5.f * i + 5.f);
			tDesc.pTerrainBox = pTerrainBox;
			//pMonster = dynamic_cast<CMonster_Default*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_GameObject_ShortMonster")));
			//NULL_CHECK_RETURN(pMonster, E_FAIL);
			//pRoom->Add_Monster(pMonster);
			DescList.push_back(tDesc);
		}
		CMonster_Factory::GetInstance()->Add_Monsters(pRoom, DescList);
		

		// 오브젝트 셋팅

		// 룸매니저 투입
		CRoom_Manager::GetInstance()->Add_Room(pRoom, ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Layer_Room"));
	}

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

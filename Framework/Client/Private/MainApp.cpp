#include "MainApp.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "Player.h"
#include "Room.h"
#include "Dagger.h"
#include "Item_Base.h"
#include "Room_Manager.h"
#include "Monster_Factory.h"
#include "Collider_OBB.h"
#include "PlayerEffect.h"
#include "Stat_Manager.h"

CMainApp::CMainApp()
	: m_pGameInstance{ CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CMainApp::Initialize()
{
	ENGINE_DESC		EngineDesc{};

	EngineDesc.hWnd = g_hWnd;
	EngineDesc.eWinMode = WINMODE::MODE_WIN;
	EngineDesc.iWinSizeX = g_iWinSizeX;
	EngineDesc.iWinSizeY = g_iWinSizeY;
	EngineDesc.iNumLevels = ENUM_CLASS(LEVEL::LEVEL_END);

	if (FAILED(m_pGameInstance->Initialize_Engine(EngineDesc, &m_pGraphic_Device)))
		return E_FAIL;

	if (FAILED(Ready_Default_Setting()))
		return E_FAIL;

	if (FAILED(Ready_Prototype_ForStatic()))
		return E_FAIL;

	if (FAILED(Ready_Item_Setting()))
		return E_FAIL;

	if (FAILED(Start_Level(LEVEL::LEVEL_LOGO)))
		return E_FAIL;

	if (FAILED(Ready_Manager_Setting()))
		return E_FAIL;

	Ready_Key_Setting();
	Ready_Font_Setting();
	return S_OK;
}

void CMainApp::Update(_float fTimeDelta)
{
	m_pGameInstance->Update_Engine(fTimeDelta);
}

HRESULT CMainApp::Render()
{
	m_pGameInstance->Render_Begin(D3DXCOLOR(0.f, 0.f, 1.f, 1.f));

	m_pGameInstance->Draw();

	m_pGameInstance->Render_End();

	return S_OK;
}

HRESULT CMainApp::Ready_Default_Setting()
{
	g_hCursor = LoadCursorFromFile(L"Resources/Sephiria/UI/Cursor/Cursor_Arrow.cur");

	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_ForStatic()
{
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), CVIBuffer_Rect::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Cube"), CVIBuffer_Cube::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"), CTransform::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_TerrainBox"), CVIBuffer_TerrainBox::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_TerrainBox_Top"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/BleakSwordDX/Terrain/Forest/ArenaTex_%d.png"), 25))))
		return E_FAIL;
	/* Prototype_Component_Texture_TerrainBox_Side */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_TerrainBox_Side"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/BleakSwordDX/Terrain/Basic/BaseArenaTex.png"), 1))))
		return E_FAIL;

	//나무 텍스처 추가, 15개
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_SHARED), TEXT("Prototype_Component_Texture_Tree"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/BleakSwordDX/Object/Tree/ForestTrees_%d.png"), 16))))
		return E_FAIL;

	/* Prototype_Component_Texture_Sky */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Sky"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/BleakSwordDX/SkyBox/Sky.png"), 1))))
		return E_FAIL;


	// Prototype_Component_Texture_Player Bin\Resources\BleakSwordDX\Terrain\Forest
	// --- CTexture
	// Roll
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Player_Roll"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Player/Player_Basic_Roll0%d.png"), 7))))
		return E_FAIL;
	// Air
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Player_Air"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Player/Player_Basic_Air0%d.png"), 3))))
		return E_FAIL;
	// Down
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Player_Down"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Player/Player_Basic_Down0%d.png"), 1))))
		return E_FAIL;
	// Idle (Lower, Upper)
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Player_Idle_Lower"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Player/Player_Basic_Idle_Lower0%d.png"), 6))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Player_Idle_Upper"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Player/Player_Basic_Idle_Upper0%d.png"), 6))))
		return E_FAIL;
	// Move (Lower, Upper)
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Player_Move_Lower"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Player/Player_Basic_Move_Lower0%d.png"), 8))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Player_Move_Upper"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Player/Player_Basic_Move_Upper0%d.png"), 8))))
		return E_FAIL;
	// Attack (Lower, Upper)
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Player_Attack_Lower"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Player/Player_Basic_Attack_Lower0%d.png"), 3))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Player_Attack_Upper"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Player/Player_Basic_Attack_Upper0%d.png"), 3))))
		return E_FAIL;
	// GreatSwordHeavyAttack (Lower, Upper)
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Player_GreatSwordHeavyAttack_Lower"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Player/Player_Basic_GreatSwordHeavyAttack_Lower0%d.png"), 10))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Player_GreatSwordHeavyAttack_Upper"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Player/Player_Basic_GreatSwordHeavyAttack_Upper0%d.png"), 10))))
		return E_FAIL;
	// WhirlWind_Ready
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Player_WhirlWind_Ready"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Player/Player_Basic_WhirlWind_Ready0%d.png"), 4))))
		return E_FAIL;
	// WhirlWind_Cycle
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Player_WhirlWind_Cycle"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Player/Player_Basic_WhirlWind_Cycle0%d.png"), 8))))
		return E_FAIL;
	

	// Prototype_Component_Texture_Effect
	// --- CTexture
	// Blade0_Swing0
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Effect_Blade0_Swing0"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Effect/Blade0_Swing0_%d.png"), 3))))
		return E_FAIL;
	// Blade0_Swing1
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Effect_Blade0_Swing1"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Effect/Blade0_Swing1_R%d.png"), 3))))
		return E_FAIL;


	//-------------

	/* Prototype_GameObject_Player */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Player"), CPlayer::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_Weapon_Dagger */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Weapon_Dagger"), CDagger::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_Component_Texture_Weapon_Dagger*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Weapon_Dagger"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Player_Weapon/Weapon_Dagger0.png"), 1))))
		return E_FAIL;

	/* Prototype_Component_PlayerStats */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_PlayerStats"), CPlayerStats::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_Component_Animator  */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Animator"), CAnimator::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_Room */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Room"), CRoom::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_Component_Collider_OBB */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Collider_OBB"), CCollider_OBB::Create(m_pGraphic_Device))))
		return E_FAIL;

	//-------------
	/* Prototype_GameObject_Item  */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Item"), CItem_Base::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Item"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/UI/Item/Item_Icon_%d.png"), g_ItemDataBase.size()))))
		return E_FAIL;

	/* Prototype_GameObject_PlayerEffect */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_PlayerEffect"),
		CPlayerEffect::Create(m_pGraphic_Device))))
		return E_FAIL;

	return S_OK;
}

void CMainApp::Ready_Key_Setting()
{
	m_pGameInstance->AddTrackingKey(VK_UP);
	m_pGameInstance->AddTrackingKey(VK_DOWN);
	m_pGameInstance->AddTrackingKey(VK_LEFT);
	m_pGameInstance->AddTrackingKey(VK_RIGHT);
	m_pGameInstance->AddTrackingKey(VK_SPACE);
	m_pGameInstance->AddTrackingKey(VK_LBUTTON);
	m_pGameInstance->AddTrackingKey(VK_RBUTTON);
	m_pGameInstance->AddTrackingKey(VK_TAB);
	m_pGameInstance->AddTrackingKey(VK_RETURN);
	m_pGameInstance->AddTrackingKey(VK_ESCAPE);
	m_pGameInstance->AddTrackingKey('W');
	m_pGameInstance->AddTrackingKey('A');
	m_pGameInstance->AddTrackingKey('S');
	m_pGameInstance->AddTrackingKey('D');
	m_pGameInstance->AddTrackingKey('V');
	m_pGameInstance->AddTrackingKey('C');
	m_pGameInstance->AddTrackingKey('P');
	m_pGameInstance->AddTrackingKey('Q');
	m_pGameInstance->AddTrackingKey('E');
	m_pGameInstance->AddTrackingKey('L');
	m_pGameInstance->AddTrackingKey('Z');
	m_pGameInstance->AddTrackingKey('R');
	m_pGameInstance->AddTrackingKey('F');
	// 임시 테스트용
#if _DEBUG
	m_pGameInstance->AddTrackingKey('J');
	m_pGameInstance->AddTrackingKey('K');
#endif

}

void CMainApp::Ready_Font_Setting()
{
	if (FAILED(m_pGameInstance->Ready_Font(TEXT("UI_Font_18"), TEXT("../Bin/Resources/Font/Galmuri9.ttf"), TEXT("Galmuri9 Regular"), 0, 18, 900)))
		MSG_BOX(TEXT("FAILED to Font"));
	if (FAILED(m_pGameInstance->Ready_Font(TEXT("UI_Font_16"), TEXT("../Bin/Resources/Font/Galmuri9.ttf"), TEXT("Galmuri9 Regular"), 0, 16, 900)))
		MSG_BOX(TEXT("FAILED to Font"));
	if (FAILED(m_pGameInstance->Ready_Font(TEXT("UI_Font_14"), TEXT("../Bin/Resources/Font/Galmuri9.ttf"), TEXT("Galmuri9 Regular"), 0, 14, 900)))
		MSG_BOX(TEXT("FAILED to Font"));
	if (FAILED(m_pGameInstance->Ready_Font(TEXT("UI_Font_12"), TEXT("../Bin/Resources/Font/Galmuri9.ttf"), TEXT("Galmuri9 Regular"), 14, 13, 700)))
		MSG_BOX(TEXT("FAILED to Font"));

	if (FAILED(m_pGameInstance->Ready_Font(TEXT("UI_Font_Logo"), TEXT("../Bin/Resources/Font/Galmuri9.ttf"), TEXT("Galmuri9 Regular"), 31, 36, 400)))
		MSG_BOX(TEXT("FAILED to Font"));
}

HRESULT CMainApp::Ready_Item_Setting()
{
	CItemObject::ITEMOBJECT_DESC* pDesc = new CItemObject::ITEMOBJECT_DESC[g_ItemDataBase.size()];
	for (_uint i = 0; i < g_ItemDataBase.size(); i++)
	{
		pDesc[i].iItemID = g_ItemDataBase[i].m_iItemID;
		pDesc[i].iItemTextureID = g_ItemDataBase[i].m_iItemTextureID;
		pDesc[i].iItemType = ENUM_CLASS(g_ItemDataBase[i].m_eType);
		pDesc[i].iRarity = ENUM_CLASS(g_ItemDataBase[i].m_eRarity);
		pDesc[i].szDescription = g_ItemDataBase[i].m_szDescription;
		pDesc[i].szName = g_ItemDataBase[i].m_szName;
		pDesc[i].iItemValue = g_ItemDataBase[i].m_iItemValue;
	}

	m_pGameInstance->Setting_Item(pDesc, g_ItemDataBase.size(), ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Item"));

	Safe_Delete_Array(pDesc);

	return S_OK;
}

HRESULT CMainApp::Start_Level(LEVEL eStartLevelID)
{
  	if (FAILED(m_pGameInstance->Open_Level(ENUM_CLASS(LEVEL::LEVEL_LOADING), CLevel_Loading::Create(m_pGraphic_Device, eStartLevelID))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Manager_Setting()
{
	return S_OK;
}

CMainApp* CMainApp::Create()
{
	CMainApp* pInstance = new CMainApp();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CMainApp"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainApp::Free()
{
	__super::Free();

	Safe_Release(m_pGraphic_Device);
	CRoom_Manager::GetInstance()->Free();
	CMonster_Factory::GetInstance()->Free();
	CStat_Manager::GetInstance()->Free();
	m_pGameInstance->Release_Engine();
	Safe_Release(m_pGameInstance);

#ifdef _DEBUG

	// 디버그용 콘솔창 제거
	FreeConsole();

#endif // _DEBUG
}
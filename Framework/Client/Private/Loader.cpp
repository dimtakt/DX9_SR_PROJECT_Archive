#include "Loader.h"

#include "GameInstance.h"

#include "BackGround.h"
#include "Title_BackGround.h"
#include "Terrain.h"
#include "Camera_Free.h"
#include "Camera_Follow.h"
#include "Camera_Mouse.h"
#include "Player.h"
#include "Mp_Player.h"
#include "Hp_Player.h"
#include "Hud_States_Frame.h"
#include "Hud_Exp_Frame.h"
#include "Hud_Wallet.h"
#include "Hud_Button.h"
#include "Hud_Buff.h"
#include "Inventory.h"
#include "Hud_Dash.h"
#include "Tree.h"
#include "MapEditObject.h"
#include "Field_Hp.h"
#include "Room.h"

#include "Monster.h"
#include "Mole_A.h"
#include "Oink_A.h"
#include "LaserGhost_D.h"
#include "Erma.h"
#include "Erma_Body.h"
#include "Erma_Hand_L.h"
#include "Erma_Hand_R.h"
#include "Erma_Head.h"

#include "TerrainBox.h"
#include "Dagger.h"
#include "Sky.h"
#include "ChapMap.h"
#include "Room_Manager.h"
#include "Status_Window.h"
#include "Potal.h"
#include "Interaction_Normal.h"

CLoader::CLoader(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device{ pGraphic_Device }
	, m_pGameInstance{ CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pGraphic_Device);
}

unsigned int APIENTRY LoadingMain(void* pArg)
{
	CLoader* pLoader = static_cast<CLoader*>(pArg);

	if (FAILED(pLoader->Loading()))
		return 1;

	return 0;
}

HRESULT CLoader::Initialize(LEVEL eNextLevelID)
{
	CRoom_Manager::GetInstance()->Clear(m_pGameInstance->Get_CurrentLevel());

	m_eNextLevelID = eNextLevelID;

	InitializeCriticalSection(&m_CriticalSection);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, LoadingMain, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Loading()
{
	EnterCriticalSection(&m_CriticalSection);

	HRESULT			hr = {};

	switch (m_eNextLevelID)
	{
	case LEVEL::LEVEL_LOGO:
		hr = Loading_For_Logo_Level();
		break;
	case LEVEL::LEVEL_TOWN:
		hr = Loading_For_Town_Level();
		break;
	case LEVEL::LEVEL_MAPEDIT:
		hr = Loading_For_MapEdit_Level();
		break;
	case LEVEL::LEVEL_STAGE1:
		hr = Loading_For_Stage1_Level();
		break;
	case LEVEL::LEVEL_STAGE2:
		hr = Loading_For_Stage2_Level();
		break;
	case LEVEL::LEVEL_STAGE3:
		hr = Loading_For_Stage3_Level();
		break;
	case LEVEL::LEVEL_STAGE4:
		hr = Loading_For_Stage4_Level();
		break;
	case LEVEL::LEVEL_BOSS1:
		hr = Loading_For_Boss1_Level();
		break;
	case LEVEL::LEVEL_SHELTER:
		hr = Loading_For_Shelter_Level();
		break;
	case LEVEL::LEVEL_BOSS2:
		hr = Loading_For_Boss2_Level();
		break;
	
	}

	if(FAILED(hr))
		return E_FAIL;

	LeaveCriticalSection(&m_CriticalSection);

	return S_OK;
}


HRESULT CLoader::Loading_For_Logo_Level()
{
	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩중입니다."));
	/* Prototype_Component_Texture_BackGround */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_LOGO), TEXT("Prototype_Component_Texture_Title_Logo"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Title/Title.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_LOGO), TEXT("Prototype_Component_Texture_Title_Back"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Title/Title_Back.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_LOGO), TEXT("Prototype_Component_Texture_Title_ReverseTree"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Title/Title_ReverseTree.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_LOGO), TEXT("Prototype_Component_Texture_Title_Tree"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Title/Title_Tree%d.png"), 16))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_LOGO), TEXT("Prototype_Component_Texture_Title_TreeBegin"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Title/Title_TreeBegin%d.png"), 28))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_LOGO), TEXT("Prototype_Component_Texture_Title_Star"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Title/Star.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_LOGO), TEXT("Prototype_Component_Texture_Title_Logo_Wave"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Title/Title_Wave.png"), 1))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("모델를 로딩중입니다."));

	lstrcpy(m_szLoadingText, TEXT("쉐이더를 로딩중입니다."));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_LOGO), TEXT("Prototype_Component_Shader_LogoRipple"),
		CShader::Create(m_pGraphic_Device, TEXT("../Bin/ShaderFiles/LogoRipple.hlsl")))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_LOGO), TEXT("Prototype_Component_Shader_StarColor"),
		CShader::Create(m_pGraphic_Device, TEXT("../Bin/ShaderFiles/StarColor.hlsl")))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_LOGO), TEXT("Prototype_Component_Shader_TitleFadeIn"),
		CShader::Create(m_pGraphic_Device, TEXT("../Bin/ShaderFiles/TitleFadeIn.hlsl")))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("게임오브젝트를 로딩중입니다."));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_LOGO), TEXT("Prototype_GameObject_Title_BackGround"),
		CTitle_BackGround::Create(m_pGraphic_Device, LEVEL::LEVEL_LOGO))))
		return E_FAIL;

	/*for (size_t i = 0; i < 100000000; i++)
	{
		int a = 1;
	}*/

	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Town_Level()
{
	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩중입니다."));

	lstrcpy(m_szLoadingText, TEXT("모델를 로딩중입니다."));

	lstrcpy(m_szLoadingText, TEXT("쉐이더를 로딩중입니다."));

	lstrcpy(m_szLoadingText, TEXT("게임오브젝트를 로딩중입니다."));
	///* Prototype_GameObject_Camera*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_TOWN), TEXT("Prototype_GameObject_Camera_Follow"),
		CCamera_Follow::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_Land*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_TOWN), TEXT("Prototype_GameObject_Room"),
		CRoom::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_Sky */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_TOWN), TEXT("Prototype_GameObject_Sky"),
		CSky::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_TerrainBox*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_TOWN), TEXT("Prototype_GameObject_TerrainBox"),
		CTerrainBox::Create(m_pGraphic_Device))))
		return E_FAIL;

	//Tree
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_TOWN), TEXT("Prototype_GameObject_Tree"),
		CTree::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_TOWN), TEXT("Prototype_GameObject_Potal"),
		CPotal::Create(m_pGraphic_Device))))
		return E_FAIL;


	/*for (size_t i = 0; i < 19000000000; i++)
	{
		int a = 1;
	}*/

	for (size_t i = 0; i < 1000; i++)
	{
		int a = 1;
	}
	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_MapEdit_Level()
{
	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩중입니다."));

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_MAPEDIT), TEXT("Prototype_Component_Texture_Interaction"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/MapEdit/Interaction_%d.png"), 9))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("모델를 로딩중입니다."));

	lstrcpy(m_szLoadingText, TEXT("쉐이더를 로딩중입니다."));

	lstrcpy(m_szLoadingText, TEXT("게임오브젝트를 로딩중입니다."));

	// Camera
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_MAPEDIT), TEXT("Prototype_GameObject_Camera_Mouse"),
		CCamera_Mouse::Create(m_pGraphic_Device))))
		return E_FAIL;

	//Tree
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_MAPEDIT), TEXT("Prototype_GameObject_Tree"),
		CTree::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_TerrainBox*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_MAPEDIT), TEXT("Prototype_GameObject_TerrainBox"),
		CTerrainBox::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_Interaction_Normal  */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_MAPEDIT), TEXT("Prototype_GameObject_Interaction_Normal"),
		CMapEditObject::Create(m_pGraphic_Device))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));
	m_isFinished = true;
	return S_OK;

	return S_OK;
}

HRESULT CLoader::Loading_For_Stage1_Level()
{
	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩중입니다."));
	/* Prototype_Component_Texture_Land */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_Component_Texture_Land"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/BleakSwordDX/Terrain/Basic/BlankTex16_00.png"), 1))))
		return E_FAIL;

	/* Prototype_Component_Texture_Monster */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_Component_Texture_Monster"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Textures/Player/Player0.png"), 1))))
		return E_FAIL;


	// Prototype_Component_Texture_Monster (Mole_A)
	// Idle
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_Component_Texture_Mole_A_Idle"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Monster/Mole_A/Duelist_Mole_Idle0%d.png"), 6))))
		return E_FAIL;
	// Move
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_Component_Texture_Mole_A_Move"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Monster/Mole_A/Duelist_Mole_Move0%d.png"), 10))))
		return E_FAIL;
	// Down
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_Component_Texture_Mole_A_Down"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Monster/Mole_A/Duelist_Mole_Down00.png"), 1))))
		return E_FAIL;
	// Attack
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_Component_Texture_Mole_A_Attack"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Monster/Mole_A/Duelist_Mole_Attack0%d.png"), 6))))
		return E_FAIL;
	// Airborne
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_Component_Texture_Mole_A_Airborne"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Monster/Mole_A/Duelist_Mole_Airborne00.png"), 1))))
		return E_FAIL;

	// Prototype_Component_Texture_Monster (Oink_A)
	// Idle
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_Component_Texture_Oink_A_Idle"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Monster/Oink_A/OinkChief_Idle%d.png"), 6))))
		return E_FAIL;
	// Move
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_Component_Texture_Oink_A_Move"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Monster/Oink_A/OinkChief_Move%d.png"), 8))))
		return E_FAIL;
	// Attack
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_Component_Texture_Oink_A_Attack"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Monster/Oink_A/OinkChief_Attack%d.png"), 9))))
		return E_FAIL;
	// ChargeReady
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_Component_Texture_Oink_A_ChargeReady"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Monster/Oink_A/OinkChief_ChargeReady%d.png"), 3))))
		return E_FAIL;
	// ChargeReady_Cycle
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_Component_Texture_Oink_A_ChargeReady_Cycle"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Monster/Oink_A/OinkChief_ChargeReady_Cycle%d.png"), 4))))
		return E_FAIL;
	// Charge_Cycle
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_Component_Texture_Oink_A_Charge_Cycle"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Monster/Oink_A/OinkChief_Charge_Cycle%d.png"), 4))))
		return E_FAIL;
	// Charge_Airborne
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_Component_Texture_Oink_A_Charge_Airborne"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Monster/Oink_A/OinkChief_Charge_Airborne%d.png"), 4))))
		return E_FAIL;
	// Charge_Down
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_Component_Texture_Oink_A_Charge_Down"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Monster/Oink_A/OinkChief_Charge_Down0.png"), 1))))
		return E_FAIL;
	// Charge_End
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_Component_Texture_Oink_A_Charge_End"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Monster/Oink_A/OinkChief_Charge_End%d.png"), 4))))
		return E_FAIL;

	// Prototype_Component_Texture_Monster (LaserGhost_D)
    // 파일명을 00, 01, ..., 09, 10, 11, 12와 같이 2자리로 불러오려면 "%02d" 포맷을 사용해야 합니다.
	// Idle
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_Component_Texture_LaserGhost_D_Idle"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Monster/LaserGhost_D/LaserGhost_Idle%02d.png"), 14))))
		return E_FAIL;
	// Move 14
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_Component_Texture_LaserGhost_D_Move"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Monster/LaserGhost_D/LaserGhost_Move%02d.png"), 14))))
		return E_FAIL;
	// Down 03
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_Component_Texture_LaserGhost_D_Down"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Monster/LaserGhost_D/LaserGhost_Down%02d.png"), 3))))
		return E_FAIL;
	// AttackReady 28
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_Component_Texture_LaserGhost_D_AttackReady"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Monster/LaserGhost_D/LaserGhost_AttackReady%02d.png"), 28))))
		return E_FAIL;
	// Attack_Start 01
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_Component_Texture_LaserGhost_D_Attack_Start"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Monster/LaserGhost_D/LaserGhost_Attack_Start%02d.png"), 1))))
		return E_FAIL;
	// Attack_Cycle 07
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_Component_Texture_LaserGhost_D_Attack_Cycle"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Monster/LaserGhost_D/LaserGhost_Attack_Cycle%02d.png"), 7))))
		return E_FAIL;
	// Attack_End 08
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_Component_Texture_LaserGhost_D_Attack_End"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Monster/LaserGhost_D/LaserGhost_Attack_End%02d.png"), 8))))
		return E_FAIL;
	// Airborne 03
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_Component_Texture_LaserGhost_D_Airborne"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Monster/LaserGhost_D/LaserGhost_Airborne%02d.png"), 3))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("모델을 로딩중입니다."));

	lstrcpy(m_szLoadingText, TEXT("쉐이더를 로딩중입니다."));

	lstrcpy(m_szLoadingText, TEXT("게임오브젝트를 로딩중입니다."));

	///* Prototype_GameObject_Camera*/
 	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_GameObject_Camera_Follow"),
		CCamera_Follow::Create(m_pGraphic_Device))))
		return E_FAIL;

	// Camera
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_GameObject_Camera_Mouse"),
		CCamera_Mouse::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_Land*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_GameObject_Room"),
		CRoom::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_Monster */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_GameObject_ShortMonster"),
		CMonster::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_GameObject_Monster_Mole_A"),
		CMole_A::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_GameObject_Monster_Oink_A"),
		COink_A::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_GameObject_Monster_LaserGhost_D"),
		CLaserGhost_D::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_Sky */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_GameObject_Sky"),
		CSky::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_TerrainBox*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_GameObject_TerrainBox"),
		CTerrainBox::Create(m_pGraphic_Device))))
		return E_FAIL;

	//Tree
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_GameObject_Tree"),
		CTree::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_GameObject_Potal"),
		CPotal::Create(m_pGraphic_Device))))
		return E_FAIL;

	//lstrcpy(m_szLoadingText, TEXT("룸 데이터를 로딩중입니다."));
	
	//CRoom_Default* pRoom = nullptr;

	//for (size_t num = 0; num < 5; num++)
	//{

	//	pRoom = dynamic_cast<CRoom_Default*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_GameObject_Room")));
	//	NULL_CHECK_RETURN(pRoom, E_FAIL);
	//	pRoom->Enter();
	//	/*if (num == 0)
	//	{
	//		pRoom->Enter();
	//	}*/
	//	// 지형 셋팅
	//	CLand* pLand = nullptr;
	//	CLand::LANDOBJDESC pDesc{};
	//	pDesc.vScale = _float3(10.f, 1.5f, 10.f);
	//	pDesc.vPosition = _float3(static_cast<_float>(num) * 5.f + 2.f, 0.f, 0.f);
	//	pLand = dynamic_cast<CLand*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_GameObject_Land"), &pDesc));
	//	NULL_CHECK_RETURN(pLand, E_FAIL);
	//	pRoom->Add_Land(pLand);


	//	// 몬스터 셋팅
	//	CMonster_Default* pMonster = nullptr;
	//	for (size_t i = 0; i < 20; i++)
	//	{
	//		pMonster = dynamic_cast<CMonster_Default*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_GameObject_ShortMonster")));
	//		NULL_CHECK_RETURN(pMonster, E_FAIL);
	//		pRoom->Add_Monster(pMonster);
	//	}

	//	// 오브젝트 셋팅

	//	// 룸매니저 투입
	//	m_pGameInstance->Add_Room(pRoom, ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Layer_Room"));
	//}


	for (size_t i = 0; i < 10000000000; i++)
	{
		int a = 1;
	}


	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));



	//lstrcpy(m_szLoadingText, m_pGameInstance->Ping());
	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Stage2_Level()
{
	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩중입니다."));

	// Prototype_Component_Texture_Monster (Mole_A)
	// Idle
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE2), TEXT("Prototype_Component_Texture_Mole_A_Idle"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Monster/Mole_A/Duelist_Mole_Idle0%d.png"), 6))))
		return E_FAIL;
	// Move
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE2), TEXT("Prototype_Component_Texture_Mole_A_Move"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Monster/Mole_A/Duelist_Mole_Move0%d.png"), 10))))
		return E_FAIL;
	// Down
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE2), TEXT("Prototype_Component_Texture_Mole_A_Down"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Monster/Mole_A/Duelist_Mole_Down00.png"), 1))))
		return E_FAIL;
	// Attack
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE2), TEXT("Prototype_Component_Texture_Mole_A_Attack"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Monster/Mole_A/Duelist_Mole_Attack0%d.png"), 6))))
		return E_FAIL;
	// Airborne
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE2), TEXT("Prototype_Component_Texture_Mole_A_Airborne"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Monster/Mole_A/Duelist_Mole_Airborne00.png"), 1))))
		return E_FAIL;

	// Prototype_Component_Texture_Monster (Oink_A)
	// Idle
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE2), TEXT("Prototype_Component_Texture_Oink_A_Idle"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Monster/Oink_A/OinkChief_Idle%d.png"), 6))))
		return E_FAIL;
	// Move
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE2), TEXT("Prototype_Component_Texture_Oink_A_Move"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Monster/Oink_A/OinkChief_Move%d.png"), 8))))
		return E_FAIL;
	// Attack
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE2), TEXT("Prototype_Component_Texture_Oink_A_Attack"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Monster/Oink_A/OinkChief_Attack%d.png"), 9))))
		return E_FAIL;
	// ChargeReady
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE2), TEXT("Prototype_Component_Texture_Oink_A_ChargeReady"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Monster/Oink_A/OinkChief_ChargeReady%d.png"), 3))))
		return E_FAIL;
	// ChargeReady_Cycle
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE2), TEXT("Prototype_Component_Texture_Oink_A_ChargeReady_Cycle"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Monster/Oink_A/OinkChief_ChargeReady_Cycle%d.png"), 4))))
		return E_FAIL;
	// Charge_Cycle
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE2), TEXT("Prototype_Component_Texture_Oink_A_Charge_Cycle"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Monster/Oink_A/OinkChief_Charge_Cycle%d.png"), 4))))
		return E_FAIL;
	// Charge_Airborne
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE2), TEXT("Prototype_Component_Texture_Oink_A_Charge_Airborne"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Monster/Oink_A/OinkChief_Charge_Airborne%d.png"), 4))))
		return E_FAIL;
	// Charge_Down
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE2), TEXT("Prototype_Component_Texture_Oink_A_Charge_Down"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Monster/Oink_A/OinkChief_Charge_Down0.png"), 1))))
		return E_FAIL;
	// Charge_End
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE2), TEXT("Prototype_Component_Texture_Oink_A_Charge_End"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Monster/Oink_A/OinkChief_Charge_End%d.png"), 4))))
		return E_FAIL;
	
	lstrcpy(m_szLoadingText, TEXT("모델을 로딩중입니다."));

	lstrcpy(m_szLoadingText, TEXT("쉐이더를 로딩중입니다."));

	lstrcpy(m_szLoadingText, TEXT("게임오브젝트를 로딩중입니다."));

	///* Prototype_GameObject_Camera*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE2), TEXT("Prototype_GameObject_Camera_Follow"),
		CCamera_Follow::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_Land*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE2), TEXT("Prototype_GameObject_Room"),
		CRoom::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_Sky */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE2), TEXT("Prototype_GameObject_Sky"),
		CSky::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_TerrainBox*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE2), TEXT("Prototype_GameObject_TerrainBox"),
		CTerrainBox::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_Monster */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE2), TEXT("Prototype_GameObject_ShortMonster"),
		CMonster::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE2), TEXT("Prototype_GameObject_Monster_Mole_A"),
		CMole_A::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE2), TEXT("Prototype_GameObject_Monster_Oink_A"),
		COink_A::Create(m_pGraphic_Device))))
		return E_FAIL;

	//Tree
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE2), TEXT("Prototype_GameObject_Tree"),
		CTree::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE2), TEXT("Prototype_GameObject_Potal"),
		CPotal::Create(m_pGraphic_Device))))
		return E_FAIL;

	
	for (size_t i = 0; i < 10000000000; i++)
	{
		int a = 1;
	}

	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	//lstrcpy(m_szLoadingText, m_pGameInstance->Ping());
	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Stage3_Level()
{
	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩중입니다."));

	// Prototype_Component_Texture_Monster (Mole_A)
		// Idle
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE3), TEXT("Prototype_Component_Texture_Mole_A_Idle"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Monster/Mole_A/Duelist_Mole_Idle0%d.png"), 6))))
		return E_FAIL;
	// Move
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE3), TEXT("Prototype_Component_Texture_Mole_A_Move"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Monster/Mole_A/Duelist_Mole_Move0%d.png"), 10))))
		return E_FAIL;
	// Down
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE3), TEXT("Prototype_Component_Texture_Mole_A_Down"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Monster/Mole_A/Duelist_Mole_Down00.png"), 1))))
		return E_FAIL;
	// Attack
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE3), TEXT("Prototype_Component_Texture_Mole_A_Attack"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Monster/Mole_A/Duelist_Mole_Attack0%d.png"), 6))))
		return E_FAIL;
	// Airborne
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE3), TEXT("Prototype_Component_Texture_Mole_A_Airborne"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Monster/Mole_A/Duelist_Mole_Airborne00.png"), 1))))
		return E_FAIL;

	// Prototype_Component_Texture_Monster (Oink_A)
	// Idle
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE3), TEXT("Prototype_Component_Texture_Oink_A_Idle"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Monster/Oink_A/OinkChief_Idle%d.png"), 6))))
		return E_FAIL;
	// Move
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE3), TEXT("Prototype_Component_Texture_Oink_A_Move"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Monster/Oink_A/OinkChief_Move%d.png"), 8))))
		return E_FAIL;
	// Attack
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE3), TEXT("Prototype_Component_Texture_Oink_A_Attack"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Monster/Oink_A/OinkChief_Attack%d.png"), 9))))
		return E_FAIL;
	// ChargeReady
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE3), TEXT("Prototype_Component_Texture_Oink_A_ChargeReady"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Monster/Oink_A/OinkChief_ChargeReady%d.png"), 3))))
		return E_FAIL;
	// ChargeReady_Cycle
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE3), TEXT("Prototype_Component_Texture_Oink_A_ChargeReady_Cycle"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Monster/Oink_A/OinkChief_ChargeReady_Cycle%d.png"), 4))))
		return E_FAIL;
	// Charge_Cycle
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE3), TEXT("Prototype_Component_Texture_Oink_A_Charge_Cycle"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Monster/Oink_A/OinkChief_Charge_Cycle%d.png"), 4))))
		return E_FAIL;
	// Charge_Airborne
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE3), TEXT("Prototype_Component_Texture_Oink_A_Charge_Airborne"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Monster/Oink_A/OinkChief_Charge_Airborne%d.png"), 4))))
		return E_FAIL;
	// Charge_Down
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE3), TEXT("Prototype_Component_Texture_Oink_A_Charge_Down"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Monster/Oink_A/OinkChief_Charge_Down0.png"), 1))))
		return E_FAIL;
	// Charge_End
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE3), TEXT("Prototype_Component_Texture_Oink_A_Charge_End"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Monster/Oink_A/OinkChief_Charge_End%d.png"), 4))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("모델를 로딩중입니다."));

	lstrcpy(m_szLoadingText, TEXT("쉐이더를 로딩중입니다."));

	lstrcpy(m_szLoadingText, TEXT("게임오브젝트를 로딩중입니다."));
	///* Prototype_GameObject_Camera*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE3), TEXT("Prototype_GameObject_Camera_Follow"),
		CCamera_Follow::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_Land*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE3), TEXT("Prototype_GameObject_Room"),
		CRoom::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_Sky */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE3), TEXT("Prototype_GameObject_Sky"),
		CSky::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_TerrainBox*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE3), TEXT("Prototype_GameObject_TerrainBox"),
		CTerrainBox::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_Monster */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE3), TEXT("Prototype_GameObject_ShortMonster"),
		CMonster::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE3), TEXT("Prototype_GameObject_Monster_Mole_A"),
		CMole_A::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE3), TEXT("Prototype_GameObject_Monster_Oink_A"),
		COink_A::Create(m_pGraphic_Device))))
		return E_FAIL;

	//Tree
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE3), TEXT("Prototype_GameObject_Tree"),
		CTree::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE3), TEXT("Prototype_GameObject_Potal"),
		CPotal::Create(m_pGraphic_Device))))
		return E_FAIL;


	for (size_t i = 0; i < 10000000000; i++)
	{
		int a = 1;
	}

	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Stage4_Level()
{
	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩중입니다."));
	lstrcpy(m_szLoadingText, TEXT("모델를 로딩중입니다."));

	lstrcpy(m_szLoadingText, TEXT("쉐이더를 로딩중입니다."));

	lstrcpy(m_szLoadingText, TEXT("게임오브젝트를 로딩중입니다."));
	///* Prototype_GameObject_Camera*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE4), TEXT("Prototype_GameObject_Camera_Follow"),
		CCamera_Follow::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_Land*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE4), TEXT("Prototype_GameObject_Room"),
		CRoom::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_Sky */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE4), TEXT("Prototype_GameObject_Sky"),
		CSky::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_TerrainBox*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE4), TEXT("Prototype_GameObject_TerrainBox"),
		CTerrainBox::Create(m_pGraphic_Device))))
		return E_FAIL;

	//Tree
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE4), TEXT("Prototype_GameObject_Tree"),
		CTree::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE4), TEXT("Prototype_GameObject_Potal"),
		CPotal::Create(m_pGraphic_Device))))
		return E_FAIL;


	for (size_t i = 0; i < 10000000000; i++)
	{
		int a = 1;
	}

	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Boss1_Level()
{
	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩중입니다."));

	// Prototype_Component_Texture_Boss (Erma_)
	// Idle
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_BOSS1), TEXT("Prototype_Component_Boss_Erma_Idle"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Boss/Erma/Armadillo/Armadillo_Idle%02d.png"), 6))))
		return E_FAIL;
	// Move
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_BOSS1), TEXT("Prototype_Component_Boss_Erma_Move"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Boss/Erma/Armadillo/Armadillo_Move%02d.png"), 8))))
		return E_FAIL;
	// Airborne
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_BOSS1), TEXT("Prototype_Component_Boss_Erma_Airborne"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Boss/Erma/Armadillo/Armadillo_Airborne%02d.png"), 8))))
		return E_FAIL;
	// Down
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_BOSS1), TEXT("Prototype_Component_Boss_Erma_Down"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Boss/Erma/Armadillo/Armadillo_Down%02d.png"), 4))))
		return E_FAIL;
	// Down_End
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_BOSS1), TEXT("Prototype_Component_Boss_Erma_Down_End"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Boss/Erma/Armadillo/Armadillo_Down03.png"), 1))))
		return E_FAIL;
	// Stun
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_BOSS1), TEXT("Prototype_Component_Boss_Erma_Stun"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Boss/Erma/Armadillo/Armadillo_Stun%02d.png"), 6))))
		return E_FAIL;
	// Enter_Progress (Core로 변신)
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_BOSS1), TEXT("Prototype_Component_Boss_Erma_Enter_Progress"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Boss/Erma/Armadillo/Armadillo%02d.png"), 19))))
		return E_FAIL;
	// Enter_End (변신 완)
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_BOSS1), TEXT("Prototype_Component_Boss_Erma_Enter_End"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Boss/Erma/Armadillo/Armadillo_GolemCore.png"), 1))))
		return E_FAIL;
	// Entered (합체 완 / texture은 nullptr로, 기체 탑승한 상태)
	// 이는 상태로 관리

	// Prototype_Component_Texture_Boss (Body)
	// BigGolem_Body_Broken
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_BOSS1), TEXT("Prototype_Component_Boss_Erma_BigGolem_Body_Broken"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Boss/Erma/Body/BigGolem_Body_Broken.png"), 1))))
		return E_FAIL;
	// BigGolem_Body0
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_BOSS1), TEXT("Prototype_Component_Boss_Erma_BigGolem_Body"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Boss/Erma/Body/BigGolem_Body0.png"), 1))))
		return E_FAIL;

	// Prototype_Component_Texture_Boss (Hand)
	// Normal
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_BOSS1), TEXT("Prototype_Component_Boss_Erma_BigGolem_Hand_Normal"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Boss/Erma/Hand/SeparateByState/BigGolem_Hand_Normal.png"), 1))))
		return E_FAIL;
	// LaserStart
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_BOSS1), TEXT("Prototype_Component_Boss_Erma_BigGolem_Hand_LaserStart"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Boss/Erma/Hand/SeparateByState/BigGolem_Hand_LaserStart%02d.png"), 5))))
		return E_FAIL;
	// LaserProgress_Start
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_BOSS1), TEXT("Prototype_Component_Boss_Erma_BigGolem_Hand_LaserProgress_Start"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Boss/Erma/Hand/SeparateByState/BigGolem_Hand_LaserProgress_Start%02d.png"), 5))))
		return E_FAIL;
	// LaserProgress_Cycle
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_BOSS1), TEXT("Prototype_Component_Boss_Erma_BigGolem_Hand_LaserProgress_Cycle"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Boss/Erma/Hand/SeparateByState/BigGolem_Hand_LaserProgress_Cycle%02d.png"), 4))))
		return E_FAIL;
	// Laser_End
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_BOSS1), TEXT("Prototype_Component_Boss_Erma_BigGolem_Hand_LaserEnd"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Boss/Erma/Hand/SeparateByState/BigGolem_Hand_LaserEnd%02d.png"), 4))))
		return E_FAIL;

	// Prototype_Component_Texture_Boss (Head)
	// Normal
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_BOSS1), TEXT("Prototype_Component_Boss_Erma_BigGolem_Head_Normal"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Boss/Erma/Head/SeparateByState/BigGolem_Head_Normal.png"), 1))))
		return E_FAIL;
	// AttackStart (9)
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_BOSS1), TEXT("Prototype_Component_Boss_Erma_BigGolem_Head_AttackStart"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Boss/Erma/Head/SeparateByState/BigGolem_Head_AttackStart%02d.png"), 9))))
		return E_FAIL;
	// AttackProgress
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_BOSS1), TEXT("Prototype_Component_Boss_Erma_BigGolem_Head_AttackProgress"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Boss/Erma/Head/SeparateByState/BigGolem_Head_AttackProgress.png"), 1))))
		return E_FAIL;
	// AttackEnd (5)
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_BOSS1), TEXT("Prototype_Component_Boss_Erma_BigGolem_Head_AttackEnd"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Boss/Erma/Head/SeparateByState/BigGolem_Head_AttackEnd%02d.png"), 5))))
		return E_FAIL;
	// DramaticDying
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_BOSS1), TEXT("Prototype_Component_Boss_Erma_BigGolem_Head_DramaticDying"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Boss/Erma/Head/SeparateByState/BigGolem_DramaticDying.png"), 1))))
		return E_FAIL;
	// Broken
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_BOSS1), TEXT("Prototype_Component_Boss_Erma_BigGolem_Head_Broken"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Boss/Erma/Head/SeparateByState/BigGolem_Head_Broken.png"), 1))))
		return E_FAIL;
	// Standby
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_BOSS1), TEXT("Prototype_Component_Boss_Erma_BigGolem_Head_Standby"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Boss/Erma/Head/SeparateByState/BigGolem_Head_Standby.png"), 1))))
		return E_FAIL;

	// Prototype_Component_Texture_Boss (Effect)
	// LaserStart (4)
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Boss_Erma_BigGolem_LaserStart"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Boss/Erma/Effect/laser/Merged/SeparateByState/BigGolem_LaserStart%02d.png"), 4))))
		return E_FAIL;
	// LaserProgress (2)
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Boss_Erma_BigGolem_LaserProgress"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Boss/Erma/Effect/laser/Merged/SeparateByState/BigGolem_LaserProgress%02d.png"), 2))))
		return E_FAIL;
	// LaserEnd (6)
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Boss_Erma_BigGolem_LaserEnd"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Boss/Erma/Effect/laser/Merged/SeparateByState/BigGolem_LaserEnd%02d.png"), 6))))
		return E_FAIL;
	
	// HeadStart_tFX (7)
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Boss_Erma_Effect_HeadStart"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Boss/Erma/Effect/HeadStart_tFX%02d.png"), 7))))
		return E_FAIL;
	// HeadEnd_FX (5)
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Boss_Erma_Effect_HeadEnd"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Boss/Erma/Effect/HeadEnd_FX%02d.png"), 5))))
		return E_FAIL;
	// Missile_Alert_ (18)
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Boss_Erma_Missile_Alert"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Boss/Erma/Effect/Missile_Alert_%02d.png"), 18))))
		return E_FAIL;
	// Missile_Dirt_ (5)
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Boss_Erma_Missile_Dirt"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Boss/Erma/Effect/Missile_Dirt_%02d.png"), 5))))
		return E_FAIL;
	// Missile_Upper_ (12)
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Boss_Erma_Missile_Upper"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Boss/Erma/Effect/Missile_Upper_%02d.png"), 12))))
		return E_FAIL;
	// Missile_Upper_Light_ (12)
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Boss_Erma_Missile_Upper_Light"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Boss/Erma/Effect/Missile_Upper_Light_%02d.png"), 12))))
		return E_FAIL;
	// Missile_Lower_ (3)
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Boss_Erma_Missile_Lower"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Boss/Erma/Effect/Missile_Lower_%02d.png"), 3))))
		return E_FAIL;
	// Missile_Lower_Light_ (17)
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Boss_Erma_Missile_Lower_Light"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/Boss/Erma/Effect/Missile_Lower_Light_%02d.png"), 17))))
		return E_FAIL;


	lstrcpy(m_szLoadingText, TEXT("모델를 로딩중입니다."));

	lstrcpy(m_szLoadingText, TEXT("쉐이더를 로딩중입니다."));

	lstrcpy(m_szLoadingText, TEXT("게임오브젝트를 로딩중입니다."));

	///* Prototype_GameObject_Camera*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_BOSS1), TEXT("Prototype_GameObject_Camera_Follow"),
		CCamera_Follow::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_Land*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_BOSS1), TEXT("Prototype_GameObject_Room"),
		CRoom::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_Sky */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_BOSS1), TEXT("Prototype_GameObject_Sky"),
		CSky::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_TerrainBox*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_BOSS1), TEXT("Prototype_GameObject_TerrainBox"),
		CTerrainBox::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_Boss_Erma */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_BOSS1), TEXT("Prototype_GameObject_Boss_Erma"),
		CErma::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_BOSS1), TEXT("Prototype_GameObject_Boss_Erma_Body"),
		CErma_Body::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_BOSS1), TEXT("Prototype_GameObject_Boss_Erma_Hand_L"),
		CErma_Hand_L::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_BOSS1), TEXT("Prototype_GameObject_Boss_Erma_Hand_R"),
		CErma_Hand_R::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_BOSS1), TEXT("Prototype_GameObject_Boss_Erma_Head"),
		CErma_Head::Create(m_pGraphic_Device))))
		return E_FAIL;

	//Tree
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_BOSS1), TEXT("Prototype_GameObject_Tree"),
		CTree::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_BOSS1), TEXT("Prototype_GameObject_Potal"),
		CPotal::Create(m_pGraphic_Device))))
		return E_FAIL;





	for (size_t i = 0; i < 10000000000; i++)
	{
		int a = 1;
	}

	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Shelter_Level()
{
	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩중입니다."));
	lstrcpy(m_szLoadingText, TEXT("모델를 로딩중입니다."));

	lstrcpy(m_szLoadingText, TEXT("쉐이더를 로딩중입니다."));

	lstrcpy(m_szLoadingText, TEXT("게임오브젝트를 로딩중입니다."));
	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));
	return S_OK;
}

HRESULT CLoader::Loading_For_Boss2_Level()
{
	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩중입니다."));
	lstrcpy(m_szLoadingText, TEXT("모델를 로딩중입니다."));

	lstrcpy(m_szLoadingText, TEXT("쉐이더를 로딩중입니다."));

	lstrcpy(m_szLoadingText, TEXT("게임오브젝트를 로딩중입니다."));
	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));
	return S_OK;
}

CLoader* CLoader::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eNextLevelID)
{
	CLoader* pInstance = new CLoader(pGraphic_Device);

	if (FAILED(pInstance->Initialize(eNextLevelID)))
	{
		MSG_BOX(TEXT("Failed to Created : CLoader"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoader::Free()
{  

	WaitForSingleObject(m_hThread, INFINITE);

	CloseHandle(m_hThread);

	DeleteCriticalSection(&m_CriticalSection);

	Safe_Release(m_pGameInstance);
	Safe_Release(m_pGraphic_Device);

	__super::Free();
}

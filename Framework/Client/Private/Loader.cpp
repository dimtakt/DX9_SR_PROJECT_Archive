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
	case LEVEL::LEVEL_MAPEDIT:
		hr = Loading_For_MapEdit_Level();
		break;
	case LEVEL::LEVEL_STAGE1:
		hr = Loading_For_Stage1_Level();
		break;

	case LEVEL::LEVEL_STAGE2:
		hr = Loading_For_Stage2_Level();
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

	for (size_t i = 0; i < 100000000; i++)
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
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Sephiria/InteractionObject/MapEdit/Interaction_%d.png"), 6))))
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

	return E_NOTIMPL;
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

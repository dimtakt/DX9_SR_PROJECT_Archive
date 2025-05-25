#include "Loader.h"

#include "GameInstance.h"

#include "BackGround.h"
#include "Terrain.h"
#include "Camera_Follow.h"
#include "Camera_Mouse.h"
#include "Player.h"
#include "Mp_Player.h"
#include "Hp_Player.h"
#include "Hud_States_Frame.h"
#include "Hud_Exp_Frame.h"
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
	case LEVEL::LEVEL_GAMEPLAY:
		hr = Loading_For_GamePlay_Level();
		break;
	}

	if (FAILED(hr))
		return E_FAIL;

	LeaveCriticalSection(&m_CriticalSection);

	return S_OK;
}

HRESULT CLoader::Loading_For_Logo_Level()
{
	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩중입니다."));
	/* Prototype_Component_Texture_BackGround */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_LOGO), TEXT("Prototype_Component_Texture_BackGround"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Textures/Default%d.jpg"), 2))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("모델를 로딩중입니다."));

	lstrcpy(m_szLoadingText, TEXT("쉐이더를 로딩중입니다."));

	lstrcpy(m_szLoadingText, TEXT("게임오브젝트를 로딩중입니다."));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_LOGO), TEXT("Prototype_GameObject_BackGround"),
		CBackGround::Create(m_pGraphic_Device))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_GamePlay_Level()
{
	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩중입니다."));
	/* Prototype_Component_Texture_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Textures/Terrain/Tile0.jpg"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_Texture_Player"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Textures/Player/Player0.png"), 1))))
		return E_FAIL;
	
	/* Prototype_Component_Hud_States */
	D3DXCOLOR vRectCol = { 0.816, 0.255, 0.267, 1.f };
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Rect_UI_Hud_States_Hp"), CVIBuffer_Rect::Create(m_pGraphic_Device, vRectCol))))
		return E_FAIL;
	vRectCol = { 0.525, 0.737, 0.965, 1.f };
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Rect_UI_Hud_States_Mp"), CVIBuffer_Rect::Create(m_pGraphic_Device, vRectCol))))
		return E_FAIL;
	vRectCol = { 0.f, 0.f, 0.f, 1.f };
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Rect_UI_Hud_States_Fream"), CVIBuffer_Rect::Create(m_pGraphic_Device, vRectCol))))
		return E_FAIL;

	/* Prototype_Component_Hud_Exp */
	vRectCol = { 0.200, 0.184, 0.231, 1.f };
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Rect_UI_Hud_Exp_Fream_1"), CVIBuffer_Rect::Create(m_pGraphic_Device, vRectCol))))
		return E_FAIL;
	vRectCol = { 0.431, 0.412, 0.463, 1.f };
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Rect_UI_Hud_Exp_Fream_2"), CVIBuffer_Rect::Create(m_pGraphic_Device, vRectCol))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("모델을 로딩중입니다."));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pGraphic_Device, 200, 200))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("쉐이더를 로딩중입니다."));

	lstrcpy(m_szLoadingText, TEXT("게임오브젝트를 로딩중입니다."));

	/* Prototype_GameObject_Terrain*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_GameObject_Terrain"),
		CTerrain::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_Camera*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_GameObject_Camera_Follow"),
		CCamera_Follow::Create(m_pGraphic_Device))))
		return E_FAIL;

	//플레이어 스테이터스 UI
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_GameObject_UI_Hud_States"),
		CHud_States_Frame::Create(m_pGraphic_Device, LEVEL::LEVEL_GAMEPLAY))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_GameObject_UI_Hud_Exp"),
		CHud_Exp_Frame::Create(m_pGraphic_Device, LEVEL::LEVEL_GAMEPLAY))))
		return E_FAIL;

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
	__super::Free();

	WaitForSingleObject(m_hThread, INFINITE);

	CloseHandle(m_hThread);

	DeleteCriticalSection(&m_CriticalSection);

	Safe_Release(m_pGameInstance);
	Safe_Release(m_pGraphic_Device);
}

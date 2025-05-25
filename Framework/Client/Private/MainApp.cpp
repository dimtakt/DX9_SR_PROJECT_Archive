#include "MainApp.h"
#include "GameInstance.h"

#include "Level_Loading.h"
#include "Player.h"

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

	if (FAILED(Start_Level(LEVEL::LEVEL_LOGO)))
		return E_FAIL;

	Ready_Key_Setting();

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

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"), CTransform::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_Component_Texture_Player */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Player"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Bin/Resources/Textures/Player/Player_Basic_Roll00.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Player"), CPlayer::Create(m_pGraphic_Device))))
		return E_FAIL;
	
	/* Prototype_Component_PlayerStats */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_PlayerStats"), CPlayerStats::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_Component_Animator  */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Animator"), CAnimator::Create(m_pGraphic_Device))))
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
}

HRESULT CMainApp::Start_Level(LEVEL eStartLevelID)
{
	if (FAILED(m_pGameInstance->Open_Level(ENUM_CLASS(LEVEL::LEVEL_LOADING), CLevel_Loading::Create(m_pGraphic_Device, eStartLevelID))))
		return E_FAIL;

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
	m_pGameInstance->Release_Engine();
	Safe_Release(m_pGameInstance);

#ifdef _DEBUG

	// 디버그용 콘솔창 제거
	FreeConsole();

#endif // _DEBUG
}

//#include "Level_GamePlay.h"
//
//
//#pragma endregion
//
//
//
//CLevel_GamePlay::CLevel_GamePlay(LPDIRECT3DDEVICE9 pGraphic_Device)
//	: CLevel{ pGraphic_Device }
//{
//}
//
//HRESULT CLevel_GamePlay::Initialize()
//{
//
//
//
//
//
//
//
//	return S_OK;
//}
//
//void CLevel_GamePlay::Update(_float fTimeDelta)
//{
//	if (m_pGameInstance->IsKeyDown(VK_RETURN))
//	{
//		if (FAILED(m_pGameInstance->Open_Level(ENUM_CLASS(LEVEL::LEVEL_LOADING), CLevel_Loading::Create(m_pGraphic_Device, LEVEL::LEVEL_STAGE1))))
//			return;
//
//		CRoom_Manager::GetInstance()->Clear(ENUM_CLASS(LEVEL::LEVEL_STATIC));
//	}
//	
//}
//
//HRESULT CLevel_GamePlay::Render()
//{
//	SetWindowText(g_hWnd, TEXT("게임플레이레벨입니다."));
//
//	return S_OK;
//}
//
//HRESULT CLevel_GamePlay::Ready_Light()
//{
//	D3DLIGHT9	LightDesc;
//	ZeroMemory(&LightDesc, sizeof(D3DLIGHT9));
//
//	LightDesc.Type = D3DLIGHT_DIRECTIONAL;
//	//LightDesc.Position = _float3(10.f, 3.f, 5.f);
//	//LightDesc.Direction = _float3(1.f, -1.f, 1.f);
//	//LightDesc.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
//	//LightDesc.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
//	LightDesc.Diffuse = { 1.f, 1.f, 1.f, 1.f };
//	LightDesc.Specular = { 1.f, 1.f, 1.f, 1.f };
//	LightDesc.Ambient = { 1.f, 1.f, 1.f, 1.f };
//
//	LightDesc.Direction = { 1.f, -1.f, 1.f };
//
//
//	if (FAILED(m_pGameInstance->Ready_Light(&LightDesc, 0)))
//		return E_FAIL;
//
//	return S_OK;
//}
//
//HRESULT CLevel_GamePlay::Ready_Animations()
//{
//
//	return S_OK;
//}
//
//HRESULT CLevel_GamePlay::Ready_Layer_Camera(const _wstring& strLayerTag)
//{
//	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STATIC), strLayerTag,
//		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Camera_Follow"))))
//		return E_FAIL;
//
//	return S_OK;
//}
//
//HRESULT CLevel_GamePlay::Ready_Layer_BackGround(const _wstring& strLayerTag)
//{
//	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STATIC), strLayerTag,
//		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_TerrainBox"))))
//		return E_FAIL;
//
//	return S_OK;
//}
//
//HRESULT CLevel_GamePlay::Ready_Layer_Player(const _wstring& strLayerTag)
//{
//	
//
//	return S_OK;
//}
//
//HRESULT CLevel_GamePlay::Ready_Layer_Weapon(const _wstring& strLayerTag)
//{
//	
//
//	return S_OK;
//}
//
//
//
//
//CLevel_GamePlay* CLevel_GamePlay::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
//{
//	CLevel_GamePlay* pInstance = new CLevel_GamePlay(pGraphic_Device);
//
//	if (FAILED(pInstance->Initialize()))
//	{
//		MSG_BOX(TEXT("Failed to Created : CLevel_GamePlay"));
//		Safe_Release(pInstance);
//	}
//
//	return pInstance;
//}
//
//void CLevel_GamePlay::Free()
//{
//	__super::Free();
//}

#include "Level_Shelter.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "Room_Manager.h"

CLevel_Shelter::CLevel_Shelter(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel{ pGraphic_Device }
{
}

HRESULT CLevel_Shelter::Initialize()
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
	return E_NOTIMPL;
}

HRESULT CLevel_Shelter::Ready_Layer_Camera(const _wstring& strLayerTag)
{
	return E_NOTIMPL;
}

HRESULT CLevel_Shelter::Ready_Layer_BackGround(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_SHELTER), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STAGE3), TEXT("Prototype_GameObject_Sky"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Shelter::Ready_Layer_Player(const _wstring& strLayerTag)
{
	return E_NOTIMPL;
}

HRESULT CLevel_Shelter::Ready_Layer_UI(const _wstring& strLayerTag)
{
	return S_OK;
}

HRESULT CLevel_Shelter::Ready_Layer_Room(const _wstring& strLayerTag)
{
	return E_NOTIMPL;
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
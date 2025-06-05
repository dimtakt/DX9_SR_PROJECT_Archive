#include "Level_Shelter.h"
#include "GameInstance.h"
#include "Level_Loading.h"

CLevel_Shelter::CLevel_Shelter(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel{ pGraphic_Device }
{
}

HRESULT CLevel_Shelter::Initialize()
{
	g_hCursor = LoadCursorFromFile(L"Resources/Sephiria/UI/Cursor/Cursor_Combat.cur");

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
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

HRESULT CLevel_Shelter::Ready_Layer_BackGround(const _wstring& strLayerTag)
{

	return S_OK;
}

HRESULT CLevel_Shelter::Ready_Layer_UI(const _wstring& strLayerTag)
{
	return S_OK;
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
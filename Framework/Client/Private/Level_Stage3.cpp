#include "Level_Stage3.h"
#include "GameInstance.h"
#include "Level_Loading.h"

CLevel_Stage3::CLevel_Stage3(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel{ pGraphic_Device }
{
}

HRESULT CLevel_Stage3::Initialize()
{
	g_hCursor = LoadCursorFromFile(L"Resources/Sephiria/UI/Cursor/Cursor_Combat.cur");

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	return S_OK;
}

void CLevel_Stage3::Update(_float fTimeDelta)
{

	return;
}



HRESULT CLevel_Stage3::Render()
{
	SetWindowText(g_hWnd, TEXT("스테이지3 레벨입니다."));

	return S_OK;
}

HRESULT CLevel_Stage3::Ready_Layer_BackGround(const _wstring& strLayerTag)
{

	return S_OK;
}

HRESULT CLevel_Stage3::Ready_Layer_UI(const _wstring& strLayerTag)
{
	return S_OK;
}

CLevel_Stage3* CLevel_Stage3::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_Stage3* pInstance = new CLevel_Stage3(pGraphic_Device);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CLevel_Stage3"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Stage3::Free()
{
	__super::Free();
}
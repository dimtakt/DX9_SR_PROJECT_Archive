#include "Level_Stage4.h"
#include "GameInstance.h"
#include "Level_Loading.h"

CLevel_Stage4::CLevel_Stage4(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel{ pGraphic_Device }
{
}

HRESULT CLevel_Stage4::Initialize()
{
	g_hCursor = LoadCursorFromFile(L"Resources/Sephiria/UI/Cursor/Cursor_Combat.cur");

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	return S_OK;
}

void CLevel_Stage4::Update(_float fTimeDelta)
{

	return;
}



HRESULT CLevel_Stage4::Render()
{
	SetWindowText(g_hWnd, TEXT("스테이지4 레벨입니다."));

	return S_OK;
}

HRESULT CLevel_Stage4::Ready_Layer_BackGround(const _wstring& strLayerTag)
{

	return S_OK;
}

HRESULT CLevel_Stage4::Ready_Layer_UI(const _wstring& strLayerTag)
{
	return S_OK;
}

CLevel_Stage4* CLevel_Stage4::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_Stage4* pInstance = new CLevel_Stage4(pGraphic_Device);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CLevel_Stage4"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Stage4::Free()
{
	__super::Free();
}
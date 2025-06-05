#include "Level_Boss2.h"
#include "GameInstance.h"
#include "Level_Loading.h"

CLevel_Boss2::CLevel_Boss2(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel{ pGraphic_Device }
{
}

HRESULT CLevel_Boss2::Initialize()
{
	g_hCursor = LoadCursorFromFile(L"Resources/Sephiria/UI/Cursor/Cursor_Combat.cur");

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	return S_OK;
}

void CLevel_Boss2::Update(_float fTimeDelta)
{

	return;
}



HRESULT CLevel_Boss2::Render()
{
	SetWindowText(g_hWnd, TEXT("보스2 레벨입니다."));

	return S_OK;
}

HRESULT CLevel_Boss2::Ready_Layer_BackGround(const _wstring& strLayerTag)
{

	return S_OK;
}

HRESULT CLevel_Boss2::Ready_Layer_UI(const _wstring& strLayerTag)
{
	return S_OK;
}

CLevel_Boss2* CLevel_Boss2::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_Boss2* pInstance = new CLevel_Boss2(pGraphic_Device);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CLevel_Boss2"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Boss2::Free()
{
	__super::Free();
}
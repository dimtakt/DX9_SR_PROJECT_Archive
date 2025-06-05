#include "Level_Town.h"
#include "GameInstance.h"
#include "Level_Loading.h"

CLevel_Town::CLevel_Town(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel{ pGraphic_Device }
{
}

HRESULT CLevel_Town::Initialize()
{
	g_hCursor = LoadCursorFromFile(L"Resources/Sephiria/UI/Cursor/Cursor_Combat.cur");

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	return S_OK;
}

void CLevel_Town::Update(_float fTimeDelta)
{

	return ;
}



HRESULT CLevel_Town::Render()
{
	SetWindowText(g_hWnd, TEXT("타운 레벨입니다."));

	return S_OK;
}

HRESULT CLevel_Town::Ready_Layer_BackGround(const _wstring& strLayerTag)
{

	return S_OK;
}

HRESULT CLevel_Town::Ready_Layer_UI(const _wstring& strLayerTag)
{
	return S_OK;
}

CLevel_Town* CLevel_Town::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_Town* pInstance = new CLevel_Town(pGraphic_Device);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CLevel_Town"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Town::Free()
{
	__super::Free();
}
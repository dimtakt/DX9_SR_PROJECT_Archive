#include "Level_GamePlay.h"

#include "GameInstance.h"
#include "Camera_Free.h"
#include "Player.h"
#include "Hud_Buff.h"
#include "Level_Loading.h"
#include "Field_Hp.h"
#include "Animation.h"

// Animations?
#include "../Public/Animations/Anim_Player_Idle.h"
#include "../Public/Animations/Anim_Player_Attack.h"


CLevel_GamePlay::CLevel_GamePlay(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel{ pGraphic_Device }
{
}

HRESULT CLevel_GamePlay::Initialize()
{
	g_hCursor = LoadCursorFromFile(L"Resources/Sephiria/UI/Cursor/Cursor_Combat.cur");

	if (FAILED(Ready_Light()))
		return E_FAIL;

	if (FAILED(Ready_Animations()))
		return E_FAIL;


	//if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
	//	return E_FAIL;

	if (FAILED(Ready_Layer_Weapon(TEXT("Layer_Weapon"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI(TEXT("Layer_UI"))))
		return E_FAIL;


	return S_OK;
}

void CLevel_GamePlay::Update(_float fTimeDelta)
{
	if (m_pGameInstance->IsKeyDown(VK_RETURN))
	{
		if (FAILED(m_pGameInstance->Open_Level(ENUM_CLASS(LEVEL::LEVEL_LOADING), CLevel_Loading::Create(m_pGraphic_Device, LEVEL::LEVEL_STAGE1))))
			return;
	}
}

HRESULT CLevel_GamePlay::Render()
{
	SetWindowText(g_hWnd, TEXT("게임플레이레벨입니다."));

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Light()
{
	D3DLIGHT9	LightDesc;
	ZeroMemory(&LightDesc, sizeof(D3DLIGHT9));

	LightDesc.Type = D3DLIGHT_DIRECTIONAL;
	//LightDesc.Position = _float3(10.f, 3.f, 5.f);
	//LightDesc.Direction = _float3(1.f, -1.f, 1.f);
	//LightDesc.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//LightDesc.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightDesc.Diffuse = { 1.f, 1.f, 1.f, 1.f };
	LightDesc.Specular = { 1.f, 1.f, 1.f, 1.f };
	LightDesc.Ambient = { 1.f, 1.f, 1.f, 1.f };

	LightDesc.Direction = { 1.f, -1.f, 1.f };


	if (FAILED(m_pGameInstance->Ready_Light(&LightDesc, 0)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Animations()
{
	if (FAILED(m_pGameInstance->Insert_Animation(L"Player_Idle", CAnim_Player_Idle::Create())))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Insert_Animation(L"Player_Attack", CAnim_Player_Attack::Create())))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Camera(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_GameObject_Camera_Follow"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_BackGround(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_GameObject_TerrainBox"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Player(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STATIC), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Player"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Weapon(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STATIC), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Weapon_Dagger"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_UI(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_GameObject_UI_Hud_States"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_GameObject_UI_Hud_Exp"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_GameObject_UI_Hud_Wallet"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_GameObject_UI_Hud_Button"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_GameObject_UI_Hud_Dash"))))
		return E_FAIL;
	CHud_Buff::UIHUD_BUFF_DESC Desc{};
	Desc.fX = 190.f;
	Desc.fY = 100.f;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_GameObject_UI_Hud_Buff"), &Desc)))
		return E_FAIL;

	Desc.fX = 227.f;
	Desc.fY = 100.f;
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_GameObject_UI_Hud_Buff"), &Desc)))
		return E_FAIL;

	Desc.fX = 264.f;
	Desc.fY = 100.f;
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_GameObject_UI_Hud_Buff"), &Desc)))
		return E_FAIL;

	CField_Hp::UI_FIELD_HP_DESC Field_HP_Desc{};
	Field_HP_Desc.iTarget_Index = 0;
	Field_HP_Desc.fY = 30.f;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_GameObject_UI_Field_Hp"), &Field_HP_Desc)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_GameObject_UI_Inventory"))))
		return E_FAIL;

	return S_OK;
}

CLevel_GamePlay* CLevel_GamePlay::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_GamePlay* pInstance = new CLevel_GamePlay(pGraphic_Device);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CLevel_GamePlay"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_GamePlay::Free()
{
	__super::Free();
}

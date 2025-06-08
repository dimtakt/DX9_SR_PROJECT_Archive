#include "Minimap.h"
#include "GameInstance.h"
#include "Minimap_Frame.h"
#include "Minimap_Button.h"
#include "Minimap_Node.h"
#include "Room_Manager.h"
#include "Stat_Manager.h"
CMinimap::CMinimap(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CMinimap::CMinimap(const CMinimap& Prototype) : CUIObject(Prototype), m_eLevel(Prototype.m_eLevel)
{
}

HRESULT CMinimap::Initialize_Prototype(LEVEL eLevel)
{
	m_eLevel = eLevel;

	if (FAILED(Ready_ChildPrototype(eLevel)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMinimap::Initialize(void* pArg)
{
	UIOBJECT_DESC* Desc = static_cast<UIOBJECT_DESC*>(pArg);

	m_bisOpen = false;
	m_fSizeX = g_iWinSizeX;
	m_fSizeY = g_iWinSizeY;
	m_fX = m_fSizeX * 0.5f;
	m_fY = m_fSizeY * 0.5f;
	m_fZ = UI_DEPTH::MiniMap;
	m_iWinSizeX = g_iWinSizeX;
	m_iWinSizeY = g_iWinSizeY;

	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
	__super::Update_Position();

	if (FAILED(Ready_Children()))
		return E_FAIL;

	m_pGameInstance->Add_UIObject(Desc->m_iLevel, TEXT("UI_Minimap"), this);

	return S_OK;
}

void CMinimap::Priority_Update(_float fTimeDelta)
{

	__super::Priority_Update(fTimeDelta);
	if (m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_LOADING) || m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_LOGO) || m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_MAPEDIT))
		return;
	if (!m_bIsUpdate)
		return;
	if (!m_bisOpen)
		return;
}

void CMinimap::Update(_float fTimeDelta)
{
	if (m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_LOADING) || m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_LOGO) || m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_MAPEDIT))
		return;

	if (!m_bIsUpdate)
		return;

	if (CStat_Manager::GetInstance()->Get_Battle())
	{
		m_bisOpen = false;
		return;
	}

	if (m_pGameInstance->IsKeyDown(VK_TAB))
		UI_Switch();

	if (!m_bisOpen)
		return;

	vector<pair<_int, _int>> pTemp = CRoom_Manager::GetInstance()->Get_RoomIndex();
	CRoom* pRoom = CRoom_Manager::GetInstance()->Get_CurrentRoom();
	__super::Update(fTimeDelta);

}

void CMinimap::Late_Update(_float fTimeDelta)
{
	if (m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_LOADING) || m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_LOGO) || m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_MAPEDIT))
		return;

	if (!m_bIsUpdate)
		return;

	if (!m_bisOpen)
		return;

	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
	__super::Late_Update(fTimeDelta);

}

HRESULT CMinimap::Render()
{
	m_pGraphic_Device->SetTexture(0, NULL);
	m_pVIBufferCom->Bind_Buffers();
	__super::Begin();
	m_pVIBufferCom->Render();
	__super::End();

	Font_Render();
	return S_OK;
}

void CMinimap::UI_Switch()
{
	if (m_bisOpen)
	{
		m_pGameInstance->All_Update_On();
		m_bisOpen = false;
	}
	else
	{
		m_pGameInstance->All_Update_Off();
		m_bIsUpdate = true;
		m_bisOpen = true;
	}
}

HRESULT CMinimap::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Rect_UI_BalckRect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMinimap::Ready_ChildPrototype(LEVEL eLevel)
{
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_Minimap_Frame"),
		CMinimap_Frame::Create(m_pGraphic_Device, eLevel))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_Minimap_Button"),
		CMinimap_Button::Create(m_pGraphic_Device, eLevel))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMinimap::Ready_Children()
{
	CUIObject* pGameObject = nullptr;

	CMinimap_Button::MINIMAP_BUTTON_DESC Desc{};

	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Minimap_Frame")));
	if (nullptr == pGameObject)
		return E_FAIL;
	Add_Child(pGameObject);
	
	vector<pair<_int, _int>> pTemp = CRoom_Manager::GetInstance()->Get_RoomIndex();

	for (_int i = 0; i < pTemp.size(); ++i)
	{
		Desc.fX = pTemp[i].first;
		Desc.fY = -pTemp[i].second;
		Desc.vRoomPos = static_cast<CTransform*>(CRoom_Manager::GetInstance()->Find_Room(pTemp[i].first, pTemp[i].second)->Get_TerrainBox()->Find_Component(TEXT("Com_Transform_TerrainBox")))->Get_State(STATE::POSITION);
		Desc.RoomID = CRoom_Manager::GetInstance()->Find_Room(pTemp[i].first, pTemp[i].second)->GetID();

		if (i == 0)
			Desc.fZ = 99;
		else
			Desc.fZ = ENUM_CLASS(CRoom_Manager::GetInstance()->Find_Room(pTemp[i].first, pTemp[i].second)->Get_RoomType());

		pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Minimap_Button"), &Desc));
		if (nullptr == pGameObject)
			return E_FAIL;
		Add_Child(pGameObject);
	}

	return S_OK;
}

void CMinimap::Font_Render()
{
	TCHAR szText[64];
	m_vTexRect.left = 210+1;
	m_vTexRect.top = 130+1;
	m_vTexRect.right = m_fSizeX;
	m_vTexRect.bottom = m_fSizeY;

	_stprintf_s(szText, TEXT("MAP"));
	m_pGameInstance->Render_Font(TEXT("UI_Font_28_Minimap"), szText, m_vTexRect, D3DXCOLOR(0.f, 0.f, 0.f, 1.0f), DT_LEFT | DT_TOP);

	m_vTexRect.left = 210;
	m_vTexRect.top = 130;
	_stprintf_s(szText, TEXT("MAP"));
	m_pGameInstance->Render_Font(TEXT("UI_Font_28_Minimap"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.0f), DT_LEFT | DT_TOP);

	m_vTexRect.left = 0;
	m_vTexRect.top = 0;
	m_vTexRect.right = m_fSizeX + 1;
	m_vTexRect.bottom = m_fSizeY - 130 + 1;

	_stprintf_s(szText, TEXT("즉시 이동하려면 방을 클릭하세요."));
	m_pGameInstance->Render_Font(TEXT("UI_Font_24_Minimap"), szText, m_vTexRect, D3DXCOLOR(0.f, 0.f, 0.f, 1.0f), DT_CENTER | DT_BOTTOM);

	m_vTexRect.left = 0;
	m_vTexRect.top = 0;
	m_vTexRect.right = m_fSizeX;
	m_vTexRect.bottom = m_fSizeY - 130;

	_stprintf_s(szText, TEXT("즉시 이동하려면 방을 클릭하세요."));
	m_pGameInstance->Render_Font(TEXT("UI_Font_24_Minimap"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.0f), DT_CENTER | DT_BOTTOM);

}

CMinimap* CMinimap::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
	CMinimap* pInstance = new CMinimap(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eLevel)))
	{
		MSG_BOX(TEXT("Failed to Created : CMinimap"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CMinimap::Clone(void* pArg)
{
	CMinimap* pInstance = new CMinimap(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CMinimap"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMinimap::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
}

#include "Minimap_Button.h"
#include "GameInstance.h"
#include "Minimap_Icon.h"
#include "Minimap_Player.h"
#include "Room_Manager.h"
#include "Minimap.h"
#include "Minimap_Node.h"
#include "Player.h"
CMinimap_Button::CMinimap_Button(LPDIRECT3DDEVICE9 pGraphic_Device) : CButton(pGraphic_Device)
{
}

CMinimap_Button::CMinimap_Button(const CMinimap_Button& Prototype) : CButton(Prototype), m_eLevel(Prototype.m_eLevel)
{
}

HRESULT CMinimap_Button::Initialize_Prototype(LEVEL eLevel)
{
	m_eLevel = eLevel;

	if (FAILED(Ready_ChildPrototype(eLevel)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMinimap_Button::Initialize(void* pArg)
{

	MINIMAP_BUTTON_DESC* Desc = static_cast<MINIMAP_BUTTON_DESC*>(pArg);
	
	m_vRoomPos = Desc->vRoomPos;
	m_iRoomID = Desc->RoomID;

	m_fSpeed = m_pGameInstance->Rand(3, 6);

	m_iEventType = Desc->fZ;

	m_fSizeX = 64;
	m_fSizeY = 64;
	m_fX = Desc->fX * 64;
	m_fY = Desc->fY * 64;
	m_fZ = UI_DEPTH::MINIMAP_BUTTON;
	m_iWinSizeX = g_iWinSizeX;
	m_iWinSizeY = g_iWinSizeY;

	
	if (FAILED(CUIObject::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
	__super::Update_Position();

	if (FAILED(Ready_Children(Desc->fX, Desc->fY)))
		return E_FAIL;

	return S_OK;
}

void CMinimap_Button::Priority_Update(_float fTimeDelta)
{
	CUIObject::Priority_Update(fTimeDelta);
}

void CMinimap_Button::Update(_float fTimeDelta)
{
	m_fFrame += fTimeDelta * m_fSpeed;
	
	if (m_fFrame > 1)
	{
		m_fFrame = 0;
		m_iTexIndex = m_pGameInstance->Rand(0, 4);
	}

	if (m_pGameInstance->IsKeyDown(VK_F1))
	{
		m_bClearRoom = true;
	}
	if (CRoom_Manager::GetInstance()->Get_CurrentRoom() == CRoom_Manager::GetInstance()->Get_RoomByID(m_iRoomID))
	{
		m_bIsPlayer = true;
		m_bClearRoom = true;
	}
	else
	{
		m_bIsPlayer = false;
	}
	if (!m_bClearRoom)
		return;
	if (Check_Key_Down(g_hWnd, VK_LBUTTON))
	{
		static_cast<CMinimap*>(m_pParent)->UI_Switch();
		CRoom_Manager::GetInstance()->Enter_Room(m_iRoomID);
		static_cast<CPlayer*>(m_pGameInstance->Get_GameObject(m_pGameInstance->Get_CurrentLevel(), TEXT("Layer_Player")))->Change_TerrainBox(CRoom_Manager::GetInstance()->Get_RoomByID(m_iRoomID)->Get_TerrainBox(), m_iRoomID);
		static_cast<CTransform*>(m_pGameInstance->Get_Component(m_pGameInstance->Get_CurrentLevel(), TEXT("Layer_Player"), TEXT("Com_Transform")))->Set_State(STATE::POSITION, m_vRoomPos);
	}

	CUIObject::Update(fTimeDelta);
}

void CMinimap_Button::Late_Update(_float fTimeDelta)
{
	if (!m_bClearRoom)
		return;

	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);

	for (_int i = 0; i < m_vecChildren.size(); i++)
	{
		if (i == 1 && !m_bIsPlayer)
			continue;

		m_vecChildren[i]->Late_Update(fTimeDelta);
	}
}

HRESULT CMinimap_Button::Render()
{
	if (Button_Pick())
		__super::Render_Button(m_iTexIndex + 4);
	else
		__super::Render_Button(m_iTexIndex);
	return S_OK;
}

HRESULT CMinimap_Button::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Rect_Map_Tile"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMinimap_Button::Ready_ChildPrototype(LEVEL eLevel)
{
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_Minimap_Icon"),
		CMinimap_Icon::Create(m_pGraphic_Device, eLevel))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_Minimap_Player"),
		CMinimap_Player::Create(m_pGraphic_Device, eLevel))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_Minimap_Node"),
		CMinimap_Node::Create(m_pGraphic_Device, eLevel))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMinimap_Button::Ready_Children(_float fX, _float fY)
{
	CUIObject* pGameObject = nullptr;

	UIOBJECT_DESC Desc{};
	
	Desc.fZ = m_iEventType;

	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Minimap_Icon"),&Desc));
	if (nullptr == pGameObject)
		return E_FAIL;
	Add_Child(pGameObject);

	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Minimap_Player")));
	if (nullptr == pGameObject)
		return E_FAIL;
	Add_Child(pGameObject);

	vector<pair<_int, _int>> pTemp = CRoom_Manager::GetInstance()->Get_RoomIndex();


	int fx1 = fX;
	int fy1 = fY;

	for (_int j = 0; j < pTemp.size(); ++j)
	{
		int fx2 = pTemp[j].first;
		int fy2 = pTemp[j].second;

		if (fx1 == fx2 && fy1 == fy2)	//자기자신 제외
			continue;

		if (abs(fx1 - fx2) >= 2 || abs(fy1 - fy2) >= 2)		//2칸 이상 제외
			continue;

		if (abs(fx1 - fx2) >= 1 && abs(fy1 - fy2) >= 1)		//대각선 제외
			continue;


		if (fx1 > fx2)		//왼쪽방
			Desc.fZ = 0;
		else if (fx1 < fx2)	//오른쪽방
			Desc.fZ = 1;
		else if (fy1 > fy2)	//윗방
			Desc.fZ = 2;
		else				//아래방
			Desc.fZ = 3;

		Desc.fX = fx1;
		Desc.fY = fy1;

		pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Minimap_Node"), &Desc));
		if (nullptr == pGameObject)
			return E_FAIL;
		Add_Child(pGameObject);
	}

	return S_OK;
}

_bool CMinimap_Button::Button_Pick()
{
	POINT			ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	RECT			rcUI = { m_vWorldPos.x - m_fSizeX * 0.5f, m_vWorldPos.y - m_fSizeY * 0.5f, m_vWorldPos.x + m_fSizeX * 0.5f, +m_vWorldPos.y + m_fSizeY * 0.5f };

	return PtInRect(&rcUI, ptMouse);
}

CMinimap_Button* CMinimap_Button::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
	CMinimap_Button* pInstance = new CMinimap_Button(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eLevel)))
	{
		MSG_BOX(TEXT("Failed to Created : CMinimap_Button"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CMinimap_Button::Clone(void* pArg)
{
	CMinimap_Button* pInstance = new CMinimap_Button(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CMinimap_Button"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMinimap_Button::Free()
{
	__super::Free();
}

#include "Minimap_Button.h"
#include "GameInstance.h"
#include "Minimap_Icon.h"
#include "Minimap_Player.h"
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

	UIOBJECT_DESC* Desc = static_cast<UIOBJECT_DESC*>(pArg);
	m_fSpeed = m_pGameInstance->Rand(3, 6);

	m_fSizeX = 50;
	m_fSizeY = 50;
	m_fX = -200 + Desc->fX * 50;
	m_fY = -200 + Desc->fY * 50;
	m_fZ = UI_DEPTH::MiniMap;
	m_iWinSizeX = g_iWinSizeX;
	m_iWinSizeY = g_iWinSizeY;

	
	if (FAILED(CUIObject::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
	__super::Update_Position();

	if (FAILED(Ready_Children()))
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

	CUIObject::Update(fTimeDelta);
}

void CMinimap_Button::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);

	m_vecChildren[0]->Late_Update(fTimeDelta);

	if (m_bIsPlayer)
		m_vecChildren[1]->Late_Update(fTimeDelta);
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

	return S_OK;
}

HRESULT CMinimap_Button::Ready_Children()
{
	CUIObject* pGameObject = nullptr;

	UIOBJECT_DESC Desc{};
	
	Desc.fZ = m_pGameInstance->Rand(0, 7);
	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Minimap_Icon"),&Desc));
	if (nullptr == pGameObject)
		return E_FAIL;
	Add_Child(pGameObject);

	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Minimap_Player"), &Desc));
	if (nullptr == pGameObject)
		return E_FAIL;
	Add_Child(pGameObject);

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

#include "ChapMap.h"
#include "GameInstance.h"
#include "ChapMap_Frame.h"
#include "ChapMap_Button.h"
#include "ChapMap_PlayerSymbol.h"
CChapMap::CChapMap(LPDIRECT3DDEVICE9 pGraphic_Device) : CButton(pGraphic_Device)
{
}

CChapMap::CChapMap(const CChapMap& Prototype) : CButton(Prototype), m_eLevel{ Prototype.m_eLevel }
{
}

HRESULT CChapMap::Initialize_Prototype(LEVEL eLevel)
{
	m_eLevel = eLevel;

	if (FAILED(Ready_ChildPrototype(eLevel)))
		return E_FAIL;

	return S_OK;
}

HRESULT CChapMap::Initialize(void* pArg)
{
	m_fSizeX = 0;
	m_fSizeY = 0;
	m_fX = g_iWinSizeX * 0.5f;
	m_fY = 0;
	m_fZ = UI_DEPTH::CHATERMAP;
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

	return S_OK;
}

void CChapMap::Priority_Update(_float fTimeDelta)
{
	if(m_pGameInstance->IsKeyDown('F'))
		if(m_bRender)
			m_bRender = false;
		else
		{
			m_bRender = true;
			/*m_fY = m_pPlayerSymbol->Player_OffsetPos();
			__super::Update_Position();*/
		}
	__super::Priority_Update(fTimeDelta);
}

void CChapMap::Update(_float fTimeDelta)
{
	if (!m_bRender)
		return;

	Scroll_Map();

	__super::Update(fTimeDelta);
}

void CChapMap::Late_Update(_float fTimeDelta)
{
	if (!m_bRender)
		return;

	__super::Late_Update(fTimeDelta);
}

HRESULT CChapMap::Render()
{
	return S_OK;
}

void CChapMap::Player_Offset(_float fX, _float fY, _uint iLineIndex)
{
	m_pPlayerSymbol->Player_Move(fX, fY);
	m_iPlayerLineIndex = iLineIndex;
	//m_bRender = false;
}

void CChapMap::Scroll_Map()
{
	POINT	ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	if (m_pGameInstance->IsKeyDown(VK_LBUTTON))
	{
		m_iMouseY = ptMouse.y;
	}
	else if (m_pGameInstance->IsKeyUp(VK_LBUTTON))
	{
		m_iMouseY = 0;
	}

	if (m_pGameInstance->IsKeyHold(VK_LBUTTON))
	{
		if (m_fY >= _float(g_iWinSizeX) * -3 * 0.25 && m_fY <= 160 + g_iWinSizeX * 0.28)
		{
			m_fY += ptMouse.y - m_iMouseY;
			m_iMouseY = ptMouse.y;
		}
		
	}

	if (m_fY < _float(g_iWinSizeX) * -3 * 0.25)
		m_fY = _int(_float(g_iWinSizeX) * -3 * 0.25);
	else if (m_fY > 160 + g_iWinSizeX * 0.28)
		m_fY = _int(160 + g_iWinSizeX * 0.28);

	if (g_ScrollValue > 0 || g_ScrollValue < 0)
		m_iScrollValue = g_ScrollValue;

	if (m_iScrollValue > 0 && m_fY <= 160 + g_iWinSizeX * 0.28)
	{
		m_fY += m_iScrollValue * 0.25 + 15;
		m_iScrollValue -= 10;
	}
	else if (m_iScrollValue < 0 && m_fY >= _float(g_iWinSizeX) * -3 * 0.25)
	{
		m_fY += m_iScrollValue * 0.25 - 15;
		m_iScrollValue += 10;
	}
	if (m_iScrollValue >= -30 && m_iScrollValue <= 30)
		m_iScrollValue = 0;

	__super::Update_Position();
}

HRESULT CChapMap::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;
	return S_OK;
}

HRESULT CChapMap::Ready_ChildPrototype(LEVEL eLevel)
{
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_ChapMap_Frame"),
		CChapMap_Frame::Create(m_pGraphic_Device, eLevel))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_ChapMap_Button"),
		CChapMap_Button::Create(m_pGraphic_Device, eLevel))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_ChapMap_Player"),
		CChapMap_PlayerSymbol::Create(m_pGraphic_Device))))
		return E_FAIL;
	return S_OK;
}

HRESULT CChapMap::Ready_Children()
{
	CUIObject* pGameObject = nullptr;

	CUIObject::UIOBJECT_DESC Desc{};

	for (int i = -1; i < 4; ++i)
	{
		Desc.fY = i;
		pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_ChapMap_Frame"),&Desc));
		if (nullptr == pGameObject)
			return E_FAIL;
		Add_Child(pGameObject);
	}

	for (_int i = 0; i < g_MapDB.size(); ++i)
	{		
		Desc.fX = i;

		pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_ChapMap_Button"), &Desc));
		if (nullptr == pGameObject)
			return E_FAIL;
		Add_Child(pGameObject);
	}


	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_ChapMap_Player")));
	if (nullptr == pGameObject)
		return E_FAIL;
	Add_Child(pGameObject);
	m_pPlayerSymbol = dynamic_cast<CChapMap_PlayerSymbol*>(pGameObject);
	Safe_AddRef(m_pPlayerSymbol);

	m_pPlayerSymbol->Player_Move(0, -100);
	m_iPlayerLineIndex = 0;
	return S_OK;
}

CChapMap* CChapMap::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
	CChapMap* pInstance = new CChapMap(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eLevel)))
	{
		MSG_BOX(TEXT("Failed to Create : CChapMap"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CChapMap::Clone(void* pArg)
{
	CChapMap* pInstance = new CChapMap(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CChapMap"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CChapMap::Free()
{
	__super::Free();
	Safe_Release(m_pPlayerSymbol);
}

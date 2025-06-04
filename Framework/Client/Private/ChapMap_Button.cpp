#include "ChapMap_Button.h"
#include "GameInstance.h"
#include "ChapMap_MapSymbol.h"
#include "Chapmap_Boss.h"
#include "ChapMap.h"
#include "ChapMap_Line.h"
#include "Level_Loading.h"
#include "Room_Manager.h"
CChapMap_Button::CChapMap_Button(LPDIRECT3DDEVICE9 pGraphic_Device) : CButton{ pGraphic_Device }
{
}

CChapMap_Button::CChapMap_Button(const CChapMap_Button& Prototype) : CButton(Prototype), m_eLevel(Prototype.m_eLevel)
{
}

HRESULT CChapMap_Button::Initialize_Prototype(LEVEL eLevel)
{
	m_eLevel = eLevel;

	if (FAILED(Ready_ChildPrototype(eLevel)))
		return E_FAIL;

	return S_OK;
}

HRESULT CChapMap_Button::Initialize(void* pArg)
{
	UIOBJECT_DESC* Desc = static_cast<UIOBJECT_DESC*>(pArg);

	m_fSizeX = 230;
	m_fSizeY = 230;
	
	m_iMapID = g_MapDB[Desc->fX].m_iMapID;
	m_iMapTex = g_MapDB[Desc->fX].m_iImage;

	if (g_MapDB[Desc->fX].m_iLine == 0 || g_MapDB[Desc->fX].m_iLine == 4)
		m_fX = 0;
	else if (g_MapDB[Desc->fX].m_iMapID % 2 == 1)
		m_fX = -m_fSizeX;
	else
		m_fX = m_fSizeX;

	m_fY = -100 + g_MapDB[Desc->fX].m_iLine * (m_fSizeY * 1.8);

	m_fZ = UI_DEPTH::CHATERMAP_BUTTON;
	m_iWinSizeX = g_iWinSizeX;
	m_iWinSizeY = g_iWinSizeY;

	if (FAILED(CUIObject::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Children()))
		return E_FAIL;

	if (FAILED(Ready_Line()))
		return E_FAIL;
	m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
	__super::Update_Position();


	return S_OK;
}

void CChapMap_Button::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);

}

void CChapMap_Button::Update(_float fTimeDelta)
{
	Line_Render_Setting();
		
	if(m_iPlayerLine + 1 == g_MapDB[m_iMapID].m_iLine)
		Click_Event();

	__super::Update(fTimeDelta);
}

void CChapMap_Button::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
	for (_int i = 0; i < m_vecLine.size(); ++i)
		m_vecLine[i]->Late_Update(fTimeDelta);

	__super::Late_Update(fTimeDelta);
}

HRESULT CChapMap_Button::Render()
{
	SetUp_RenderState();
	
	if (m_iPlayerLine + 1 == g_MapDB[m_iMapID].m_iLine)
	{
		if (FAILED(CButton::Bind_ButtonTex_Double(g_hWnd, 0, 1)))
			return E_FAIL;
	}
	else if (m_iPlayerLine  == g_MapDB[m_iMapID].m_iLine)
	{
		if (FAILED(CButton::Bind_ButtonTex_Single(g_hWnd, 0)))
			return E_FAIL;
	}
	else
	{
		if (FAILED(CButton::Bind_ButtonTex_Single(g_hWnd, 2)))
			return E_FAIL;
	}
	Reset_RenderState();
	return S_OK;
}

void CChapMap_Button::Click_Event()
{
	if (Check_Key_Down(g_hWnd, VK_LBUTTON))
	{
		static_cast<CChapMap*>(m_pParent)->Player_Offset(m_fX, m_fY, g_MapDB[m_iMapID].m_iLine, m_iMapID);
		
		CRoom_Manager::GetInstance()->Clear(m_pGameInstance->Get_CurrentLevel());

		//레벨 오픈 터짐
		if (FAILED(m_pGameInstance->Open_Level(ENUM_CLASS(LEVEL::LEVEL_LOADING), CLevel_Loading::Create(m_pGraphic_Device, g_MapDB[m_iMapID].m_eMapIndex))))
			return;

	}
} 

void CChapMap_Button::Line_Render_Setting()
{
	//이전 라인 끄기
	if (m_lineRender && m_vecLine.size() >= 8 && g_MapDB[m_iMapID].m_iLine < static_cast<CChapMap*>(m_pParent)->Get_Player_Line())
	{
		if (static_cast<CChapMap*>(m_pParent)->Get_Player_Map() % 2 == 1)
		{
			for (_int i = 4; i < 8; ++i)
			{
				m_vecLine[i]->isRend_Off();
				m_lineRender = false;
			}
			for (_int i = 0; i < 4; ++i)
			{
				m_vecLine[i]->Set_TexIndex(1);
			}
		}
		else
		{
			for (_int i = 0; i < 4; ++i)
			{
				m_vecLine[i]->isRend_Off();
				m_lineRender = false;
			}
			for (_int i = 4; i < 8; ++i)
			{
				m_vecLine[i]->Set_TexIndex(1);
			}
		}
	}

	//4번째 라인 전용
	if (static_cast<CChapMap*>(m_pParent)->Get_Player_Line() == 4)
	{
		for (_int i = 0; i < m_vecLine.size(); ++i)
		{
			m_vecLine[i]->Set_TexIndex(1);
		}
	}
	m_iPlayerLine = static_cast<CChapMap*>(m_pParent)->Get_Player_Line();

	if (static_cast<CChapMap*>(m_pParent)->Get_Player_Map() == m_iMapID)
	{
		for (_int i = 0; i < m_vecLine.size(); ++i)
		{
			m_vecLine[i]->Set_TexIndex(0);
		}
	}
	else if (m_iPlayerLine == g_MapDB[m_iMapID].m_iLine)
	{
		for (_int i = 0; i < m_vecLine.size(); ++i)
		{
			m_vecLine[i]->isRend_Off();
		}
	}

	if (g_MapDB[m_iMapID].m_iLine > m_iPlayerLine)
	{
		for (_int i = 0; i < m_vecLine.size(); ++i)
		{
			m_vecLine[i]->isRend_Off();
		}
	}

	if (m_iMapID == static_cast<CChapMap*>(m_pParent)->Get_Player_Map())
	{
		for (_int i = 0; i < m_vecLine.size(); ++i)
		{
			m_vecLine[i]->isRend_On();
		}

	}
}

HRESULT CChapMap_Button::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	_tchar szTemp[128] = {};
	wsprintf(szTemp, TEXT("Prototype_Component_Texture_Rect_ChapMap_Node_%d"), m_iMapTex);
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), szTemp,
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

void CChapMap_Button::SetUp_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
}

void CChapMap_Button::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	m_pGraphic_Device->SetTexture(0, NULL);
}

HRESULT CChapMap_Button::Ready_ChildPrototype(LEVEL eLevel)
{
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_ChapMap_Symbol"),
		CChapMap_MapSymbol::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_ChapMap_Boss"),
		CChapmap_Boss::Create(m_pGraphic_Device))))
		return E_FAIL;

	return S_OK;
}

HRESULT CChapMap_Button::Ready_Children()
{
	CUIObject* pGameObject = nullptr;

	CChapMap_MapSymbol::MAP_EVENT_DESC Desc{};

	Desc.iMax_Event_Count = g_MapDB[m_iMapID].m_vecEvent.size();

	for (_int i = 0; i < g_MapDB[m_iMapID].m_vecEvent.size(); ++i)
	{
		Desc.eMap_EVENT = g_MapDB[m_iMapID].m_vecEvent[i];
		Desc.iEvent_Count = i;
		pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_ChapMap_Symbol"), &Desc));
		if (nullptr == pGameObject)
			return E_FAIL;
		Add_Child(pGameObject);
	}

	CChapmap_Boss::MAP_BOSS_DESC Desc_Boss{};

	if (g_MapDB[m_iMapID].m_eBossType != MAP_BOSS::MAP_BOSS_END)
	{
		Desc_Boss.eMap_Boss = g_MapDB[m_iMapID].m_eBossType;

		pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_ChapMap_Boss"), &Desc_Boss));
		if (nullptr == pGameObject)
			return E_FAIL;
		Add_Child(pGameObject);
	}

	return S_OK;
}

HRESULT CChapMap_Button::Ready_Line()
{
	CUIObject* pGameObject = nullptr;
	CChapMap_Line::MAP_LINE_DESC Line_Desc{};

	if (g_MapDB[m_iMapID].m_iLine == 0)
	{
		for (_int i = 0; i <= 2; i += 2)
		{
			for (_int j = 0; j < 4; ++j)
			{
				Line_Desc.fAngle = -25 + (i * 25);
				Line_Desc.iIndex = g_MapDB[m_iMapID].m_iLine;
				Line_Desc.fX = i;
				Line_Desc.fY = j;
				Line_Desc.bisDefalut = false;
				pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_ChapMap_Line"), &Line_Desc));
				if (nullptr == pGameObject)
					return E_FAIL;
				Add_Child(pGameObject);
				m_vecLine.push_back(dynamic_cast<CChapMap_Line*>(pGameObject));
				Safe_AddRef(pGameObject);
			}
		}

		Line_Desc.fAngle = 0;
		Line_Desc.iIndex = g_MapDB[m_iMapID].m_iLine;
		Line_Desc.fX = 0;
		Line_Desc.fY = 0;
		Line_Desc.bisDefalut = true;
		pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_ChapMap_Line"), &Line_Desc));
		if (nullptr == pGameObject)
			return E_FAIL;
		Add_Child(pGameObject);
		m_vecLine.push_back(dynamic_cast<CChapMap_Line*>(pGameObject));
		Safe_AddRef(pGameObject);
	}
	else if (g_MapDB[m_iMapID].m_iLine > 0 && g_MapDB[m_iMapID].m_iLine < 3)
	{
		if (g_MapDB[m_iMapID].m_iMapID % 2 == 1)
		{
			for (_int i = 1; i <= 2; ++i)
			{
				for (_int j = 0; j < 4; ++j)
				{
					Line_Desc.fAngle = -45 + (i * 45);
					Line_Desc.iIndex = g_MapDB[m_iMapID].m_iLine;
					Line_Desc.fX = i;
					Line_Desc.fY = j;
					Line_Desc.bisDefalut = false;
					pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_ChapMap_Line"), &Line_Desc));
					if (nullptr == pGameObject)
						return E_FAIL;
					Add_Child(pGameObject);
					m_vecLine.push_back(dynamic_cast<CChapMap_Line*>(pGameObject));
					Safe_AddRef(pGameObject);
				}
			}
		}
		else
		{
			for (_int i = 0; i <= 1; ++i)
			{
				for (_int j = 0; j < 4; ++j)
				{
					Line_Desc.fAngle = -45 + (i * 45);
					Line_Desc.iIndex = g_MapDB[m_iMapID].m_iLine;
					Line_Desc.fX = i;
					Line_Desc.fY = j;
					Line_Desc.bisDefalut = false;
					pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_ChapMap_Line"), &Line_Desc));
					if (nullptr == pGameObject)
						return E_FAIL;
					Add_Child(pGameObject);
					m_vecLine.push_back(dynamic_cast<CChapMap_Line*>(pGameObject));
					Safe_AddRef(pGameObject);
				}
			}
		}
	}
	else if (g_MapDB[m_iMapID].m_iLine == 3)
	{
		if (g_MapDB[m_iMapID].m_iMapID % 2)
		{
			for (_int i = 2; i <= 2; ++i)
			{
				for (_int j = 0; j < 4; ++j)
				{
					Line_Desc.fAngle = -30 + (i * 30);
					Line_Desc.iIndex = g_MapDB[m_iMapID].m_iLine;
					Line_Desc.fX = i;
					Line_Desc.fY = j;
					Line_Desc.bisDefalut = false;
					pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_ChapMap_Line"), &Line_Desc));
					if (nullptr == pGameObject)
						return E_FAIL;
					Add_Child(pGameObject);
					m_vecLine.push_back(dynamic_cast<CChapMap_Line*>(pGameObject));
					Safe_AddRef(pGameObject);
				}
			}
		}
		else
		{
			for (_int i = 0; i <= 0; ++i)
			{
				for (_int j = 0; j < 4; ++j)
				{
					Line_Desc.fAngle = -25 + (i * 25);
					Line_Desc.iIndex = g_MapDB[m_iMapID].m_iLine;
					Line_Desc.fX = i;
					Line_Desc.fY = j;
					Line_Desc.bisDefalut = false;
					pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_ChapMap_Line"), &Line_Desc));
					if (nullptr == pGameObject)
						return E_FAIL;
					Add_Child(pGameObject);
					m_vecLine.push_back(dynamic_cast<CChapMap_Line*>(pGameObject));
					Safe_AddRef(pGameObject);
				}
			}
		}
	}
	return S_OK;
}

CChapMap_Button* CChapMap_Button::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
	CChapMap_Button* pInstance = new CChapMap_Button(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eLevel)))
	{
		MSG_BOX(TEXT("Failed to Created : CChapMap_Button"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CChapMap_Button::Clone(void* pArg)
{
	CChapMap_Button* pInstance = new CChapMap_Button(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CChapMap_Button"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CChapMap_Button::Free()
{
	for (_int i = 0; i < m_vecLine.size(); ++i)
		Safe_Release(m_vecLine[i]);
	m_vecLine.clear();

	__super::Free();
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}

#include "ChapMap_Button.h"
#include "GameInstance.h"
#include "ChapMap_MapSymbol.h"
#include "Chapmap_Boss.h"
#include "ChapMap.h"
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
	
	m_iMapIndex = g_MapDB[Desc->fX].m_iMapID;
	m_iMapTex = g_MapDB[Desc->fX].m_iImage;

	if (g_MapDB[Desc->fX].m_iLine == 0 || g_MapDB[Desc->fX].m_iLine == 4)
		m_fX = 0;
	else if (g_MapDB[Desc->fX].m_iMapIndex % 2 == 0)
		m_fX = -m_fSizeX;
	else
		m_fX = m_fSizeX;

	m_fY = -100 + g_MapDB[Desc->fX].m_iLine * (m_fSizeY * 1.8);

	m_fZ = UI_DEPTH::CHATERMAP;
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

void CChapMap_Button::Priority_Update(_float fTimeDelta)
{
	
	__super::Priority_Update(fTimeDelta);

}

void CChapMap_Button::Update(_float fTimeDelta)
{
	m_iPlayerLine = static_cast<CChapMap*>(m_pParent)->Get_Player_Line();
	
	if(m_iPlayerLine + 1 == g_MapDB[m_iMapIndex].m_iLine)
		Click_Event();

	__super::Update(fTimeDelta);
}

void CChapMap_Button::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
	__super::Late_Update(fTimeDelta);
}

HRESULT CChapMap_Button::Render()
{
	SetUp_RenderState();
	
	if (m_iPlayerLine + 1 == g_MapDB[m_iMapIndex].m_iLine)
	{
		if (FAILED(CButton::Bind_ButtonTex_Double(g_hWnd, 0, 1)))
			return E_FAIL;
	}
	else if (m_iPlayerLine  == g_MapDB[m_iMapIndex].m_iLine)
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
		static_cast<CChapMap*>(m_pParent)->Player_Offset(m_fX, m_fY, g_MapDB[m_iMapIndex].m_iLine);
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
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), szTemp,
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

	Desc.iMax_Event_Count = g_MapDB[m_iMapIndex].m_vecEvent.size();

	for (_int i = 0; i < g_MapDB[m_iMapIndex].m_vecEvent.size(); ++i)
	{
		Desc.eMap_EVENT = g_MapDB[m_iMapIndex].m_vecEvent[i];
		Desc.iEvent_Count = i;
		pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_ChapMap_Symbol"), &Desc));
		if (nullptr == pGameObject)
			return E_FAIL;
		Add_Child(pGameObject);
	}

	CChapmap_Boss::MAP_BOSS_DESC Desc_Boss{};

	if (g_MapDB[m_iMapIndex].m_eBossType != MAP_BOSS::MAP_BOSS_END)
	{
		Desc_Boss.eMap_Boss = g_MapDB[m_iMapIndex].m_eBossType;

		pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_ChapMap_Boss"), &Desc_Boss));
		if (nullptr == pGameObject)
			return E_FAIL;
		Add_Child(pGameObject);
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
	__super::Free();
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}

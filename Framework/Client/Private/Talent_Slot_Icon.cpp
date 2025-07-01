#include "Talent_Slot_Icon.h"
#include "GameInstance.h"
#include "Talent_Icon_Selete.h"
#include "Talent_Slot.h"
#include "Stat_Manager.h"
#include "Talent_Button_Tip.h"
CTalent_Slot_Icon::CTalent_Slot_Icon(LPDIRECT3DDEVICE9 pGraphic_Device) : CButton(pGraphic_Device)
{
}

CTalent_Slot_Icon::CTalent_Slot_Icon(const CTalent_Slot_Icon& Prototype) : CButton(Prototype), m_eLevel(Prototype.m_eLevel)
{
}

HRESULT CTalent_Slot_Icon::Initialize_Prototype(LEVEL eLevel)
{
	m_eLevel = eLevel;

	if (FAILED(Ready_ChildPrototype(eLevel)))
		return E_FAIL;

	return S_OK;
}

HRESULT CTalent_Slot_Icon::Initialize(void* pArg)
{
	UIOBJECT_DESC* Desc = static_cast<UIOBJECT_DESC*>(pArg);

	m_iTexIdex = Desc->fX;
	m_iIndex = Desc->fZ;
	m_fSizeX = 64;
	m_fSizeY = 64;

	if (Desc->fX == 0)
		m_fX = -48;
	else
		m_fX = 48;
	m_fY = 110;
	m_fZ = UI_DEPTH::TALENT;
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

void CTalent_Slot_Icon::Priority_Update(_float fTimeDelta)
{
	CUIObject::Priority_Update(fTimeDelta);
}

void CTalent_Slot_Icon::Update(_float fTimeDelta)
{
	Special_Gift_Update();
	CUIObject::Update(fTimeDelta);
}

void CTalent_Slot_Icon::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
	if(isPick(g_hWnd))
		CUIObject::Late_Update(fTimeDelta);
}

HRESULT CTalent_Slot_Icon::Render()
{
	_int iTemp = static_cast<CTalent_Slot*>(m_pParent)->Get_Value();
	if (m_iTexIdex == 0 && iTemp >= 10)
	{
		if (FAILED(CButton::Bind_ButtonTex_Single(g_hWnd, m_iTexIdex + 2)))
			return E_FAIL;
	}
	else if(m_iTexIdex == 1 && iTemp >= 20)
	{
		if (FAILED(CButton::Bind_ButtonTex_Single(g_hWnd, m_iTexIdex + 2)))
			return E_FAIL;
	}
	else
	{
		if (FAILED(CButton::Bind_ButtonTex_Single(g_hWnd, m_iTexIdex )))
			return E_FAIL;
	}
	return S_OK;
}

HRESULT CTalent_Slot_Icon::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	_tchar	szTextureFilePath[MAX_PATH];
	wsprintf(szTextureFilePath, TEXT("Prototype_Component_Texture_Rect_Talent_Slot_Icon_%d"), m_iIndex);

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), szTextureFilePath,
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;


	return S_OK;
}

HRESULT CTalent_Slot_Icon::Ready_ChildPrototype(LEVEL eLevel)
{
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_Talent_Icon_Selete"),
		CTalent_Icon_Selete::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_Talent_Button_Tip"),
		CTalent_Button_Tip::Create(m_pGraphic_Device))))
		return E_FAIL;

	return S_OK;
}

HRESULT CTalent_Slot_Icon::Ready_Children()
{
	CUIObject* pGameObject = nullptr;
	CUIObject::UIOBJECT_DESC Desc = {};


	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Talent_Icon_Selete")));
	if (nullptr == pGameObject)
		return E_FAIL;
	Add_Child(pGameObject);

	Desc.fZ = m_iIndex;
	Desc.fX = m_iTexIdex;

	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Talent_Button_Tip"), &Desc));
	if (nullptr == pGameObject)
		return E_FAIL;
	Add_Child(pGameObject);

	return S_OK;
}

void CTalent_Slot_Icon::Special_Gift_Update()
{
	_int iPoint = static_cast<CTalent_Slot*>(m_pParent)->Get_Value();

	if (m_iTexIdex == 0 && iPoint >= 10 && m_bIsGetGift == false)
		m_bIsGetGift = true;
	else if (m_iTexIdex == 0 && iPoint < 10 && m_bIsGetGift == true)
		m_bIsGetGift = false;

	if (m_iTexIdex == 1 && iPoint >= 20 && m_bIsGetGift == false)
		m_bIsGetGift = true;
	else if (m_iTexIdex == 1 && iPoint < 20 && m_bIsGetGift == true)
		m_bIsGetGift = false;


	if (m_bGiftCheck != m_bIsGetGift)
	{
		switch (m_iIndex)
		{
		case 0:
			if (m_iTexIdex == 0 && m_bIsGetGift == true)
				CStat_Manager::GetInstance()->Cal_Stats(STAT_INFO::CRITICALDAMAGE, 20);
			else if	(m_iTexIdex == 0 && m_bIsGetGift == false)
				CStat_Manager::GetInstance()->Cal_Stats(STAT_INFO::CRITICALDAMAGE, -20);

			if (m_iTexIdex == 1 && m_bIsGetGift == true)
				CStat_Manager::GetInstance()->Cal_Stats(STAT_INFO::CULCRITICAL, 20);
			else if (m_iTexIdex == 1 && m_bIsGetGift == false)
				CStat_Manager::GetInstance()->Cal_Stats(STAT_INFO::CULCRITICAL, -20);
			break;
		case 1:
			if (m_iTexIdex == 0 && m_bIsGetGift == true)
				CStat_Manager::GetInstance()->Cal_Stats(STAT_INFO::REGENDASH, 10.f);
			else if (m_iTexIdex == 0 && m_bIsGetGift == false)
				CStat_Manager::GetInstance()->Cal_Stats(STAT_INFO::REGENDASH, 10.f);

			if (m_iTexIdex == 1 && m_bIsGetGift == true)
				CStat_Manager::GetInstance()->Cal_Stats(STAT_INFO::MAXDASH, 1);
			else if (m_iTexIdex == 1 && m_bIsGetGift == false)
				CStat_Manager::GetInstance()->Cal_Stats(STAT_INFO::MAXDASH, -1);
			break;
		case 2:
			if (m_iTexIdex == 0 && m_bIsGetGift == true)
				CStat_Manager::GetInstance()->Cal_Stats(STAT_INFO::MAXHP, 10);
			else if (m_iTexIdex == 0 && m_bIsGetGift == false)
				CStat_Manager::GetInstance()->Cal_Stats(STAT_INFO::MAXHP, -10);

			if (m_iTexIdex == 1 && m_bIsGetGift == true)
				CStat_Manager::GetInstance()->Cal_Stats(STAT_INFO::CULDEF, 10);
			else if (m_iTexIdex == 1 && m_bIsGetGift == false)
				CStat_Manager::GetInstance()->Cal_Stats(STAT_INFO::CULDEF, -10);
			break;
		case 3:
			if (m_iTexIdex == 0 && m_bIsGetGift == true)
				CStat_Manager::GetInstance()->Cal_Stats(STAT_INFO::CULDAMAGE, 10);
			else if (m_iTexIdex == 0 && m_bIsGetGift == false)
				CStat_Manager::GetInstance()->Cal_Stats(STAT_INFO::CULDAMAGE, -10);

			if (m_iTexIdex == 1 && m_bIsGetGift == true)
			{
				CStat_Manager::GetInstance()->Cal_Stats(STAT_INFO::MAXMP, 20);
				CStat_Manager::GetInstance()->Cal_Stats(STAT_INFO::MAXHP, 10);
			}
			else if (m_iTexIdex == 1 && m_bIsGetGift == false)
			{
				CStat_Manager::GetInstance()->Cal_Stats(STAT_INFO::MAXMP, -20);
				CStat_Manager::GetInstance()->Cal_Stats(STAT_INFO::MAXHP, -10);
			}
			break;
		}
		m_bGiftCheck = m_bIsGetGift;
	}
}

CTalent_Slot_Icon* CTalent_Slot_Icon::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
	CTalent_Slot_Icon* pInstance = new CTalent_Slot_Icon(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eLevel)))
	{
		MSG_BOX(TEXT("Failed to Created : CTalent_Slot_Icon"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CTalent_Slot_Icon::Clone(void* pArg)
{
	CTalent_Slot_Icon* pInstance = new CTalent_Slot_Icon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CTalent_Slot_Icon"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTalent_Slot_Icon::Free()
{
	__super::Free();
}

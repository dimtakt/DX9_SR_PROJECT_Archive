#include "Ending_Mole.h"
#include "GameInstance.h"

CEnding_Mole::CEnding_Mole(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CEnding_Mole::CEnding_Mole(const CEnding_Mole& Prototype) : CUIObject(Prototype)
{
}

HRESULT CEnding_Mole::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEnding_Mole::Initialize(void* pArg)
{
	m_fSizeX = 512;
	m_fSizeY = 512;
	m_fX = 0;
	m_fY = 0;
	m_fZ = 0.1f;
	m_iWinSizeX = g_iWinSizeX;
	m_iWinSizeY = g_iWinSizeY;

	if (FAILED(CUIObject::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	
	m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
	__super::Update_Position();

	if (FAILED(Ready_Object()))
		return E_FAIL;

	return S_OK;
}

void CEnding_Mole::Priority_Update(_float fTimeDelta)
{
}

void CEnding_Mole::Update(_float fTimeDelta)
{
	if (m_pGameInstance->IsKeyDown('F'))
	{
		m_pChat->StartToEnd_Chat_Normal();
	}
	m_fAccTime += fTimeDelta;
	if (m_fAccTime >= 0.1)
	{
		m_fAccTime = 0;

	
		++m_iTexindex;
	

		if (m_iTexindex >= 9)
			m_iTexindex = 0;
	}
}

void CEnding_Mole::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CEnding_Mole::Render()
{
	if (FAILED(m_pTextureCom->Bind_Texture(m_iTexindex)))
		return E_FAIL;
	m_pVIBufferCom->Bind_Buffers();

	__super::Begin();
	m_pVIBufferCom->Render();

	//Render_Font();
	__super::End();

	return S_OK;
}

HRESULT CEnding_Mole::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Rect_Ending_Mole"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CEnding_Mole::Ready_Object()
{
	CField_Npc_Chat::FIELD_CHAT_DESC Desc {};

	Desc.pTransform = m_pTransformCom;
	Desc.szChatTag = TEXT("Ending_Chat");
	Desc.m_iLevel = ENUM_CLASS(LEVEL::LEVEL_BOSS2);
	Desc.fY = -100;
	
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_BOSS2), TEXT("Layer_UI_Chat"),
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_UI_Field_Npc_Chat"), &Desc)))
		return E_FAIL;

	m_pChat = static_cast<CField_Npc_Chat*>(m_pGameInstance->Find_UIObj(ENUM_CLASS(LEVEL::LEVEL_BOSS2), TEXT("Ending_Chat")));

	m_pChat->Add_Chat(TEXT("세피 사람들을 지키고"));
	m_pChat->Add_Chat(TEXT("행복하게 살았습니다."));
	m_pChat->Add_Chat(TEXT("게임을 봐주셔서 감사하며"));
	m_pChat->Add_Chat(TEXT("모두 고생하셨습니다!"));

	return S_OK;
}

CEnding_Mole* CEnding_Mole::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CEnding_Mole* pInstance = new CEnding_Mole(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CEnding_Mole"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CEnding_Mole::Clone(void* pArg)
{
	CEnding_Mole* pInstance = new CEnding_Mole(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CEnding_Mole"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEnding_Mole::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}

#include "Field_Npc_Chat.h"
#include "GameInstance.h"
#include "Stat_Manager.h"
#include "UI_KeyGuide.h"
#include "Field_Npc_Face.h"
#include "Stat_Manager.h"
CField_Npc_Chat::CField_Npc_Chat(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CField_Npc_Chat::CField_Npc_Chat(const CField_Npc_Chat& Prototype) : CUIObject(Prototype), m_eLevel(Prototype.m_eLevel)
{
}

void CField_Npc_Chat::Add_Chat(_wstring szChatting)
{
	_wstring szText = szChatting;
	m_vecChatting.push_back(szText);
	++m_iVecIndex;
}

void CField_Npc_Chat::On_Chat(_int iFaceNum, _bool bIsFace)
{
	m_bIsOn = true;
	if(bIsFace)
		static_cast<CField_Npc_Face*>(m_pGameInstance->Find_UIObj(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("UI_NpcFace")))->Render_SmallFace_On(iFaceNum);
}

void CField_Npc_Chat::Off_Chat()
{
	m_bIsOn = false;
	static_cast<CField_Npc_Face*>(m_pGameInstance->Find_UIObj(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("UI_NpcFace")))->Render_SmallFace_Off();
}

void CField_Npc_Chat::StartToEnd_Chat()
{
	m_bIsOn = false;
	if (!m_bIschat)
	{
		Start_Chat();
		CStat_Manager::GetInstance()->Set_UIOpen(true);
	}
	else if (!m_bIsFinish)
		return;
	else if (m_iChatIndex < m_iVecIndex-1)
		Next_Chat();
	else
	{
		End_Chat();
		CStat_Manager::GetInstance()->Set_UIOpen(false);
	}
}

void CField_Npc_Chat::Start_Chat()
{
	m_bIsOn = false;
	m_bIschat = true;
	m_szRenderText = {};
	m_iTextIndex = 0;
}

void CField_Npc_Chat::Next_Chat()
{
	m_bIsFinish = false;
	m_bIsRenderchat = false;
	m_szRenderText = {};
	m_iTextIndex = 0;
	++m_iChatIndex;
}

void CField_Npc_Chat::End_Chat()
{
	m_bIschat = false;

}

void CField_Npc_Chat::Cinematic_Chat(_int iFaceNum, _bool bIsFace)
{
	m_bIsOn = false;
	if (!m_bIschat)
	{

		m_pGameInstance->All_Update_Off();
		m_bIsUpdate = true;
		Start_Chat();
		CStat_Manager::GetInstance()->Set_UIOpen(true);
		
		static_cast<CField_Npc_Face*>(m_pGameInstance->Find_UIObj(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("UI_NpcFace")))->Render_Face_On(iFaceNum, bIsFace);
		
	}
	else if (!m_bIsFinish)
		return;
	else if (m_iChatIndex < m_iVecIndex - 1)
		Next_Chat();
	else
	{
		m_pGameInstance->All_Update_On();
		End_Chat();
		CStat_Manager::GetInstance()->Set_UIOpen(false);
		static_cast<CField_Npc_Face*>(m_pGameInstance->Find_UIObj(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("UI_NpcFace")))->Render_Face_Off();
	}
}

HRESULT CField_Npc_Chat::Initialize_Prototype(LEVEL eLevel)
{
	m_eLevel = eLevel;

	if (FAILED(Ready_ChildPrototype(eLevel)))
		return E_FAIL;
	return S_OK;
}

HRESULT CField_Npc_Chat::Initialize(void* pArg)
{
	FIELD_CHAT_DESC* Desc = static_cast<FIELD_CHAT_DESC*>(pArg);

	m_fSizeX = 256.f;
	m_fSizeY = 256.f;
	m_fX = 0.f;
	m_fY = Desc->fY;
	m_fZ = UI_DEPTH::FILED_CHAT;
	m_iWinSizeX = g_iWinSizeX;
	m_iWinSizeY = g_iWinSizeY;
	m_fDeleyTime = 0.13f;
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
	__super::Update_Position();
	if (FAILED(Ready_Children()))
		return E_FAIL;

	m_pTarget_Transform = Desc->pTransform;
	m_pGameInstance->Add_UIObject(Desc->m_iLevel, Desc->szChatTag, this);
	
	return S_OK;
}

void CField_Npc_Chat::Priority_Update(_float fTimeDelta)
{
	if (!m_bIsUpdate)
		return;
}

void CField_Npc_Chat::Update(_float fTimeDelta)
{
	if (!m_bIsUpdate)
		return;

	if (!m_bIschat)
		return;

	if (m_vecChatting.size() == 0)
		return;

	if (!m_bIsRenderchat && m_iChatIndex < m_iVecIndex)
	{
		m_szAllText = m_vecChatting[m_iChatIndex];
		m_bIsRenderchat = true;
	}
	else
	{
		if (m_iTextIndex < m_szAllText.size())
		{
			m_fAccTime += fTimeDelta;

			if (m_fAccTime >= m_fDeleyTime)
			{
				m_fAccTime = 0.f;

				m_szRenderText += m_szAllText[m_iTextIndex];
				m_iTextIndex++;
			}
		
		}
		else
		{
			m_bIsFinish = true;
		}
	}
}

void CField_Npc_Chat::Late_Update(_float fTimeDelta)
{
	if (!m_bIsUpdate)
		return;

	if (m_bIschat)
	{
		Target_Pos();
		m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI_BLEND, this);

		if (m_bIsFinish)
			m_vecChildren[0]->Late_Update(fTimeDelta);
	} else	if (m_bIsOn)
	{
		Target_Pos();
		m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI_BLEND, this);
		m_vecChildren[1]->Late_Update(fTimeDelta);
	}
}

HRESULT CField_Npc_Chat::Render()
{
	if (m_bIschat)
	{
		if (FAILED(m_pTextureCom->Bind_Texture(0)))
			return E_FAIL;
		m_pVIBufferCom->Bind_Buffers();
		__super::Begin();
		m_pVIBufferCom->Render();
		__super::End();

		Render_Font();
	}
	else if (m_bIsOn)
	{
		On_Chat_Font();
	}
	
	
	return S_OK;
}

HRESULT CField_Npc_Chat::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Rect_Chat"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;
	return S_OK;
}

HRESULT CField_Npc_Chat::Ready_ChildPrototype(LEVEL eLevel)
{
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_Chat_Guide"),
		CUI_KeyGuide::Create(m_pGraphic_Device))))
		return E_FAIL;

	return S_OK;
}

HRESULT CField_Npc_Chat::Ready_Children()
{
	CUIObject* pGameObject = nullptr;
	CUI_KeyGuide::KEYGUIDE_DESC Desc{};
	
	Desc.fX = 95;
	Desc.fY = -20;
	Desc.fZ = 4;
	Desc.pTransform = m_pTransformCom;
	Desc.bTarget = true;
	Desc.strKey = TEXT("F");
	Desc.Default = false;
	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Chat_Guide"), &Desc));
	if (nullptr == pGameObject)
		return E_FAIL;
	Add_Child(pGameObject);

	Desc.fX = -30;
	Desc.fY = -20;
	Desc.fZ = 4;
	Desc.pTransform = m_pTransformCom;
	Desc.bTarget = true;
	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Chat_Guide"), &Desc));
	if (nullptr == pGameObject)
		return E_FAIL;
	Add_Child(pGameObject);
	return S_OK;
}

void CField_Npc_Chat::Target_Pos()
{
	_float3 Target_Pos = m_pTarget_Transform->Get_State(STATE::POSITION);

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &m_OldViewMatrix);
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &m_OldProjMatrix);

	_float3 vWinPos{};
	_float4x4 vVPMatrix = m_OldViewMatrix * m_OldProjMatrix;
	D3DXVec3TransformCoord(&vWinPos, &Target_Pos, &vVPMatrix);

	m_iWinPosX = (vWinPos.x + 1.f) * 0.5 * g_iWinSizeX + m_fX;
	m_iWinPosY = (-vWinPos.y + 1.f) * 0.5 * g_iWinSizeY + m_fY;

	m_vWorldPos.x = m_iWinPosX - m_iWinSizeX * 0.5f;
	m_vWorldPos.y = -m_iWinPosY + m_iWinSizeY * 0.5f;
	m_vWorldPos.z = UI_DEPTH::FILED_ITEM;;

	m_pTransformCom->Set_State(STATE::POSITION, m_vWorldPos);
}

void CField_Npc_Chat::Render_Font()
{
	m_vTexRect.left = m_iWinPosX - 100;
	m_vTexRect.right = m_iWinPosX + 100;
	m_vTexRect.top = m_iWinPosY - 115;
	m_vTexRect.bottom = m_iWinPosY + 100;


	m_pGameInstance->Render_Font(TEXT("UI_Font_18"), m_szRenderText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), DT_LEFT | DT_VCENTER | DT_SINGLELINE);

	//m_vTexRect.left += 35;
	//m_vTexRect.right += 70;
	//_stprintf_s(szText, TEXT("구매하기"));
	//m_pGameInstance->Render_Font(TEXT("UI_Font_18"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), DT_LEFT | DT_VCENTER | DT_SINGLELINE);

}

void CField_Npc_Chat::On_Chat_Font()
{
	TCHAR szText[MAX_PATH];

	m_vTexRect.left = m_iWinPosX - 80;
	m_vTexRect.right = m_iWinPosX + 120;
	m_vTexRect.top = m_iWinPosY + 10;
	m_vTexRect.bottom = m_iWinPosY + 80;

	_stprintf_s(szText, TEXT("대화하기"));
	m_pGameInstance->Render_Font(TEXT("UI_Font_18"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), DT_CENTER | DT_TOP);

}

CField_Npc_Chat* CField_Npc_Chat::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
	CField_Npc_Chat* pInstance = new CField_Npc_Chat(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eLevel)))
	{
		MSG_BOX(TEXT("Failed to Created : CField_Npc_Chat"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CField_Npc_Chat::Clone(void* pArg)
{
	CField_Npc_Chat* pInstance = new CField_Npc_Chat(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CField_Npc_Chat"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CField_Npc_Chat::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}

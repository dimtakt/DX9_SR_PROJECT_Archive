#include "Event_Circle.h"
#include "Event_ClashKey.h"
#include "Event_Circle_Guide.h"
#include "GameInstance.h"
#include "Event_Result.h"
CEvent_Circle::CEvent_Circle(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CEvent_Circle::CEvent_Circle(const CEvent_Circle& Prototype) : CUIObject(Prototype), m_eLevel(Prototype.m_eLevel)
{
}

HRESULT CEvent_Circle::Initialize_Prototype(LEVEL eLevel)
{
	m_eLevel = eLevel;

	if (FAILED(Ready_ChildPrototype(eLevel)))
		return E_FAIL;

	return S_OK;
}

HRESULT CEvent_Circle::Initialize(void* pArg)
{
	UIOBJECT_DESC* Desc = static_cast<UIOBJECT_DESC*>(pArg);

	m_bIsRender = true;
	m_fCircle_Speed = 1.2;
	m_fCircle_Size = 2.5;
	m_iTexIndex = 0;
	m_fSizeX = 300;
	m_fSizeY = 300;
	m_fX = Desc->fX;
	m_fY = Desc->fY;
	m_fZ = UI_DEPTH::EVENT_AZ;

	Set_String();

	m_iWinSizeX = g_iWinSizeX;
	m_iWinSizeY = g_iWinSizeY;

	m_eState = CIRCLE_STATE::CIRCLE_PLAYING;
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

void CEvent_Circle::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CEvent_Circle::Update(_float fTimeDelta)
{
	if (!m_bIsRender)
		return;

	_int iInput_Key{};

	if(m_bIsKeyInput)
		iInput_Key = KeyCheck();

	if (iInput_Key != 0)
	{
		if (iInput_Key == m_iInsertKey && m_fCircle_Size <= 1.08 && m_fCircle_Size >= 0.92)
		{
			static_cast<CEvent_ClashKey*>(m_vecChildren[0])->Input_KeySetting(1);
			static_cast<CEvent_Result*>(m_vecChildren[2])->Perfect_Setting();

			m_iTexIndex = 2;
			m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
			m_bIsOver = true;

			m_eState = CIRCLE_STATE::CIRCLE_PERFECT;

		}
		else
		{
			static_cast<CEvent_ClashKey*>(m_vecChildren[0])->Input_KeySetting(3);
			static_cast<CEvent_Result*>(m_vecChildren[2])->Fail_Setting();

			m_iTexIndex = 1;
			m_bIsOver = true;

			m_eState = CIRCLE_STATE::CIRCLE_FAIL;
		}
		m_bIsClick = true;
	}
	if (m_fCircle_Size > 0.5 && !m_bIsClick)
	{
		m_fCircle_Size -= m_fCircle_Speed * fTimeDelta;
		m_pTransformCom->Scaling(m_fSizeX * m_fCircle_Size, m_fSizeY * m_fCircle_Size, 1.f);
	}
	else if (m_fCircle_Size <= 0.5 && !m_bIsClick)
	{
		m_bIsOver = true;
		static_cast<CEvent_ClashKey*>(m_vecChildren[0])->Input_KeySetting(3);
		static_cast<CEvent_Result*>(m_vecChildren[2])->Fail_Setting();

		m_iTexIndex = 1;
		m_bIsOver = true;
		m_eState = CIRCLE_STATE::CIRCLE_FAIL;
	}
	__super::Update(fTimeDelta);
}

void CEvent_Circle::Late_Update(_float fTimeDelta)
{
	if (!m_bIsRender)
		return;

	if (m_bIsOver)
	{
		m_fAcctime += fTimeDelta;

		if (m_fAcctime > 0.8)
		{
			m_fAcctime = 0.f;
			m_bIsRender = false;
		}
	}
	__super::Late_Update(fTimeDelta);
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI_BLEND, this);
}

HRESULT CEvent_Circle::Render()
{
	Set_Renderstate();
	if (FAILED(m_pTextureCom->Bind_Texture(m_iTexIndex)))
		return E_FAIL;

	m_pVIBufferCom->Bind_Buffers();
	__super::Begin();
	m_pVIBufferCom->Render();
	__super::End();

	Reset_Renderstate();
	return S_OK;
}

HRESULT CEvent_Circle::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Rect_EventCircle"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

void CEvent_Circle::Set_Renderstate()
{
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
}

void CEvent_Circle::Reset_Renderstate()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
}

_int CEvent_Circle::KeyCheck()
{
	if (m_pGameInstance->IsKeyDown('Q'))
	{
		m_bIsKeyInput = false;
		return 1;
	}
	else if (m_pGameInstance->IsKeyDown('W'))
	{
		m_bIsKeyInput = false;
		return 2;
	}
	else if (m_pGameInstance->IsKeyDown('E'))
	{
		m_bIsKeyInput = false;
		return 3;
	}
	else if (m_pGameInstance->IsKeyDown('R'))
	{
		m_bIsKeyInput = false;
		return 4;
	}
	else
		return 0;
}

HRESULT CEvent_Circle::Ready_ChildPrototype(LEVEL eLevel)
{
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Clash_Key"),
		CEvent_ClashKey::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Circle_Guide"),
		CEvent_Circle_Guide::Create(m_pGraphic_Device, m_eLevel))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Clash_Result"),
		CEvent_Result::Create(m_pGraphic_Device))))
		return E_FAIL;
	return S_OK;

}

HRESULT CEvent_Circle::Ready_Children()
{
	CUIObject* pGameObject = nullptr;

	CEvent_ClashKey::SLOT_KEYGUIDE_DESC Desc{};

	Desc.strKey = m_strKey;

	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Clash_Key"),&Desc));
	if (nullptr == pGameObject)
		return E_FAIL;
	Add_Child(pGameObject);

	Desc.fSizeX = m_fSizeX;
	Desc.fSizeY = m_fSizeY;

	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Circle_Guide"), &Desc));
	if (nullptr == pGameObject)
		return E_FAIL;
	Add_Child(pGameObject);

	CEvent_Result::RESUlT_DESC Result_Desc{};
	Result_Desc.fSizeX = 300;
	Result_Desc.fSizeY = 100;
	Result_Desc.fX = 0;
	Result_Desc.fY = 50;
	Result_Desc.strFont = TEXT("UI_Font_30");

	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Clash_Result"),&Result_Desc));
	if (nullptr == pGameObject)
		return E_FAIL;
	Add_Child(pGameObject);

	return S_OK;
}

void CEvent_Circle::Set_String()
{
	_int iStrIndex = (int)m_pGameInstance->Rand(0, 4);
	
	switch (iStrIndex)
	{
	case 0:
		m_strKey = 'Q';
		m_iInsertKey = 1;
		break;
	case 1:
		m_strKey = 'W';
		m_iInsertKey = 2;
		break;
	case 2:
		m_strKey = 'E';
		m_iInsertKey = 3;
		break;
	case 3:
		m_strKey = 'R';
		m_iInsertKey = 4;
		break;
	}
}

CEvent_Circle* CEvent_Circle::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
	CEvent_Circle* pInstance = new CEvent_Circle(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eLevel)))
	{
		MSG_BOX(TEXT("Failed to Created : CEvent_Circle"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CEvent_Circle::Clone(void* pArg)
{
	CEvent_Circle* pInstance = new CEvent_Circle(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CEvent_Circle"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEvent_Circle::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}

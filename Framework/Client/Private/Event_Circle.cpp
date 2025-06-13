#include "Event_Circle.h"
#include "Event_ClashKey.h"
#include "Event_Circle_Guide.h"
#include "GameInstance.h"
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
	m_fCircle_Size = 2;
	m_iTexIndex = 0;
	m_strKey = 'Q';

	m_fSizeX = 200;
	m_fSizeY = 200;
	m_fX = 0;
	m_fY = 0;
	m_fZ = UI_DEPTH::EVENT_AZ;

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

void CEvent_Circle::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CEvent_Circle::Update(_float fTimeDelta)
{
	_int iInput_Key = KeyCheck();

	if (iInput_Key != 0)
	{
		if (iInput_Key == 1 && m_fCircle_Size <= 1.08 && m_fCircle_Size >= 0.92)
		{
			m_iTexIndex = 2;
		}
		else
		{
			m_iTexIndex = 1;
		}
		m_bIsClick = true;
	}
	if (m_fCircle_Size > 0.5 && !m_bIsClick)
	{
		m_fCircle_Size -= 0.01;
		m_pTransformCom->Scaling(m_fSizeX * m_fCircle_Size, m_fSizeY * m_fCircle_Size, 1.f);
	}
	__super::Update(fTimeDelta);
}

void CEvent_Circle::Late_Update(_float fTimeDelta)
{
	
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI_BLEND, this);
	__super::Late_Update(fTimeDelta);
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
		return 1;
	else if (m_pGameInstance->IsKeyDown('W'))
		return 2;
	else if (m_pGameInstance->IsKeyDown('E'))
		return 3;
	else if (m_pGameInstance->IsKeyDown('R'))
		return 4;
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

	return S_OK;

}

HRESULT CEvent_Circle::Ready_Children()
{
	CUIObject* pGameObject = nullptr;

	CEvent_ClashKey::SLOT_KEYGUIDE_DESC Desc{};

	Desc.strKey = 'Q';

	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Clash_Key"),&Desc));
	if (nullptr == pGameObject)
		return E_FAIL;
	Add_Child(pGameObject);

	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Circle_Guide"), &Desc));
	if (nullptr == pGameObject)
		return E_FAIL;
	Add_Child(pGameObject);
	return S_OK;
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

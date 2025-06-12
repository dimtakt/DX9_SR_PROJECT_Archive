#include "Event_ClashKey.h"
#include "GameInstance.h"
CEvent_ClashKey::CEvent_ClashKey(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CEvent_ClashKey::CEvent_ClashKey(const CEvent_ClashKey& Prototype) : CUIObject(Prototype), m_strKey(Prototype.m_strKey)
{
}

void CEvent_ClashKey::Input_KeySetting()
{
	m_fY = -20;
	m_iTexIndex = 2;
	__super::Update_Position();
}

void CEvent_ClashKey::Clear_KeySetting()
{
	m_fY = 0;
	m_iTexIndex = 1;
	__super::Update_Position();
}

void CEvent_ClashKey::Reset_KeySetting()
{
	m_fY = 0;
	m_iTexIndex = 0;
	__super::Update_Position();
}

HRESULT CEvent_ClashKey::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEvent_ClashKey::Initialize(void* pArg)
{
	SLOT_KEYGUIDE_DESC* Desc = static_cast<SLOT_KEYGUIDE_DESC*>(pArg);

	m_strKey = Desc->strKey;
	m_iTexIndex = 0;
	m_fSizeX = 50;
	m_fSizeY = 50;
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

	return S_OK;
}

void CEvent_ClashKey::Priority_Update(_float fTimeDelta)
{
}

void CEvent_ClashKey::Update(_float fTimeDelta)
{

}

void CEvent_ClashKey::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI_BLEND, this);
}

HRESULT CEvent_ClashKey::Render()
{
	if (FAILED(m_pTextureCom->Bind_Texture(m_iTexIndex)))
		return E_FAIL;

	m_pVIBufferCom->Bind_Buffers();
	__super::Begin();
	m_pVIBufferCom->Render();
	__super::End();

	Font_Rect_Update();
	m_pGameInstance->Render_Font(TEXT("UI_Font_22"), m_strKey, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	return S_OK;
}

HRESULT CEvent_ClashKey::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Rect_EventKey"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

CEvent_ClashKey* CEvent_ClashKey::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CEvent_ClashKey* pInstance = new CEvent_ClashKey(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CEvent_ClashKey"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CEvent_ClashKey::Clone(void* pArg)
{
	CEvent_ClashKey* pInstance = new CEvent_ClashKey(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CEvent_ClashKey"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEvent_ClashKey::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}

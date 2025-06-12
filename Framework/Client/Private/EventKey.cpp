#include "EventKey.h"
#include "GameInstance.h"
CEventKey::CEventKey(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CEventKey::CEventKey(const CEventKey& Prototype) : CUIObject(Prototype), m_strKey(Prototype.m_strKey)
{
}

HRESULT CEventKey::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEventKey::Initialize(void* pArg)
{
	SLOT_KEYGUIDE_DESC* Desc = static_cast<SLOT_KEYGUIDE_DESC*>(pArg);

	m_strKey = Desc->strKey;

	m_fSizeX = 50;
	m_fSizeY = 50;
	m_fX = Desc->fX * 55 - (Desc->fZ - 1) * 55 / 2;;
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

void CEventKey::Priority_Update(_float fTimeDelta)
{
}

void CEventKey::Update(_float fTimeDelta)
{
}

void CEventKey::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI_BLEND, this);
}

HRESULT CEventKey::Render()
{
	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;

	m_pVIBufferCom->Bind_Buffers();
	__super::Begin();
	m_pVIBufferCom->Render();
	__super::End();

	Font_Rect_Update();
	m_pGameInstance->Render_Font(TEXT("UI_Font_14"), TEXT("Q"), m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	return S_OK;
}

HRESULT CEventKey::Ready_Components()
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

CEventKey* CEventKey::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CEventKey* pInstance = new CEventKey(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CEventKey"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CEventKey::Clone(void* pArg)
{
	CEventKey* pInstance = new CEventKey(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CEventKey"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEventKey::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}

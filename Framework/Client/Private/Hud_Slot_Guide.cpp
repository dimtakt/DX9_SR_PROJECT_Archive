#include "Hud_Slot_Guide.h"
#include "GameInstance.h"
CHud_Slot_Guide::CHud_Slot_Guide(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CHud_Slot_Guide::CHud_Slot_Guide(const CHud_Slot_Guide& Prototype) : CUIObject(Prototype), m_strKey(Prototype.m_strKey)
{
}

HRESULT CHud_Slot_Guide::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CHud_Slot_Guide::Initialize(void* pArg)
{
	SLOT_KEYGUIDE_DESC* Desc = static_cast<SLOT_KEYGUIDE_DESC*>(pArg);
	
	m_strKey = Desc->strKey;

	m_fSizeX = 20;
	m_fSizeY = 20;
	m_fX = 0;
	m_fY = 22;
	m_fZ = UI_DEPTH::KEY_GUIDE;

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

void CHud_Slot_Guide::Priority_Update(_float fTimeDelta)
{
}

void CHud_Slot_Guide::Update(_float fTimeDelta)
{
}

void CHud_Slot_Guide::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CHud_Slot_Guide::Render()
{
	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;

	m_pVIBufferCom->Bind_Buffers();
	__super::Begin();
	m_pVIBufferCom->Render();
	__super::End();

	Font_Rect_Update();
	m_pGameInstance->Render_Font(TEXT("UI_Font_14"), m_strKey, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	return S_OK;
}

HRESULT CHud_Slot_Guide::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Rect_UI_Hud_KeyInputGuide"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

CHud_Slot_Guide* CHud_Slot_Guide::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CHud_Slot_Guide* pInstance = new CHud_Slot_Guide(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CHud_Slot_Guide"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CHud_Slot_Guide::Clone(void* pArg)
{
	CHud_Slot_Guide* pInstance = new CHud_Slot_Guide(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CHud_Slot_Guide"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CHud_Slot_Guide::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}

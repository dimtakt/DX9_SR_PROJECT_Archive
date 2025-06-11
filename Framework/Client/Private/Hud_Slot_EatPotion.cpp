#include "Hud_Slot_EatPotion.h"
#include "GameInstance.h"

CHud_Slot_EatPotion::CHud_Slot_EatPotion(LPDIRECT3DDEVICE9 pGraphic_Device) : CProgressBar(pGraphic_Device)
{
}

CHud_Slot_EatPotion::CHud_Slot_EatPotion(const CHud_Slot_EatPotion& Prototype) : CProgressBar(Prototype)
{
}

HRESULT CHud_Slot_EatPotion::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CHud_Slot_EatPotion::Initialize(void* pArg)
{
	m_fSizeX = 50;
	m_fSizeY = 5;
	m_fX = 0;
	m_fY = -32;
	m_fZ = UI_DEPTH::KEY_GUIDE;
	m_iWinSizeX = g_iWinSizeX;
	m_iWinSizeY = g_iWinSizeY;

	if (FAILED(CUIObject::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
	__super::Update_Position();
	return S_OK;
}

void CHud_Slot_EatPotion::Priority_Update(_float fTimeDelta)
{

}

void CHud_Slot_EatPotion::Update(_float fTimeDelta)
{
}

void CHud_Slot_EatPotion::Late_Update(_float fTimeDelta)
{
	Progress_UpdateX();
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI_BLEND, this);
}

HRESULT CHud_Slot_EatPotion::Render()
{
	m_pGraphic_Device->SetTexture(0, NULL);
	m_pVIBufferCom->Bind_Buffers();
	__super::Begin();
	m_pVIBufferCom->Render();
	__super::End();

	return S_OK;
}

void CHud_Slot_EatPotion::Progerss_Set(_int iCulValue, _int iMaxValue)
{
	m_iCulValue = iCulValue;
	m_iCulMaxValue = iMaxValue;
}

HRESULT CHud_Slot_EatPotion::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Rect_UI_WhiteRect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	return S_OK;
}

CHud_Slot_EatPotion* CHud_Slot_EatPotion::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CHud_Slot_EatPotion* pInstance = new CHud_Slot_EatPotion(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CHud_Slot_EatPotion"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CHud_Slot_EatPotion::Clone(void* pArg)
{
	CHud_Slot_EatPotion* pInstance = new CHud_Slot_EatPotion(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CHud_Slot_EatPotion"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CHud_Slot_EatPotion::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
}
#include "Hud_Slot_CoolTime.h"
#include "GameInstance.h"

CHud_Slot_CoolTime::CHud_Slot_CoolTime(LPDIRECT3DDEVICE9 pGraphic_Device) : CProgressBar(pGraphic_Device)
{
}

CHud_Slot_CoolTime::CHud_Slot_CoolTime(const CHud_Slot_CoolTime& Prototype) : CProgressBar(Prototype)
{
}

HRESULT CHud_Slot_CoolTime::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CHud_Slot_CoolTime::Initialize(void* pArg)
{
	m_fSizeX = 610;
	m_fSizeY = 40;
	m_fX = 0;
	m_fY = 0;
	m_fZ = UI_DEPTH::BOSSHP;
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

void CHud_Slot_CoolTime::Priority_Update(_float fTimeDelta)
{

}

void CHud_Slot_CoolTime::Update(_float fTimeDelta)
{
}

void CHud_Slot_CoolTime::Late_Update(_float fTimeDelta)
{
	Progress_UpdateX();
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CHud_Slot_CoolTime::Render()
{
	m_pGraphic_Device->SetTexture(0, NULL);
	m_pVIBufferCom->Bind_Buffers();
	__super::Begin();
	m_pVIBufferCom->Render();
	__super::End();
	return S_OK;
}

void CHud_Slot_CoolTime::HpBar_Set(_int iCulValue, _int iMaxValue)
{
	m_iCulValue = iCulValue;
	m_iCulMaxValue = iMaxValue;
}


HRESULT CHud_Slot_CoolTime::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Rect_UI_Hud_States_Hp"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	return S_OK;
}

CHud_Slot_CoolTime* CHud_Slot_CoolTime::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CHud_Slot_CoolTime* pInstance = new CHud_Slot_CoolTime(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CHud_Slot_CoolTime"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CHud_Slot_CoolTime::Clone(void* pArg)
{
	CHud_Slot_CoolTime* pInstance = new CHud_Slot_CoolTime(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CHud_Slot_CoolTime"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CHud_Slot_CoolTime::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
}
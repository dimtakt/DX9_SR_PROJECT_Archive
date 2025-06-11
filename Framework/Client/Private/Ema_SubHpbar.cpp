#include "Ema_SubHpbar.h"
#include "GameInstance.h"

CEma_SubHpbar::CEma_SubHpbar(LPDIRECT3DDEVICE9 pGraphic_Device) : CProgressBar(pGraphic_Device)
{
}

CEma_SubHpbar::CEma_SubHpbar(const CEma_SubHpbar& Prototype) : CProgressBar(Prototype)
{
}

HRESULT CEma_SubHpbar::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEma_SubHpbar::Initialize(void* pArg)
{
	m_fSizeX = 85;
	m_fSizeY = 25;
	m_fX = 0;
	m_fY = -30;
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

void CEma_SubHpbar::Priority_Update(_float fTimeDelta)
{

}

void CEma_SubHpbar::Update(_float fTimeDelta)
{
}

void CEma_SubHpbar::Late_Update(_float fTimeDelta)
{
	Progress_UpdateX();
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CEma_SubHpbar::Render()
{
	m_pGraphic_Device->SetTexture(0, NULL);
	m_pVIBufferCom->Bind_Buffers();
	__super::Begin();
	m_pVIBufferCom->Render();
	__super::End();
	return S_OK;
}

void CEma_SubHpbar::HpBar_Set(_int iCulValue, _int iMaxValue)
{
	m_iCulValue = iCulValue;
	m_iCulMaxValue = iMaxValue;
}


HRESULT CEma_SubHpbar::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Rect_UI_Hud_States_Hp"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	return S_OK;
}

CEma_SubHpbar* CEma_SubHpbar::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CEma_SubHpbar* pInstance = new CEma_SubHpbar(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CEma_SubHpbar"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CEma_SubHpbar::Clone(void* pArg)
{
	CEma_SubHpbar* pInstance = new CEma_SubHpbar(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CEma_SubHpbar"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEma_SubHpbar::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
}
#include "Ema_Hpbar.h"
#include "GameInstance.h"

CEma_Hpbar::CEma_Hpbar(LPDIRECT3DDEVICE9 pGraphic_Device) : CProgressBar(pGraphic_Device)
{
}

CEma_Hpbar::CEma_Hpbar(const CEma_Hpbar& Prototype) : CProgressBar(Prototype)
{
}

HRESULT CEma_Hpbar::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEma_Hpbar::Initialize(void* pArg)
{
	m_fSizeX = 590;
	m_fSizeY = 40;
	m_fX = 0;
	m_fY = -5;
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

void CEma_Hpbar::Priority_Update(_float fTimeDelta)
{

}

void CEma_Hpbar::Update(_float fTimeDelta)
{
}

void CEma_Hpbar::Late_Update(_float fTimeDelta)
{
	Progress_UpdateX();
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CEma_Hpbar::Render()
{
	m_pGraphic_Device->SetTexture(0, NULL);
	m_pVIBufferCom->Bind_Buffers();
	__super::Begin();
	m_pVIBufferCom->Render();
	__super::End();
	return S_OK;
}

void CEma_Hpbar::HpBar_Set(_int iCulValue, _int iMaxValue)
{
	m_iCulValue = iCulValue;
	m_iCulMaxValue = iMaxValue;
}


HRESULT CEma_Hpbar::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Rect_UI_GreenblueRect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	return S_OK;
}

CEma_Hpbar* CEma_Hpbar::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CEma_Hpbar* pInstance = new CEma_Hpbar(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CEma_Hpbar"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CEma_Hpbar::Clone(void* pArg)
{
	CEma_Hpbar* pInstance = new CEma_Hpbar(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CEma_Hpbar"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEma_Hpbar::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
}
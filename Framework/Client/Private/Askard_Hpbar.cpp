#include "Askard_Hpbar.h"
#include "GameInstance.h"

CAskard_Hpbar::CAskard_Hpbar(LPDIRECT3DDEVICE9 pGraphic_Device) : CProgressBar(pGraphic_Device)
{
}

CAskard_Hpbar::CAskard_Hpbar(const CAskard_Hpbar& Prototype) : CProgressBar(Prototype)
{
}

HRESULT CAskard_Hpbar::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CAskard_Hpbar::Initialize(void* pArg)
{
	m_iCulMaxValue = 100;
	m_iCulValue = 100;
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

void CAskard_Hpbar::Priority_Update(_float fTimeDelta)
{

}

void CAskard_Hpbar::Update(_float fTimeDelta)
{
	if (m_pGameInstance->IsKeyDown(VK_F3))
		m_iCulValue -= 1;
}

void CAskard_Hpbar::Late_Update(_float fTimeDelta)
{
	Progress_UpdateX();
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CAskard_Hpbar::Render()
{
	m_pGraphic_Device->SetTexture(0, NULL);
	m_pVIBufferCom->Bind_Buffers();
	__super::Begin();
	m_pVIBufferCom->Render();
	__super::End();
	return S_OK;
}


HRESULT CAskard_Hpbar::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Rect_UI_Hud_States_Hp"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	return S_OK;
}

CAskard_Hpbar* CAskard_Hpbar::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CAskard_Hpbar* pInstance = new CAskard_Hpbar(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CAskard_Hpbar"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CAskard_Hpbar::Clone(void* pArg)
{
	CAskard_Hpbar* pInstance = new CAskard_Hpbar(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CAskard_Hpbar"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CAskard_Hpbar::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
}
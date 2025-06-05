#include "Gacha_Button_Icon.h"
#include "GameInstance.h"

CGacha_Button_Icon::CGacha_Button_Icon(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CGacha_Button_Icon::CGacha_Button_Icon(const CGacha_Button_Icon& Prototype) : CUIObject(Prototype)
{
}

HRESULT CGacha_Button_Icon::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CGacha_Button_Icon::Initialize(void* pArg)
{
	m_fSizeX = 37;
	m_fSizeY = 37;
	m_fX = -60;
	m_fY = 0;
	m_fZ = UI_DEPTH::GACHA;
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

void CGacha_Button_Icon::Priority_Update(_float fTimeDelta)
{
}

void CGacha_Button_Icon::Update(_float fTimeDelta)
{
}

void CGacha_Button_Icon::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CGacha_Button_Icon::Render()
{
	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;
	m_pVIBufferCom->Bind_Buffers();
	__super::Begin();
	m_pVIBufferCom->Render();
	__super::End();

	return S_OK;
}

HRESULT CGacha_Button_Icon::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Rect_UI_Hud_Dice_Symbol"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

CGacha_Button_Icon* CGacha_Button_Icon::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CGacha_Button_Icon* pInstance = new CGacha_Button_Icon(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CGacha_Button_Icon"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CGacha_Button_Icon::Clone(void* pArg)
{
	CGacha_Button_Icon* pInstance = new CGacha_Button_Icon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CGacha_Button_Icon"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CGacha_Button_Icon::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}

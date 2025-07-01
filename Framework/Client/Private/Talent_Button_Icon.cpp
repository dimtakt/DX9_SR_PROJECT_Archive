#include "Talent_Button_Icon.h"
#include "GameInstance.h"
CTalent_Button_Icon::CTalent_Button_Icon(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CTalent_Button_Icon::CTalent_Button_Icon(const CTalent_Button_Icon& Prototype) : CUIObject(Prototype)
{
}

HRESULT CTalent_Button_Icon::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTalent_Button_Icon::Initialize(void* pArg)
{
	m_fSizeX = 40;
	m_fSizeY = 40;
	m_fX = -50;
	m_fY = -3;
	m_fZ = UI_DEPTH::TALENT;
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

void CTalent_Button_Icon::Priority_Update(_float fTimeDelta)
{
}

void CTalent_Button_Icon::Update(_float fTimeDelta)
{
}

void CTalent_Button_Icon::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CTalent_Button_Icon::Render()
{
	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;
	m_pVIBufferCom->Bind_Buffers();

	__super::Begin();
	m_pVIBufferCom->Render();
	return S_OK;
}

HRESULT CTalent_Button_Icon::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Rect_Talent_Button_Icon"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

CTalent_Button_Icon* CTalent_Button_Icon::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTalent_Button_Icon* pInstance = new CTalent_Button_Icon(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CTalent_Button_Icon"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CTalent_Button_Icon::Clone(void* pArg)
{
	CTalent_Button_Icon* pInstance = new CTalent_Button_Icon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CTalent_Button_Icon"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTalent_Button_Icon::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}

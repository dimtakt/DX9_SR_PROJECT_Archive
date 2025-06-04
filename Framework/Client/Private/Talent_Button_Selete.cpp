#include "Talent_Button_Selete.h"
#include "GameInstance.h"

CTalent_Button_Selete::CTalent_Button_Selete(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CTalent_Button_Selete::CTalent_Button_Selete(const CTalent_Button_Selete& Prototype) : CUIObject(Prototype)
{
}

HRESULT CTalent_Button_Selete::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTalent_Button_Selete::Initialize(void* pArg)
{
	UIOBJECT_DESC* Desc = static_cast<UIOBJECT_DESC*>(pArg);

	m_iIndex = Desc->fZ;

	if (Desc->fZ == 1 || Desc->fZ == 2)
	{
		m_fSizeX = 55;
		m_fSizeY = 55;
	}
	else
	{
		m_fSizeX = 64;
		m_fSizeY = 64;
	}
	
	m_fX = 0;
	m_fY = 0;
	m_fZ = UI_DEPTH::TALENT_Selete;
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

void CTalent_Button_Selete::Priority_Update(_float fTimeDelta)
{
}

void CTalent_Button_Selete::Update(_float fTimeDelta)
{
}

void CTalent_Button_Selete::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CTalent_Button_Selete::Render()
{
	if (FAILED(m_pTextureCom->Bind_Texture(m_iIndex)))
		return E_FAIL;
	m_pVIBufferCom->Bind_Buffers();

	__super::Begin();
	m_pVIBufferCom->Render();
	return S_OK;
}

HRESULT CTalent_Button_Selete::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Rect_Talent_Slot_Button_Selete"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

CTalent_Button_Selete* CTalent_Button_Selete::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTalent_Button_Selete* pInstance = new CTalent_Button_Selete(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CTalent_Button_Selete"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CTalent_Button_Selete::Clone(void* pArg)
{
	CTalent_Button_Selete* pInstance = new CTalent_Button_Selete(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CTalent_Button_Selete"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTalent_Button_Selete::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}

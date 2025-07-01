#include "Event_Result.h"
#include "GameInstance.h"
CEvent_Result::CEvent_Result(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CEvent_Result::CEvent_Result(const CEvent_Result& Prototype) : CUIObject(Prototype), m_strKey(Prototype.m_strKey)
{
}

void CEvent_Result::Perfect_Setting()
{
	m_bIsRender = true;
	m_strKey = TEXT("PERFECT");
	m_iTexIndex = 2;
}

void CEvent_Result::Succes_Setting()
{
	m_bIsRender = true;
	m_strKey = TEXT("SUCCES");
	m_iTexIndex = 0;
}

void CEvent_Result::Fail_Setting()
{
	m_bIsRender = true;
	m_strKey = TEXT("FAIL");
	m_iTexIndex = 1;
}

HRESULT CEvent_Result::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEvent_Result::Initialize(void* pArg)
{
	RESUlT_DESC* Desc = static_cast<RESUlT_DESC*>(pArg);

	m_iTexIndex = 0;
	m_strFont = Desc->strFont;
	m_fSizeX = Desc->fSizeX;
	m_fSizeY = Desc->fSizeY;
	m_fX = Desc->fX;
	m_fY = Desc->fY;
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

void CEvent_Result::Priority_Update(_float fTimeDelta)
{
}

void CEvent_Result::Update(_float fTimeDelta)
{
}

void CEvent_Result::Late_Update(_float fTimeDelta)
{
	if(m_bIsRender)
		m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI_BLEND, this);
}

HRESULT CEvent_Result::Render()
{
	Set_Renderstate();
	if (FAILED(m_pTextureCom->Bind_Texture(m_iTexIndex)))
		return E_FAIL;

	m_pVIBufferCom->Bind_Buffers();
	__super::Begin();
	m_pVIBufferCom->Render();
	__super::End();

	Font_Rect_Update();
	m_pGameInstance->Render_Font(m_strFont, m_strKey, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	
	Reset_Renderstate();
	return S_OK;
}

HRESULT CEvent_Result::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Rect_Result"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

void CEvent_Result::Set_Renderstate()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
}

void CEvent_Result::Reset_Renderstate()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
}

CEvent_Result* CEvent_Result::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CEvent_Result* pInstance = new CEvent_Result(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CEvent_Result"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CEvent_Result::Clone(void* pArg)
{
	CEvent_Result* pInstance = new CEvent_Result(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CEvent_Result"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEvent_Result::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}

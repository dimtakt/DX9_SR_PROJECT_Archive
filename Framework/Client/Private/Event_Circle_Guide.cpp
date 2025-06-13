#include "Event_Circle_Guide.h"
#include "GameInstance.h"
CEvent_Circle_Guide::CEvent_Circle_Guide(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CEvent_Circle_Guide::CEvent_Circle_Guide(const CEvent_Circle_Guide& Prototype) : CUIObject(Prototype), m_eLevel(Prototype.m_eLevel)
{
}

HRESULT CEvent_Circle_Guide::Initialize_Prototype(LEVEL eLevel)
{
	m_eLevel = eLevel;
	return S_OK;
}

HRESULT CEvent_Circle_Guide::Initialize(void* pArg)
{
	m_fSizeX = 200;
	m_fSizeY = 200;
	m_fX = 0;
	m_fY = 0;
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

void CEvent_Circle_Guide::Priority_Update(_float fTimeDelta)
{
}

void CEvent_Circle_Guide::Update(_float fTimeDelta)
{
}

void CEvent_Circle_Guide::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI_BLEND, this);
}

HRESULT CEvent_Circle_Guide::Render()
{
	Set_Renderstate();
	if (FAILED(m_pTextureCom->Bind_Texture(3)))
		return E_FAIL;

	m_pVIBufferCom->Bind_Buffers();
	__super::Begin();
	m_pVIBufferCom->Render();
	__super::End();

	Reset_Renderstate();
	return S_OK;
}

HRESULT CEvent_Circle_Guide::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Rect_EventCircle"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

void CEvent_Circle_Guide::Set_Renderstate()
{
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
}

void CEvent_Circle_Guide::Reset_Renderstate()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
}

CEvent_Circle_Guide* CEvent_Circle_Guide::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
	CEvent_Circle_Guide* pInstance = new CEvent_Circle_Guide(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eLevel)))
	{
		MSG_BOX(TEXT("Failed to Created : CEvent_Circle_Guide"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CEvent_Circle_Guide::Clone(void* pArg)
{
	CEvent_Circle_Guide* pInstance = new CEvent_Circle_Guide(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CEvent_Circle_Guide"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEvent_Circle_Guide::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}

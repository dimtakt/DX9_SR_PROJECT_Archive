#include "Dash_Gauge.h"
#include "GameInstance.h"
CDash_Gauge::CDash_Gauge(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CDash_Gauge::CDash_Gauge(const CDash_Gauge& Prototype) : CUIObject(Prototype), m_eLevel(Prototype.m_eLevel)
{
}

HRESULT CDash_Gauge::Initialize_Prototype(LEVEL eLevel)
{
    m_eLevel = eLevel;

    if (FAILED(Ready_ChildPrototype(eLevel)))
        return E_FAIL;

    return S_OK;
}

HRESULT CDash_Gauge::Initialize(void* pArg)
{
	UIOBJECT_DESC Desc{};

	Desc.fSizeX = 32;
	Desc.fSizeY = 32;
	Desc.fX = 0;
	Desc.fY = 0;
	Desc.fZ = 0.f;
	Desc.iWinSizeX = g_iWinSizeX;
	Desc.iWinSizeY = g_iWinSizeY;

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
	__super::Update_Position();

	if (FAILED(Ready_Children()))
		return E_FAIL;

	return S_OK;
}

void CDash_Gauge::Priority_Update(_float fTimeDelta)
{
}

void CDash_Gauge::Update(_float fTimeDelta)
{
}

void CDash_Gauge::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CDash_Gauge::Render()
{
	SetUp_RenderState();

	m_pTransformCom->Bind_Matrix();
	if (FAILED(m_pTextureCom->Bind_Texture(1)))
		return E_FAIL;
	m_pVIBufferCom->Bind_Buffers();

	__super::Begin();
	m_pVIBufferCom->Render();
	__super::End();

	Reset_RenderState();
	return S_OK;
}

HRESULT CDash_Gauge::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Rect_UI_Hud_Dash_Symbol"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

void CDash_Gauge::SetUp_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
}

void CDash_Gauge::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	m_pGraphic_Device->SetTexture(0, NULL);
}

HRESULT CDash_Gauge::Ready_ChildPrototype(LEVEL eLevel)
{
	return S_OK;
}

HRESULT CDash_Gauge::Ready_Children()
{
	return S_OK;
}

CDash_Gauge* CDash_Gauge::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
	CDash_Gauge* pInstance = new CDash_Gauge(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eLevel)))
	{
		MSG_BOX(TEXT("Failed to Created : CDash_Gauge"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CDash_Gauge::Clone(void* pArg)
{
	CDash_Gauge* pInstance = new CDash_Gauge(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CDash_Gauge"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CDash_Gauge::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}

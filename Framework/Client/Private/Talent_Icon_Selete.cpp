#include "Talent_Icon_Selete.h"
#include "GameInstance.h"

CTalent_Icon_Selete::CTalent_Icon_Selete(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CTalent_Icon_Selete::CTalent_Icon_Selete(const CTalent_Icon_Selete& Prototype) : CUIObject(Prototype)
{
}

HRESULT CTalent_Icon_Selete::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTalent_Icon_Selete::Initialize(void* pArg)
{

	m_fSizeX = 64;
	m_fSizeY = 64;
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

void CTalent_Icon_Selete::Priority_Update(_float fTimeDelta)
{
}

void CTalent_Icon_Selete::Update(_float fTimeDelta)
{
}

void CTalent_Icon_Selete::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CTalent_Icon_Selete::Render()
{
	SetUp_RenderState();

	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;
	m_pVIBufferCom->Bind_Buffers();

	__super::Begin();
	m_pVIBufferCom->Render();
	Reset_RenderState();
	return S_OK;
}

HRESULT CTalent_Icon_Selete::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Rect_Talent_Slot_Icon_Selete"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

void CTalent_Icon_Selete::SetUp_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
}

void CTalent_Icon_Selete::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	m_pGraphic_Device->SetTexture(0, NULL);
}

CTalent_Icon_Selete* CTalent_Icon_Selete::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTalent_Icon_Selete* pInstance = new CTalent_Icon_Selete(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CTalent_Icon_Selete"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CTalent_Icon_Selete::Clone(void* pArg)
{
	CTalent_Icon_Selete* pInstance = new CTalent_Icon_Selete(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CTalent_Icon_Selete"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTalent_Icon_Selete::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}

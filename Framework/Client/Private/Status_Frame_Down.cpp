#include "Status_Frame_Down.h"
#include "GameInstance.h"
CStatus_Frame_Down::CStatus_Frame_Down(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CStatus_Frame_Down::CStatus_Frame_Down(const CStatus_Frame_Down& Prototype) : CUIObject(Prototype), m_eLevel{ Prototype.m_eLevel }
{
}

HRESULT CStatus_Frame_Down::Initialize_Prototype(LEVEL eLevel)
{
	m_eLevel = eLevel;

	if (FAILED(Ready_ChildPrototype(eLevel)))
		return E_FAIL;

	return S_OK;
}

HRESULT CStatus_Frame_Down::Initialize(void* pArg)
{
	m_fSizeX = 410;
	m_fSizeY = 410;
	m_fX = 0;
	m_fY = 300;
	m_fZ = UI_DEPTH::PLAYER_STAUTS;
	m_iWinSizeX = g_iWinSizeX;
	m_iWinSizeY = g_iWinSizeY;

	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
	__super::Update_Position();

	if (FAILED(Ready_Children()))
		return E_FAIL;

	return S_OK;
}

void CStatus_Frame_Down::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CStatus_Frame_Down::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CStatus_Frame_Down::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
	__super::Late_Update(fTimeDelta);
}

HRESULT CStatus_Frame_Down::Render()
{
	SetUp_RenderState();

	if (FAILED(m_pTextureCom->Bind_Texture(2)))
		return E_FAIL;
	m_pVIBufferCom->Bind_Buffers();

	__super::Begin();
	m_pVIBufferCom->Render();
	__super::End();

	Reset_RenderState();
	return S_OK;
}

HRESULT CStatus_Frame_Down::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Rect_Status_Window_Frame"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CStatus_Frame_Down::Ready_ChildPrototype(LEVEL eLevel)
{
	return S_OK;
}

HRESULT CStatus_Frame_Down::Ready_Children()
{
	return S_OK;
}

void CStatus_Frame_Down::SetUp_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

}

void CStatus_Frame_Down::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	m_pGraphic_Device->SetTexture(0, NULL);

}

CStatus_Frame_Down* CStatus_Frame_Down::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
	CStatus_Frame_Down* pInstance = new CStatus_Frame_Down(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eLevel)))
	{
		MSG_BOX(TEXT("Failed to Create : CStatus_Frame_Down"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CStatus_Frame_Down::Clone(void* pArg)
{
	CStatus_Frame_Down* pInstance = new CStatus_Frame_Down(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CStatus_Frame_Down"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CStatus_Frame_Down::Free()
{
	__super::Free();
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}

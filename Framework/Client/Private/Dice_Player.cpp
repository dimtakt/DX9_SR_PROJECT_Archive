#include "Dice_Player.h"
#include "GameInstance.h"

CDice_Player::CDice_Player(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CDice_Player::CDice_Player(const CDice_Player& Prototype) : CUIObject(Prototype)
{
}

HRESULT CDice_Player::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CDice_Player::Initialize(void* pArg)
{
	UIOBJECT_DESC Desc{};
	Desc.fSizeX = 32;
	Desc.fSizeY = 32;
	Desc.fX = -32;
	Desc.fY = 0;
	Desc.iWinSizeX = g_iWinSizeX;
	Desc.iWinSizeY = g_iWinSizeY;

	if (FAILED(CUIObject::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
	__super::Update_Position();

	return S_OK;
}

void CDice_Player::Priority_Update(_float fTimeDelta)
{
}

void CDice_Player::Update(_float fTimeDelta)
{
}

void CDice_Player::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CDice_Player::Render()
{
	SetUp_RenderState();

	m_pTransformCom->Bind_Matrix();
	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;
	m_pVIBufferCom->Bind_Buffers();

	__super::Begin();
	m_pVIBufferCom->Render();
	__super::End();

	Reset_RenderState();
	return S_OK;
}

HRESULT CDice_Player::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Rect_UI_Hud_Dice_Symbol"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

void CDice_Player::SetUp_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
}

void CDice_Player::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	m_pGraphic_Device->SetTexture(0, NULL);
}

CDice_Player* CDice_Player::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CDice_Player* pInstance = new CDice_Player(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CDice_Player"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CDice_Player::Clone(void* pArg)
{
	CDice_Player* pInstance = new CDice_Player(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CDice_Player"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CDice_Player::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}

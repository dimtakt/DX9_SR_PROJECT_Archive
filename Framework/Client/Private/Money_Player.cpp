#include "Money_Player.h"
#include "GameInstance.h"
CMoney_Player::CMoney_Player(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CMoney_Player::CMoney_Player(const CMoney_Player& Prototype) : CUIObject(Prototype)
{
}

HRESULT CMoney_Player::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMoney_Player::Initialize(void* pArg)
{
	m_fSizeX = 32;
	m_fSizeY = 32;
	m_fX = -32;
	m_fY = 0;
	m_fZ = UI_DEPTH::HUD_WALLET;
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

void CMoney_Player::Priority_Update(_float fTimeDelta)
{
}

void CMoney_Player::Update(_float fTimeDelta)
{
}

void CMoney_Player::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CMoney_Player::Render()
{
	SetUp_RenderState();

	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;
	m_pVIBufferCom->Bind_Buffers();

	__super::Begin();
	m_pVIBufferCom->Render();
	Render_Font();
	__super::End();

	Reset_RenderState();
	return S_OK;
}

HRESULT CMoney_Player::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Rect_UI_Hud_Money_Symbol"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

void CMoney_Player::SetUp_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
}

void CMoney_Player::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	m_pGraphic_Device->SetTexture(0, NULL);
}

void CMoney_Player::Render_Font()
{
	Font_Rect_Update();
	TCHAR szText[64];
	m_vTexRect.left -= 400;
	m_vTexRect.right -= m_fSizeX;
	_stprintf_s(szText, TEXT("%d"), m_iValue);
	m_pGameInstance->Render_Font(TEXT("UI_Font_18"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
}

CMoney_Player* CMoney_Player::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMoney_Player* pInstance = new CMoney_Player(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CMoney_Player"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CMoney_Player::Clone(void* pArg)
{
	CMoney_Player* pInstance = new CMoney_Player(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CMoney_Player"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMoney_Player::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}

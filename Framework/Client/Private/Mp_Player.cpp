#include "Mp_Player.h"
#include "GameInstance.h"

CMp_Player::CMp_Player(LPDIRECT3DDEVICE9 pGraphic_Device) : CProgressBar(pGraphic_Device)
{
}

CMp_Player::CMp_Player(const CMp_Player& Prototype) : CProgressBar(Prototype)
{
}

HRESULT CMp_Player::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMp_Player::Initialize(void* pArg)
{
	m_iCulMaxValue = 200;
	m_iCulValue = 200;
	m_fSizeX = 180;
	m_fSizeY = 20;
	m_fX = 0;
	m_fY = 10;
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

void CMp_Player::Priority_Update(_float fTimeDelta)
{

}

void CMp_Player::Update(_float fTimeDelta)
{
}

void CMp_Player::Late_Update(_float fTimeDelta)
{
	Progress_UpdateX();
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CMp_Player::Render()
{
	m_pVIBufferCom->Bind_Buffers();
	__super::Begin();
	m_pVIBufferCom->Render();
	Render_Font();
	__super::End();

	return S_OK;
}


HRESULT CMp_Player::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_Texture_Rect_UI_Hud_States_Mp"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	return S_OK;
}

void CMp_Player::Render_Font()
{
	Font_Rect_Update();

	TCHAR szText[64];
	_stprintf_s(szText, TEXT("%d/%d"), m_iCulValue, m_iCulMaxValue);

	m_vTexRect.left += 2;
	m_vTexRect.top += 2;
	m_vTexRect.right += 2;
	m_vTexRect.bottom += 2;
	m_pGameInstance->Render_Font(TEXT("UI_Font_14"), szText, m_vTexRect, D3DXCOLOR(0.f, 0.f, 0.f, 1.f), DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	m_vTexRect.left -= 2;
	m_vTexRect.top -= 2;
	m_vTexRect.right -= 2;
	m_vTexRect.bottom -= 2;
	m_pGameInstance->Render_Font(TEXT("UI_Font_14"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), DT_CENTER | DT_VCENTER | DT_SINGLELINE);

}

CMp_Player* CMp_Player::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMp_Player* pInstance = new CMp_Player(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CMp_Player"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CMp_Player::Clone(void* pArg)
{
	CMp_Player* pInstance = new CMp_Player(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CMp_Player"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMp_Player::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
}

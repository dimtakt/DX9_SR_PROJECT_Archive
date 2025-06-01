#include "Hp_Player.h"
#include "GameInstance.h"
#include "Client_Defines_Event.h"

CHp_Player::CHp_Player(LPDIRECT3DDEVICE9 pGraphic_Device) : CProgressBar( pGraphic_Device )
{
}

CHp_Player::CHp_Player(const CHp_Player& Prototype) : CProgressBar( Prototype )
{
}

HRESULT CHp_Player::Initialize_Prototype()
{
 	return S_OK;
}

HRESULT CHp_Player::Initialize(void* pArg)
{
	m_iCulMaxValue = 200;
	m_iCulValue = 200;

	m_fSizeX = 180;
	m_fSizeY = 20;
	m_fX = 0;
	m_fY = -15;
	m_fZ = UI_DEPTH::HUD_STATUS;
	m_iWinSizeX = g_iWinSizeX;
	m_iWinSizeY = g_iWinSizeY;

	if (FAILED(CUIObject::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
	__super::Update_Position();

	m_pGameInstance->Subscribe(ENUM_CLASS(EVENT_TYPE::PLAYERTSTATCHANGE), this);

	return S_OK;
}

void CHp_Player::Priority_Update(_float fTimeDelta)
{

}

void CHp_Player::Update(_float fTimeDelta)
{
	
}

void CHp_Player::Late_Update(_float fTimeDelta)
{ 

	Progress_UpdateX();
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CHp_Player::Render()
{
	m_pVIBufferCom->Bind_Buffers();
	__super::Begin();
	m_pVIBufferCom->Render();
	Render_Font();
	__super::End();
	
	return S_OK;
}


HRESULT CHp_Player::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Rect_UI_Hud_States_Hp"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	return S_OK;
}

void CHp_Player::Render_Font()
{

	TCHAR szText[64];
	_stprintf_s(szText, TEXT("%d/%d"), m_iCulValue, m_iCulMaxValue);
	
	Font_Rect_Update();

	m_vTexRect.left += 2;
	m_vTexRect.top += 2;
	m_vTexRect.right += 2;
	m_vTexRect.bottom += 2;
	m_pGameInstance->Render_Font(TEXT("UI_Font_18"), szText, m_vTexRect, D3DXCOLOR(0.f, 0.f, 0.f, 1.f), DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	m_vTexRect.left -= 2;
	m_vTexRect.top -= 2;
	m_vTexRect.right -= 2;
	m_vTexRect.bottom -= 2;
	m_pGameInstance->Render_Font(TEXT("UI_Font_18"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), DT_CENTER | DT_VCENTER | DT_SINGLELINE);

}

void CHp_Player::OnEvent(_uint iTypeindex, const EVENTDATA* pData)
{
	if (static_cast<EVENT_TYPE>(iTypeindex) == EVENT_TYPE::PLAYERTSTATCHANGE) {
		auto pStat = static_cast<const STATCHANGE*>(pData);
		if (pStat->eStatType == STAT_INFO::CULHP) {
			m_iCulValue = pStat->fValue;
		}
	}
}


CHp_Player* CHp_Player::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CHp_Player* pInstance = new CHp_Player(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CHp_Player"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CHp_Player::Clone(void* pArg)
{
	CHp_Player* pInstance = new CHp_Player(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CHp_Player"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CHp_Player::Free()
{
	m_pGameInstance->Unsubscribe(ENUM_CLASS(EVENT_TYPE::PLAYERTSTATCHANGE), this);
	__super::Free();
	Safe_Release(m_pVIBufferCom);
}
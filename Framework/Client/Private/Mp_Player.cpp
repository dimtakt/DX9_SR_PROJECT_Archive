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

	UIOBJECT_DESC Desc{};
	Desc.fSizeX = 180;
	Desc.fSizeY = 20;
	Desc.fX = 0;
	Desc.fY = 10;
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

void CMp_Player::Priority_Update(_float fTimeDelta)
{
	if (GetKeyState('C') < 0)
	{
		m_iCulValue -= 1;
	}
	if (GetKeyState('V') < 0)
	{
		m_iCulValue += 1;
	}
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
	m_pTransformCom->Bind_Matrix();
	m_pVIBufferCom->Bind_Buffers();
	__super::Begin();
	m_pVIBufferCom->Render();
	__super::End();

	return S_OK;
}


HRESULT CMp_Player::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Rect_UI_Hud_States_Mp"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	return S_OK;
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

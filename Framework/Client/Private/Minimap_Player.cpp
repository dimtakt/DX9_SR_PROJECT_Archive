#include "Minimap_Player.h"
#include "GameInstance.h"

CMinimap_Player::CMinimap_Player(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CMinimap_Player::CMinimap_Player(const CMinimap_Player& Prototype) : CUIObject(Prototype)
{
}

HRESULT CMinimap_Player::Initialize_Prototype(LEVEL eLevel)
{
	m_eLEVEL = eLevel;

	return S_OK;
}

HRESULT CMinimap_Player::Initialize(void* pArg)
{
	m_fSizeX = 64;
	m_fSizeY = 64;
	m_fX = 0;
	m_fY = 0;
	m_fZ = UI_DEPTH::MINIMAP_BUTTON;
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

void CMinimap_Player::Priority_Update(_float fTimeDelta)
{
}

void CMinimap_Player::Update(_float fTimeDelta)
{
}

void CMinimap_Player::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CMinimap_Player::Render()
{
	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;
	m_pVIBufferCom->Bind_Buffers();
	__super::Begin();
	m_pVIBufferCom->Render();
	__super::End();

	return S_OK;
}

HRESULT CMinimap_Player::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Rect_Arrow"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

CMinimap_Player* CMinimap_Player::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
	CMinimap_Player* pInstance = new CMinimap_Player(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eLevel)))
	{
		MSG_BOX(TEXT("Failed to Created : CMinimap_Player"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CMinimap_Player::Clone(void* pArg)
{
	CMinimap_Player* pInstance = new CMinimap_Player(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CMinimap_Player"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMinimap_Player::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}

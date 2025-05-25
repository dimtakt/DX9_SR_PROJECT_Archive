#include "Hud_Wallet.h"
#include "GameInstance.h"
#include "Money_Player.h"
#include "Dice_Player.h"

CHud_Wallet::CHud_Wallet(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CHud_Wallet::CHud_Wallet(const CHud_Wallet& Prototype) : CUIObject(Prototype), m_eLevel{Prototype.m_eLevel}
{
}

HRESULT CHud_Wallet::Initialize_Prototype(LEVEL eLevel)
{
	m_eLevel = eLevel;

	if (FAILED(Ready_ChildPrototype(eLevel)))
		return E_FAIL;

	return S_OK;
}

HRESULT CHud_Wallet::Initialize(void* pArg)
{
	UIOBJECT_DESC Desc{};

	Desc.fSizeX = 0;
	Desc.fSizeY = 0;
	Desc.fX = g_iWinSizeX;
	Desc.fY = g_iWinSizeY - 32.f;;
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

void CHud_Wallet::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CHud_Wallet::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CHud_Wallet::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CHud_Wallet::Render()
{
	return S_OK;
}

HRESULT CHud_Wallet::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CHud_Wallet::Ready_ChildPrototype(LEVEL eLevel)
{
 	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_Wallet_Money"),
		CMoney_Player::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_Wallet_Dice"),
		CDice_Player::Create(m_pGraphic_Device))))
		return E_FAIL;

	return S_OK;
}

HRESULT CHud_Wallet::Ready_Children()
{
	CUIObject* pGameObject = nullptr;

	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Wallet_Money")));
	if (nullptr == pGameObject)
		return E_FAIL;
	Add_Child(pGameObject);

	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Wallet_Dice")));
	if (nullptr == pGameObject)
		return E_FAIL;
	Add_Child(pGameObject);

	return S_OK;
}

CHud_Wallet* CHud_Wallet::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
	CHud_Wallet* pInstance = new CHud_Wallet(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eLevel)))
	{
		MSG_BOX(TEXT("Failed to Create : CHud_Wallet"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CHud_Wallet::Clone(void* pArg)
{
	CHud_Wallet* pInstance = new CHud_Wallet(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CHud_Wallet"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CHud_Wallet::Free()
{
	__super::Free();
}

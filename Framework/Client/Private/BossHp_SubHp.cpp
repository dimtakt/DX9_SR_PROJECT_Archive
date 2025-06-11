#include "BossHp_SubHp.h"
#include "GameInstance.h"

CBossHp_SubHp::CBossHp_SubHp(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CBossHp_SubHp::CBossHp_SubHp(const CBossHp_SubHp& Prototype) : CUIObject(Prototype), m_eLevel(Prototype.m_eLevel)
{
}

HRESULT CBossHp_SubHp::Initialize_Prototype(LEVEL eLevel)
{
	m_eLevel = eLevel;

	if (FAILED(Ready_ChildPrototype(eLevel)))
		return E_FAIL;

	return S_OK;
}

HRESULT CBossHp_SubHp::Initialize(void* pArg)
{
	UIOBJECT_DESC* Desc = static_cast<UIOBJECT_DESC*>(pArg);

	m_fSizeX = 78;
	m_fSizeY = 25;
	m_fX = 0;
	m_fY = -30;
	m_fZ = UI_DEPTH::BOSSHP;
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

void CBossHp_SubHp::Priority_Update(_float fTimeDelta)
{
	if (m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_LOADING) || m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_LOGO) || m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_MAPEDIT))
		return;

	if (!m_bIsUpdate)
		return;

	__super::Priority_Update(fTimeDelta);
}

void CBossHp_SubHp::Update(_float fTimeDelta)
{
	if (m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_LOADING) || m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_LOGO) || m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_MAPEDIT))
		return;

	if (!m_bIsUpdate)
		return;

	__super::Update(fTimeDelta);
}

void CBossHp_SubHp::Late_Update(_float fTimeDelta)
{
	if (m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_LOADING) || m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_LOGO) || m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_MAPEDIT))
		return;

	if (!m_bIsUpdate)
		return;

	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);

	__super::Late_Update(fTimeDelta);
}

HRESULT CBossHp_SubHp::Render()
{
	m_pGraphic_Device->SetTexture(0, NULL);

	m_pVIBufferCom->Bind_Buffers();
	__super::Begin();
	m_pVIBufferCom->Render();
	__super::End();

	return S_OK;
}

HRESULT CBossHp_SubHp::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Rect_UI_BalckRect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CBossHp_SubHp::Ready_ChildPrototype(LEVEL eLevel)
{
	//if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_BossHp_Ema_Frame"),
	//	CEma_Frame::Create(m_pGraphic_Device))))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CBossHp_SubHp::Ready_Children()
{
	CUIObject* pGameObject = nullptr;

	//pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_BossHp_Ema_Frame")));
	//if (nullptr == pGameObject)
	//	return E_FAIL;
	//Add_Child(pGameObject);

	return S_OK;
}


CBossHp_SubHp* CBossHp_SubHp::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
	CBossHp_SubHp* pInstance = new CBossHp_SubHp(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eLevel)))
	{
		MSG_BOX(TEXT("Failed to Created : CBossHp_SubHp"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CBossHp_SubHp::Clone(void* pArg)
{
	CBossHp_SubHp* pInstance = new CBossHp_SubHp(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CBossHp_SubHp"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBossHp_SubHp::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
}

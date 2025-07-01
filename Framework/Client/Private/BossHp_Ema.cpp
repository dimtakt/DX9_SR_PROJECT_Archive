#include "BossHp_Ema.h"
#include "GameInstance.h"
#include "Ema_Frame.h"
#include "BossHp_SubHp.h"
#include "Ema_SubHpbar.h"
#include "Ema_Hpbar.h"
CBossHp_Ema::CBossHp_Ema(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CBossHp_Ema::CBossHp_Ema(const CBossHp_Ema& Prototype) : CUIObject(Prototype), m_eLevel(Prototype.m_eLevel)
{
}

void CBossHp_Ema::Render_Hpbar(_int iCulTopHp, _int iCulTopMaxHp, _int iCulBottomHp, _int iCulBottomMaxHp, _float fTimeDelta)
{
	if (!m_bIsUpdate)
		return;
	
	static_cast<CEma_SubHpbar*>(m_vecChildren[1])->HpBar_Set(iCulTopHp, iCulTopMaxHp);

	static_cast<CEma_Hpbar*>(m_vecChildren[2])->HpBar_Set(iCulBottomHp, iCulBottomMaxHp);

	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
	__super::Late_Update(fTimeDelta);
}

HRESULT CBossHp_Ema::Initialize_Prototype(LEVEL eLevel)
{
	m_eLevel = eLevel;

	if (FAILED(Ready_ChildPrototype(eLevel)))
		return E_FAIL;

	return S_OK;
}

HRESULT CBossHp_Ema::Initialize(void* pArg)
{
	UIOBJECT_DESC* Desc = static_cast<UIOBJECT_DESC*>(pArg);
	m_fSizeX = 610;
	m_fSizeY = 45;
	m_fX = g_iWinSizeX * 0.5f;
	m_fY = g_iWinSizeY - 130;
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

	m_pGameInstance->Add_UIObject(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("BossHp_Ema"), this);

	return S_OK;
}

void CBossHp_Ema::Priority_Update(_float fTimeDelta)
{

}

void CBossHp_Ema::Update(_float fTimeDelta)
{

}

void CBossHp_Ema::Late_Update(_float fTimeDelta)
{
}

HRESULT CBossHp_Ema::Render()
{
	m_pGraphic_Device->SetTexture(0, NULL);

	m_pVIBufferCom->Bind_Buffers();
	__super::Begin();
	m_pVIBufferCom->Render();
	__super::End();

	return S_OK;
}

HRESULT CBossHp_Ema::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Rect_UI_BalckRect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CBossHp_Ema::Ready_ChildPrototype(LEVEL eLevel)
{
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_BossHp_Ema_SubHpFrame"),
		CBossHp_SubHp::Create(m_pGraphic_Device,m_eLevel))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_BossHp_Ema_SubHpBar"),
		CEma_SubHpbar::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_BossHp_Ema_HpBar"),
		CEma_Hpbar::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_BossHp_Ema_Frame"),
		CEma_Frame::Create(m_pGraphic_Device))))
		return E_FAIL;

	return S_OK;
}

HRESULT CBossHp_Ema::Ready_Children()
{
	CUIObject* pGameObject = nullptr;
	
	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_BossHp_Ema_SubHpFrame")));
	if (nullptr == pGameObject)
		return E_FAIL;
	Add_Child(pGameObject);

	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_BossHp_Ema_SubHpBar")));
	if (nullptr == pGameObject)
		return E_FAIL;
	Add_Child(pGameObject);

	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_BossHp_Ema_HpBar")));
	if (nullptr == pGameObject)
		return E_FAIL;
	Add_Child(pGameObject);

	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_BossHp_Ema_Frame")));
	if (nullptr == pGameObject)
		return E_FAIL;
	Add_Child(pGameObject);

	return S_OK;
}


CBossHp_Ema* CBossHp_Ema::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
	CBossHp_Ema* pInstance = new CBossHp_Ema(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eLevel)))
	{
		MSG_BOX(TEXT("Failed to Created : CBossHp_Ema"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CBossHp_Ema::Clone(void* pArg)
{
	CBossHp_Ema* pInstance = new CBossHp_Ema(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CBossHp_Ema"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBossHp_Ema::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
}

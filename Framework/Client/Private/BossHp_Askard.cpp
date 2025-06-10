#include "BossHp_Askard.h"
#include "GameInstance.h"
#include "Askard_Frame.h"

CBossHp_Askard::CBossHp_Askard(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CBossHp_Askard::CBossHp_Askard(const CBossHp_Askard& Prototype) : CUIObject(Prototype), m_eLevel(Prototype.m_eLevel)
{
}

HRESULT CBossHp_Askard::Initialize_Prototype(LEVEL eLevel)
{
	m_eLevel = eLevel;

	if (FAILED(Ready_ChildPrototype(eLevel)))
		return E_FAIL;

	return S_OK;
}

HRESULT CBossHp_Askard::Initialize(void* pArg)
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

	return S_OK;
}

void CBossHp_Askard::Priority_Update(_float fTimeDelta)
{
	if (!m_bIsUpdate)
		return;

	__super::Priority_Update(fTimeDelta);
}

void CBossHp_Askard::Update(_float fTimeDelta)
{
	if (!m_bIsUpdate)
		return;

	__super::Update(fTimeDelta);
}

void CBossHp_Askard::Late_Update(_float fTimeDelta)
{
	if (!m_bIsUpdate)
		return;

	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);

	__super::Late_Update(fTimeDelta);
}

HRESULT CBossHp_Askard::Render()
{
	m_pGraphic_Device->SetTexture(0, NULL);

	m_pVIBufferCom->Bind_Buffers();
	__super::Begin();
	m_pVIBufferCom->Render();
	__super::End();

	return S_OK;
}

HRESULT CBossHp_Askard::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Rect_UI_BalckRect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CBossHp_Askard::Ready_ChildPrototype(LEVEL eLevel)
{
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_BossHp_Askard_Frame"),
		CAskard_Frame::Create(m_pGraphic_Device))))
		return E_FAIL;

	return S_OK;
}

HRESULT CBossHp_Askard::Ready_Children()
{
	CUIObject* pGameObject = nullptr;

	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_BossHp_Askard_Frame")));
	if (nullptr == pGameObject)
		return E_FAIL;
	Add_Child(pGameObject);

	return S_OK;
}


CBossHp_Askard* CBossHp_Askard::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
	CBossHp_Askard* pInstance = new CBossHp_Askard(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eLevel)))
	{
		MSG_BOX(TEXT("Failed to Created : CBossHp_Askard"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CBossHp_Askard::Clone(void* pArg)
{
	CBossHp_Askard* pInstance = new CBossHp_Askard(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CBossHp_Askard"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBossHp_Askard::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
}

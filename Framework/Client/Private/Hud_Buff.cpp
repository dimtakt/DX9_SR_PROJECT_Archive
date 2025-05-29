#include "Hud_Buff.h"
#include "GameInstance.h"
#include "Buff_Type_Base.h"

CHud_Buff::CHud_Buff(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CHud_Buff::CHud_Buff(const CHud_Buff& Prototype) : CUIObject(Prototype), m_eLevel(Prototype.m_eLevel)
{
}

HRESULT CHud_Buff::Initialize_Prototype(LEVEL eLevel)
{
	m_eLevel = eLevel;

	if (FAILED(Ready_ChildPrototype(eLevel)))
		return E_FAIL;

	return S_OK;
}

HRESULT CHud_Buff::Initialize(void* pArg)
{
	UIHUD_BUFF_DESC* DescMy = static_cast<UIHUD_BUFF_DESC*>(pArg);

	m_fSizeX = 0;
	m_fSizeY = 0;
	m_fX = DescMy->fX;
	m_fY = DescMy->fY;
	m_fZ = 0.f;
	m_iWinSizeX = g_iWinSizeX;
	m_iWinSizeY = g_iWinSizeY;

	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Reday_SyncingObject()))
		return E_FAIL;

	m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
	__super::Update_Position();

	if (FAILED(Ready_Children()))
		return E_FAIL;

	return S_OK;
}

void CHud_Buff::Priority_Update(_float fTimeDelta)
{
	//임시
	//플레이어가 가진 칸별 버프 인덱스주소를 넣어서 사용 예정
	m_vecChildren[2]->Priority_Update(fTimeDelta);
}

void CHud_Buff::Update(_float fTimeDelta)
{
	m_vecChildren[2]->Update(fTimeDelta);
}

void CHud_Buff::Late_Update(_float fTimeDelta)
{
	m_vecChildren[2]->Late_Update(fTimeDelta);
}

HRESULT CHud_Buff::Render()
{
	return S_OK;
}

HRESULT CHud_Buff::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;
	return S_OK;
}

HRESULT CHud_Buff::Ready_ChildPrototype(LEVEL eLevel)
{
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_Buff_Type_Base"),
		CBuff_Type_Base::Create(m_pGraphic_Device, eLevel))))
		return E_FAIL;

	return S_OK;
}

HRESULT CHud_Buff::Ready_Children()
{
	CUIObject* pGameObject = nullptr;
	CBuff_Type_Base::UIHUD_BUFF_TYPE_DESC Desc{};

	for (_int i = 0; i < 50; ++i)
	{
		Desc.iBuff_Index = i;
		Desc.iBuff_Type = i % 2;
		pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Buff_Type_Base"), &Desc));
		if (nullptr == pGameObject)
			return E_FAIL;
		Add_Child(pGameObject);
	}

	return S_OK;
}

HRESULT CHud_Buff::Reday_SyncingObject()
{
	m_pPlayerStatsCom = dynamic_cast<CPlayerStats*>(m_pGameInstance->Get_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Layer_Player"), TEXT("Com_PlayerStats"), 0));
	if (m_pPlayerStatsCom == nullptr)
	{
		MSG_BOX(TEXT("Failed to Syncing : CHud_Dash"));
		return E_FAIL;
	}
	Safe_AddRef(m_pPlayerStatsCom);

	return S_OK;
}

CHud_Buff* CHud_Buff::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
	CHud_Buff* pInstance = new CHud_Buff(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eLevel)))
	{
		MSG_BOX(TEXT("Failed to Create : CHud_Buff"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CHud_Buff::Clone(void* pArg)
{
	CHud_Buff* pInstance = new CHud_Buff(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CHud_Buff"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CHud_Buff::Free()
{
	__super::Free();

	Safe_Release(m_pPlayerStatsCom);
}

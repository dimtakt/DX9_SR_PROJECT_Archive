#include "Hud_States_Frame.h"
#include "GameInstance.h"
#include "Hp_Player.h"
#include "Mp_Player.h"
CHud_States_Frame::CHud_States_Frame(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CHud_States_Frame::CHud_States_Frame(const CHud_States_Frame& Prototype) : CUIObject(Prototype), m_eLevel(Prototype.m_eLevel)
{
}

HRESULT CHud_States_Frame::Initialize_Prototype(LEVEL eLevel)
{
	m_eLevel = eLevel;

	if(FAILED(Ready_ChildPrototype(eLevel)))
		return E_FAIL;

	return S_OK;
}

HRESULT CHud_States_Frame::Initialize(void* pArg)
{
	UIOBJECT_DESC Desc{};

	Desc.fSizeX = 190.f;
	Desc.fSizeY = 55.f;
	Desc.fX = 20 + Desc.fSizeX * 0.5f;
	Desc.fY = 20 + Desc.fSizeY * 0.5f;
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

void CHud_States_Frame::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CHud_States_Frame::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CHud_States_Frame::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);

	__super::Late_Update(fTimeDelta);
}

HRESULT CHud_States_Frame::Render()
{
	m_pGraphic_Device->SetTexture(0, NULL);

	m_pTransformCom->Bind_Matrix();
	m_pVIBufferCom->Bind_Buffers();
	__super::Begin();
	m_pVIBufferCom->Render();
	__super::End();

	return S_OK;
}

HRESULT CHud_States_Frame::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Rect_UI_BalckRect_Texture"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CHud_States_Frame::Ready_ChildPrototype(LEVEL eLevel)
{
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_Hp"),
		CHp_Player::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_Mp"),
		CMp_Player::Create(m_pGraphic_Device))))
		return E_FAIL;

	return S_OK;
}

HRESULT CHud_States_Frame::Ready_Children()
{
	CUIObject* pGameObject = nullptr;

	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Hp")));
	if (nullptr == pGameObject)
		return E_FAIL;
	Add_Child(pGameObject);

	pGameObject = nullptr;
	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Mp")));
	if (nullptr == pGameObject)
		return E_FAIL;
	Add_Child(pGameObject);

	return S_OK;
}


CHud_States_Frame* CHud_States_Frame::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
	CHud_States_Frame* pInstance = new CHud_States_Frame(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eLevel)))
	{
		MSG_BOX(TEXT("Failed to Created : CHud_States_Frame"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CHud_States_Frame::Clone(void* pArg)
{
	CHud_States_Frame* pInstance = new CHud_States_Frame(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CHud_States_Frame"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CHud_States_Frame::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
}

#include "Buff_Type_Base.h"
#include "GameInstance.h"
#include "Buff_Icon.h"
#include "Buff_Fream.h"

CBuff_Type_Base::CBuff_Type_Base(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CBuff_Type_Base::CBuff_Type_Base(const CBuff_Type_Base& Prototype) : CUIObject(Prototype), m_eLevel(Prototype.m_eLevel), m_iBuff_Type(Prototype.m_iBuff_Type), m_iBuff_Index(Prototype.m_iBuff_Index)
{
}

HRESULT CBuff_Type_Base::Initialize_Prototype(LEVEL eLevel)
{
	m_eLevel = eLevel;

	if (FAILED(Ready_ChildPrototype(eLevel)))
		return E_FAIL;

	return S_OK;
}

HRESULT CBuff_Type_Base::Initialize(void* pArg)
{
	UIHUD_BUFF_TYPE_DESC* DescMy = static_cast<UIHUD_BUFF_TYPE_DESC*>(pArg);

	m_iBuff_Type = DescMy->iBuff_Type;
	m_iBuff_Index = DescMy->iBuff_Index;

	UIOBJECT_DESC Desc{};
	Desc.fSizeX = 32;
	Desc.fSizeY = 32;
	Desc.fX = 0.f;
	Desc.fY = 0.f;
	Desc.iWinSizeX = g_iWinSizeX;
	Desc.iWinSizeY = g_iWinSizeY;

	if (FAILED(CUIObject::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
	__super::Update_Position();

	if (FAILED(Ready_Children()))
		return E_FAIL;

	return S_OK;
}

void CBuff_Type_Base::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CBuff_Type_Base::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CBuff_Type_Base::Late_Update(_float fTimeDelta)
{
	
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
	__super::Late_Update(fTimeDelta);
}

HRESULT CBuff_Type_Base::Render()
{
	SetUp_RenderState();

	m_pTransformCom->Bind_Matrix();
	if (FAILED(m_pTextureCom->Bind_Texture(m_iBuff_Type)))
		return E_FAIL;
	m_pVIBufferCom->Bind_Buffers();

	__super::Begin();
	m_pVIBufferCom->Render();
	__super::End();

	Reset_RenderState();
	return S_OK;
}

HRESULT CBuff_Type_Base::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_Texture_Rect_UI_Hud_Buff_Base"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

void CBuff_Type_Base::SetUp_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
}

void CBuff_Type_Base::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	m_pGraphic_Device->SetTexture(0, NULL);
}

HRESULT CBuff_Type_Base::Ready_ChildPrototype(LEVEL eLevel)
{
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_Buff_Fream"),
		CBuff_Fream::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_Buff_Icon"),
		CBuff_Icon::Create(m_pGraphic_Device))))
		return E_FAIL;

	return S_OK;
}

HRESULT CBuff_Type_Base::Ready_Children()
{
	CUIObject* pGameObject = nullptr;
	CBuff_Icon::UI_BUFF_ICON Desc{};

	//버프 프레임
	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Buff_Fream")));
	if (nullptr == pGameObject)
		return E_FAIL;
	Add_Child(pGameObject);

	//버프 아이콘
	Desc.iBuff_Index = m_iBuff_Index;
	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Buff_Icon"),&Desc));
	if (nullptr == pGameObject)
		return E_FAIL;
	Add_Child(pGameObject);

	return S_OK;
}

CBuff_Type_Base* CBuff_Type_Base::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
	CBuff_Type_Base* pInstance = new CBuff_Type_Base(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eLevel)))
	{
		MSG_BOX(TEXT("Failed to Created : CBuff_Type_Base"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CBuff_Type_Base::Clone(void* pArg)
{
	CBuff_Type_Base* pInstance = new CBuff_Type_Base(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CBuff_Type_Base"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBuff_Type_Base::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}

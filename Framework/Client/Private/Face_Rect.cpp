#include "Face_Rect.h"
#include "GameInstance.h"
CFace_Rect::CFace_Rect(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CFace_Rect::CFace_Rect(const CFace_Rect& Prototype) : CUIObject(Prototype), m_eLevel(Prototype.m_eLevel)
{
}

HRESULT CFace_Rect::Initialize_Prototype(LEVEL eLevel)
{
	m_eLevel = eLevel;

	if (FAILED(Ready_ChildPrototype(eLevel)))
		return E_FAIL;

	return S_OK;
}

HRESULT CFace_Rect::Initialize(void* pArg)
{
	UIOBJECT_DESC* Desc = static_cast<UIOBJECT_DESC*>(pArg);
	if(Desc->fZ == 0)
		m_fY = -400 + g_iWinSizeY * 0.5f;
	else
		m_fY = 400 + g_iWinSizeY * 0.5f;

	m_fSizeX = g_iWinSizeX;
	m_fSizeY = g_iWinSizeY * 0.5;
	m_fX = g_iWinSizeX * 0.5f;
	m_fZ = UI_DEPTH::FILED_CHAT;
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

void CFace_Rect::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CFace_Rect::Update(_float fTimeDelta)
{

	__super::Update(fTimeDelta);
}

void CFace_Rect::Late_Update(_float fTimeDelta)
{
	if (m_pParent != nullptr)
	{
		m_pParent = nullptr;
		__super::Update_Position();
	}
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
	__super::Late_Update(fTimeDelta);
}

HRESULT CFace_Rect::Render()
{
	m_pGraphic_Device->SetTexture(0, NULL);
	m_pVIBufferCom->Bind_Buffers();
	__super::Begin();
	m_pVIBufferCom->Render();
	__super::End();
	return S_OK;
}

HRESULT CFace_Rect::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Rect_UI_BalckRect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CFace_Rect::Ready_ChildPrototype(LEVEL eLevel)
{

	return S_OK;
}

HRESULT CFace_Rect::Ready_Children()
{
	CUIObject* pGameObject = nullptr;
	return S_OK;
}

CFace_Rect* CFace_Rect::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
	CFace_Rect* pInstance = new CFace_Rect(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eLevel)))
	{
		MSG_BOX(TEXT("Failed to Created : CFace_Rect"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CFace_Rect::Clone(void* pArg)
{
	CFace_Rect* pInstance = new CFace_Rect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CFace_Rect"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CFace_Rect::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
}

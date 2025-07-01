#include "Status_Stat_Frame_Type.h"
#include "GameInstance.h"
CStatus_Stat_Frame_Type::CStatus_Stat_Frame_Type(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CStatus_Stat_Frame_Type::CStatus_Stat_Frame_Type(const CStatus_Stat_Frame_Type& Prototype) : CUIObject(Prototype), m_eLevel{ Prototype.m_eLevel }
{
}

HRESULT CStatus_Stat_Frame_Type::Initialize_Prototype(LEVEL eLevel)
{
	m_eLevel = eLevel;

	if (FAILED(Ready_ChildPrototype(eLevel)))
		return E_FAIL;

	return S_OK;
}

HRESULT CStatus_Stat_Frame_Type::Initialize(void* pArg)
{

	UIOBJECT_DESC* Desc = static_cast<UIOBJECT_DESC*>(pArg);

	m_fSizeX = 365;
	m_fSizeY = Desc->fSizeY;
	m_fX = 0;
	m_fY = Desc->fY;
	m_fZ = UI_DEPTH::PLAYER_STAUTS;
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

void CStatus_Stat_Frame_Type::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CStatus_Stat_Frame_Type::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CStatus_Stat_Frame_Type::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
	__super::Late_Update(fTimeDelta);
}

HRESULT CStatus_Stat_Frame_Type::Render()
{
	if (FAILED(m_pTextureCom->Bind_Texture(6)))
		return E_FAIL;
	m_pVIBufferCom->Bind_Buffers();

	__super::Begin();
	m_pVIBufferCom->Render();
	__super::End();
	return S_OK;
}

HRESULT CStatus_Stat_Frame_Type::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Rect_Status_Window_Frame"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CStatus_Stat_Frame_Type::Ready_ChildPrototype(LEVEL eLevel)
{
	return S_OK;
}

HRESULT CStatus_Stat_Frame_Type::Ready_Children()
{
	return S_OK;
}

CStatus_Stat_Frame_Type* CStatus_Stat_Frame_Type::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
	CStatus_Stat_Frame_Type* pInstance = new CStatus_Stat_Frame_Type(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eLevel)))
	{
		MSG_BOX(TEXT("Failed to Create : CStatus_Stat_Frame_Type"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CStatus_Stat_Frame_Type::Clone(void* pArg)
{
	CStatus_Stat_Frame_Type* pInstance = new CStatus_Stat_Frame_Type(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CStatus_Stat_Frame_Type"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CStatus_Stat_Frame_Type::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}

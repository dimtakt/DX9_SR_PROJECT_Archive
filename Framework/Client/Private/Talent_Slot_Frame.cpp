#include "Talent_Slot_Frame.h"
#include "GameInstance.h"
#include "Stat_Manager.h"
CTalent_Slot_Frame::CTalent_Slot_Frame(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CTalent_Slot_Frame::CTalent_Slot_Frame(const CTalent_Slot_Frame& Prototype) : CUIObject(Prototype)
{
}

HRESULT CTalent_Slot_Frame::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTalent_Slot_Frame::Initialize(void* pArg)
{
	UIOBJECT_DESC* Desc = static_cast<UIOBJECT_DESC*>(pArg);

	m_iIndex = Desc->fZ;

	m_fSizeX = 390;
	m_fSizeY = 390;
	m_fX = 0;
	m_fY = -220;
	m_fZ = UI_DEPTH::TALENT;
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

void CTalent_Slot_Frame::Priority_Update(_float fTimeDelta)
{
}

void CTalent_Slot_Frame::Update(_float fTimeDelta)
{
}

void CTalent_Slot_Frame::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CTalent_Slot_Frame::Render()
{
	if (FAILED(m_pTextureCom->Bind_Texture(m_iIndex)))
		return E_FAIL;
	m_pVIBufferCom->Bind_Buffers();

	__super::Begin();
	m_pVIBufferCom->Render();
	return S_OK;
}

HRESULT CTalent_Slot_Frame::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Rect_Talent_Slot_Frame"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

CTalent_Slot_Frame* CTalent_Slot_Frame::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTalent_Slot_Frame* pInstance = new CTalent_Slot_Frame(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CTalent_Slot_Frame"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CTalent_Slot_Frame::Clone(void* pArg)
{
	CTalent_Slot_Frame* pInstance = new CTalent_Slot_Frame(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CTalent_Slot_Frame"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTalent_Slot_Frame::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}

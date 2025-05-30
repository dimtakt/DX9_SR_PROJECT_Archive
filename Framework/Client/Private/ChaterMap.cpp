#include "ChaterMap.h"
#include "GameInstance.h"
CChaterMap::CChaterMap(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CChaterMap::CChaterMap(const CChaterMap& Prototype) : CUIObject(Prototype), m_eLevel{ Prototype.m_eLevel }
{
}

HRESULT CChaterMap::Initialize_Prototype(LEVEL eLevel)
{
	m_eLevel = eLevel;

	if (FAILED(Ready_ChildPrototype(eLevel)))
		return E_FAIL;

	return S_OK;
}

HRESULT CChaterMap::Initialize(void* pArg)
{
	m_fSizeX = 0;
	m_fSizeY = 0;
	m_fX = g_iWinSizeX;
	m_fY = g_iWinSizeY - 32.f;;
	m_fZ = 0.f;
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

void CChaterMap::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CChaterMap::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CChaterMap::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CChaterMap::Render()
{
	return S_OK;
}

HRESULT CChaterMap::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_Texture_Rect_UI_Hud_Inven_Symbol"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CChaterMap::Ready_ChildPrototype(LEVEL eLevel)
{
	return S_OK;
}

HRESULT CChaterMap::Ready_Children()
{
	return S_OK;
}

CChaterMap* CChaterMap::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
	CChaterMap* pInstance = new CChaterMap(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eLevel)))
	{
		MSG_BOX(TEXT("Failed to Create : CChaterMap"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CChaterMap::Clone(void* pArg)
{
	CChaterMap* pInstance = new CChaterMap(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CChaterMap"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CChaterMap::Free()
{
	__super::Free();
}

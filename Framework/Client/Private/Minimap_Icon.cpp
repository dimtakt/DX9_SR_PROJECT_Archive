#include "Minimap_Icon.h"
#include "GameInstance.h"

CMinimap_Icon::CMinimap_Icon(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CMinimap_Icon::CMinimap_Icon(const CMinimap_Icon& Prototype) : CUIObject(Prototype)
{
}

HRESULT CMinimap_Icon::Initialize_Prototype(LEVEL eLevel)
{
	m_eLEVEL = eLevel;

	return S_OK;
}

HRESULT CMinimap_Icon::Initialize(void* pArg)
{
	UIOBJECT_DESC* Desc = static_cast<UIOBJECT_DESC*>(pArg);
	m_fSpeed = m_pGameInstance->Rand(3, 6);
	m_iValue = Desc->fZ;
	m_fSizeX = 64;
	m_fSizeY = 64;
	m_fX = 0;
	m_fY = 0;
	m_fZ = UI_DEPTH::MINIMAP_BUTTON;
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

void CMinimap_Icon::Priority_Update(_float fTimeDelta)
{
	m_fFrame += fTimeDelta * m_fSpeed;

	if (m_fFrame > 1)
	{
		m_fFrame = 0;
		m_iTexIndex = m_pGameInstance->Rand(0, 3);
	}
}

void CMinimap_Icon::Update(_float fTimeDelta)
{
}

void CMinimap_Icon::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CMinimap_Icon::Render()
{
	if (FAILED(m_pTextureCom->Bind_Texture(m_iTexIndex)))
		return E_FAIL;
	m_pVIBufferCom->Bind_Buffers();
	__super::Begin();
	m_pVIBufferCom->Render();
	__super::End();

	return S_OK;
}

HRESULT CMinimap_Icon::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), Value_Text(),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

_wstring CMinimap_Icon::Value_Text()
{
	// 	enum class ROOM_INFO { EVENT_NORMAL,  EVENT_SHOP, EVENT_HP, EVENT_EXP,  EVENT_ARTEFACT, EVENT_STONE, EVENT_ENDPOTAL, EVENT_END };

	switch (m_iValue)
	{
	case 99:
		return TEXT("Prototype_Component_Texture_Rect_Minimap_Icon_UpStair");
	case 0:
		return TEXT("Prototype_Component_Texture_Rect_Enemy");
	case 1:
		return TEXT("Prototype_Component_Texture_Rect_Shop");
	case 2:
		return TEXT("Prototype_Component_Texture_Rect_Hp");
	case 3:
		return TEXT("Prototype_Component_Texture_Rect_Luck");
	case 4:
		return TEXT("Prototype_Component_Texture_Rect_Sephirite");
	case 5:
		return TEXT("Prototype_Component_Texture_Rect_Sephirite");
	case 6:
		return TEXT("Prototype_Component_Texture_Rect_Minimap_Icon_DownStair");
	}
}

CMinimap_Icon* CMinimap_Icon::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
	CMinimap_Icon* pInstance = new CMinimap_Icon(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eLevel)))
	{
		MSG_BOX(TEXT("Failed to Created : CMinimap_Icon"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CMinimap_Icon::Clone(void* pArg)
{
	CMinimap_Icon* pInstance = new CMinimap_Icon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CMinimap_Icon"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMinimap_Icon::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}

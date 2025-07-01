#include "Minimap_Node.h"
#include "GameInstance.h"

CMinimap_Node::CMinimap_Node(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CMinimap_Node::CMinimap_Node(const CMinimap_Node& Prototype) : CUIObject(Prototype), m_eLevel(Prototype.m_eLevel)
{
}

HRESULT CMinimap_Node::Initialize_Prototype(LEVEL eLevel)
{
	m_eLevel = eLevel;

	return S_OK;
}

HRESULT CMinimap_Node::Initialize(void* pArg)
{
	UIOBJECT_DESC* Desc = static_cast<UIOBJECT_DESC*>(pArg);

	m_fX = Desc->fX;
	m_fY = Desc->fY;

	switch (int(Desc->fZ))
	{
	case 0:
		m_fSizeX = 32;
		m_fSizeY = 3;
		m_fX -= 32;
		m_fY = 0;
		break;
	case 1:
		m_fSizeX = 32;
		m_fSizeY = 3;
		m_fX += 32;
		m_fY = 0;
		break;
	case 2:
		m_fSizeX = 3;
		m_fSizeY = 32;
		m_fX = 0;
		m_fY -= 32;
		break;
	case 3:
		m_fSizeX = 3;
		m_fSizeY = 32;
		m_fX = 0;
		m_fY += 32;
		break;

	}

	m_fZ = UI_DEPTH::MiniMap;
	m_iWinSizeX = g_iWinSizeX;
	m_iWinSizeY = g_iWinSizeY;

	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
	__super::Update_Position();

	return S_OK;
}

void CMinimap_Node::Priority_Update(_float fTimeDelta)
{
}

void CMinimap_Node::Update(_float fTimeDelta)
{
}

void CMinimap_Node::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CMinimap_Node::Render()
{
	m_pGraphic_Device->SetTexture(0, NULL);
	m_pVIBufferCom->Bind_Buffers();
	__super::Begin();
	m_pVIBufferCom->Render();
	__super::End();
	return S_OK;
}

HRESULT CMinimap_Node::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Rect_UI_BrownRect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	return S_OK;
}

CMinimap_Node* CMinimap_Node::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
	CMinimap_Node* pInstance = new CMinimap_Node(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eLevel)))
	{
		MSG_BOX(TEXT("Failed to Created : CMinimap_Node"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CMinimap_Node::Clone(void* pArg)
{
	CMinimap_Node* pInstance = new CMinimap_Node(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CMinimap_Node"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMinimap_Node::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
}

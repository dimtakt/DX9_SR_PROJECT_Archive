#include "Minimap_Frame.h"
#include "GameInstance.h"

CMinimap_Frame::CMinimap_Frame(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CMinimap_Frame::CMinimap_Frame(const CMinimap_Frame& Prototype) : CUIObject(Prototype)
{
}

HRESULT CMinimap_Frame::Initialize_Prototype(LEVEL eLevel)
{
	m_eLEVEL = eLevel;

	return S_OK;
}

HRESULT CMinimap_Frame::Initialize(void* pArg)
{
	m_fSizeX = 1500;
	m_fSizeY = 1700;
	m_fX = 0;
	m_fY = 0;
	m_fZ = UI_DEPTH::MiniMap;
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

void CMinimap_Frame::Priority_Update(_float fTimeDelta)
{
}

void CMinimap_Frame::Update(_float fTimeDelta)
{
}

void CMinimap_Frame::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CMinimap_Frame::Render()
{
	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;
	m_pVIBufferCom->Bind_Buffers();
	__super::Begin();
	m_pVIBufferCom->Render();
	__super::End();

	return S_OK;
}

HRESULT CMinimap_Frame::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Rect_Minimap"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

CMinimap_Frame* CMinimap_Frame::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
	CMinimap_Frame* pInstance = new CMinimap_Frame(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eLevel)))
	{
		MSG_BOX(TEXT("Failed to Created : CMinimap_Frame"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CMinimap_Frame::Clone(void* pArg)
{
	CMinimap_Frame* pInstance = new CMinimap_Frame(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CMinimap_Frame"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMinimap_Frame::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}

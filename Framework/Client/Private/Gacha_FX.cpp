#include "Gacha_FX.h"
#include "GameInstance.h"

CGacha_FX::CGacha_FX(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CGacha_FX::CGacha_FX(const CGacha_FX& Prototype) : CUIObject(Prototype)
{
}

HRESULT CGacha_FX::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CGacha_FX::Initialize(void* pArg)
{
	UIOBJECT_DESC* Desc = static_cast<UIOBJECT_DESC*>(pArg);
	
	m_iTexIndex = Desc->fZ;

	m_fSizeX = 64;
	m_fSizeY = 64;
	m_fX = Desc->fX;
	m_fY = 100 + g_iWinSizeY * 0.5 + Desc->fY;
	m_fZ = UI_DEPTH::GACHA;
	m_iWinSizeX = g_iWinSizeX;
	m_iWinSizeY = g_iWinSizeY;
	m_fRange = m_pGameInstance->Rand(15, 20);
	m_fSpeed = m_pGameInstance->Rand(0.5, 1);
	if (FAILED(CUIObject::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
	__super::Update_Position();

	return S_OK;
}

void CGacha_FX::Priority_Update(_float fTimeDelta)
{
}

void CGacha_FX::Update(_float fTimeDelta)
{

}

void CGacha_FX::Late_Update(_float fTimeDelta)
{
	m_fAccTime += fTimeDelta;
	_float Offset = sinf(m_fAccTime * m_fSpeed) * m_fRange;
	m_vWorldPos.y = m_fY + Offset;
	m_pTransformCom->Set_State(STATE::POSITION, _float3{ m_vWorldPos.x - m_iWinSizeX * 0.5f, -m_vWorldPos.y + m_iWinSizeY * 0.5f, m_vWorldPos.z });
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CGacha_FX::Render()
{
	if (FAILED(m_pTextureCom->Bind_Texture(m_iTexIndex)))
		return E_FAIL;
	m_pVIBufferCom->Bind_Buffers();
	__super::Begin();
	m_pVIBufferCom->Render();
	__super::End();

	return S_OK;
}

HRESULT CGacha_FX::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Rect_GachaFx"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

CGacha_FX* CGacha_FX::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CGacha_FX* pInstance = new CGacha_FX(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CGacha_FX"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CGacha_FX::Clone(void* pArg)
{
	CGacha_FX* pInstance = new CGacha_FX(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CGacha_FX"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CGacha_FX::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}

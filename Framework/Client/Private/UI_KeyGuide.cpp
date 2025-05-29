#include "UI_KeyGuide.h"
#include "GameInstance.h"
CUI_KeyGuide::CUI_KeyGuide(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CUI_KeyGuide::CUI_KeyGuide(const CUI_KeyGuide& Prototype) : CUIObject(Prototype), m_strKey(Prototype.m_strKey)
{
}

HRESULT CUI_KeyGuide::Initialize_Prototype(const _wstring& strPrototypeTag)
{
	m_strKey = strPrototypeTag;

	return S_OK;
}

HRESULT CUI_KeyGuide::Initialize(void* pArg)
{
	m_fSizeX = 16;
	m_fSizeY = 16;
	m_fX = -2 - m_fSizeX * 0.5;
	m_fY = 2 + m_fSizeY * 0.5;
	m_fZ = 0.f;
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

void CUI_KeyGuide::Priority_Update(_float fTimeDelta)
{
}

void CUI_KeyGuide::Update(_float fTimeDelta)
{
}

void CUI_KeyGuide::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CUI_KeyGuide::Render()
{
	m_pVIBufferCom->Bind_Buffers();
	__super::Begin();
	m_pVIBufferCom->Render();
	__super::End();

	return S_OK;
}

HRESULT CUI_KeyGuide::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_Texture_Rect_UI_BalckRect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	return S_OK;
}

CUI_KeyGuide* CUI_KeyGuide::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _wstring& strPrototypeTag)
{
	CUI_KeyGuide* pInstance = new CUI_KeyGuide(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(strPrototypeTag)))
	{
		MSG_BOX(TEXT("Failed to Created : CUI_KeyGuide"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CUI_KeyGuide::Clone(void* pArg)
{
	CUI_KeyGuide* pInstance = new CUI_KeyGuide(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CUI_KeyGuide"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_KeyGuide::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
}

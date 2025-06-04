#include "Status_Stat_Frame_Name.h"
#include "GameInstance.h"
CStatus_Stat_Frame_Name::CStatus_Stat_Frame_Name(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CStatus_Stat_Frame_Name::CStatus_Stat_Frame_Name(const CStatus_Stat_Frame_Name& Prototype) : CUIObject(Prototype), m_eLevel{ Prototype.m_eLevel }
{
}

HRESULT CStatus_Stat_Frame_Name::Initialize_Prototype(LEVEL eLevel)
{
    m_eLevel = eLevel;

    if (FAILED(Ready_ChildPrototype(eLevel)))
        return E_FAIL;

    return S_OK;
}

HRESULT CStatus_Stat_Frame_Name::Initialize(void* pArg)
{
	m_fSizeX = 260;
	m_fSizeY = 150;
	m_fX = 0;
	m_fY = -160;
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

void CStatus_Stat_Frame_Name::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CStatus_Stat_Frame_Name::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CStatus_Stat_Frame_Name::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
	__super::Late_Update(fTimeDelta);
}

HRESULT CStatus_Stat_Frame_Name::Render()
{
	if (FAILED(m_pTextureCom->Bind_Texture(5)))
		return E_FAIL;
	m_pVIBufferCom->Bind_Buffers();

	__super::Begin();
	m_pVIBufferCom->Render();
	__super::End();
	Font_Render();
	return S_OK;
}

HRESULT CStatus_Stat_Frame_Name::Ready_Components()
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

HRESULT CStatus_Stat_Frame_Name::Ready_ChildPrototype(LEVEL eLevel)
{
    return S_OK;
}

HRESULT CStatus_Stat_Frame_Name::Ready_Children()
{
    return S_OK;
}

void CStatus_Stat_Frame_Name::Font_Render()
{
	TCHAR szText[64];
	Font_Rect_Update();
	_stprintf_s(szText, TEXT("Ä³¸¯ÅÍ ½ºÅÈ"));
	m_pGameInstance->Render_Font(TEXT("UI_Font_18_Stat"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

CStatus_Stat_Frame_Name* CStatus_Stat_Frame_Name::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
	CStatus_Stat_Frame_Name* pInstance = new CStatus_Stat_Frame_Name(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eLevel)))
	{
		MSG_BOX(TEXT("Failed to Create : CStatus_Stat_Frame_Name"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CStatus_Stat_Frame_Name::Clone(void* pArg)
{
	CStatus_Stat_Frame_Name* pInstance = new CStatus_Stat_Frame_Name(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CStatus_Stat_Frame_Name"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CStatus_Stat_Frame_Name::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}

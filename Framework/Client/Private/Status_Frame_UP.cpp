#include "Status_Frame_UP.h"
#include "GameInstance.h"
#include "Status_Element_Icon.h"
#include "Stat_Manager.h"
CStatus_Frame_UP::CStatus_Frame_UP(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CStatus_Frame_UP::CStatus_Frame_UP(const CStatus_Frame_UP& Prototype) : CUIObject(Prototype), m_eLevel{ Prototype.m_eLevel }
{
}

HRESULT CStatus_Frame_UP::Initialize_Prototype(LEVEL eLevel)
{
    m_eLevel = eLevel;

    if (FAILED(Ready_ChildPrototype(eLevel)))
        return E_FAIL;

    return S_OK;
}

HRESULT CStatus_Frame_UP::Initialize(void* pArg)
{
	m_fSizeX = 410;
	m_fSizeY = 410;
	m_fX = 0;
	m_fY = -300;
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

void CStatus_Frame_UP::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CStatus_Frame_UP::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CStatus_Frame_UP::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
	__super::Late_Update(fTimeDelta);
}

HRESULT CStatus_Frame_UP::Render()
{
	SetUp_RenderState();

	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;
	m_pVIBufferCom->Bind_Buffers();

	__super::Begin();
	m_pVIBufferCom->Render();
	__super::End();
	Font_Render();
	Reset_RenderState();
	return S_OK;
}

HRESULT CStatus_Frame_UP::Ready_Components()
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

HRESULT CStatus_Frame_UP::Ready_ChildPrototype(LEVEL eLevel)
{
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_Status_Element_Icon"),
		CStatus_Element_Icon::Create(m_pGraphic_Device, eLevel))))
		return E_FAIL;
}

HRESULT CStatus_Frame_UP::Ready_Children()
{
	CUIObject* pGameObject = nullptr;
	
	for (_int i = 8; i < 12; ++i)
	{
		UIOBJECT_DESC Desc{};

		Desc.fX = -90 + (i - 7) * 50;
		Desc.fZ = i;
		pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Status_Element_Icon"),&Desc));
		if (nullptr == pGameObject)
			return E_FAIL;
		Add_Child(pGameObject);
	}
    return S_OK;
}

void CStatus_Frame_UP::SetUp_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

}

void CStatus_Frame_UP::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	m_pGraphic_Device->SetTexture(0, NULL);

}

void CStatus_Frame_UP::Font_Render()
{
	TCHAR szText[64];
	CUIObject::Font_Rect_Update();
	
	m_vTexRect.left += 80;
	m_vTexRect.top += 32;
	m_vTexRect.right -= 80;
	_int iLv = CStat_Manager::GetInstance()->Get_CurStats()[ENUM_CLASS(STAT_INFO::LEVEL)];
	_stprintf_s(szText, TEXT("LV %d"), iLv);
	m_pGameInstance->Render_Font(TEXT("UI_Font_18"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	
	_stprintf_s(szText, TEXT("¼¼ÇÇ"));
	m_pGameInstance->Render_Font(TEXT("UI_Font_22"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), DT_CENTER | DT_VCENTER | DT_SINGLELINE);

}

CStatus_Frame_UP* CStatus_Frame_UP::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
	CStatus_Frame_UP* pInstance = new CStatus_Frame_UP(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eLevel)))
	{
		MSG_BOX(TEXT("Failed to Create : CStatus_Frame_UP"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CStatus_Frame_UP::Clone(void* pArg)
{
	CStatus_Frame_UP* pInstance = new CStatus_Frame_UP(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CStatus_Frame_UP"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CStatus_Frame_UP::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}

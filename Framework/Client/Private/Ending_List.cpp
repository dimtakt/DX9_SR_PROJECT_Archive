#include "Ending_List.h"
#include "GameInstance.h"

CEnding_List::CEnding_List(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CEnding_List::CEnding_List(const CEnding_List& Prototype) : CUIObject(Prototype)
{
}

HRESULT CEnding_List::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEnding_List::Initialize(void* pArg)
{
	m_fSizeX = 400;
	m_fSizeY = 400;
	m_fX = -200;
	m_fY = -20;
	m_fZ = 0.32f;
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

void CEnding_List::Priority_Update(_float fTimeDelta)
{
}

void CEnding_List::Update(_float fTimeDelta)
{

}

void CEnding_List::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CEnding_List::Render()
{
	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;
	m_pVIBufferCom->Bind_Buffers();

	__super::Begin();
	m_pVIBufferCom->Render();

	Font_Render();
	__super::End();

	return S_OK;
}

HRESULT CEnding_List::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Rect_List"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

void CEnding_List::Font_Render()
{
	TCHAR szText[MAX_PATH];
	
	CUIObject::Font_Rect_Update();
	m_vTexRect.left += 110;
	m_vTexRect.right += 110;

	m_vTexRect.top += 115;
	_stprintf_s(szText, TEXT("ÆÀÀå: Á¶Á¤È¯"));
	m_pGameInstance->Render_Font(TEXT("UI_Font_25"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.0f), DT_LEFT | DT_TOP);

	m_vTexRect.top += 40;
	_stprintf_s(szText, TEXT("ÆÀ¿ø: ÀÓÀººñ"));
	m_pGameInstance->Render_Font(TEXT("UI_Font_25"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.0f), DT_LEFT | DT_TOP);

	m_vTexRect.top += 35;
	_stprintf_s(szText, TEXT("ÆÀ¿ø: ±è±âÈÆ"));
	m_pGameInstance->Render_Font(TEXT("UI_Font_25"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.0f), DT_LEFT | DT_TOP);
	
	m_vTexRect.top += 35;
	_stprintf_s(szText, TEXT("ÆÀ¿ø: ±è¹ü¼ö"));
	m_pGameInstance->Render_Font(TEXT("UI_Font_25"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.0f), DT_LEFT | DT_TOP);

	m_vTexRect.top += 55;
	m_vTexRect.left -= 220;
	m_vTexRect.right -= 220;
	_stprintf_s(szText, TEXT("»ì·ÁÁà.."));
	m_pGameInstance->Render_Font(TEXT("UI_Font_13_Stat"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.0f), DT_RIGHT | DT_TOP);

}

CEnding_List* CEnding_List::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CEnding_List* pInstance = new CEnding_List(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CEnding_List"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CEnding_List::Clone(void* pArg)
{
	CEnding_List* pInstance = new CEnding_List(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CEnding_List"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEnding_List::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}

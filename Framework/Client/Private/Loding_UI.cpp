#include "Loding_UI.h"
#include "GameInstance.h"
#include "Loding_Ani.h"
CLoding_UI::CLoding_UI(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CLoding_UI::CLoding_UI(const CLoding_UI& Prototype) : CUIObject(Prototype), m_eLevel(Prototype.m_eLevel)
{
}

HRESULT CLoding_UI::Initialize_Prototype(LEVEL eLevel)
{
	m_eLevel = eLevel;

	if (FAILED(Ready_ChildPrototype(eLevel)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoding_UI::Initialize(void* pArg)
{
	m_fSizeX = g_iWinSizeX;
	m_fSizeY = g_iWinSizeY;
	m_fX = m_fSizeX * 0.5f;
	m_fY = m_fSizeY * 0.5f;
	m_fZ = UI_DEPTH::LODING;
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

void CLoding_UI::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CLoding_UI::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CLoding_UI::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);

	__super::Late_Update(fTimeDelta);
}

HRESULT CLoding_UI::Render()
{
	m_pGraphic_Device->SetTexture(0, NULL);

	m_pVIBufferCom->Bind_Buffers();
	__super::Begin();
	m_pVIBufferCom->Render();
	Font_Render();
	__super::End();

	return S_OK;
}

HRESULT CLoding_UI::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Rect_UI_BalckRect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoding_UI::Ready_ChildPrototype(LEVEL eLevel)
{
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_Loding_Ani"),
		CLoding_Ani::Create(m_pGraphic_Device, m_eLevel))))
		return E_FAIL;
	return S_OK;
}

HRESULT CLoding_UI::Ready_Children()
{
	CUIObject* pGameObject = nullptr;

	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Loding_Ani")));
	if (nullptr == pGameObject)
		return E_FAIL;
	Add_Child(pGameObject);

	return S_OK;
}

void CLoding_UI::Font_Render()
{
	TCHAR szText[64] = TEXT("FIRST FLOOR");
	
	///DT_VCENTER | DT_SINGLELINE
	m_vTexRect.left = 0;
	m_vTexRect.top = 50 + g_iWinSizeY * 0.5;
	m_vTexRect.right = g_iWinSizeX;
	m_vTexRect.bottom = g_iWinSizeY;
	m_pGameInstance->Render_Font(TEXT("UI_Font_40"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), DT_CENTER | DT_TOP);

	_stprintf_s(szText, TEXT("두더지 기지"));
	m_vTexRect.top = 150 + g_iWinSizeY * 0.5;
	m_pGameInstance->Render_Font(TEXT("UI_Font_50"), szText, m_vTexRect, D3DXCOLOR(0.f, 0.f, 1.f, 1.f), DT_CENTER | DT_TOP);

	_stprintf_s(szText, TEXT("당신은 탑의 운명을 바꾸기 위해 마을을 떠나 탑 아래로 내려갑니다."));
	m_vTexRect.top = 220 + g_iWinSizeY * 0.5;
	m_pGameInstance->Render_Font(TEXT("UI_Font_30"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), DT_CENTER | DT_TOP);

	_stprintf_s(szText, TEXT("토끼마을 아래에는 두더지들이 무리를 지어 살고 있습니다."));
	m_vTexRect.top = 260 + g_iWinSizeY * 0.5;
	m_pGameInstance->Render_Font(TEXT("UI_Font_30"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), DT_CENTER | DT_TOP);

}

CLoding_UI* CLoding_UI::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
	CLoding_UI* pInstance = new CLoding_UI(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eLevel)))
	{
		MSG_BOX(TEXT("Failed to Created : CLoding_UI"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CLoding_UI::Clone(void* pArg)
{
	CLoding_UI* pInstance = new CLoding_UI(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CLoding_UI"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLoding_UI::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
}

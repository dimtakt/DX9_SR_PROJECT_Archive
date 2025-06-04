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

	LOADINGDESC* pDesc = static_cast<LOADINGDESC*>(pArg);

	m_eLevel = pDesc->pNewLevel;

	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
	__super::Update_Position();

	if (FAILED(Ready_Children(pArg)))
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

HRESULT CLoding_UI::Ready_Components(void* pArg)
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

HRESULT CLoding_UI::Ready_Children(void* pArg)
{
	CUIObject* pGameObject = nullptr;

	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_UI_Loding_Ani"), pArg));
	if (nullptr == pGameObject)
		return E_FAIL;
	Add_Child(pGameObject);

	return S_OK;
}

void CLoding_UI::Font_Render()
{

	if (m_eLevel == LEVEL::LEVEL_STAGE1)
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
	else if (m_eLevel == LEVEL::LEVEL_STAGE2)
	{
		TCHAR szText[64] = TEXT("SECOND FLOOR");

		///DT_VCENTER | DT_SINGLELINE
		m_vTexRect.left = 0;
		m_vTexRect.top = 50 + g_iWinSizeY * 0.5;
		m_vTexRect.right = g_iWinSizeX;
		m_vTexRect.bottom = g_iWinSizeY;
		m_pGameInstance->Render_Font(TEXT("UI_Font_40"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), DT_CENTER | DT_TOP);

		_stprintf_s(szText, TEXT("초목지대"));
		m_vTexRect.top = 100 + g_iWinSizeY * 0.5;
		m_pGameInstance->Render_Font(TEXT("UI_Font_50"), szText, m_vTexRect, D3DXCOLOR(0.f, 1.f, 0.f, 1.f), DT_CENTER | DT_TOP);

		_stprintf_s(szText, TEXT("당신은 두더지 기지를 빠져나와 햇살이 내리쬐는 초목지대에 진입합니다."));
		m_vTexRect.top = 220 + g_iWinSizeY * 0.5;
		m_pGameInstance->Render_Font(TEXT("UI_Font_30"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), DT_CENTER | DT_TOP);

		_stprintf_s(szText, TEXT("평화로워 보이는 풍경 너머에는 폐허가 된 망르과 약탈자들, 광신도들만이 당신을 반겨주고 있습니다."));
		m_vTexRect.top = 260 + g_iWinSizeY * 0.5;
		m_pGameInstance->Render_Font(TEXT("UI_Font_30"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), DT_CENTER | DT_TOP);
	}
	else if (m_eLevel == LEVEL::LEVEL_STAGE3)
	{
		TCHAR szText[64] = TEXT("THIRD FLOOR");

		///DT_VCENTER | DT_SINGLELINE
		m_vTexRect.left = 0;
		m_vTexRect.top = 50 + g_iWinSizeY * 0.5;
		m_vTexRect.right = g_iWinSizeX;
		m_vTexRect.bottom = g_iWinSizeY;
		m_pGameInstance->Render_Font(TEXT("UI_Font_40"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), DT_CENTER | DT_TOP);

		_stprintf_s(szText, TEXT("도서관"));
		m_vTexRect.top = 100 + g_iWinSizeY * 0.5;
		m_pGameInstance->Render_Font(TEXT("UI_Font_50"), szText, m_vTexRect, D3DXCOLOR(0.f, 0.f, 1.f, 1.f), DT_CENTER | DT_TOP);

		_stprintf_s(szText, TEXT("당신은 초목지대의 거대한 유적에 들어갑니다."));
		m_vTexRect.top = 220 + g_iWinSizeY * 0.5;
		m_pGameInstance->Render_Font(TEXT("UI_Font_30"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), DT_CENTER | DT_TOP);

		_stprintf_s(szText, TEXT("도서관이라 불리는 이곳은 동물들이 탐에 정착하기 전부터 존재하던 곳입니다.."));
		m_vTexRect.top = 260 + g_iWinSizeY * 0.5;
		m_pGameInstance->Render_Font(TEXT("UI_Font_30"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), DT_CENTER | DT_TOP);

		_stprintf_s(szText, TEXT("비록 오랜 시간이 지났지만, 유적의 경비는 삼엄합니다."));
		m_vTexRect.top = 300 + g_iWinSizeY * 0.5;
		m_pGameInstance->Render_Font(TEXT("UI_Font_30"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), DT_CENTER | DT_TOP);
	}
	else if (m_eLevel == LEVEL::LEVEL_STAGE4)
	{
		TCHAR szText[64] = TEXT("FOURTH FLOOR");

		///DT_VCENTER | DT_SINGLELINE
		m_vTexRect.left = 0;
		m_vTexRect.top = 50 + g_iWinSizeY * 0.5;
		m_vTexRect.right = g_iWinSizeX;
		m_vTexRect.bottom = g_iWinSizeY;
		m_pGameInstance->Render_Font(TEXT("UI_Font_40"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), DT_CENTER | DT_TOP);

		_stprintf_s(szText, TEXT("황무지"));
		m_vTexRect.top = 100 + g_iWinSizeY * 0.5;
		m_pGameInstance->Render_Font(TEXT("UI_Font_50"), szText, m_vTexRect, D3DXCOLOR(0.f, 0.f, 1.f, 1.f), DT_CENTER | DT_TOP);

		_stprintf_s(szText, TEXT("당신은 도서관의 거대한 장치를 이용하여 아래로 내려갑니다."));
		m_vTexRect.top = 220 + g_iWinSizeY * 0.5;
		m_pGameInstance->Render_Font(TEXT("UI_Font_30"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), DT_CENTER | DT_TOP);

		_stprintf_s(szText, TEXT("과거 푸르렀던 황무지는 탑의 가장 바닥과 가깝다고 전해져 옵니다."));
		m_vTexRect.top = 260 + g_iWinSizeY * 0.5;
		m_pGameInstance->Render_Font(TEXT("UI_Font_30"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), DT_CENTER | DT_TOP);

		_stprintf_s(szText, TEXT("이제는 광신도와 강한 악마들, 그리고 과거 살던 동물의 무덤뿐입니다."));
		m_vTexRect.top = 300 + g_iWinSizeY * 0.5;
		m_pGameInstance->Render_Font(TEXT("UI_Font_30"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), DT_CENTER | DT_TOP);
	}


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

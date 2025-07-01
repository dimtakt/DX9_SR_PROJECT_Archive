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

	if (m_eLevel == LEVEL::LEVEL_TOWN)
	{
		
		TCHAR szText[64] = TEXT("TOWN");

		///DT_VCENTER | DT_SINGLELINE
		m_vTexRect.left = 0;
		m_vTexRect.top = 100 + g_iWinSizeY * 0.5;
		m_vTexRect.right = g_iWinSizeX;
		m_vTexRect.bottom = g_iWinSizeY;
		m_pGameInstance->Render_Font(TEXT("UI_Font_40"), szText, m_vTexRect, D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.f), DT_CENTER | DT_TOP);

		_stprintf_s(szText, TEXT("마을"));
		m_vTexRect.top = 150 + g_iWinSizeY * 0.5;
		m_pGameInstance->Render_Font(TEXT("UI_Font_50"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), DT_CENTER | DT_TOP);

		_stprintf_s(szText, TEXT("집을 잃은 주민들이 살아가고 있습니다."));
		m_vTexRect.top = 220 + g_iWinSizeY * 0.5;
		m_pGameInstance->Render_Font(TEXT("UI_Font_30"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), DT_CENTER | DT_TOP);

		_stprintf_s(szText, TEXT("모든 주민이 당신을 바라봅니다..."));
		m_vTexRect.top = 260 + g_iWinSizeY * 0.5;
		m_pGameInstance->Render_Font(TEXT("UI_Font_30"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), DT_CENTER | DT_TOP);
	}
	else if (m_eLevel == LEVEL::LEVEL_STAGE1)
	{
		TCHAR szText[64] = TEXT("FIRST FLOOR");

		///DT_VCENTER | DT_SINGLELINE
		m_vTexRect.left = 0;
		m_vTexRect.top = 60 + g_iWinSizeY * 0.5;
		m_vTexRect.right = g_iWinSizeX;
		m_vTexRect.bottom = g_iWinSizeY;
		m_pGameInstance->Render_Font(TEXT("UI_Font_40"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), DT_CENTER | DT_TOP);

		_stprintf_s(szText, TEXT("검은 오솔길"));
		m_vTexRect.top = 135 + g_iWinSizeY * 0.5;
		m_pGameInstance->Render_Font(TEXT("UI_Font_50"), szText, m_vTexRect, D3DXCOLOR(0.f, 0.f, 1.f, 1.f), DT_CENTER | DT_TOP);

		_stprintf_s(szText, TEXT("검은색 나무가 무성한 것 같다."));
		m_vTexRect.top = 220 + g_iWinSizeY * 0.5;
		m_pGameInstance->Render_Font(TEXT("UI_Font_30"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), DT_CENTER | DT_TOP);

		_stprintf_s(szText, TEXT("누군가의 시선이 느껴진다."));
		m_vTexRect.top = 260 + g_iWinSizeY * 0.5;
		m_pGameInstance->Render_Font(TEXT("UI_Font_30"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), DT_CENTER | DT_TOP);
	}
	else if (m_eLevel == LEVEL::LEVEL_STAGE2)
	{
		TCHAR szText[64] = TEXT("SECOND FLOOR");

		///DT_VCENTER | DT_SINGLELINE
		m_vTexRect.left = 0;
		m_vTexRect.top = 60 + g_iWinSizeY * 0.5;
		m_vTexRect.right = g_iWinSizeX;
		m_vTexRect.bottom = g_iWinSizeY;
		m_pGameInstance->Render_Font(TEXT("UI_Font_40"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), DT_CENTER | DT_TOP);

		_stprintf_s(szText, TEXT("검은 숲"));
		m_vTexRect.top = 135 + g_iWinSizeY * 0.5;
		m_pGameInstance->Render_Font(TEXT("UI_Font_50"), szText, m_vTexRect, D3DXCOLOR(0.f, 1.f, 0.f, 1.f), DT_CENTER | DT_TOP);

		_stprintf_s(szText, TEXT("누군가 지나간 흔적이 보인다."));
		m_vTexRect.top = 220 + g_iWinSizeY * 0.5;
		m_pGameInstance->Render_Font(TEXT("UI_Font_30"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), DT_CENTER | DT_TOP);

		_stprintf_s(szText, TEXT("앞에 발자국이 끊겨 있다..."));
		m_vTexRect.top = 260 + g_iWinSizeY * 0.5;
		m_pGameInstance->Render_Font(TEXT("UI_Font_30"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), DT_CENTER | DT_TOP);
	}
	else if (m_eLevel == LEVEL::LEVEL_STAGE3)
	{
		TCHAR szText[64] = TEXT("THIRD FLOOR");

		///DT_VCENTER | DT_SINGLELINE
		m_vTexRect.left = 0;
		m_vTexRect.top = 60 + g_iWinSizeY * 0.5;
		m_vTexRect.right = g_iWinSizeX;
		m_vTexRect.bottom = g_iWinSizeY;
		m_pGameInstance->Render_Font(TEXT("UI_Font_40"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), DT_CENTER | DT_TOP);

		_stprintf_s(szText, TEXT("폐허"));
		m_vTexRect.top = 135 + g_iWinSizeY * 0.5;
		m_pGameInstance->Render_Font(TEXT("UI_Font_50"), szText, m_vTexRect, D3DXCOLOR(0.f, 0.f, 1.f, 1.f), DT_CENTER | DT_TOP);

		_stprintf_s(szText, TEXT("주위에 낡은 건물들이 보인다."));
		m_vTexRect.top = 220 + g_iWinSizeY * 0.5;
		m_pGameInstance->Render_Font(TEXT("UI_Font_30"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), DT_CENTER | DT_TOP);

		_stprintf_s(szText, TEXT("아직 남은 사람들이 있을까?"));
		m_vTexRect.top = 260 + g_iWinSizeY * 0.5;
		m_pGameInstance->Render_Font(TEXT("UI_Font_30"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), DT_CENTER | DT_TOP);
	}
	else if (m_eLevel == LEVEL::LEVEL_STAGE4)
	{
		TCHAR szText[64] = TEXT("FOURTH FLOOR");

		///DT_VCENTER | DT_SINGLELINE
		m_vTexRect.left = 0;
		m_vTexRect.top = 60 + g_iWinSizeY * 0.5;
		m_vTexRect.right = g_iWinSizeX;
		m_vTexRect.bottom = g_iWinSizeY;
		m_pGameInstance->Render_Font(TEXT("UI_Font_40"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), DT_CENTER | DT_TOP);

		_stprintf_s(szText, TEXT("폐허 내부"));
		m_vTexRect.top = 135 + g_iWinSizeY * 0.5;
		m_pGameInstance->Render_Font(TEXT("UI_Font_50"), szText, m_vTexRect, D3DXCOLOR(0.f, 0.f, 1.f, 1.f), DT_CENTER | DT_TOP);

		_stprintf_s(szText, TEXT("누군가 도망가는 소리가 들린다.."));
		m_vTexRect.top = 220 + g_iWinSizeY * 0.5;
		m_pGameInstance->Render_Font(TEXT("UI_Font_30"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), DT_CENTER | DT_TOP);

		_stprintf_s(szText, TEXT("쫒아가보자"));
		m_vTexRect.top = 260 + g_iWinSizeY * 0.5;
		m_pGameInstance->Render_Font(TEXT("UI_Font_30"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), DT_CENTER | DT_TOP);
	}
	else if (m_eLevel == LEVEL::LEVEL_BOSS1)
	{
		TCHAR szText[64] = TEXT("ERMA");

		///DT_VCENTER | DT_SINGLELINE
		m_vTexRect.left = 0;
		m_vTexRect.top = 110 + g_iWinSizeY * 0.5;
		m_vTexRect.right = g_iWinSizeX;
		m_vTexRect.bottom = g_iWinSizeY;
		m_pGameInstance->Render_Font(TEXT("UI_Font_40"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), DT_CENTER | DT_TOP);

		_stprintf_s(szText, TEXT("에르마"));
		m_vTexRect.top = 160 + g_iWinSizeY * 0.5;
		m_pGameInstance->Render_Font(TEXT("UI_Font_50"), szText, m_vTexRect, D3DXCOLOR(0.f, 0.f, 1.f, 1.f), DT_CENTER | DT_TOP);

		_stprintf_s(szText, TEXT("에르마는 겁이 많습니다."));
		m_vTexRect.top = 220 + g_iWinSizeY * 0.5;
		m_pGameInstance->Render_Font(TEXT("UI_Font_30"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), DT_CENTER | DT_TOP);

		_stprintf_s(szText, TEXT("공포가 한계에 다다르면 무슨 짓을 할지 모르죠..."));
		m_vTexRect.top = 260 + g_iWinSizeY * 0.5;
		m_pGameInstance->Render_Font(TEXT("UI_Font_30"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), DT_CENTER | DT_TOP);
	}
	else if (m_eLevel == LEVEL::LEVEL_SHELTER)
	{
		TCHAR szText[64] = TEXT("SHELTER");

		///DT_VCENTER | DT_SINGLELINE
		m_vTexRect.left = 0;
		m_vTexRect.top = 120 + g_iWinSizeY * 0.5;
		m_vTexRect.right = g_iWinSizeX;
		m_vTexRect.bottom = g_iWinSizeY;
		m_pGameInstance->Render_Font(TEXT("UI_Font_40"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), DT_CENTER | DT_TOP);

		_stprintf_s(szText, TEXT("복도"));
		m_vTexRect.top = 160 + g_iWinSizeY * 0.5;
		m_pGameInstance->Render_Font(TEXT("UI_Font_50"), szText, m_vTexRect, D3DXCOLOR(0.f, 0.f, 1.f, 1.f), DT_CENTER | DT_TOP);

		_stprintf_s(szText, TEXT("당신은 지친 몸을 이끌고 마지막 방으로 이동합니다."));
		m_vTexRect.top = 220 + g_iWinSizeY * 0.5;
		m_pGameInstance->Render_Font(TEXT("UI_Font_30"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), DT_CENTER | DT_TOP);

		_stprintf_s(szText, TEXT("당신이 쉴 수 있는 마지막 장소인 것 같습니다."));
		m_vTexRect.top = 260 + g_iWinSizeY * 0.5;
		m_pGameInstance->Render_Font(TEXT("UI_Font_30"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), DT_CENTER | DT_TOP);
	}
	else if (m_eLevel == LEVEL::LEVEL_BOSS2)
	{
		TCHAR szText[64] = TEXT("ASKARD");

		///DT_VCENTER | DT_SINGLELINE
		m_vTexRect.left = 0;
		m_vTexRect.top = 120 + g_iWinSizeY * 0.5;
		m_vTexRect.right = g_iWinSizeX;
		m_vTexRect.bottom = g_iWinSizeY;
		m_pGameInstance->Render_Font(TEXT("UI_Font_40"), szText, m_vTexRect, D3DXCOLOR(1.f, 0.f, 0.f, 1.f), DT_CENTER | DT_TOP);

		_stprintf_s(szText, TEXT("아스카드"));
		m_vTexRect.top = 160 + g_iWinSizeY * 0.5;
		m_pGameInstance->Render_Font(TEXT("UI_Font_50"), szText, m_vTexRect, D3DXCOLOR(1.f, 0.f, 0.3f, 1.f), DT_CENTER | DT_TOP);

		_stprintf_s(szText, TEXT("당신은 아스카드에게 찾아갑니다."));
		m_vTexRect.top = 220 + g_iWinSizeY * 0.5;
		m_pGameInstance->Render_Font(TEXT("UI_Font_30"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), DT_CENTER | DT_TOP);

		_stprintf_s(szText, TEXT("조심하세요. 그는 귀여운 토끼라도 봐주지 않습니다."));
		m_vTexRect.top = 260 + g_iWinSizeY * 0.5;
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

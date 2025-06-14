#include "Field_Npc_Face.h"
#include "GameInstance.h"
#include "Face_Rect.h"
#include "Face_Rect_Small.h"
CField_Npc_Face::CField_Npc_Face(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CField_Npc_Face::CField_Npc_Face(const CField_Npc_Face& Prototype) : CUIObject(Prototype)
{
}

void CField_Npc_Face::Render_SmallFace_On(_int iNumber)
{
	if (!m_bIsRender)
	{
		m_iTexIndex = iNumber;
		m_pTransformCom->Scaling(m_fSizeX * 0.7, m_fSizeY * 0.7, 1.f);
		m_bIsSmallRender = true;
	}
}

void CField_Npc_Face::Render_SmallFace_Off()
{
	m_bIsSmallRender = false;
}

void CField_Npc_Face::Render_Face_On(_int iNumber, _bool bCinematic)
{
	m_bCinematicFace = bCinematic;
	m_iTexIndex = iNumber;
	m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
	m_bIsRender = true;
}

void CField_Npc_Face::Render_Face_Off()
{
	m_bCinematicFace = false;
	m_bIsRender = false;
}

HRESULT CField_Npc_Face::Initialize_Prototype(LEVEL eLevel)
{
	m_eLevel = eLevel;

	if (FAILED(Ready_ChildPrototype(eLevel)))
		return E_FAIL;

	return S_OK;
}

HRESULT CField_Npc_Face::Initialize(void* pArg)
{
	m_fSizeX = 256;
	m_fSizeY = 256;
	m_fX = 430 + g_iWinSizeX * 0.5;
	m_fY = 120 + g_iWinSizeY * 0.5;
	m_fZ = UI_DEPTH::FILED_CHAT;
	m_iWinSizeX = g_iWinSizeX;
	m_iWinSizeY = g_iWinSizeY;
	if (FAILED(CUIObject::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
	__super::Update_Position();

	if (FAILED(Ready_Children()))
		return E_FAIL;

	m_pGameInstance->Add_UIObject(ENUM_CLASS(m_eLevel), TEXT("UI_NpcFace"), this);
	return S_OK;
}

void CField_Npc_Face::Priority_Update(_float fTimeDelta)
{
}

void CField_Npc_Face::Update(_float fTimeDelta)
{

}

void CField_Npc_Face::Late_Update(_float fTimeDelta)
{

	if (m_bIsRender)
	{
		if(m_bCinematicFace)
			m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
		
		m_vecChildren[0]->Late_Update(fTimeDelta);
		m_vecChildren[1]->Late_Update(fTimeDelta);
	}
	else if (m_bIsSmallRender)
	{
		m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
		m_vecChildren[2]->Late_Update(fTimeDelta);
	}
}

HRESULT CField_Npc_Face::Render()
{
	if (FAILED(m_pTextureCom->Bind_Texture(m_iTexIndex)))
		return E_FAIL;
	m_pVIBufferCom->Bind_Buffers();
	__super::Begin();
	m_pVIBufferCom->Render();
	__super::End();

	if (m_bIsRender)
		Render_Font();
	else if (m_bIsSmallRender)
	{
		Render_Font_Small();
		m_bIsSmallRender = false;
	}
	return S_OK;
}

HRESULT CField_Npc_Face::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Rect_NpcFace"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

void CField_Npc_Face::Render_Font()
{
	TCHAR szText[MAX_PATH];
	CUIObject::Font_Rect_Update();

	m_vTexRect.left -= 300;
	m_vTexRect.right -= 300;

	m_vTexRect.top += 150;
	m_vTexRect.bottom += 150;

	switch (m_iTexIndex)
	{
	case 0:
		_stprintf_s(szText, TEXT("°©¿Ê ±â»ç"));
		break;
	case 1:
		_stprintf_s(szText, TEXT("¸¶À» »óÀÎ"));
		break;
	case 2:
		_stprintf_s(szText, TEXT("´ëÀåÀåÀÌ"));
		break;
	case 3:
		_stprintf_s(szText, TEXT("Åä³¢ Àå·Î"));
		break;
	case 4:
		_stprintf_s(szText, TEXT("¸ðÇè°¡"));
		break;
	}
	m_pGameInstance->Render_Font(TEXT("UI_Font_30"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.0f), DT_RIGHT | DT_TOP);

	m_vTexRect.top += 40;
	m_vTexRect.bottom += 40;
	switch (m_iTexIndex)
	{
	case 0:
		_stprintf_s(szText, TEXT("¹ÙµÏÀÌ"));
		break;
	case 1:
		_stprintf_s(szText, TEXT("¹Ù¹Ù"));
		break;
	case 2:
		_stprintf_s(szText, TEXT("ÄË"));
		break;
	case 3:
		_stprintf_s(szText, TEXT("¶óÀÏ¸®"));
		break;
	case 4:
		_stprintf_s(szText, TEXT("Àª½¼"));
		break;
	}
	m_pGameInstance->Render_Font(TEXT("UI_Font_30"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.0f), DT_RIGHT | DT_TOP);

}

void CField_Npc_Face::Render_Font_Small()
{
	TCHAR szText[MAX_PATH];
	CUIObject::Font_Rect_Update();

	m_vTexRect.left -= 240;
	m_vTexRect.right -= 240;

	m_vTexRect.top += 120;
	m_vTexRect.bottom += 120;

	switch (m_iTexIndex)
	{
	case 0:
		_stprintf_s(szText, TEXT("°©¿Ê ±â»ç"));
		break;
	case 1:
		_stprintf_s(szText, TEXT("¸¶À» »óÀÎ"));
		break;
	case 2:
		_stprintf_s(szText, TEXT("´ëÀåÀåÀÌ"));
		break;
	case 3:
		_stprintf_s(szText, TEXT("Åä³¢ Àå·Î"));
		break;
	case 4:
		_stprintf_s(szText, TEXT("¸ðÇè°¡"));
		break;
	}
	m_pGameInstance->Render_Font(TEXT("UI_Font_30"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.0f), DT_RIGHT | DT_TOP);

	m_vTexRect.top += 40;
	m_vTexRect.bottom += 40;
	switch (m_iTexIndex)
	{
	case 0:
		_stprintf_s(szText, TEXT("¹ÙµÏÀÌ"));
		break;
	case 1:
		_stprintf_s(szText, TEXT("¹Ù¹Ù"));
		break;
	case 2:
		_stprintf_s(szText, TEXT("ÄË"));
		break;
	case 3:
		_stprintf_s(szText, TEXT("¶óÀÏ¸®"));
		break;
	case 4:
		_stprintf_s(szText, TEXT("Àª½¼"));
		break;
	}
	m_pGameInstance->Render_Font(TEXT("UI_Font_30"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.0f), DT_RIGHT | DT_TOP);
}

HRESULT CField_Npc_Face::Ready_ChildPrototype(LEVEL eLevel)
{
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_Field_Face_Rect"),
		CFace_Rect::Create(m_pGraphic_Device, eLevel))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_Field_Face_Rect_Small"),
		CFace_Rect_Small::Create(m_pGraphic_Device, eLevel))))
		return E_FAIL;
	return S_OK;
}

HRESULT CField_Npc_Face::Ready_Children()
{
	CUIObject* pGameObject = nullptr;

	UIOBJECT_DESC Desc{};

	for (_int i = 0; i < 2; ++i)
	{
		Desc.fZ = i;
		pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Field_Face_Rect"), &Desc));
		if (nullptr == pGameObject)
			return E_FAIL;
		Add_Child(pGameObject);
	}

	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Field_Face_Rect_Small")));
	if (nullptr == pGameObject)
		return E_FAIL;
	Add_Child(pGameObject);
	return S_OK;
}

CField_Npc_Face* CField_Npc_Face::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
	CField_Npc_Face* pInstance = new CField_Npc_Face(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eLevel)))
	{
		MSG_BOX(TEXT("Failed to Created : CField_Npc_Face"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CField_Npc_Face::Clone(void* pArg)
{
	CField_Npc_Face* pInstance = new CField_Npc_Face(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CField_Npc_Face"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CField_Npc_Face::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}

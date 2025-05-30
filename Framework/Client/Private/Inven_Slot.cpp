#include "Inven_Slot.h"
#include "GameInstance.h"
#include "Inven_Slot_Selete.h"
#include "Client_Defines_Item.h"

CInven_Slot::CInven_Slot(LPDIRECT3DDEVICE9 pGraphic_Device) : CButton{ pGraphic_Device }
{
}

CInven_Slot::CInven_Slot(const CInven_Slot& Prototype) : CButton(Prototype), m_eLevel(Prototype.m_eLevel), m_bIsOver(false)
{
}

HRESULT CInven_Slot::Initialize_Prototype(LEVEL eLevel)
{
	m_eLevel = eLevel;

	if (FAILED(Ready_ChildPrototype(eLevel)))
		return E_FAIL;

	return S_OK;
}

HRESULT CInven_Slot::Initialize(void* pArg)
{
	INVEN_SLOT_DESC* Desc = static_cast<INVEN_SLOT_DESC*>(pArg);
	m_iSlotIndex = Desc->iSlotInedx;

	m_fSizeX = 76;
	m_fSizeY = 76;
	m_fX = -203 + Desc->fX * (m_fSizeX + 5);
	m_fY = -151 + Desc->fY * (m_fSizeY + 5);
	m_fZ = 0.2f;
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

	return S_OK;
}

void CInven_Slot::Priority_Update(_float fTimeDelta)
{
	if (m_pSlotItem == nullptr)
		m_iItemCount = 0;

	m_iSlotGradeCount = 0;
	__super::Priority_Update(fTimeDelta);
}

void CInven_Slot::Update(_float fTimeDelta)
{
	if (CUIObject::isPick(g_hWnd) && m_pGameInstance->IsKeyDown('R') && m_pSlotItem != nullptr)
		m_pSlotItem->IsRotation_Slate();

	__super::Update(fTimeDelta);
}

void CInven_Slot::Late_Update(_float fTimeDelta)
{
	if (CUIObject::isPick(g_hWnd))
		m_bIsOver = true;
	else
		m_bIsOver = false;

	Setting_Item();

	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);

	if (m_bIsOver)
		__super::Late_Update(fTimeDelta);

	if (m_pSlotItem != nullptr)
	{
		m_pSlotItem->Late_Update(fTimeDelta, m_pTransformCom->Get_State(STATE::POSITION));
	}
}

HRESULT CInven_Slot::Render()
{
	SetUp_RenderState();
	if (FAILED(CButton::Bind_ButtonTex_Single(g_hWnd, m_iSlotItem_Tex)))
		return E_FAIL;
	Reset_RenderState();
	Render_Font();
	return S_OK;
}

_bool CInven_Slot::IsKey_Down_Check()
{
	return 	Check_Key_Down(g_hWnd, VK_LBUTTON);
}

_bool CInven_Slot::IsKey_Up_Check()
{
	return 	Check_Key_UP(g_hWnd, VK_LBUTTON);
}

CItem_Base* CInven_Slot::Pop_Item()
{
	return m_pSlotItem;
}

void CInven_Slot::Push_Item(CItem_Base* pItem)
{
	m_pSlotItem = pItem;;
}

void CInven_Slot::ItemRender()
{

	m_pSlotItem->IsSelete();
}

_int CInven_Slot::Slot_Info(ITEM_INFO eInfo)
{
	switch (eInfo)
	{
	case Client::ITEM_INFO::ITEM_TYPE:
		return static_cast<_int>(m_eItemType);
	case Client::ITEM_INFO::ITEM_VALUE:
		return m_iItemValue;
	case ITEM_INFO::ITEM_COUNT:
		return m_iItemCount;
	}
}

void CInven_Slot::Add_GradeCount(_int iValue)
{
	m_iSlotGradeCount += iValue;
}

void CInven_Slot::Setting_Item()
{
	if (m_pSlotItem == nullptr)
	{
		m_iSlotItem_MaxGrade = 0;
		m_iSlotItem_Tex = 0;
		m_iItemValue = 0;
		m_eItemType = ITEM_TYPE::ITEM_TYPE_END;
		return;
	}

	m_eItemType = static_cast<ITEM_TYPE>(m_pSlotItem->Item_Info()->iItemType);
	m_iItemValue = m_pSlotItem->Item_Info()->iItemValue;

	switch (m_eItemType)
	{
	case ITEM_TYPE::ARTEFACT:
		m_iSlotItem_MaxGrade = m_iItemValue;
		switch (m_pSlotItem->Item_Info()->iRarity)
		{
		case ENUM_CLASS(ITEM_RARITY::NORMAL):
			m_iSlotItem_Tex = 2;
			break;
		case ENUM_CLASS(ITEM_RARITY::RARE):
			m_iSlotItem_Tex = 3;
			break;
		case ENUM_CLASS(ITEM_RARITY::EPIC):
			m_iSlotItem_Tex = 4;
			break;
		case ENUM_CLASS(ITEM_RARITY::LEGENDARY):
			m_iSlotItem_Tex = 5;
			break;
		}
		break;
	case ITEM_TYPE::SKILLBOOK:
		m_iSlotItem_MaxGrade = m_iItemValue;
		switch (m_pSlotItem->Item_Info()->iRarity)
		{
		case ENUM_CLASS(ITEM_RARITY::NORMAL):
			m_iSlotItem_Tex = 2;
			break;
		case ENUM_CLASS(ITEM_RARITY::RARE):
			m_iSlotItem_Tex = 3;
			break;
		case ENUM_CLASS(ITEM_RARITY::EPIC):
			m_iSlotItem_Tex = 4;
			break;
		case ENUM_CLASS(ITEM_RARITY::LEGENDARY):
			m_iSlotItem_Tex = 5;
			break;
		}
		break;
	case ITEM_TYPE::STONE:
		m_iSlotItem_Tex = 6;
		break;
	case ITEM_TYPE::POTION:
		m_iSlotItem_MaxGrade = g_PotionDataBase[m_iItemValue].m_iMaxPotion;
		m_iSlotItem_Tex = 0;
		break;
	}
}

HRESULT CInven_Slot::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_Texture_Rect_Slot_Inven"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

void CInven_Slot::SetUp_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
}

void CInven_Slot::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	m_pGraphic_Device->SetTexture(0, NULL);
}

HRESULT CInven_Slot::Ready_ChildPrototype(LEVEL eLevel)
{
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_Inven_Slot_Selete"),
		CInven_Slot_Selete::Create(m_pGraphic_Device, eLevel))))
		return E_FAIL;

	return S_OK;
}

HRESULT CInven_Slot::Ready_Children()
{
	CUIObject* pGameObject = nullptr;

	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Inven_Slot_Selete")));
	if (nullptr == pGameObject)
		return E_FAIL;
	Add_Child(pGameObject);

	return S_OK;
}

void CInven_Slot::Render_Font()
{
	Font_Rect_Update();
	m_vTexRect.left += 11;
	m_vTexRect.top += 11;
	m_vTexRect.right -= 11;
	m_vTexRect.bottom -= 11;
	TCHAR szText[64];
	D3DXCOLOR TexColor{};
	if (m_pSlotItem == nullptr)
	{
		if (m_iSlotGradeCount == 0)
			return;
		if (m_bIsOver)
			return;
		if (m_iSlotGradeCount > 0)
		{
			_stprintf_s(szText, TEXT("+%d"),m_iSlotGradeCount);
			m_pGameInstance->Render_Font(TEXT("UI_Font_12"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), DT_TOP | DT_LEFT);
			return;
		}
		else
		{
			_stprintf_s(szText, TEXT("%d"),m_iSlotGradeCount);
			m_pGameInstance->Render_Font(TEXT("UI_Font_12"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), DT_TOP | DT_LEFT);
			return;
		}
	}
	
	switch (m_eItemType)
	{
	case Client::ITEM_TYPE::ARTEFACT:
		if (m_iSlotGradeCount < 0)
			TexColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);
		else if (m_iSlotGradeCount > m_iSlotItem_MaxGrade)
			TexColor = D3DXCOLOR(1.f, 0.647f, 0.f, 1.f);
		else if(m_iSlotGradeCount == m_iSlotItem_MaxGrade)
			TexColor = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);
		else
			TexColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		
		_stprintf_s(szText, TEXT("%d/%d"), m_iSlotGradeCount, m_iSlotItem_MaxGrade);
		m_pGameInstance->Render_Font(TEXT("UI_Font_12"), szText, m_vTexRect, TexColor, DT_TOP | DT_LEFT);
		break;
	case Client::ITEM_TYPE::STONE:
		break;
	case Client::ITEM_TYPE::POTION:
		if(m_iItemCount >= m_iSlotItem_MaxGrade)
			TexColor = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);
		else
			TexColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		_stprintf_s(szText, TEXT("%d/%d"),m_iItemCount, m_iSlotItem_MaxGrade);
		m_pGameInstance->Render_Font(TEXT("UI_Font_12"), szText, m_vTexRect, TexColor, DT_RIGHT | DT_BOTTOM);
		break;
	case Client::ITEM_TYPE::SKILLBOOK:
		if (m_iSlotGradeCount < 0)
			TexColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);
		else if (m_iSlotGradeCount > m_iSlotItem_MaxGrade)
			TexColor = D3DXCOLOR(1.f, 0.647f, 0.f, 1.f);
		else if (m_iSlotGradeCount == m_iSlotItem_MaxGrade)
			TexColor = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);
		else
			TexColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

		_stprintf_s(szText, TEXT("%d/%d"),m_iSlotGradeCount, m_iSlotItem_MaxGrade);
		m_pGameInstance->Render_Font(TEXT("UI_Font_12"), szText, m_vTexRect, TexColor, DT_TOP | DT_LEFT);
		break;
	}

}


CInven_Slot* CInven_Slot::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
	CInven_Slot* pInstance = new CInven_Slot(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eLevel)))
	{
		MSG_BOX(TEXT("Failed to Created : CInven_Slot"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CInven_Slot::Clone(void* pArg)
{
	CInven_Slot* pInstance = new CInven_Slot(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CInven_Slot"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CInven_Slot::Free()
{
	__super::Free();
}

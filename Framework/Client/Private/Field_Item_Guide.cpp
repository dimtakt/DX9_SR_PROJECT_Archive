#include "Field_Item_Guide.h"
#include "GameInstance.h"
#include "Stat_Manager.h"
CField_Item_Guide::CField_Item_Guide(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CField_Item_Guide::CField_Item_Guide(const CField_Item_Guide& Prototype) : CUIObject(Prototype), m_eLevel(Prototype.m_eLevel)
{
}

HRESULT CField_Item_Guide::Initialize_Prototype(LEVEL eLevel)
{
	m_eLevel = eLevel;

	if (FAILED(Ready_ChildPrototype(eLevel)))
		return E_FAIL;
}

HRESULT CField_Item_Guide::Initialize(void* pArg)
{
	UIOBJECT_DESC* Desc = static_cast<UIOBJECT_DESC*>(pArg);

	m_fSizeX = 22.f;
	m_fSizeY = 22.f;
	m_fX = -40.f;
	m_fY = 40.f;
	m_fZ = UI_DEPTH::FILED_ITEM;
	m_iWinSizeX = g_iWinSizeX;
	m_iWinSizeY = g_iWinSizeY;

	m_iItemID = Desc->fZ;

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

void CField_Item_Guide::Priority_Update(_float fTimeDelta)
{

}

void CField_Item_Guide::Update(_float fTimeDelta)
{

}

void CField_Item_Guide::Late_Update(_float fTimeDelta)
{
	if (CStat_Manager::GetInstance()->Get_UIOpen())
		return;

	Target_Pos();
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI_BLEND, this);
}

HRESULT CField_Item_Guide::Render()
{
	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;
	m_pVIBufferCom->Bind_Buffers();
	__super::Begin();
	m_pVIBufferCom->Render();
	__super::End();
	
	Render_Font();
	return S_OK;
}

HRESULT CField_Item_Guide::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Rect_UI_Hud_KeyInputGuide"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;
	return S_OK;
}

HRESULT CField_Item_Guide::Ready_ChildPrototype(LEVEL eLevel)
{
	return S_OK;
}

HRESULT CField_Item_Guide::Ready_Children()
{
	CUIObject* pGameObject = nullptr;

	return S_OK;
}

void CField_Item_Guide::Target_Pos()
{
	_float3 Target_Pos = m_pParent->Get_WorldPos();

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &m_OldViewMatrix);
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &m_OldProjMatrix);

	_float3 vWinPos{};
	_float4x4 vVPMatrix = m_OldViewMatrix * m_OldProjMatrix;
	D3DXVec3TransformCoord(&vWinPos, &Target_Pos, &vVPMatrix);

	m_iWinPosX = (vWinPos.x + 1.f) * 0.5 * g_iWinSizeX + m_fX;
	m_iWinPosY = (-vWinPos.y + 1.f) * 0.5 * g_iWinSizeY + m_fY;

	m_vWorldPos.x = m_iWinPosX - m_iWinSizeX * 0.5f;
	m_vWorldPos.y = -m_iWinPosY + m_iWinSizeY * 0.5f;
	m_vWorldPos.z = UI_DEPTH::FILED_ITEM;;

	m_pTransformCom->Set_State(STATE::POSITION, m_vWorldPos);
}

void CField_Item_Guide::Render_Font()
{
	TCHAR szText[64];
	m_vTexRect.left = m_iWinPosX - 20;
	m_vTexRect.right = m_iWinPosX + 20;
	m_vTexRect.top = m_iWinPosY - 20;
	m_vTexRect.bottom = m_iWinPosY + 20;

	_stprintf_s(szText, TEXT("F"));
	m_pGameInstance->Render_Font(TEXT("UI_Font_18"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	m_vTexRect.left += 35;
	m_vTexRect.right += 70;
	_stprintf_s(szText, TEXT("구매하기"));
	m_pGameInstance->Render_Font(TEXT("UI_Font_18"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), DT_LEFT | DT_VCENTER | DT_SINGLELINE);


	D3DXCOLOR TexColor{};

	switch (g_ItemDataBase[m_iItemID].m_eRarity)
	{
	case ITEM_RARITY::NORMAL:
		TexColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		break;
	case ITEM_RARITY::RARE:
		TexColor = D3DXCOLOR(0.3f, 1.f, 0.f, 1.f);
		break;
	case ITEM_RARITY::EPIC:
		TexColor = D3DXCOLOR(0.f, 0.75f, 1.f, 1.f);
		break;
	case ITEM_RARITY::LEGENDARY:
		TexColor = D3DXCOLOR(1.f, 1.f, 0.f, 1.f);
		break;
	}

	m_vTexRect.left = m_iWinPosX - 100 - m_fX;
	m_vTexRect.right = m_iWinPosX + 100 - m_fX;
	m_vTexRect.top = m_iWinPosY - 100;
	m_vTexRect.bottom = m_iWinPosY - 80;
	
	m_pGameInstance->Render_Font(TEXT("UI_Font_18"), g_ItemDataBase[m_iItemID].m_szName, m_vTexRect, TexColor, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

}

CField_Item_Guide* CField_Item_Guide::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
	CField_Item_Guide* pInstance = new CField_Item_Guide(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eLevel)))
	{
		MSG_BOX(TEXT("Failed to Created : CField_Item_Guide"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CField_Item_Guide::Clone(void* pArg)
{
	CField_Item_Guide* pInstance = new CField_Item_Guide(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CField_Item_Guide"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CField_Item_Guide::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}

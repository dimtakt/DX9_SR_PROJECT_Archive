#include "Field_Item.h"
#include "GameInstance.h"
CField_Item::CField_Item(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CField_Item::CField_Item(const CField_Item& Prototype) : CUIObject(Prototype), m_eLevel(Prototype.m_eLevel)
{
}

void CField_Item::Render_Field_Item()
{
	Target_Pos();
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CField_Item::Initialize_Prototype(LEVEL eLevel)
{
	m_eLevel = eLevel;

	if (FAILED(Ready_ChildPrototype(eLevel)))
		return E_FAIL;
}

HRESULT CField_Item::Initialize(void* pArg)
{
	FIELD_ITEM_DESC* Desc = static_cast<FIELD_ITEM_DESC*>(pArg);

	m_fSizeX = 64.f;
	m_fSizeY = 64.f;
	m_fX = 0;
	m_fY = 0;
	m_fZ = UI_DEPTH::FILED_ITEM;
	m_iWinSizeX = g_iWinSizeX;
	m_iWinSizeY = g_iWinSizeY;

	m_iItemID = Desc->m_iItemID;
	m_iItemTex = g_ItemDataBase[m_iItemID].m_iItemTextureID;


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

void CField_Item::Priority_Update(_float fTimeDelta)
{

}

void CField_Item::Update(_float fTimeDelta)
{

}

void CField_Item::Late_Update(_float fTimeDelta)
{

}

HRESULT CField_Item::Render()
{
	if (FAILED(m_pTextureCom->Bind_Texture(m_iItemTex)))
		return E_FAIL;
	m_pVIBufferCom->Bind_Buffers();

	__super::Begin();
	m_pVIBufferCom->Render();

//	Render_Font();
	__super::End();

	return S_OK;
}

HRESULT CField_Item::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Rect_UI_Hud_Dice_Symbol"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CField_Item::Ready_ChildPrototype(LEVEL eLevel)
{
	return S_OK;
}

HRESULT CField_Item::Ready_Children()
{
	CUIObject* pGameObject = nullptr;

	return S_OK;
}

void CField_Item::Target_Pos()
{
	_float3 Target_Pos = m_vTargetPos;

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &m_OldViewMatrix);
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &m_OldProjMatrix);

	_float3 vWinPos{};
	_float4x4 vVPMatrix = m_OldViewMatrix * m_OldProjMatrix;
	D3DXVec3TransformCoord(&vWinPos, &Target_Pos, &vVPMatrix);

	_float fWinPosX = (vWinPos.x + 1.f) * 0.5 * g_iWinSizeX;
	_float fWinPosY = (-vWinPos.y + 1.f) * 0.5 * g_iWinSizeY;

	m_vWorldPos.x = fWinPosX - m_iWinSizeX * 0.5f;
	m_vWorldPos.y = -fWinPosY + m_iWinSizeY * 0.5f;


	m_pTransformCom->Set_State(STATE::POSITION, m_vWorldPos);
}

CField_Item* CField_Item::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
	CField_Item* pInstance = new CField_Item(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eLevel)))
	{
		MSG_BOX(TEXT("Failed to Created : CField_Item"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CField_Item::Clone(void* pArg)
{
	CField_Item* pInstance = new CField_Item(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CHp_Player"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CField_Item::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
}

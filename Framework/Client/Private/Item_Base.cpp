#include "Item_Base.h"
#include "GameInstance.h"
CItem_Base::CItem_Base(LPDIRECT3DDEVICE9 pGraphic_Device) : CItemObject(pGraphic_Device)
{
}

CItem_Base::CItem_Base(const CItem_Base& Prototype) : CItemObject(Prototype), m_bisSelete(false)
{
}

HRESULT CItem_Base::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CItem_Base::Initialize(void* pArg)
{
	CItemObject::ITEMOBJECT_DESC* Desc = static_cast<ITEMOBJECT_DESC*>(pArg);

	m_fSizeX = 55;
	m_fSizeY = 55;
	m_fX = g_iWinSizeX * 0.5;
	m_fY = g_iWinSizeY * 0.5;
	m_fZ = 0.f;
	m_iWinSizeX = g_iWinSizeX;
	m_iWinSizeY = g_iWinSizeY;

	if (FAILED(CUIObject::Initialize()))
		return E_FAIL;

	m_iItemID = Desc->iItemID;
	m_iItemType = Desc->iItemType;
	m_iRarity = Desc->iRarity;
	m_szName = Desc->szName;
	m_szDescription = Desc->szDescription;
	m_iItemTextureID = Desc->iItemTextureID;
	m_iItemValue = Desc->iItemValue;
	m_iArtefact_Value = Desc->iArtefact_Value;


	if (FAILED(Ready_Componet()))
		return E_FAIL;

	m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
	__super::Update_Position();

	return S_OK;
}

void CItem_Base::Priority_Update(_float fTimeDelta)
{
	m_bIsTooltip = false;
}

void CItem_Base::Update(_float fTimeDelta)
{

}

void CItem_Base::Late_Update(_float fTimeDelta, _float3 fPos)
{
	if (m_pGameInstance->IsKeyUp(VK_LBUTTON))
		m_bisSelete = false;
	m_vDefaultPos = fPos;
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CItem_Base::Render()
{
	
	if (FAILED(m_pTextureCom->Bind_Texture(m_iItemTextureID)))
		return E_FAIL;
	m_pVIBufferCom->Bind_Buffers();
	if (m_bIsQuickSlot_Render)
	{
		if (FAILED(QuickSlot_Render()))
			return E_FAIL;
		m_bIsQuickSlot_Render = false;
	}
	else if (!m_bIsTooltip_Render)
	{
		m_pTransformCom->Rotation(_float3{ 0.f,0.f,-1.f }, D3DXToRadian(m_fAngle));
		m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
		m_pTransformCom->Set_State(STATE::POSITION, m_vDefaultPos);

		if (FAILED(Default_Render()))
			return E_FAIL;

		if (m_bisSelete)
			if (FAILED(Selete_Render()))
				return E_FAIL;

		if (m_bIsTooltip)
			m_bIsTooltip_Render = true;
		else
			m_bIsTooltip_Render = false;

	}
	else if (m_bIsTooltip && !m_bIsTooltip_Slate_Render)
	{
		if (FAILED(Tooltip_Render()))
			return E_FAIL;

		if(m_iItemType == 1)
			m_bIsTooltip_Slate_Render = true;
		else
		{
			m_bIsTooltip = false;
			m_bIsTooltip_Render = false;
		}
	}
	else if(m_bIsTooltip_Slate_Render)
	{
		if (m_iItemType == 1)
		{
			if (FAILED(Tooltip_Slate_Render()))
				return E_FAIL;
		}

		m_bIsTooltip = false;
		m_bIsTooltip_Render = false;
		m_bIsTooltip_Slate_Render = false;
	}
	return S_OK;
}

void CItem_Base::IsSelete()
{
	m_bisSelete = true;
}

void CItem_Base::IsRotation_Slate()
{
	if (m_iItemType == ENUM_CLASS(ITEM_TYPE::STONE) && g_SlateDataBase[m_iItemValue].m_bRotation)
	{
		m_fAngle += 90.f;

		if (m_fAngle >= 360.f)
			m_fAngle = 0.f;
	}
}

void CItem_Base::IsTooltip(_float3 TooltipPos)
{
	m_vTooltipPos = TooltipPos;
	m_bIsTooltip = true;
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI_BLEND, this);
}

void CItem_Base::IsTooltip_Slate(_float3 TooltipSlotPos)
{
	m_vTooltipSlatePos = TooltipSlotPos;
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI_BLEND, this);
}

void CItem_Base::IsQuickSlot_Render(_float3 QuickSlotPos)
{
	m_bIsQuickSlot_Render = true;
	m_vQuickSlotPos = QuickSlotPos;
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI_BLEND, this);
}


HRESULT CItem_Base::Default_Render()
{
	__super::Begin();
	m_pVIBufferCom->Render();
	__super::End();

	return S_OK;
}

HRESULT CItem_Base::Selete_Render()
{
	POINT			ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	m_pTransformCom->Scaling(m_fSizeX * 2.f, m_fSizeY * 2.f, 1.f);
	m_pTransformCom->Set_State(STATE::POSITION, _float3{ ptMouse.x - m_iWinSizeX * 0.5f, -ptMouse.y + m_iWinSizeY * 0.5f, 0.f });

	__super::Begin();
	m_pVIBufferCom->Render();
	__super::End();
	m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
	return S_OK;
}

HRESULT CItem_Base::Tooltip_Render()
{
	m_pTransformCom->Rotation(_float3{ 0.f,0.f,-1.f }, D3DXToRadian(0));
	m_pTransformCom->Set_State(STATE::POSITION, m_vTooltipPos);
	__super::Begin();
	m_pVIBufferCom->Render();
	__super::End();

	return S_OK;
}

HRESULT CItem_Base::Tooltip_Slate_Render()
{
	m_pTransformCom->Scaling(m_fSizeX * 0.5f, m_fSizeY * 0.5f, 1.f);
	m_pTransformCom->Rotation(_float3{ 0.f,0.f,-1.f }, D3DXToRadian(m_fAngle));
	m_pTransformCom->Set_State(STATE::POSITION, m_vTooltipSlatePos);
	__super::Begin();
	m_pVIBufferCom->Render();
	__super::End();

	return S_OK;
}

HRESULT CItem_Base::QuickSlot_Render()
{
	_float3 vPos{};
	
	vPos.x = -(g_iWinSizeX * 0.5) + m_vQuickSlotPos.x;
	vPos.y = (g_iWinSizeY * 0.5) - m_vQuickSlotPos.y;
	vPos.z = 0;

	if(m_iItemType == ENUM_CLASS(ITEM_TYPE::POTION))
		m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
	else
		m_pTransformCom->Scaling(m_fSizeX * 0.6, m_fSizeY*0.6, 1.f);

	m_pTransformCom->Rotation(_float3{ 0.f,0.f,-1.f }, D3DXToRadian(0));
	m_pTransformCom->Set_State(STATE::POSITION, vPos);
	__super::Begin();
	m_pVIBufferCom->Render();
	__super::End();

	return S_OK;
}

HRESULT CItem_Base::Ready_Componet()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Item"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	return S_OK;
}

CItem_Base* CItem_Base::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CItem_Base* pInstance = new CItem_Base(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CItem_Base"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CItem_Base::Clone(void* pArg)
{
	CItem_Base* pInstance = new CItem_Base(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CItem_Base"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CItem_Base::Free()
{
	__super::Free();
}

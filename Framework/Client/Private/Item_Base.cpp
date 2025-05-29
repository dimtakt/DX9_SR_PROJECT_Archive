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

	if (FAILED(Ready_Componet()))
		return E_FAIL;

	m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
	__super::Update_Position();

	return S_OK;
}

void CItem_Base::Priority_Update(_float fTimeDelta)
{
}

void CItem_Base::Update(_float fTimeDelta)
{

}

void CItem_Base::Late_Update(_float fTimeDelta, _float3 fPos)
{
	if (m_pGameInstance->IsKeyUp(VK_LBUTTON))
		m_bisSelete = false;

	m_pTransformCom->Rotation(_float3{ 0.f,0.f,-1.f }, D3DXToRadian(m_fAngle));

	m_pTransformCom->Set_State(STATE::POSITION, fPos);
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CItem_Base::Render()
{
	SetUp_RenderState();

	if (FAILED(m_pTextureCom->Bind_Texture(m_iItemTextureID)))
		return E_FAIL;
	m_pVIBufferCom->Bind_Buffers();

	if (FAILED(Default_Render()))
		return E_FAIL;

	if (m_bisSelete)
		if (FAILED(Selete_Render()))
			return E_FAIL;

	Reset_RenderState();

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

void CItem_Base::SetUp_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
}

void CItem_Base::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	m_pGraphic_Device->SetTexture(0, NULL);
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

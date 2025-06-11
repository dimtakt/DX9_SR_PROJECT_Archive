#include "UI_KeyGuide.h"
#include "GameInstance.h"
CUI_KeyGuide::CUI_KeyGuide(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CUI_KeyGuide::CUI_KeyGuide(const CUI_KeyGuide& Prototype) : CUIObject(Prototype), m_strKey(Prototype.m_strKey)
{
}

HRESULT CUI_KeyGuide::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_KeyGuide::Initialize(void* pArg)
{
	KEYGUIDE_DESC* Desc = static_cast<KEYGUIDE_DESC*>(pArg);
	m_fSizeX = 25;
	m_fSizeY = 25;
	m_strKey = Desc->strKey;

	if (Desc->Default)
	{
		m_fX = -m_fSizeX * 0.5;
		m_fY = m_fSizeY * 0.5;
		m_eRenderGroup = RENDERGROUP::RG_BLEND;
	}
	else
	{
		m_fX = Desc->fX;
		m_fY = Desc->fY;
		m_eRenderGroup = static_cast<RENDERGROUP>(Desc->fZ);
		if (Desc->bTarget)
		{
			m_bTarget = true;
			m_pTarget_Transform = Desc->pTransform;
		}
	}
	m_fZ = UI_DEPTH::KEY_GUIDE;


	m_iWinSizeX = g_iWinSizeX;
	m_iWinSizeY = g_iWinSizeY;

	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
	__super::Update_Position();

	return S_OK;
}

void CUI_KeyGuide::Priority_Update(_float fTimeDelta)
{
}

void CUI_KeyGuide::Update(_float fTimeDelta)
{
}

void CUI_KeyGuide::Late_Update(_float fTimeDelta)
{
	if (m_bTarget)
	{
		m_vWorldPos.x = m_pParent->Get_WorldPos().x + m_fX;
		m_vWorldPos.y = m_pParent->Get_WorldPos().y + m_fY;

		m_vTexRect.left = m_pParent->Get_WorldPos().x + g_iWinSizeX * 0.5 + m_fX - (m_fSizeX * 0.5);
		m_vTexRect.right = m_pParent->Get_WorldPos().x + g_iWinSizeX * 0.5 + m_fX + (m_fSizeX * 0.5);
		m_vTexRect.top = -m_pParent->Get_WorldPos().y + g_iWinSizeY * 0.5 - m_fY - (m_fSizeY * 0.5);
		m_vTexRect.bottom = -m_pParent->Get_WorldPos().y + g_iWinSizeY * 0.5 - m_fY + (m_fSizeY * 0.5);

		m_pTransformCom->Set_State(STATE::POSITION, m_vWorldPos);
	}

	m_pGameInstance->Add_RenderGroup(m_eRenderGroup, this);
}

HRESULT CUI_KeyGuide::Render()
{
	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;
	
	m_pVIBufferCom->Bind_Buffers();
	__super::Begin();
	m_pVIBufferCom->Render();
	__super::End();
	if (m_bTarget)
	{

		m_pGameInstance->Render_Font(TEXT("UI_Font_18"), m_strKey, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
	else
	{
		Font_Rect_Update();
		m_pGameInstance->Render_Font(TEXT("UI_Font_18"), m_strKey, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
	return S_OK;
}

HRESULT CUI_KeyGuide::Ready_Components()
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

void CUI_KeyGuide::Target_Pos()
{
	_float3 Target_Pos = m_pTarget_Transform->Get_State(STATE::POSITION);

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &m_OldViewMatrix);
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &m_OldProjMatrix);

	_float3 vWinPos{};
	_float4x4 vVPMatrix = m_OldViewMatrix * m_OldProjMatrix;
	D3DXVec3TransformCoord(&vWinPos, &Target_Pos, &vVPMatrix);

	m_iWinPosX = (vWinPos.x + 1.f) * 0.5 * g_iWinSizeX + m_fX;
	m_iWinPosY = (-vWinPos.y + 1.f) * 0.5 * g_iWinSizeY + m_fY;

	m_vWorldPos.x = m_iWinPosX - m_iWinSizeX * 0.5f;
	m_vWorldPos.y = -m_iWinPosY + m_iWinSizeY * 0.5f;
	m_vWorldPos.z = UI_DEPTH::KEY_GUIDE;;

	m_pTransformCom->Set_State(STATE::POSITION, m_vWorldPos);
}

CUI_KeyGuide* CUI_KeyGuide::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUI_KeyGuide* pInstance = new CUI_KeyGuide(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUI_KeyGuide"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CUI_KeyGuide::Clone(void* pArg)
{
	CUI_KeyGuide* pInstance = new CUI_KeyGuide(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CUI_KeyGuide"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_KeyGuide::Free()
{
	m_pTarget_Transform = nullptr;

	__super::Free();
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}

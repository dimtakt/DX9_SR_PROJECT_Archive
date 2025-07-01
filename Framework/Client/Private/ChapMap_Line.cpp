#include "ChapMap_Line.h"
#include "GameInstance.h"

CChapMap_Line::CChapMap_Line(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CChapMap_Line::CChapMap_Line(const CChapMap_Line& Prototype) : CUIObject(Prototype)
{
}

HRESULT CChapMap_Line::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CChapMap_Line::Initialize(void* pArg)
{
	MAP_LINE_DESC* Desc = static_cast<MAP_LINE_DESC*>(pArg);
	
	m_iTexIndex = 1;
	m_fAngle = Desc->fAngle;
	m_iIndex = Desc->iIndex;

	m_bisDefalut = Desc->bisDefalut;
	
	_float BaseY = 75;
	if (Desc->iIndex == 0 || Desc->iIndex == 3)
	{
		if (Desc->fX == 0) { // 왼쪽 대각선
			m_fX = -70 - Desc->fY * 40;
			m_fY = BaseY + Desc->fY * 80;
		}
		else if (Desc->fX == 1) { // 수직선
			m_fX = 0;
			m_fY = BaseY + Desc->fY * 80;
		}
		else if (Desc->fX == 2) { // 오른쪽 대각선
			m_fX = 70 + Desc->fY * 40;
			m_fY = BaseY + Desc->fY * 80;
		}
	}
	else if(Desc->iIndex == 1 || Desc->iIndex == 2)
	{
		if (Desc->fX == 0) { // 왼쪽 대각선
			m_fX = -70 - Desc->fY * 80;
			m_fY = BaseY + Desc->fY * 80;
		}
		else if (Desc->fX == 1) { // 수직선
			m_fX = 0;
			m_fY = BaseY + Desc->fY * 80;
		}
		else if (Desc->fX == 2) { // 오른쪽 대각선
			m_fX = 70 + Desc->fY * 80;
			m_fY = BaseY + Desc->fY * 80;
		}
	}

	if (m_bisDefalut)
	{
		m_fX = 0;
		m_fY = -85;
		m_iTexIndex = 1;
	}
	m_fSizeX = 48;
	m_fSizeY = 64;
	m_fZ = UI_DEPTH::CHATERMAP_LINE;
	m_iWinSizeX = g_iWinSizeX;
	m_iWinSizeY = g_iWinSizeY;
	
	
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	_float3 vRotation = { 0.f, 0.f, 1.f };
	m_pTransformCom->Rotation(vRotation, D3DXToRadian(m_fAngle));

	m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
	__super::Update_Position();

	return S_OK;
}

void CChapMap_Line::Priority_Update(_float fTimeDelta)
{
}

void CChapMap_Line::Update(_float fTimeDelta)
{
}

void CChapMap_Line::Late_Update(_float fTimeDelta)
{
	if(m_bisRand)
		m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
	
	if (m_bisDefalut)
	{
		m_iTexIndex = 1;
		m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
	}
}

HRESULT CChapMap_Line::Render()
{
	if (FAILED(m_pTextureCom->Bind_Texture(m_iTexIndex)))
		return E_FAIL;
	m_pVIBufferCom->Bind_Buffers();

	__super::Begin();
	m_pVIBufferCom->Render();
	__super::End();

	return S_OK;
}

HRESULT CChapMap_Line::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Rect_ChapMap_Node_Line"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	return S_OK;
}

CChapMap_Line* CChapMap_Line::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CChapMap_Line* pInstance = new CChapMap_Line(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CChapMap_Line"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CChapMap_Line::Clone(void* pArg)
{
	CChapMap_Line* pInstance = new CChapMap_Line(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CChapMap_Line"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CChapMap_Line::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}

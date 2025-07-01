#include "Chapmap_Boss.h"
#include "GameInstance.h"

CChapmap_Boss::CChapmap_Boss(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CChapmap_Boss::CChapmap_Boss(const CChapmap_Boss& Prototype) : CUIObject(Prototype)
{
}

HRESULT CChapmap_Boss::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CChapmap_Boss::Initialize(void* pArg)
{
	MAP_BOSS_DESC* Desc = static_cast<MAP_BOSS_DESC*>(pArg);

	m_iMapTex = ENUM_CLASS(Desc->eMap_Boss);
	m_fSizeX = 150;
	m_fSizeY = 150;

	m_fX = 0.f;
	m_fY = -90.f;
	m_fZ = UI_DEPTH::CHATERMAP_BUTTON;
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

void CChapmap_Boss::Priority_Update(_float fTimeDelta)
{
}

void CChapmap_Boss::Update(_float fTimeDelta)
{
}

void CChapmap_Boss::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CChapmap_Boss::Render()
{
	if (FAILED(m_pTextureCom->Bind_Texture(m_iMapTex)))
		return E_FAIL;
	m_pVIBufferCom->Bind_Buffers();

	__super::Begin();
	m_pVIBufferCom->Render();
	__super::End();
	return S_OK;
}

HRESULT CChapmap_Boss::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Rect_ChapMap_Node_Boss"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	return S_OK;
}

CChapmap_Boss* CChapmap_Boss::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CChapmap_Boss* pInstance = new CChapmap_Boss(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CChapmap_Boss"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CChapmap_Boss::Clone(void* pArg)
{
	CChapmap_Boss* pInstance = new CChapmap_Boss(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CChapmap_Boss"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CChapmap_Boss::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}

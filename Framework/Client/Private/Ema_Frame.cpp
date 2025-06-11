#include "Ema_Frame.h"
#include "GameInstance.h"

CEma_Frame::CEma_Frame(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CEma_Frame::CEma_Frame(const CEma_Frame& Prototype) : CUIObject(Prototype)
{
}

HRESULT CEma_Frame::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEma_Frame::Initialize(void* pArg)
{
	m_fSizeX = 1024;
	m_fSizeY = 1024;
	m_fX = 0;
	m_fY = -15;
	m_fZ = UI_DEPTH::BOSSHP;
	m_iWinSizeX = g_iWinSizeX;
	m_iWinSizeY = g_iWinSizeY;

	if (FAILED(CUIObject::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
	__super::Update_Position();

	return S_OK;
}

void CEma_Frame::Priority_Update(_float fTimeDelta)
{
}

void CEma_Frame::Update(_float fTimeDelta)
{
}

void CEma_Frame::Late_Update(_float fTimeDelta)
{

	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CEma_Frame::Render()
{
	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;
	m_pVIBufferCom->Bind_Buffers();

	__super::Begin();
	m_pVIBufferCom->Render();
	__super::End();

	return S_OK;
}

HRESULT CEma_Frame::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Rect_BossHp"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

CEma_Frame* CEma_Frame::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CEma_Frame* pInstance = new CEma_Frame(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CEma_Frame"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CEma_Frame::Clone(void* pArg)
{
	CEma_Frame* pInstance = new CEma_Frame(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CEma_Frame"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEma_Frame::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}

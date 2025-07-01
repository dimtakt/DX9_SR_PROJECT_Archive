#include "Loding_Ani.h"
#include "Loding_UI.h"
#include "GameInstance.h"
#include "Animation.h"
CLoding_Ani::CLoding_Ani(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CLoding_Ani::CLoding_Ani(const CLoding_Ani& Prototype) : CUIObject(Prototype), m_eLevel{ Prototype.m_eLevel }
{
}

HRESULT CLoding_Ani::Initialize_Prototype(LEVEL eLevel)
{
	m_eLevel = eLevel;

	return S_OK;
}

HRESULT CLoding_Ani::Initialize(void* pArg)
{

	CLoding_UI::LOADINGDESC* desc = static_cast<CLoding_UI::LOADINGDESC*>(pArg);
	//m_fSpeed = 0;
	//m_iTexIndex = 0;

	m_eLevel = desc->pNewLevel;

	if (m_eLevel == LEVEL::LEVEL_TOWN || m_eLevel == LEVEL::LEVEL_BOSS1 || m_eLevel == LEVEL::LEVEL_SHELTER || m_eLevel == LEVEL::LEVEL_BOSS2) {
		m_fSizeX = g_iWinSizeX + 50;
		m_fSizeY = g_iWinSizeY + 280;
		m_fX = 0;
		m_fY = -120;
		m_fZ = UI_DEPTH::LODING;
		m_iWinSizeX = g_iWinSizeX;
		m_iWinSizeY = g_iWinSizeY;
	}
	else {
		m_fSizeX = 260 + g_iWinSizeX;
		m_fSizeY = 260 + g_iWinSizeX;
		m_fX = 0;
		m_fY = -120;
		m_fZ = UI_DEPTH::LODING;
		m_iWinSizeX = g_iWinSizeX;
		m_iWinSizeY = g_iWinSizeY;
	}
	

	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
	__super::Update_Position();

	return S_OK;
}

void CLoding_Ani::Priority_Update(_float fTimeDelta)
{
}

void CLoding_Ani::Update(_float fTimeDelta)
{
	
	//m_fSpeed += 10 * fTimeDelta;

	//if (m_fSpeed >= 1) 
	//{
	//m_iTexIndex += m_fSpeed;
	//m_fSpeed = 0;
	//}

	//if (m_iTexIndex >= 20)
	//	m_iTexIndex = 0;
}

void CLoding_Ani::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CLoding_Ani::Render()
{
	/*if (FAILED(m_pTextureCom->Bind_Texture(m_iTexIndex)))
		return E_FAIL;*/
	
	m_pAnimatorCom->Update_State();
	m_pVIBufferCom->Bind_Buffers();

	__super::Begin();
	m_pVIBufferCom->Render();
	__super::End();

	return S_OK;
}

HRESULT CLoding_Ani::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;


	CAnimator::ANIMSTATE_DESC StartAnimStateDesc{};
	StartAnimStateDesc.strTimerTag = L"Animator_Loading";

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Animator"),
		TEXT("Com_Animator"), reinterpret_cast<CComponent**>(&m_pAnimatorCom), &StartAnimStateDesc)))
		return E_FAIL;

	if (m_eLevel == LEVEL::LEVEL_TOWN)
	{
		if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Town_Loading"),
			TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
			return E_FAIL;
	}
	else if (m_eLevel == LEVEL::LEVEL_STAGE1)
	{
		if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Rect_Loding_1"),
			TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
			return E_FAIL;
	}
	else if (m_eLevel == LEVEL::LEVEL_STAGE2)
	{
		if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Rect_Loding_2"),
			TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
			return E_FAIL;
	}
	else if (m_eLevel == LEVEL::LEVEL_STAGE3)
	{
		if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Rect_Loding_3_BG"),
			TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
			return E_FAIL;
	}
	else if (m_eLevel == LEVEL::LEVEL_STAGE4)
	{
		if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Rect_Loding_4_BG"),
			TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
			return E_FAIL;
	}
	else if (m_eLevel == LEVEL::LEVEL_BOSS1)
	{
		if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Town_Loading"),
			TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
			return E_FAIL;
	}
	else if (m_eLevel == LEVEL::LEVEL_SHELTER)
	{
		if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Town_Loading"),
			TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
			return E_FAIL;
	}
	else if (m_eLevel == LEVEL::LEVEL_BOSS2)
	{
		if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Town_Loading"),
			TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
			return E_FAIL;
	}
	else {
		if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Rect_Loding_1"),
			TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
			return E_FAIL;
	}

	m_pAnimatorCom->Add_State(L"Loading", { m_pTextureCom, 4, true });

	return S_OK;
}

CLoding_Ani* CLoding_Ani::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
	CLoding_Ani* pInstance = new CLoding_Ani(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eLevel)))
	{
		MSG_BOX(TEXT("Failed to Create : CLoding_Ani"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CLoding_Ani::Clone(void* pArg)
{
	CLoding_Ani* pInstance = new CLoding_Ani(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CLoding_Ani"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLoding_Ani::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pAnimatorCom);
}

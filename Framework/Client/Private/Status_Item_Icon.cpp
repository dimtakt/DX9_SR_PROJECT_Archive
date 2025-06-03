#include "Status_Item_Icon.h"
#include "GameInstance.h"
#include "Stat_Manager.h"
CStatus_Item_Icon::CStatus_Item_Icon(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CStatus_Item_Icon::CStatus_Item_Icon(const CStatus_Item_Icon& Prototype) : CUIObject(Prototype), m_eLevel{ Prototype.m_eLevel }
{
}

HRESULT CStatus_Item_Icon::Initialize_Prototype(LEVEL eLevel)
{
	m_eLevel = eLevel;
	return S_OK;
}

HRESULT CStatus_Item_Icon::Initialize(void* pArg)
{
	//m_iCulItemIndex = CStat_Manager::GetInstance()->Get_CurStats();
	m_fSizeX = 128;
	m_fSizeY = 128;
	m_fX = 0;
	m_fY = 0;
	m_fZ = UI_DEPTH::PLAYER_STAUTS;
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

void CStatus_Item_Icon::Priority_Update(_float fTimeDelta)
{
}

void CStatus_Item_Icon::Update(_float fTimeDelta)
{

}

void CStatus_Item_Icon::Late_Update(_float fTimeDelta)
{
	//바뀌는 무기가 있으면 인덱스 값으로 설정하게 변경할 예정
	//m_iCulItemIndex = CStat_Manager::GetInstance()->Get_CurStats();
	IconSetting();
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CStatus_Item_Icon::Render()
{
	SetUp_RenderState();

	if (FAILED(m_pTextureCom->Bind_Texture(m_iCulItemIndex)))
		return E_FAIL;
	m_pVIBufferCom->Bind_Buffers();

	__super::Begin();
	m_pVIBufferCom->Render();
	__super::End();
	Reset_RenderState();
	return S_OK;
}

HRESULT CStatus_Item_Icon::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Rect_Status_Item"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

void CStatus_Item_Icon::SetUp_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
}

void CStatus_Item_Icon::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	m_pGraphic_Device->SetTexture(0, NULL);
}

void CStatus_Item_Icon::IconSetting()
{
	if (m_iPreItemIndex != m_iCulItemIndex)
	{
		switch (m_iCulItemIndex)
		{
		case 0:
			m_fSizeX = 128;
			m_fSizeY = 128;
			m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
			break;
		case 1:
			m_fSizeX = 96;
			m_fSizeY = 96;
			m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
			break;

		}
		m_iPreItemIndex = m_iCulItemIndex;
	}
}

CStatus_Item_Icon* CStatus_Item_Icon::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
	CStatus_Item_Icon* pInstance = new CStatus_Item_Icon(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eLevel)))
	{
		MSG_BOX(TEXT("Failed to Create : CStatus_Item_Icon"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CStatus_Item_Icon::Clone(void* pArg)
{
	CStatus_Item_Icon* pInstance = new CStatus_Item_Icon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CStatus_Item_Icon"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CStatus_Item_Icon::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}

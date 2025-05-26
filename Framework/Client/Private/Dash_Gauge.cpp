#include "Dash_Gauge.h"
#include "GameInstance.h"
CDash_Gauge::CDash_Gauge(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CDash_Gauge::CDash_Gauge(const CDash_Gauge& Prototype) : CUIObject(Prototype), m_eLevel(Prototype.m_eLevel), m_iIndex(Prototype.m_iIndex)
{
}

HRESULT CDash_Gauge::Initialize_Prototype(LEVEL eLevel)
{
    m_eLevel = eLevel;

    if (FAILED(Ready_ChildPrototype(eLevel)))
        return E_FAIL;

    return S_OK;
}

HRESULT CDash_Gauge::Initialize(void* pArg)
{
	UIHUD_DASH_GAUGE_DESC* DescMy = static_cast<UIHUD_DASH_GAUGE_DESC*>(pArg);

	m_iIndex = DescMy->iIndex;
	m_fDash = 0.f;
	m_bIsRender = true;
	UIOBJECT_DESC Desc{};

	Desc.fSizeX = 30;
	Desc.fSizeY = 30;
	Desc.fX = 0;
	Desc.fY = 0;
	Desc.fZ = 0.f;
	Desc.iWinSizeX = g_iWinSizeX;
	Desc.iWinSizeY = g_iWinSizeY;

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Reday_SyncingObject()))
		return E_FAIL;

	m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
	__super::Update_Position();

	if (FAILED(Ready_Children()))
		return E_FAIL;


	//임시 
	CPlayerStats::PLAYERSTAT_DESC PlayerStatDesc{};
	PlayerStatDesc.strName = L"테스트";
	PlayerStatDesc.iLevel = 1;
	PlayerStatDesc.iExp = 0;
	PlayerStatDesc.fHp = 50.f;
	PlayerStatDesc.fMp = 50.f;
	PlayerStatDesc.fDash = 5.f;
	PlayerStatDesc.fAtkSpeed = 1.0f;
	PlayerStatDesc.fMoveSpeed = 1.0f;
	PlayerStatDesc.fHpRegen = 0.f;
	PlayerStatDesc.fMpRegen = 0.f;
	PlayerStatDesc.fDashRegen = 0.01f;
	PlayerStatDesc.fEvade = 0.f;
	PlayerStatDesc.fDef = 0.f;
	PlayerStatDesc.fExpMultiply = 1.0f;
	PlayerStatDesc.fGoldMultiply = 1.0f;

	m_pPlayerStatsCom->Set_Stats(PlayerStatDesc);

	return S_OK;
}

void CDash_Gauge::Priority_Update(_float fTimeDelta)
{

}

void CDash_Gauge::Update(_float fTimeDelta)
{

}

void CDash_Gauge::Late_Update(_float fTimeDelta)
{
	//임시 
	m_fDash += 0.01f;
	if (m_fDash >= 5.f)
		m_fDash = 0.f;

	CPlayerStats::PLAYERSTAT_DESC PlayerStatDesc{};
	PlayerStatDesc.strName = L"테스트";
	PlayerStatDesc.iLevel = 1;
	PlayerStatDesc.iExp = 0;
	PlayerStatDesc.fHp = 50.f;
	PlayerStatDesc.fMp = 50.f;
 	PlayerStatDesc.fDash = m_fDash;
	PlayerStatDesc.fAtkSpeed = 1.0f;
	PlayerStatDesc.fMoveSpeed = 1.0f;
	PlayerStatDesc.fHpRegen = 0.f;
	PlayerStatDesc.fMpRegen = 0.f;
	PlayerStatDesc.fDashRegen = 0.01f;
	PlayerStatDesc.fEvade = 0.f;
	PlayerStatDesc.fDef = 0.f;
	PlayerStatDesc.fExpMultiply = 1.0f;
	PlayerStatDesc.fGoldMultiply = 1.0f;

	m_pPlayerStatsCom->Set_Stats(PlayerStatDesc);

	//업데이트용
	if (m_pPlayerStatsCom->Get_Stats().fDash <= m_iIndex - 1)
		m_bIsRender = false;
	else if (m_pPlayerStatsCom->Get_Stats().fDash < m_iIndex)
		m_bIsRender = true;

	if(m_bIsRender)
		m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
	
}

HRESULT CDash_Gauge::Render()
{
	SetUp_RenderState();

	m_pTransformCom->Bind_Matrix();
	if (FAILED(m_pTextureCom->Bind_Texture(1)))
		return E_FAIL;
	m_pVIBufferCom->Bind_Buffers();

	__super::Begin();
	m_pVIBufferCom->Render();
	__super::End();

	Reset_RenderState();
	return S_OK;
}

HRESULT CDash_Gauge::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_Texture_Rect_UI_Hud_Dash_Symbol"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

void CDash_Gauge::SetUp_RenderState()
{
	if (m_pPlayerStatsCom->Get_Stats().fDash < m_iIndex)
		SetUp_Render_Gauge();
	
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
}

void CDash_Gauge::Reset_RenderState()
{
	if (m_pPlayerStatsCom->Get_Stats().fDash < m_iIndex)
		Reset_Render_Gauge();

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	m_pGraphic_Device->SetTexture(0, NULL);
}

HRESULT CDash_Gauge::Ready_ChildPrototype(LEVEL eLevel)
{
	return S_OK;
}

HRESULT CDash_Gauge::Ready_Children()
{
	return S_OK;
}

HRESULT CDash_Gauge::Reday_SyncingObject()
{
	m_pPlayerStatsCom = dynamic_cast<CPlayerStats*>(m_pGameInstance->Get_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Layer_Player"), TEXT("Com_PlayerStats"), 0));
	if (m_pPlayerStatsCom == nullptr)
	{
		MSG_BOX(TEXT("Failed to Syncing : CDash_Gauge"));
		return E_FAIL;
	}
	Safe_AddRef(m_pPlayerStatsCom);

	return S_OK;
}

void CDash_Gauge::SetUp_Render_Gauge()
{
	_float4x4 matTex;
	D3DXMatrixIdentity(&matTex);
	_float fRatio = m_pPlayerStatsCom->Get_Stats().fDash - (m_iIndex - 1);
	matTex._11 = fRatio;
	matTex._22 = 1.0f;
	m_pGraphic_Device->SetTransform(D3DTS_TEXTURE0, &matTex);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);

	_float fOffsetX = (1.f - fRatio) * m_fSizeX * 0.5f;

	m_pTransformCom->Scaling(m_fSizeX * fRatio, m_fSizeY, 1.f);
	m_pTransformCom->Set_State(STATE::POSITION, _float3{ (m_vWorldPos.x - m_iWinSizeX * 0.5f) - fOffsetX, -m_vWorldPos.y + m_iWinSizeY * 0.5f, m_vWorldPos.z });

}

void CDash_Gauge::Reset_Render_Gauge()
{
	_float4x4 matTex;
	D3DXMatrixIdentity(&matTex);
	matTex._11 = 1.0f;
	matTex._22 = 1.0f;
	m_pGraphic_Device->SetTransform(D3DTS_TEXTURE0, &matTex);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);

}

CDash_Gauge* CDash_Gauge::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
	CDash_Gauge* pInstance = new CDash_Gauge(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eLevel)))
	{
		MSG_BOX(TEXT("Failed to Created : CDash_Gauge"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CDash_Gauge::Clone(void* pArg)
{
	CDash_Gauge* pInstance = new CDash_Gauge(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CDash_Gauge"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CDash_Gauge::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pPlayerStatsCom);
}

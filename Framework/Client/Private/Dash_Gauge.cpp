#include "Dash_Gauge.h"
#include "GameInstance.h"
#include "Stat_Manager.h"

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

	m_fSizeX = 30;
	m_fSizeY = 30;
	m_fX = 0;
	m_fY = 0;
	m_fZ = UI_DEPTH::HUD_DASH;;
	m_iWinSizeX = g_iWinSizeX;
	m_iWinSizeY = g_iWinSizeY;

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

void CDash_Gauge::Priority_Update(_float fTimeDelta)
{

}

void CDash_Gauge::Update(_float fTimeDelta)
{

}

void CDash_Gauge::Late_Update(_float fTimeDelta)
{
 	m_fDash = CStat_Manager::GetInstance()->Get_CurStats()[ENUM_CLASS(STAT_INFO::CULDASH)];

	//업데이트용
	if (m_fDash <= m_iIndex - 1)
		m_bIsRender = false;
	else if (m_fDash < m_iIndex)
		m_bIsRender = true;

	if(m_bIsRender)
		m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
	
}

HRESULT CDash_Gauge::Render()
{
	SetUp_RenderState();

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

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Rect_UI_Hud_Dash_Symbol"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

void CDash_Gauge::SetUp_RenderState()
{
	m_pGraphic_Device->GetTransform(D3DTS_TEXTURE0, &matOldTex);
	m_pGraphic_Device->GetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, &fOldFlags);
	m_pGraphic_Device->GetTextureStageState(0, D3DTSS_TEXCOORDINDEX, &OldTexCoordIndex);

	if (m_fDash < m_iIndex)
		SetUp_Render_Gauge();
}

void CDash_Gauge::Reset_RenderState()
{
	m_pGraphic_Device->SetTransform(D3DTS_TEXTURE0, &matOldTex);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, fOldFlags);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, OldTexCoordIndex);
}

HRESULT CDash_Gauge::Ready_ChildPrototype(LEVEL eLevel)
{
	return S_OK;
}

HRESULT CDash_Gauge::Ready_Children()
{
	return S_OK;
}

void CDash_Gauge::SetUp_Render_Gauge()
{
	_float4x4 matTex;
	D3DXMatrixIdentity(&matTex);
	_float fRatio = m_fDash - (m_iIndex - 1);
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
}

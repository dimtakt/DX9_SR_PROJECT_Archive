#include "Talent_Point.h"
#include "GameInstance.h"
#include "Stat_Manager.h"
CTalent_Point::CTalent_Point(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CTalent_Point::CTalent_Point(const CTalent_Point& Prototype) : CUIObject(Prototype)
{
}

HRESULT CTalent_Point::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTalent_Point::Initialize(void* pArg)
{
	m_fSizeX = 300;
	m_fSizeY = 250;
	m_fX = -170 + g_iWinSizeX * 0.5;;
	m_fY = -40 + g_iWinSizeY * 0.5;
	m_fZ = UI_DEPTH::TALENT;
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

void CTalent_Point::Priority_Update(_float fTimeDelta)
{
}

void CTalent_Point::Update(_float fTimeDelta)
{
}

void CTalent_Point::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CTalent_Point::Render()
{
	SetUp_RenderState();


	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;
	m_pVIBufferCom->Bind_Buffers();

	__super::Begin();
	m_pVIBufferCom->Render();
	Font_Render();
	Reset_RenderState();
	return S_OK;
}

HRESULT CTalent_Point::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Rect_Talent_Frame"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

void CTalent_Point::SetUp_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
}

void CTalent_Point::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	m_pGraphic_Device->SetTexture(0, NULL);
}

void CTalent_Point::Font_Render()
{
	_int iPoint = CStat_Manager::GetInstance()->Get_CurStats()[ENUM_CLASS(STAT_INFO::CULSTATPOINT)];
	
	TCHAR szText[64];
	m_vTexRect.left = 582 + m_fX - m_fSizeX * 0.5;
	m_vTexRect.top = 467 + m_fY - m_fSizeY * 0.5;
	m_vTexRect.right = 562 + m_fX + m_fSizeX * 0.5;
	m_vTexRect.bottom = 450 + m_fY + m_fSizeY * 0.5;


	_stprintf_s(szText, TEXT("남은 재능 포인트") );
	m_pGameInstance->Render_Font(TEXT("UI_Font_28"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.0f), DT_RIGHT | DT_TOP);
	
	m_vTexRect.left = 820 + m_fX - m_fSizeX * 0.5;
	m_vTexRect.top = 467 + m_fY - m_fSizeY * 0.5;
	m_vTexRect.right = 668 + m_fX + m_fSizeX * 0.5;
	m_vTexRect.bottom = 450 + m_fY + m_fSizeY * 0.5;
	_stprintf_s(szText, TEXT("%d"), iPoint);
	m_pGameInstance->Render_Font(TEXT("UI_Font_28"), szText, m_vTexRect, D3DXCOLOR(1.000f, 0.886f, 0.608f, 1.0f), DT_CENTER | DT_TOP);

}

CTalent_Point* CTalent_Point::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTalent_Point* pInstance = new CTalent_Point(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CTalent_Point"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CTalent_Point::Clone(void* pArg)
{
	CTalent_Point* pInstance = new CTalent_Point(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CTalent_Point"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTalent_Point::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}

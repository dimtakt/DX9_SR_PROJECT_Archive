#include "Event_Timer.h"
#include "GameInstance.h"

CEvent_Timer::CEvent_Timer(LPDIRECT3DDEVICE9 pGraphic_Device) : CProgressBar(pGraphic_Device)
{
}

CEvent_Timer::CEvent_Timer(const CEvent_Timer& Prototype) : CProgressBar(Prototype)
{
}

HRESULT CEvent_Timer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEvent_Timer::Initialize(void* pArg)
{
	m_fSizeX = 500;
	m_fSizeY = 10;
	m_fX = 0;
	m_fY = 50;
	m_fZ = UI_DEPTH::EVENT_AZ;
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

void CEvent_Timer::Priority_Update(_float fTimeDelta)
{

}

void CEvent_Timer::Update(_float fTimeDelta)
{
}

void CEvent_Timer::Late_Update(_float fTimeDelta)
{
	if (m_fCulTime <= 0)
		return;

	ProgerssX();
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI_BLEND, this);
}

HRESULT CEvent_Timer::Render()
{
	m_pGraphic_Device->SetTexture(0, NULL);
	m_pVIBufferCom->Bind_Buffers();
	__super::Begin();
	m_pVIBufferCom->Render();
	__super::End();

	Render_Font();
	return S_OK;
}

void CEvent_Timer::Progerss_Set(_float iCulValue, _float iMaxValue)
{
	m_fCulTime = iCulValue;
	m_fCulMaxTime = iMaxValue;
}

void CEvent_Timer::ProgerssX()
{
	if (m_fPreTime != m_fCulTime || m_fPreMaxTime != m_fCulMaxTime)
	{

		_float fRatio{};

		if (m_fCulTime >= m_fCulMaxTime)
		{
			m_fCulTime = m_fCulMaxTime;
			fRatio = (float)m_fCulTime / (float)m_fCulMaxTime;
		}
		else if (m_fCulTime <= 0)
		{
			m_fCulTime = 0;
			fRatio = 0.1f / (float)m_fCulMaxTime;
		}
		else
		{
			fRatio = (float)m_fCulTime / (float)m_fCulMaxTime;
		}
		_float fOffsetX = (1.f - fRatio) * m_fSizeX * 0.5f; m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);

		m_pTransformCom->Scaling(m_fSizeX * fRatio, m_fSizeY, m_fZ);
		m_pTransformCom->Set_State(STATE::POSITION, _float3{ (m_vWorldPos.x - m_iWinSizeX * 0.5f) - fOffsetX, -m_vWorldPos.y + m_iWinSizeY * 0.5f, m_vWorldPos.z });

		if (m_fCulMaxTime <= m_fCulTime)
			m_fCulTime = m_fCulMaxTime;

		m_fPreMaxTime = m_fCulMaxTime;
		m_fPreTime = m_fCulTime;
	}
}

HRESULT CEvent_Timer::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Rect_UI_BlueRect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	return S_OK;
}

void CEvent_Timer::Render_Font()
{
	m_fRealTime = m_fCulTime;
	Font_Rect_Update();
	TCHAR szText[MAX_PATH];
	_stprintf_s(szText, TEXT("%.1f ÃÊ"), m_fRealTime);
	m_pGameInstance->Render_Font(TEXT("UI_Font_18"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

CEvent_Timer* CEvent_Timer::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CEvent_Timer* pInstance = new CEvent_Timer(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CEvent_Timer"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CEvent_Timer::Clone(void* pArg)
{
	CEvent_Timer* pInstance = new CEvent_Timer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CEvent_Timer"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEvent_Timer::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
}
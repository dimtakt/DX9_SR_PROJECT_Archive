#include "ProgressBar.h"
#include "GameInstance.h"

CProgressBar::CProgressBar(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CProgressBar::CProgressBar(const CProgressBar& Prototype) : CUIObject(Prototype)
{
}

HRESULT CProgressBar::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;
	return S_OK;
}

HRESULT CProgressBar::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	return S_OK;
}

void CProgressBar::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CProgressBar::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CProgressBar::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CProgressBar::Render()
{
	return S_OK;
}

void CProgressBar::Progress_UpdateX()
{
	if (m_iPreValue != m_iCulValue || m_iPreMaxValue != m_iCulMaxValue)
	{
		
		_float fRatio{};

		if (m_iCulValue >= m_iCulMaxValue)
		{
			m_iCulValue = m_iCulMaxValue;
			fRatio = (float)m_iCulValue / (float)m_iCulMaxValue;
		}
		else if (m_iCulValue <= 0)
		{
			m_iCulValue = 0;
			fRatio = 0.1f / (float)m_iCulMaxValue;
		}
		else
		{
			fRatio = (float)m_iCulValue / (float)m_iCulMaxValue;
		}
		_float fOffsetX = (1.f - fRatio) * m_fSizeX * 0.5f;m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);

		m_pTransformCom->Scaling(m_fSizeX * fRatio, m_fSizeY, m_fZ);
 		m_pTransformCom->Set_State(STATE::POSITION, _float3{ (m_vWorldPos.x - m_iWinSizeX * 0.5f) - fOffsetX, -m_vWorldPos.y + m_iWinSizeY * 0.5f, m_vWorldPos.z });
		
		if (m_iCulMaxValue <= m_iCulValue)
			m_iCulValue = m_iCulMaxValue;

		m_iPreMaxValue = m_iCulMaxValue;
		m_iPreValue = m_iCulValue;
	}
}

void CProgressBar::Free()
{
	__super::Free();
}

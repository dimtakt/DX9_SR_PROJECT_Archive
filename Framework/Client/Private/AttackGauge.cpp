#include "AttackGauge.h"
#include "GameInstance.h"
CAttackGauge::CAttackGauge(LPDIRECT3DDEVICE9 pGraphic_Device) : CProgressBar(pGraphic_Device)
{
}

CAttackGauge::CAttackGauge(const CAttackGauge& Prototype) : CProgressBar(Prototype), m_eLevel(Prototype.m_eLevel)
{
}

void CAttackGauge::Render_Progress(CTransform* pTransform, _int iCulValue, _int iMaxValue)
{
	m_iCulValue = iCulValue;
	m_iCulMaxValue = iMaxValue;
	Target_Pos(pTransform);
	Target_ProgressBar(pTransform);
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CAttackGauge::Initialize_Prototype(LEVEL eLevel)
{
	m_eLevel = eLevel;

	if (FAILED(Ready_ChildPrototype(eLevel)))
		return E_FAIL;
	return S_OK;
}

HRESULT CAttackGauge::Initialize(void* pArg)
{
	m_iCulMaxValue = 200;
	m_iCulValue = 200;

	m_fSizeX = 70;
	m_fSizeY = 4;
	m_fX = 0;
	m_fY = 40;
	m_fZ = UI_DEPTH::FILED_HP;
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

void CAttackGauge::Priority_Update(_float fTimeDelta)
{

}

void CAttackGauge::Update(_float fTimeDelta)
{

}

void CAttackGauge::Late_Update(_float fTimeDelta)
{

}

HRESULT CAttackGauge::Render()
{
	m_pGraphic_Device->SetTexture(0, NULL);
	m_pVIBufferCom->Bind_Buffers();
	__super::Begin();
	m_pVIBufferCom->Render();
	__super::End();

	return S_OK;
}

HRESULT CAttackGauge::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Rect_UI_OrangeRect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CAttackGauge::Ready_ChildPrototype(LEVEL eLevel)
{


	return S_OK;
}

HRESULT CAttackGauge::Ready_Children()
{
	CUIObject* pGameObject = nullptr;


	return S_OK;
}

void CAttackGauge::Target_Pos(CTransform* pTransform)
{
	_float3 Target_Pos = pTransform->Get_State(STATE::POSITION);

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &m_OldViewMatrix);
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &m_OldProjMatrix);

	_float3 vWinPos{};
	_float4x4 vVPMatrix = m_OldViewMatrix * m_OldProjMatrix;
	D3DXVec3TransformCoord(&vWinPos, &Target_Pos, &vVPMatrix);

	_float fWinPosX = (vWinPos.x + 1.f) * 0.5 * g_iWinSizeX;
	_float fWinPosY = (-vWinPos.y + 1.f) * 0.5 * g_iWinSizeY;

	m_vWorldPos.x = fWinPosX - m_iWinSizeX * 0.5f;
	m_vWorldPos.y = -fWinPosY + m_iWinSizeY * 0.5f;


	m_pTransformCom->Set_State(STATE::POSITION, m_vWorldPos);
}

void CAttackGauge::Target_ProgressBar(CTransform* pTransform)
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
	_float fOffsetX = (1.f - fRatio) * m_fSizeX * 0.5f; m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);

	m_pTransformCom->Scaling(m_fSizeX * fRatio, m_fSizeY, m_fZ);
	m_pTransformCom->Set_State(STATE::POSITION, _float3{ m_vWorldPos.x - fOffsetX, m_vWorldPos.y + m_fY, m_vWorldPos.z });

	if (m_iCulMaxValue <= m_iCulValue)
		m_iCulValue = m_iCulMaxValue;

	m_iPreMaxValue = m_iCulMaxValue;
	m_iPreValue = m_iCulValue;

}

CAttackGauge* CAttackGauge::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
	CAttackGauge* pInstance = new CAttackGauge(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eLevel)))
	{
		MSG_BOX(TEXT("Failed to Created : CAttackGauge"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CAttackGauge::Clone(void* pArg)
{
	CAttackGauge* pInstance = new CAttackGauge(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CHp_Player"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CAttackGauge::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
}

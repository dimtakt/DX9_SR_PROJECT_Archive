#include "Talent_Progress.h"
#include "GameInstance.h"
#include "Client_Defines_Event.h"

CTalent_Progress::CTalent_Progress(LPDIRECT3DDEVICE9 pGraphic_Device) : CProgressBar(pGraphic_Device)
{
}

CTalent_Progress::CTalent_Progress(const CTalent_Progress& Prototype) : CProgressBar(Prototype)
{
}

HRESULT CTalent_Progress::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTalent_Progress::Initialize(void* pArg)
{
	m_iCulMaxValue = 100;
	m_iCulValue = 0;
	m_fSizeX = 170;
	m_fSizeY = 38;
	m_fX = 0;
	m_fY = -2;
	m_fZ = UI_DEPTH::TALENT;
	m_iWinSizeX = g_iWinSizeX;
	m_iWinSizeY = g_iWinSizeY;

	if (FAILED(CUIObject::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
	__super::Update_Position();

	m_pGameInstance->Subscribe(ENUM_CLASS(EVENT_TYPE::PLAYERTSTATCHANGE), this);

	return S_OK;
}

void CTalent_Progress::Priority_Update(_float fTimeDelta)
{

}

void CTalent_Progress::Update(_float fTimeDelta)
{

}

void CTalent_Progress::Late_Update(_float fTimeDelta)
{
	Progress_UpdateX();
	if (m_iCulValue != 0)
		m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CTalent_Progress::Render()
{
	m_pGraphic_Device->SetTexture(0, NULL);
	m_pVIBufferCom->Bind_Buffers();
	__super::Begin();
	m_pVIBufferCom->Render();
	if (m_isFontRender)
	__super::End();

	return S_OK;
}

void CTalent_Progress::Progress_Update(_float fCulValue)
{
	m_iCulValue = fCulValue;
}

HRESULT CTalent_Progress::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Rect_UI_OrangeRect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	return S_OK;
}

void CTalent_Progress::OnEvent(_uint iTypeindex, const EVENTDATA* pData)
{
	if (static_cast<EVENT_TYPE>(iTypeindex) == EVENT_TYPE::PLAYERTSTATCHANGE) {
		auto pStat = static_cast<const STATCHANGE*>(pData);
		if (pStat->eStatType == STAT_INFO::CULHP) {
			m_iCulValue = pStat->fValue;
		}
	}
}


CTalent_Progress* CTalent_Progress::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTalent_Progress* pInstance = new CTalent_Progress(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CTalent_Progress"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CTalent_Progress::Clone(void* pArg)
{
	CTalent_Progress* pInstance = new CTalent_Progress(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CTalent_Progress"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTalent_Progress::Free()
{
	m_pGameInstance->Unsubscribe(ENUM_CLASS(EVENT_TYPE::PLAYERTSTATCHANGE), this);
	__super::Free();
	Safe_Release(m_pVIBufferCom);
}
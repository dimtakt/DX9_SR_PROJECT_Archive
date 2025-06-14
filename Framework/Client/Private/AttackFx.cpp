#include "AttackFx.h"
#include "GameInstance.h"
#include "AttackGauge.h"
CAttackFx::CAttackFx(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CAttackFx::CAttackFx(const CAttackFx& Prototype) : CUIObject(Prototype), m_eLevel(Prototype.m_eLevel)
{
}

void CAttackFx::Render_Frame(CTransform* pTransform, _int iCulValue, _int iMaxValue)
{
	Target_Pos(pTransform);
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
	static_cast<CAttackGauge*>(m_vecChildren[0])->Render_Progress(pTransform, iCulValue, iMaxValue);

}

HRESULT CAttackFx::Initialize_Prototype(LEVEL eLevel)
{
	m_eLevel = eLevel;
	
	if (FAILED(Ready_ChildPrototype(eLevel)))
		return E_FAIL;
	return S_OK;
}

HRESULT CAttackFx::Initialize(void* pArg)
{
	m_fSizeX = 75;
	m_fSizeY = 7;
	m_fX = 0;
	m_fY = 40;
	m_fZ = UI_DEPTH::FILED_HP_FRAME;
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

void CAttackFx::Priority_Update(_float fTimeDelta)
{
}

void CAttackFx::Update(_float fTimeDelta)
{

}

void CAttackFx::Late_Update(_float fTimeDelta)
{
}

HRESULT CAttackFx::Render()
{
	m_pVIBufferCom->Bind_Buffers();
	__super::Begin();
	m_pVIBufferCom->Render();
	__super::End();

	return S_OK;
}

HRESULT CAttackFx::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Rect_UI_WhiteRect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	return S_OK;
}

void CAttackFx::Target_Pos(CTransform* pTransform)
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


	m_pTransformCom->Set_State(STATE::POSITION, _float3{ m_vWorldPos.x + m_fX, m_vWorldPos.y + m_fY, m_vWorldPos.z });
}

HRESULT CAttackFx::Ready_ChildPrototype(LEVEL eLevel)
{
	return S_OK;
}

HRESULT CAttackFx::Ready_Children()
{
	CUIObject* pGameObject = nullptr;

	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS( LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Attack_Gauge")));
	if (nullptr == pGameObject)
		return E_FAIL;
	Add_Child(pGameObject);

	return S_OK;
}

CAttackFx* CAttackFx::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
	CAttackFx* pInstance = new CAttackFx(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eLevel)))
	{
		MSG_BOX(TEXT("Failed to Created : CAttackFx"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CAttackFx::Clone(void* pArg)
{
	CAttackFx* pInstance = new CAttackFx(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CAttackFx"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CAttackFx::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
}

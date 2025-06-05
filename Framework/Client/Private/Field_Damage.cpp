#include "Field_Damage.h"
#include "GameInstance.h"

CField_Damage::CField_Damage(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CField_Damage::CField_Damage(const CField_Damage& Prototype) : CUIObject(Prototype), m_eLevel(Prototype.m_eLevel)
{
}

void CField_Damage::Render_HP_Frame(CTransform* pTransform)
{
	Target_Pos(pTransform);
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CField_Damage::Initialize_Prototype(LEVEL eLevel)
{
	m_eLevel = eLevel;
	return S_OK;
}

HRESULT CField_Damage::Initialize(void* pArg)
{
	m_fSizeX = 75;
	m_fSizeY = 15;
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

	return S_OK;
}

void CField_Damage::Priority_Update(_float fTimeDelta)
{
}

void CField_Damage::Update(_float fTimeDelta)
{

}

void CField_Damage::Late_Update(_float fTimeDelta)
{
}

HRESULT CField_Damage::Render()
{

	return S_OK;
}

HRESULT CField_Damage::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	return S_OK;
}

void CField_Damage::Target_Pos(CTransform* pTransform)
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


	m_pTransformCom->Set_State(STATE::POSITION, _float3{ m_vWorldPos.x, m_vWorldPos.y + m_fY, m_vWorldPos.z });
}

CField_Damage* CField_Damage::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
	CField_Damage* pInstance = new CField_Damage(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eLevel)))
	{
		MSG_BOX(TEXT("Failed to Created : CField_Damage"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CField_Damage::Clone(void* pArg)
{
	CField_Damage* pInstance = new CField_Damage(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CField_Damage"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CField_Damage::Free()
{
	__super::Free();
}

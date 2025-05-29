#include "Field_Hp_Frame.h"
#include "GameInstance.h"

CField_Hp_Frame::CField_Hp_Frame(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CField_Hp_Frame::CField_Hp_Frame(const CField_Hp_Frame& Prototype) : CUIObject(Prototype), m_eLevel(Prototype.m_eLevel)
{
}

HRESULT CField_Hp_Frame::Initialize_Prototype(LEVEL eLevel)
{
	m_eLevel = eLevel;
	return S_OK;
}

HRESULT CField_Hp_Frame::Initialize(void* pArg)
{
	UI_FIELD_HP_FREAM_DESC* Desc = static_cast<UI_FIELD_HP_FREAM_DESC*>(pArg);

	m_pTarget_TransformCom = Desc->pTarget_TransformCom;
	m_iTarget_Index = Desc->iTarget_Index;

	m_fSizeX = Desc->fSizeX;
	m_fSizeY = Desc->fSizeY;
	m_fX = Desc->fX;
	m_fY = Desc->fY;
	m_fZ = 0.3f;
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

void CField_Hp_Frame::Priority_Update(_float fTimeDelta)
{
}

void CField_Hp_Frame::Update(_float fTimeDelta)
{
	
}

void CField_Hp_Frame::Late_Update(_float fTimeDelta)
{
	Target_Pos();
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CField_Hp_Frame::Render()
{
	m_pVIBufferCom->Bind_Buffers();
	__super::Begin();
	m_pVIBufferCom->Render();
	__super::End();

	return S_OK;
}

HRESULT CField_Hp_Frame::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_Texture_Rect_UI_BalckRect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	return S_OK;
}

void CField_Hp_Frame::Target_Pos()
{
	_float3 Target_Pos = m_pTarget_TransformCom->Get_State(STATE::POSITION);

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

CField_Hp_Frame* CField_Hp_Frame::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
	CField_Hp_Frame* pInstance = new CField_Hp_Frame(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eLevel)))
	{
		MSG_BOX(TEXT("Failed to Created : CField_Hp_Frame"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CField_Hp_Frame::Clone(void* pArg)
{
	CField_Hp_Frame* pInstance = new CField_Hp_Frame(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CField_Hp_Frame"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CField_Hp_Frame::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
}

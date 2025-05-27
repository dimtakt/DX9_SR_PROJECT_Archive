#include "Field_Hp.h"
#include "GameInstance.h"
#include "Field_Hp_Frame.h"
CField_Hp::CField_Hp(LPDIRECT3DDEVICE9 pGraphic_Device) : CProgressBar(pGraphic_Device)
{
}

CField_Hp::CField_Hp(const CField_Hp& Prototype) : CProgressBar(Prototype), m_iTarget_Index(Prototype.m_iTarget_Index), m_eLevel(Prototype.m_eLevel)
{
}

HRESULT CField_Hp::Initialize_Prototype(LEVEL eLevel)
{
	m_eLevel = eLevel;

	if (FAILED(Ready_ChildPrototype(eLevel)))
		return E_FAIL;
}

HRESULT CField_Hp::Initialize(void* pArg)
{
	UI_FIELD_HP_DESC* Desc = static_cast<UI_FIELD_HP_DESC*>(pArg);

	m_iTarget_Index = Desc->iTarget_Index;
	
	if (FAILED(Reday_SyncingObject()))
		return E_FAIL;

	m_iCulMaxValue = 200;
	m_iCulValue = 200;

	m_fSizeX = 70.f;
	m_fSizeY = 10.f;
	m_fX = 0;
	m_fY = Desc->fY;
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

void CField_Hp::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CField_Hp::Update(_float fTimeDelta)
{	
	__super::Update(fTimeDelta);
}

void CField_Hp::Late_Update(_float fTimeDelta)
{
	Target_Pos();
	Target_ProgressBar();
	__super::Late_Update(fTimeDelta);
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CField_Hp::Render()
{
	m_pVIBufferCom->Bind_Buffers();
	__super::Begin();
	m_pVIBufferCom->Render();
	__super::End();

	return S_OK;
}

HRESULT CField_Hp::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_Texture_Rect_UI_Hud_States_Hp"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CField_Hp::Ready_ChildPrototype(LEVEL eLevel)
{
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_Field_Hp_Fream"),
		CField_Hp_Frame::Create(m_pGraphic_Device, eLevel))))
		return E_FAIL;

	return S_OK;
}

HRESULT CField_Hp::Ready_Children()
{
	CUIObject* pGameObject = nullptr;

	CField_Hp_Frame::UI_FIELD_HP_FREAM_DESC Field_HP_Desc{};
	Field_HP_Desc.fSizeX = m_fSizeX + 6.f;
	Field_HP_Desc.fSizeY = m_fSizeY + 6.f;
	Field_HP_Desc.iTarget_Index = m_iTarget_Index;
	Field_HP_Desc.pTarget_TransformCom = m_pTarget_TransformCom;
	Field_HP_Desc.fX = m_fX;
	Field_HP_Desc.fY = m_fY;

	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_Field_Hp_Fream"), &Field_HP_Desc));
	if (nullptr == pGameObject)
		return E_FAIL;
	Add_Child(pGameObject);

	return S_OK;
}

HRESULT CField_Hp::Reday_SyncingObject()
{
	m_pTarget_TransformCom = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Layer_Player"), TEXT("Com_Transform"), m_iTarget_Index));
	if (m_pTarget_TransformCom == nullptr)
	{
		MSG_BOX(TEXT("Failed to Syncing : CField_Hp"));
		return E_FAIL;
	}
	Safe_AddRef(m_pTarget_TransformCom);

	return S_OK;
}

void CField_Hp::Target_Pos()
{
	_float3 Target_Pos = m_pTarget_TransformCom->Get_State(STATE::POSITION);

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &m_OldViewMatrix);
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &m_OldProjMatrix);

	_float3 vWinPos{};
	_float4x4 vVPMatrix = m_OldViewMatrix * m_OldProjMatrix;
	D3DXVec3TransformCoord(&vWinPos, &Target_Pos, &vVPMatrix);

	_int WinPosX = (vWinPos.x + 1.f) * 0.5 * g_iWinSizeX;
	_int WinPosY = (-vWinPos.y + 1.f) * 0.5 * g_iWinSizeY;

	m_vWorldPos.x = WinPosX - m_iWinSizeX * 0.5f;
	m_vWorldPos.y = -WinPosY + m_iWinSizeY * 0.5f;

	m_pTransformCom->Set_State(STATE::POSITION, m_vWorldPos);
}

void CField_Hp::Target_ProgressBar()
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

CField_Hp* CField_Hp::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
	CField_Hp* pInstance = new CField_Hp(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eLevel)))
	{
		MSG_BOX(TEXT("Failed to Created : CField_Hp"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CField_Hp::Clone(void* pArg)
{
	CField_Hp* pInstance = new CField_Hp(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CHp_Player"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CField_Hp::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTarget_TransformCom);
}

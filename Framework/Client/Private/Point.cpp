#include "Point.h"
#include "GameInstance.h"
#include "Transform.h"

CPoint::CPoint(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CPoint::CPoint(const CPoint& Prototype)
	: CGameObject{ Prototype }
{
}

HRESULT CPoint::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPoint::Initialize(void* pArg)
{
	/*if (FAILED(Ready_Components(pArg)))
		return E_FAIL;*/

	POINTDESC* desc = static_cast<POINTDESC*>(pArg);

	if (FAILED(Ready_Target(desc)))
		return E_FAIL;
	// 기본 광원 정보 등록
	//LIGHTDATA light;
	desc->desc.eType = LIGHT_TYPE::POINT;
	desc->desc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	desc->desc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);
	desc->desc.vAmbient = _float4(0.01f, 0.01f, 0.01f, 1.f); // 기본 어두움 유지
	desc->desc.fSpecPower = 64.f;
	desc->desc.fRange = 8.f;

	m_strLightID = desc->strLightID;

	LIGHTDATA* light = static_cast<LIGHTDATA*>(desc);

	m_pGameInstance->Add_Light(m_strLightID, *light);

	return S_OK;
}

void CPoint::Priority_Update(_float fTimeDelta)
{

}

void CPoint::Update(_float fTimeDelta)
{
	if (m_pTargetTransformCom)
	{
		const _float3 vTargetPos = m_pTargetTransformCom->Get_State(STATE::POSITION);

		_float3 vLightPos = vTargetPos + _float3(0.f, 1.f, 0.f);

		//_float3 vLightDir = _float3(-1.f, -1.f, 0.f); // 정면에서 보이는 사선
		//D3DXVec3Normalize(&vLightDir, &vLightDir);

		m_pGameInstance->Update_LightPosition(m_strLightID, vLightPos);
		//m_pGameInstance->Update_LightDirection(m_strLightID, vLightDir);
	}
}

void CPoint::Late_Update(_float fTimeDelta)
{
}

HRESULT CPoint::Render()
{
	return S_OK;
}

HRESULT CPoint::Ready_Components()
{
	//CTransform::TRANSFORM_DESC TransformDesc{};
	//TransformDesc.fSpeedPerSec = 5.f;
	//TransformDesc.fRotationPerSec = D3DXToRadian(90.f);
	//if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
	//	TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CPoint::Ready_Target(POINTDESC* pDesc)
{
	m_pTargetTransformCom = pDesc->pTargetTransform;
	Safe_AddRef(m_pTargetTransformCom);
	return S_OK;
}

CPoint* CPoint::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPoint* pInstance = new CPoint(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failde to Created : CBackGround"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPoint::Clone(void* pArg)
{
	CPoint* pInstance = new CPoint(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failde to Cloned : CPoint"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPoint::Free()
{
	__super::Free();
	Safe_Release(m_pTargetTransformCom);
}

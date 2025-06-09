#include "Sun.h"
#include "GameInstance.h"
#include "Transform.h"

CSun::CSun(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CSun::CSun(const CSun& Prototype)
	: CGameObject{Prototype}
{
}

HRESULT CSun::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSun::Initialize(void* pArg)
{
	/*if (FAILED(Ready_Components(pArg)))
		return E_FAIL;*/

	SUNDESC* desc = static_cast<SUNDESC*>(pArg);

	if(FAILED(Ready_Target(desc)))
		return E_FAIL; 
	LIGHTDATA light;

	light.desc.eType = desc->desc.eType;
	light.desc.vDiffuse = desc->desc.vDiffuse;
	light.desc.vSpecular = desc->desc.vSpecular;
	light.desc.vAmbient = desc->desc.vAmbient;
	light.desc.fSpecPower = desc->desc.fSpecPower;

	m_strLightID = TEXT("Sun");

	m_pGameInstance->Add_Light(m_strLightID, light);

	m_pGameInstance->Update_LightPosition(m_strLightID, desc->vLightPos);
	m_pGameInstance->Update_LightDirection(m_strLightID, desc->vLightDir);

	return S_OK;
	return S_OK;
}

void CSun::Priority_Update(_float fTimeDelta)
{

}

void CSun::Update(_float fTimeDelta)
{
	//if (m_pTargetTransformCom)
	//{
	//	const _float3 vTargetPos = m_pTargetTransformCom->Get_State(STATE::POSITION);

	//	_float3 vLightPos = vTargetPos + _float3(+3.f, +3.f, 0.f);

	//	_float3 vLightDir = _float3(-1.f, -1.f, 0.f); // 정면에서 보이는 사선
	//	D3DXVec3Normalize(&vLightDir, &vLightDir);

	//	m_pGameInstance->Update_LightPosition(m_strLightID, vLightPos);
	//	m_pGameInstance->Update_LightDirection(m_strLightID, vLightDir);
	//}
}

void CSun::Late_Update(_float fTimeDelta)
{
}

HRESULT CSun::Render()
{
	return S_OK;
}

HRESULT CSun::Ready_Components()
{
	//CTransform::TRANSFORM_DESC TransformDesc{};
	//TransformDesc.fSpeedPerSec = 5.f;
	//TransformDesc.fRotationPerSec = D3DXToRadian(90.f);
	//if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
	//	TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CSun::Ready_Target(SUNDESC* pDesc)
{
	/*m_pTargetTransformCom = pDesc->pTargetTransform;
	Safe_AddRef(m_pTargetTransformCom);*/
	return S_OK;
}

CSun* CSun::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSun* pInstance = new CSun(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failde to Created : CBackGround"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSun::Clone(void* pArg)
{
	CSun* pInstance = new CSun(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failde to Cloned : CSun"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSun::Free()
{
	__super::Free();
	//Safe_Release(m_pTargetTransformCom);
}

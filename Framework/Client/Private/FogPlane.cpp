#include "FogPlane.h"
#include "Client_Struct.h"
#include "GameInstance.h"


CFogPlane::CFogPlane(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CFogPlane::CFogPlane(const CFogPlane& Prototype)
	: CGameObject{ Prototype }
{
}

HRESULT CFogPlane::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CFogPlane::Initialize(void* pArg)
{

	if (FAILED(Ready_Components()))
		return E_FAIL;

	MAP_OBJECT_DESC* desc = static_cast<MAP_OBJECT_DESC*>(pArg);
	m_pTransformCom->Set_State(STATE::POSITION, desc->vPos + _float3(0.f, 1.15f, 0.f));
	m_pTransformCom->Scaling(desc->vScale.x, desc->vScale.y + 18.f, desc->vScale.z);
	m_pTransformCom->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));
	m_eObjType = GAMEOBJ_TYPE::FOG;

	m_pVIBufferCom->ChangeUV_FlipX(false);
	return S_OK;
}

void CFogPlane::Priority_Update(_float fTimeDelta)
{

}

void CFogPlane::Update(_float fTimeDelta)
{


}

void CFogPlane::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_BLEND, this);
}

HRESULT CFogPlane::Render()
{

	//m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, FALSE);
	//m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE); // 돌아가도 보이게 설정

	//m_pTransformCom->Bind_Matrix();

	//m_pVIBufferCom->Bind_Buffers();

	//m_pTextureCom->Bind_Texture(0); // 위
	//m_pVIBufferCom->Render();


	//m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, TRUE);

	_float4x4 ViewMatrix, ProjMatrix;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &ProjMatrix);

	if (FAILED(m_pShaderCom->Bind_Matrix("gWorldMatrix", m_pTransformCom->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("gViewMatrix", &ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("gProjMatrix", &ProjMatrix)))
		return E_FAIL;

	_float fTime = static_cast<_float>(GetTickCount()) * 0.01f;
	if (FAILED(m_pShaderCom->Set_Float("g_Time", fTime)))
		return E_FAIL;	
	if (FAILED(m_pShaderCom->Set_Float("g_FogHeight", 1.5f)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_Float("g_MaxAlpha", 0.35f)))
		return E_FAIL;

	m_pTextureCom->Bind_Texture(m_pShaderCom, "gTexture", 0);

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();

	m_pShaderCom->End();

	m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

HRESULT CFogPlane::Ready_Components()
{

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer_Rect"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For. Com_Texture_FogPlane_Top*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Fog"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC		TransformDesc{};
	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform_FogPlane"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	///* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Shader_Fog"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	return S_OK;
}


CFogPlane* CFogPlane::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CFogPlane* pInstance = new CFogPlane(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CFogPlane"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CFogPlane::Clone(void* pArg)
{
	CFogPlane* pInstance = new CFogPlane(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CFogPlane"));
		Safe_Release(pInstance);
	}

	return pInstance;
}
void CFogPlane::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
}

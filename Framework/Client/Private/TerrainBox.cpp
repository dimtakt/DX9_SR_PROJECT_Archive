#include "TerrainBox.h"
#include "Client_Struct.h"
#include "GameInstance.h"
#include "Shader.h"

CTerrainBox::CTerrainBox(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CTerrainBox::CTerrainBox(const CTerrainBox& Prototype)
	: CGameObject{ Prototype }
{
}

HRESULT CTerrainBox::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTerrainBox::Initialize(void* pArg)
{

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (pArg)
	{
		MAP_OBJECT_DESC* desc = static_cast<MAP_OBJECT_DESC*>(pArg);
		m_pTransformCom->Set_State(STATE::POSITION, desc->vPos);
		m_pTransformCom->Scaling(desc->vScale.x, desc->vScale.y, desc->vScale.z);
		m_iTopTextureIndex = desc->iTextureIndex;
		m_eObjType = GAMEOBJ_TYPE::TERRAIN;
	}
	else
	{
		m_pTransformCom->Scaling(20.f, 2.f, 20.f);	//설정 없으면 전체 기본 베이스
		m_iTopTextureIndex = 0;   
		m_iSideTextureIndex = 0;
		m_eObjType = GAMEOBJ_TYPE::TERRAIN;
	}

	_float4 vMaterialSpecular = _float4(0.2f, 0.2f, 0.2f, 1.f);
	m_pShaderCom->Set_Vector("gMaterialSpecular", &vMaterialSpecular);

	return S_OK;
}

void CTerrainBox::Priority_Update(_float fTimeDelta)
{

}

void CTerrainBox::Update(_float fTimeDelta)
{


}

void CTerrainBox::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CTerrainBox::Render()
{
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, FALSE); 
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE); // 돌아가도 보이게 설정

	m_pTransformCom->Bind_Matrix();

	m_pVIBufferCom->Bind_Buffers();

	//_float4x4 ViewMatrix, ProjMatrix;
	//m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	//m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &ProjMatrix);

	//if (FAILED(m_pShaderCom->Bind_Matrix("gWorldMatrix", m_pTransformCom->Get_WorldMatrix())))
	//	return E_FAIL;
	//if (FAILED(m_pShaderCom->Bind_Matrix("gViewMatrix", &ViewMatrix)))
	//	return E_FAIL;
	//if (FAILED(m_pShaderCom->Bind_Matrix("gProjMatrix", &ProjMatrix)))
	//	return E_FAIL;

	//vector<_wstring> vecLightKeys = { L"Sun" };
	//m_pGameInstance->Apply_ToShader(m_pShaderCom, vecLightKeys);

	//m_pTextureTop->Bind_Texture(m_pShaderCom, "gTexture", m_iTopTextureIndex);

	//m_pShaderCom->Begin(0);

	//m_pVIBufferCom->Render_Subset(0);

	//m_pShaderCom->End();

	//m_pTextureSide->Bind_Texture(m_pShaderCom, "gTexture", m_iTopTextureIndex);

	//m_pShaderCom->Begin(0);


	//m_pVIBufferCom->Render_Subset(2); // 왼
	//m_pVIBufferCom->Render_Subset(3); // 오
	//m_pVIBufferCom->Render_Subset(4); // 앞
	//m_pVIBufferCom->Render_Subset(5); // 뒤

	//m_pShaderCom->End();
 	m_pTextureTop->Bind_Texture(m_iTopTextureIndex); // 위
	m_pVIBufferCom->Render_Subset(0);

	m_pTextureSide->Bind_Texture(m_iSideTextureIndex);
	//m_pVIBufferCom->Render_Subset(1); // 아래 랜더 제외
	m_pVIBufferCom->Render_Subset(2); // 왼
	m_pVIBufferCom->Render_Subset(3); // 오
	m_pVIBufferCom->Render_Subset(4); // 앞
	m_pVIBufferCom->Render_Subset(5); // 뒤

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	return S_OK;
}

HRESULT CTerrainBox::Ready_Components()
{
	///* For.Com_VIBuffer_TerrainBox*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_TerrainBox"),
		TEXT("Com_VIBuffer_TerrainBox"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For. Com_Texture_TerrainBox_Top*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_TerrainBox_Top"),
		TEXT("Com_Texture_Terrain_Top"), reinterpret_cast<CComponent**>(&m_pTextureTop))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_TerrainBox_Side"),
		TEXT("Com_Texture_Terrain_Side"), reinterpret_cast<CComponent**>(&m_pTextureSide))))
		return E_FAIL;


	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC		TransformDesc{};
	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform_TerrainBox"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	///* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Shader_Light"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CTerrainBox::Ready_Material()
{
	D3DMATERIAL9		MtrlDesc{};

	MtrlDesc.Diffuse = { 1.f, 1.f, 1.f, 1.f };
	MtrlDesc.Specular = { 1.f, 1.f, 1.f, 1.f };
	MtrlDesc.Ambient = { 1.f, 1.f, 1.f, 1.f };

	MtrlDesc.Emissive = { 0.f, 0.f, 0.f, 0.f };
	MtrlDesc.Power = 1.f;
	m_pGraphic_Device->SetMaterial(&MtrlDesc);

	return S_OK;
}

void CTerrainBox::SetUp_OnTerrainBox(CTransform* pTransformCom, _float3 vOffset)
{
	if (this == nullptr)
		return;

	_float3     vWorldPos = pTransformCom->Get_State(STATE::POSITION);

	_float3     vLocalPos{};
	D3DXVec3TransformCoord(&vLocalPos, &vWorldPos, m_pTransformCom->Get_WorldMatrix_Inverse());

	vLocalPos.x = m_pVIBufferCom->Compute_Right(vLocalPos.x, vOffset.x);
	vLocalPos.y = m_pVIBufferCom->Compute_Height(vLocalPos) + vOffset.y;
	vLocalPos.z = m_pVIBufferCom->Compute_Look(vLocalPos.z, vOffset.z);

	D3DXVec3TransformCoord(&vWorldPos, &vLocalPos, m_pTransformCom->Get_WorldMatrix());

	pTransformCom->Set_State(STATE::POSITION, vWorldPos);
}

CTerrainBox* CTerrainBox::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTerrainBox* pInstance = new CTerrainBox(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CTerrainBox"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CTerrainBox::Clone(void* pArg)
{
	CTerrainBox* pInstance = new CTerrainBox(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CTerrainBox"));
		Safe_Release(pInstance);
	}

	return pInstance;
}
void CTerrainBox::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureTop);
	Safe_Release(m_pTextureSide);
	Safe_Release(m_pShaderCom);
}

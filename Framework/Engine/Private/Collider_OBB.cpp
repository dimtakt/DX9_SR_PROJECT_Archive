// CCollider_OBB.cpp
#include "Collider_OBB.h"
#include "VIBuffer_Cube.h"
#include "GameInstance.h"

USING(Engine)

CCollider_OBB::CCollider_OBB(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCollider(pGraphic_Device)
{
}

CCollider_OBB::CCollider_OBB(const CCollider_OBB& Prototype)
	: CCollider(Prototype)
{
	// 복사 생성 시 VIBuffer도 클론 필요 (필요 시 구현)
}

HRESULT CCollider_OBB::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCollider_OBB::Initialize(void* pArg)
{
	OBB_DESC* desc = static_cast<OBB_DESC*>(pArg);
	if (desc == nullptr)
		return E_FAIL;

	m_pOwner = desc->pOwner;
	m_pTransformRef = desc->pTransform;
	m_vScale = desc->vScale;

	return S_OK;
}

void CCollider_OBB::Update_Collider()
{
	if (!m_pTransformRef)
		return;

	m_vAxis[0] = m_pTransformRef->Get_State(STATE::RIGHT);
	m_vAxis[1] = m_pTransformRef->Get_State(STATE::UP);
	m_vAxis[2] = m_pTransformRef->Get_State(STATE::LOOK);

	for (int i = 0; i < 3; ++i)
		D3DXVec3Normalize(&m_vAxis[i], &m_vAxis[i]);

	m_vWorldCenter = m_pTransformRef->Get_State(STATE::POSITION);

	_float3 vRight = m_pTransformRef->Get_State(STATE::RIGHT);
	_float3 vUp = m_pTransformRef->Get_State(STATE::UP);
	_float3 vLook = m_pTransformRef->Get_State(STATE::LOOK);

	m_vWorldExtents.x = D3DXVec3Length(&vRight) * m_vScale.x;
	m_vWorldExtents.y = D3DXVec3Length(&vUp) * m_vScale.y;
	m_vWorldExtents.z = D3DXVec3Length(&vLook) * m_vScale.z;
}

void CCollider_OBB::Get_MatrixData(_float3& vCenter, _float3& vExtent, _float3* vAxis)
{
	vCenter = m_vWorldCenter;
	vExtent = m_vWorldExtents;
	memcpy(vAxis, m_vAxis, sizeof(_float3) * 3);
}

HRESULT CCollider_OBB::Render()
{
	if(!m_pTransformRef)
		return E_FAIL;

	// 꼭짓점 계산
	_float3 vCorner[8];
	for (int i = 0; i < 8; ++i)
	{
		vCorner[i] = m_vWorldCenter
			+ m_vAxis[0] * m_vWorldExtents.x * ((i & 1) ? 1.f : -1.f)
			+ m_vAxis[1] * m_vWorldExtents.y * ((i & 2) ? 1.f : -1.f)
			+ m_vAxis[2] * m_vWorldExtents.z * ((i & 4) ? 1.f : -1.f);
	}

	struct VTX_LINE { D3DXVECTOR3 vPos; D3DCOLOR dwColor; };
	VTX_LINE vLines[24] = {
		{vCorner[0], D3DCOLOR_ARGB(255, 255, 0, 0)}, {vCorner[1], D3DCOLOR_ARGB(255, 255, 0, 0)},
		{vCorner[1], D3DCOLOR_ARGB(255, 255, 0, 0)}, {vCorner[3], D3DCOLOR_ARGB(255, 255, 0, 0)},
		{vCorner[3], D3DCOLOR_ARGB(255, 255, 0, 0)}, {vCorner[2], D3DCOLOR_ARGB(255, 255, 0, 0)},
		{vCorner[2], D3DCOLOR_ARGB(255, 255, 0, 0)}, {vCorner[0], D3DCOLOR_ARGB(255, 255, 0, 0)},

		{vCorner[4], D3DCOLOR_ARGB(255, 255, 0, 0)}, {vCorner[5], D3DCOLOR_ARGB(255, 255, 0, 0)},
		{vCorner[5], D3DCOLOR_ARGB(255, 255, 0, 0)}, {vCorner[7], D3DCOLOR_ARGB(255, 255, 0, 0)},
		{vCorner[7], D3DCOLOR_ARGB(255, 255, 0, 0)}, {vCorner[6], D3DCOLOR_ARGB(255, 255, 0, 0)},
		{vCorner[6], D3DCOLOR_ARGB(255, 255, 0, 0)}, {vCorner[4], D3DCOLOR_ARGB(255, 255, 0, 0)},

		{vCorner[0], D3DCOLOR_ARGB(255, 255, 0, 0)}, {vCorner[4], D3DCOLOR_ARGB(255, 255, 0, 0)},
		{vCorner[1], D3DCOLOR_ARGB(255, 255, 0, 0)}, {vCorner[5], D3DCOLOR_ARGB(255, 255, 0, 0)},
		{vCorner[2], D3DCOLOR_ARGB(255, 255, 0, 0)}, {vCorner[6], D3DCOLOR_ARGB(255, 255, 0, 0)},
		{vCorner[3], D3DCOLOR_ARGB(255, 255, 0, 0)}, {vCorner[7], D3DCOLOR_ARGB(255, 255, 0, 0)}
	};

	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphic_Device->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
	m_pGraphic_Device->DrawPrimitiveUP(D3DPT_LINELIST, 12, vLines, sizeof(VTX_LINE));


	return S_OK;
}

CCollider_OBB* CCollider_OBB::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCollider_OBB* pInstance = new CCollider_OBB(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CCollider_OBB"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CCollider_OBB::Clone(void* pArg)
{
	CCollider_OBB* pInstance = new CCollider_OBB(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("CCollider_OBB Clone Failed"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCollider_OBB::Free()
{
	__super::Free();
	m_pTransformRef = nullptr;
}

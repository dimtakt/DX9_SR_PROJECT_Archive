// CCollider_OBB.cpp
#include "Collider_OBB.h"
#include "GameInstance.h"

USING(Engine)

CCollider_OBB::CCollider_OBB(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCollider(pGraphic_Device)
{
}

CCollider_OBB::CCollider_OBB(const CCollider_OBB& Prototype)
	: CCollider(Prototype)
{
	D3DXCreateBox(m_pGraphic_Device, 2.f, 2.f, 2.f, &m_pBoxMesh, nullptr);
}

HRESULT CCollider_OBB::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCollider_OBB::Initialize(void* pArg)
{
	OBB_DESC* desc = static_cast<OBB_DESC*>(pArg);
	if (desc != nullptr)
	{
		m_vScale = desc->vScale;
		m_pOwner = desc->pOwner;
	}

	return S_OK;
}

HRESULT CCollider_OBB::Render()
{
	if (!m_pBoxMesh)
		return E_FAIL;

	D3DXMATRIX matScale, matAxis, matTrans, matWorld;

	D3DXMatrixScaling(&matScale, m_vWorldExtents.x * 2.f, m_vWorldExtents.y * 2.f, m_vWorldExtents.z * 2.f);

	matAxis._11 = m_vAxis[0].x; matAxis._12 = m_vAxis[0].y; matAxis._13 = m_vAxis[0].z; matAxis._14 = 0.f;
	matAxis._21 = m_vAxis[1].x; matAxis._22 = m_vAxis[1].y; matAxis._23 = m_vAxis[1].z; matAxis._24 = 0.f;
	matAxis._31 = m_vAxis[2].x; matAxis._32 = m_vAxis[2].y; matAxis._33 = m_vAxis[2].z; matAxis._34 = 0.f;
	matAxis._41 = 0.f;         matAxis._42 = 0.f;         matAxis._43 = 0.f;         matAxis._44 = 1.f;

	D3DXMatrixTranslation(&matTrans, m_vWorldCenter.x, m_vWorldCenter.y, m_vWorldCenter.z);

	matWorld = matScale * matAxis * matTrans;
	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &matWorld);

	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pBoxMesh->DrawSubset(0);
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, TRUE);

	return S_OK;
}

void CCollider_OBB::Update_Collider(const CTransform* pTransform)
{
	_float3 vRight{}, vUp{}, vLook{}, vPos{};

	vRight = pTransform->Get_State(STATE::RIGHT);
	vUp = pTransform->Get_State(STATE::UP);
	vLook = pTransform->Get_State(STATE::LOOK);
	vPos = pTransform->Get_State(STATE::POSITION);

	m_vAxis[0] = vRight;
	m_vAxis[1] = vUp;
	m_vAxis[2] = vLook;

	for (int i = 0; i < 3; ++i)
		D3DXVec3Normalize(&m_vAxis[i], &m_vAxis[i]);

	m_vWorldCenter = vPos;

	m_vWorldExtents.x = D3DXVec3Length(&vRight) * m_vScale.x;
	m_vWorldExtents.y = D3DXVec3Length(&vUp) * m_vScale.y;
	m_vWorldExtents.z = D3DXVec3Length(&vLook) * m_vScale.z;
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
	Safe_Release(m_pBoxMesh);
}

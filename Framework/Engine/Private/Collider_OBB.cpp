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
	m_eOwnerObjType = desc->eType;

	return S_OK;
}

void CCollider_OBB::Update_Collider()
{
	//if (!m_pTransformRef)
	//	return;

	//if (m_pOwner == nullptr || m_pOwner->Get_IsDead())
	//{
	//	m_bDead = true;
	//	return;
	//}

	//// 1. 방향 벡터 추출
	//m_vAxis[0] = m_pTransformRef->Get_State(STATE::RIGHT);
	//m_vAxis[1] = m_pTransformRef->Get_State(STATE::UP);
	//m_vAxis[2] = m_pTransformRef->Get_State(STATE::LOOK);

	//// 2. 정규화
	//D3DXVec3Normalize(&m_vAxis[0], &m_vAxis[0]);
	//D3DXVec3Normalize(&m_vAxis[1], &m_vAxis[1]);
	//D3DXVec3Normalize(&m_vAxis[2], &m_vAxis[2]);

	//// 3. 반크기 (scale만 적용)
	//_float3 vScale = m_pTransformRef->Get_Scaled();
	//m_vWorldExtents.x = (vScale.x * m_vScale.x) * 0.5f;
	//m_vWorldExtents.y = (vScale.y * m_vScale.y) * 0.5f;
	//m_vWorldExtents.z = (vScale.z * m_vScale.z) * 0.5f;

	//// 4. 중심 보정
	//m_vWorldCenter = m_pTransformRef->Get_State(STATE::POSITION);

	if (!m_pTransformRef)
		return;

	if (m_pOwner == nullptr || m_pOwner->Get_IsDead())
	{
		m_bDead = true;
		return;
	}

	// 1. 회전 포함 축 벡터 (3D 축 사용)
	m_vAxis[0] = m_pTransformRef->Get_State(STATE::RIGHT);
	m_vAxis[1] = m_pTransformRef->Get_State(STATE::UP);
	m_vAxis[2] = m_pTransformRef->Get_State(STATE::LOOK);

	D3DXVec3Normalize(&m_vAxis[0], &m_vAxis[0]);
	D3DXVec3Normalize(&m_vAxis[1], &m_vAxis[1]);
	D3DXVec3Normalize(&m_vAxis[2], &m_vAxis[2]);

	// 2. 스케일 계산 (보정 포함)
	_float3 vScale = m_pTransformRef->Get_Scaled();

	m_vWorldExtents.x = max(vScale.x * m_vScale.x * 0.5f, 0.0001f);
	m_vWorldExtents.y = max(vScale.y * m_vScale.y * 0.5f, 0.0001f);
	m_vWorldExtents.z = max(vScale.z * m_vScale.z * 0.5f, 0.0001f);

	// 3. 중심 위치
	m_vWorldCenter = m_pTransformRef->Get_State(STATE::POSITION);

}

HRESULT CCollider_OBB::Render()
{
	//if (!m_pGraphic_Device)
	//	return E_FAIL;

	//// 1. 꼭짓점 좌표 계산 (XZ 평면 기준, 정밀 오차 보정)
	//const _float3& vCenter = m_vWorldCenter;
	//const _float3& vRight = m_vAxis[0]; // 정규화된 X축
	//const _float3& vLook = m_vAxis[2]; // 정규화된 Z축

	//const _float fExtentX = m_vWorldExtents.x;
	//const _float fExtentZ = m_vWorldExtents.z;

	//_float3 vCorner[5]; // 5개로 닫음 (마지막은 0번)

	//// 시계 방향 정점 계산
	//vCorner[0] = vCenter + (-vRight * fExtentX) + (-vLook * fExtentZ);
	//vCorner[1] = vCenter + (vRight * fExtentX) + (-vLook * fExtentZ);
	//vCorner[2] = vCenter + (vRight * fExtentX) + (vLook * fExtentZ);
	//vCorner[3] = vCenter + (-vRight * fExtentX) + (vLook * fExtentZ);
	//vCorner[4] = vCorner[0]; // 사각형 닫기

	//// 2. 정점 구조체 정의 (위치 + 색상)
	//struct VertexColor
	//{
	//	_float3 vPos;
	//	D3DCOLOR dwColor;
	//};

	//VertexColor vLine[5];
	//for (int i = 0; i < 5; ++i)
	//{
	//	vLine[i].vPos = vCorner[i];
	//	vLine[i].dwColor = D3DCOLOR_ARGB(255, 255, 0, 0); // 빨간색
	//}

	//_float4x4 matWorld;
	//D3DXMatrixIdentity(&matWorld);
	//m_pGraphic_Device->SetTransform(D3DTS_WORLD, &matWorld);



	//m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, FALSE);
	//m_pGraphic_Device->SetTexture(0, nullptr);
	//m_pGraphic_Device->SetPixelShader(nullptr);
	//m_pGraphic_Device->SetVertexShader(nullptr);

	//m_pGraphic_Device->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
	//m_pGraphic_Device->DrawPrimitiveUP(D3DPT_LINESTRIP, 4, vLine, sizeof(VertexColor));


	//m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, TRUE);
	const _float3& vCenter = m_vWorldCenter;
	const _float3& vRight = m_vAxis[0];   // X축
	const _float3& vUp = m_vAxis[1];   // Y축
	const _float3& vLook = m_vAxis[2];   // Z축

	const _float fExtentX = m_vWorldExtents.x;
	const _float fExtentY = m_vWorldExtents.y;
	const _float fExtentZ = m_vWorldExtents.z;

	// 8개 꼭짓점 계산
	_float3 vCorners[8];
	int i = 0;
	for (int dx = -1; dx <= 1; dx += 2)
		for (int dy = -1; dy <= 1; dy += 2)
			for (int dz = -1; dz <= 1; dz += 2)
			{
				vCorners[i++] = vCenter
					+ dx * fExtentX * vRight
					+ dy * fExtentY * vUp
					+ dz * fExtentZ * vLook;
			}

	// 정점 구조체 정의
	struct VertexColor { 
		_float3 vPos; 
		D3DCOLOR dwColor; };


	VertexColor vLine[24];
	D3DCOLOR color = D3DCOLOR_ARGB(255, 255, 0, 0);

	auto SetLine = [&](int idx, int a, int b)
		{
			vLine[idx * 2 + 0] = { vCorners[a], color };
			vLine[idx * 2 + 1] = { vCorners[b], color };
		};

	// 12개 라인 설정 (선 연결)
	SetLine(0, 0, 1); SetLine(1, 1, 3); SetLine(2, 3, 2); SetLine(3, 2, 0); // 아래면
	SetLine(4, 4, 5); SetLine(5, 5, 7); SetLine(6, 7, 6); SetLine(7, 6, 4); // 윗면
	SetLine(8, 0, 4); SetLine(9, 1, 5); SetLine(10, 2, 6); SetLine(11, 3, 7); // 옆면

	m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, FALSE);
	_float4x4 matWorld;
	D3DXMatrixIdentity(&matWorld);
	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &matWorld);
	m_pGraphic_Device->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
	m_pGraphic_Device->DrawPrimitiveUP(D3DPT_LINELIST, 12, vLine, sizeof(VertexColor));
	m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, TRUE);
	



	return S_OK;
}

void CCollider_OBB::Render_AttachedDebugBox(LPDIRECT3DDEVICE9 pDevice, const _float3& vCenter, const _float3& vRight, const _float3& vLook, const _float3& vScale, D3DCOLOR color)
{
	// 1. 축 벡터 정규화
	_float3 normRight = vRight;
	_float3 normLook = vLook;
	D3DXVec3Normalize(&normRight, &normRight);
	D3DXVec3Normalize(&normLook, &normLook);

	// 2. 반 크기
	const _float fExtentX = vScale.x * 0.5f;
	const _float fExtentZ = vScale.z * 0.5f;

	// 3. 꼭짓점 계산
	_float3 vCorner[5];
	vCorner[0] = vCenter + (-normRight * fExtentX) + (-normLook * fExtentZ);
	vCorner[1] = vCenter + (normRight * fExtentX) + (-normLook * fExtentZ);
	vCorner[2] = vCenter + (normRight * fExtentX) + (normLook * fExtentZ);
	vCorner[3] = vCenter + (-normRight * fExtentX) + (normLook * fExtentZ);
	vCorner[4] = vCorner[0];

	// 4. 정점 구조체 정의
	struct VertexColor { _float3 vPos; D3DCOLOR dwColor; };
	VertexColor vLine[5];
	for (int i = 0; i < 5; ++i)
	{
		vLine[i].vPos = vCorner[i];
		vLine[i].dwColor = color;
	}


	_float4x4 matWorld;
	D3DXMatrixIdentity(&matWorld);
	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &matWorld);


	pDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
	pDevice->DrawPrimitiveUP(D3DPT_LINESTRIP, 4, vLine, sizeof(VertexColor));
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

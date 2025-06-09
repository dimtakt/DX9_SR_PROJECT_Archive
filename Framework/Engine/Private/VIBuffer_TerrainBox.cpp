#include "VIBuffer_TerrainBox.h"

CVIBuffer_TerrainBox::CVIBuffer_TerrainBox(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CVIBuffer { pGraphic_Device }
{
}

CVIBuffer_TerrainBox::CVIBuffer_TerrainBox(const CVIBuffer_TerrainBox& Prototype)
    : CVIBuffer { Prototype }
{
}

HRESULT CVIBuffer_TerrainBox::Initialize_Prototype()
{
	m_iNumVertices = 24;
	m_iVertexStride = sizeof(VTXNORTEX);
	m_iFVF = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_NORMAL;
	m_iNumPrimitive = 12;

	m_iIndexStride = 2;
	m_iNumIndices = 36;
	m_eIndexFormat = D3DFMT_INDEX16;

	if (FAILED(m_pGraphic_Device->CreateVertexBuffer(m_iVertexStride * m_iNumVertices, 0, m_iFVF, D3DPOOL_MANAGED, &m_pVB, nullptr)))
		return E_FAIL;


	// 버텍스 버퍼
	VTXNORTEX* pVertices = { nullptr };
	m_pVertexPositions = new _float3[m_iNumVertices];
	ZeroMemory(m_pVertexPositions, sizeof(_float3) * 4);
	m_pVB->Lock(0, /*m_iNumVertices * m_iVertexStride*/0, reinterpret_cast<void**>(&pVertices), 0);

	//위
	pVertices[0].vPosition = m_pVertexPositions[0] = _float3(-0.5f, 0.5f, 0.5f);
	pVertices[0].vTexcoord = _float2{ 0.f, 0.f };
	pVertices[0].vNormal = _float3(0.f, 0.f, 0.f);

	pVertices[1].vPosition = m_pVertexPositions[1] = _float3(0.5f, 0.5f, 0.5f);
	pVertices[1].vTexcoord = _float2{ 1.f, 0.f };
	pVertices[1].vNormal = _float3(0.f, 0.f, 0.f);

	pVertices[2].vPosition = m_pVertexPositions[2] = _float3{0.5f, 0.5f, -0.5f};
	pVertices[2].vTexcoord = _float2{ 1.f, 1.f };
	pVertices[3].vNormal = _float3(0.f, 0.f, 0.f);

	pVertices[3].vPosition = m_pVertexPositions[3] = _float3{-0.5f, 0.5f, -0.5f};
	pVertices[3].vTexcoord = _float2{ 0.f, 1.f };
	pVertices[3].vNormal = _float3(0.f, 0.f, 0.f);
	//아래
	pVertices[4].vPosition = _float3(-0.5f, -0.5f, 0.5f);
	pVertices[4].vTexcoord = _float2{ 0.f, 0.f };
	pVertices[4].vNormal = _float3(0.f, 0.f, 0.f);

	pVertices[5].vPosition = _float3(0.5f, -0.5f, 0.5f);
	pVertices[5].vTexcoord = _float2{ 1.f, 0.f };
	pVertices[5].vNormal = _float3(0.f, 0.f, 0.f);

	pVertices[6].vPosition = _float3{0.5f, -0.5f, -0.5f};
	pVertices[6].vTexcoord = _float2{ 1.f, 1.f };
	pVertices[6].vNormal = _float3(0.f, 0.f, 0.f);

	pVertices[7].vPosition = _float3{ -0.5f, -0.5f,- 0.5f};
	pVertices[7].vTexcoord = _float2{ 0.f, 1.f };
	pVertices[7].vNormal = _float3(0.f, 0.f, 0.f);

	//왼
	pVertices[8].vPosition = _float3(-0.5f, 0.5f, 0.5f);
	pVertices[8].vTexcoord = _float2{ 0.f, 0.f };
	pVertices[8].vNormal = _float3(0.f, 0.f, 0.f);

	pVertices[9].vPosition = _float3(-0.5f, 0.5f, -0.5f);
	pVertices[9].vTexcoord = _float2{ 1.f, 0.f };
	pVertices[9].vNormal = _float3(0.f, 0.f, 0.f);

	pVertices[10].vPosition = _float3{-0.5f,-0.5f, -0.5f};
	pVertices[10].vTexcoord = _float2{ 1.f, 1.f };
	pVertices[10].vNormal = _float3(0.f, 0.f, 0.f);

	pVertices[11].vPosition = _float3{ -0.5f, -0.5f, 0.5f};
	pVertices[11].vTexcoord = _float2{ 0.f, 1.f };
	pVertices[11].vNormal = _float3(0.f, 0.f, 0.f);

	//오
	pVertices[12].vPosition = _float3(0.5f, 0.5f, -0.5f);
	pVertices[12].vTexcoord = _float2{ 0.f, 0.f };
	pVertices[12].vNormal = _float3(0.f, 0.f, 0.f);

	pVertices[13].vPosition = _float3(0.5f, 0.5f, 0.5f);
	pVertices[13].vTexcoord = _float2{ 1.f, 0.f };
	pVertices[13].vNormal = _float3(0.f, 0.f, 0.f);

	pVertices[14].vPosition = _float3{0.5f, -0.5f, 0.5f};
	pVertices[14].vTexcoord = _float2{ 1.f, 1.f };
	pVertices[14].vNormal = _float3(0.f, 0.f, 0.f);

	pVertices[15].vPosition = _float3{0.5f, -0.5f, -0.5f};
	pVertices[15].vTexcoord = _float2{ 0.f, 1.f };
	pVertices[15].vNormal = _float3(0.f, 0.f, 0.f);

	//앞
	pVertices[16].vPosition = _float3(-0.5f, 0.5f, -0.5f);
	pVertices[16].vTexcoord = _float2{ 0.f, 0.f };
	pVertices[16].vNormal = _float3(0.f, 0.f, 0.f);

	pVertices[17].vPosition = _float3(0.5f, 0.5f, -0.5f);
	pVertices[17].vTexcoord = _float2{ 1.f, 0.f };
	pVertices[17].vNormal = _float3(0.f, 0.f, 0.f);

	pVertices[18].vPosition = _float3{0.5f, -0.5f, -0.5f};
	pVertices[18].vTexcoord = _float2{ 1.f, 1.f };
	pVertices[18].vNormal = _float3(0.f, 0.f, 0.f);

	pVertices[19].vPosition = _float3{-0.5f, -0.5f, -0.5f};
	pVertices[19].vTexcoord = _float2{ 0.f, 1.f };
	pVertices[19].vNormal = _float3(0.f, 0.f, 0.f);

	//뒤
	pVertices[20].vPosition = _float3(-0.5f, 0.5f, 0.5f);
	pVertices[20].vTexcoord = _float2{ 0.f, 0.f };
	pVertices[20].vNormal = _float3(0.f, 0.f, 0.f);

	pVertices[21].vPosition = _float3(0.5f, 0.5f, 0.5f);
	pVertices[21].vTexcoord = _float2{ 1.f, 0.f };
	pVertices[21].vNormal = _float3(0.f, 0.f, 0.f);

	pVertices[22].vPosition = _float3{0.5f, -0.5f, 0.5f};
	pVertices[22].vTexcoord = _float2{ 1.f, 1.f };
	pVertices[22].vNormal = _float3(0.f, 0.f, 0.f);

	pVertices[23].vPosition = _float3{-0.5f, -0.5f, 0.5f};
	pVertices[23].vTexcoord = _float2{ 0.f, 1.f };
	pVertices[23].vNormal = _float3(0.f, 0.f, 0.f);


	


	// 인덱스 버퍼
	if (FAILED(m_pGraphic_Device->CreateIndexBuffer(m_iIndexStride * m_iNumIndices, 0, m_eIndexFormat, D3DPOOL_MANAGED, &m_pIB, nullptr)))
		return E_FAIL;

	_ushort* pIndices = { nullptr };

	m_pIB->Lock(0, 0, reinterpret_cast<void**>(&pIndices), 0);
	_float3 vSourDir, vDestDir, vNormal;
	for (int i = 0; i < 6; ++i)
	{
		int base = i * 4;   // 각 면 시작 정점
		int idx = i * 6;    // 인덱스 배열 위치

		pIndices[idx + 0] = base + 0;
		pIndices[idx + 1] = base + 1;
		pIndices[idx + 2] = base + 2;

		pIndices[idx + 3] = base + 0;
		pIndices[idx + 4] = base + 2;
		pIndices[idx + 5] = base + 3;

		vSourDir = pVertices[base + 2].vPosition - pVertices[base + 1].vPosition;
		vDestDir = pVertices[base + 1].vPosition - pVertices[base + 0].vPosition;
		D3DXVec3Cross(&vNormal, &vSourDir, &vDestDir);
		D3DXVec3Normalize(&vNormal, &vNormal);
		pVertices[base + 0].vNormal += vNormal;
		pVertices[base + 1].vNormal += vNormal;
		pVertices[base + 2].vNormal += vNormal;

		// 삼각형 2 (0,2,3)
		vSourDir = pVertices[base + 3].vPosition - pVertices[base + 2].vPosition;
		vDestDir = pVertices[base + 2].vPosition - pVertices[base + 0].vPosition;
		D3DXVec3Cross(&vNormal, &vSourDir, &vDestDir);
		D3DXVec3Normalize(&vNormal, &vNormal);
		pVertices[base + 0].vNormal += vNormal;
		pVertices[base + 2].vNormal += vNormal;
		pVertices[base + 3].vNormal += vNormal;

	}

	for (size_t i = 0; i < m_iNumVertices; i++)
		D3DXVec3Normalize(&pVertices[i].vNormal, &pVertices[i].vNormal);

	m_pVB->Unlock();
	m_pIB->Unlock();

    return S_OK;
}

HRESULT CVIBuffer_TerrainBox::Initialize(void* pArg)
{
    return S_OK;
}

void CVIBuffer_TerrainBox::Render_Subset(int iFaceIndex)
{
	m_pGraphic_Device->DrawIndexedPrimitive(
		D3DPT_TRIANGLELIST,
		0, 0, 24,
		iFaceIndex * 6,			// 어느면을 그릴지 설정
		2);
}

_float CVIBuffer_TerrainBox::Compute_Height(const _float3& vLocalPos)
{
	_float		fWidth = vLocalPos.x - m_pVertexPositions[0].x;
	_float		fDepth = m_pVertexPositions[0].z - vLocalPos.z;

	D3DXPLANE		Plane{};

	if (fWidth >= fDepth)
	{
		D3DXPlaneFromPoints(&Plane, &m_pVertexPositions[0], &m_pVertexPositions[1], &m_pVertexPositions[2]);
	}
	else
	{
		D3DXPlaneFromPoints(&Plane, &m_pVertexPositions[0], &m_pVertexPositions[2], &m_pVertexPositions[3]);
	}

	return (-Plane.a * vLocalPos.x - Plane.c * vLocalPos.z - Plane.d) / Plane.b;
}

_float CVIBuffer_TerrainBox::Compute_Right(const _float& vLocalPosX, const _float& fOffsetX)
{
	_float fMinX = min(min(m_pVertexPositions[0].x, m_pVertexPositions[1].x), min(m_pVertexPositions[2].x, m_pVertexPositions[3].x));
	_float fMaxX = max(max(m_pVertexPositions[0].x, m_pVertexPositions[1].x), max(m_pVertexPositions[2].x, m_pVertexPositions[3].x));

	return max(fMinX + fOffsetX, min(vLocalPosX, fMaxX - fOffsetX));
}

_float CVIBuffer_TerrainBox::Compute_Look(const _float& vLocalPosZ, const _float& fOffsetZ)
{
	_float fMinZ = min(min(m_pVertexPositions[0].z, m_pVertexPositions[1].z), min(m_pVertexPositions[2].z, m_pVertexPositions[3].z));
	_float fMaxZ = max(max(m_pVertexPositions[0].z, m_pVertexPositions[1].z), max(m_pVertexPositions[2].z, m_pVertexPositions[3].z));

	return max(fMinZ + fOffsetZ, min(vLocalPosZ, fMaxZ - fOffsetZ));
}
CComponent* CVIBuffer_TerrainBox::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CVIBuffer_TerrainBox* pInstance = new CVIBuffer_TerrainBox(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CVIBuffer_TerrainBox"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CVIBuffer_TerrainBox::Clone(void* pArg)
{
	CVIBuffer_TerrainBox* pInstance = new CVIBuffer_TerrainBox(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CVIBuffer_TerrainBox"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CVIBuffer_TerrainBox::Free()
{
	__super::Free();
}

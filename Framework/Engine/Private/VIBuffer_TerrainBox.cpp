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
	m_iVertexStride = sizeof(VTXPOSTEX);
	m_iFVF = D3DFVF_XYZ | D3DFVF_TEX1;
	m_iNumPrimitive = 12;

	m_iIndexStride = 2;
	m_iNumIndices = 36;
	m_eIndexFormat = D3DFMT_INDEX16;

	if (FAILED(m_pGraphic_Device->CreateVertexBuffer(m_iVertexStride * m_iNumVertices, 0, m_iFVF, D3DPOOL_MANAGED, &m_pVB, nullptr)))
		return E_FAIL;


	// 버텍스 버퍼
	VTXPOSTEX* pVertices = { nullptr };


	m_pVB->Lock(0, /*m_iNumVertices * m_iVertexStride*/0, reinterpret_cast<void**>(&pVertices), 0);

	//위
	pVertices[0].vPosition = _float3(-0.5f, 0.5f, 0.5f);
	pVertices[0].vTexcoord = _float2{ 0.f, 0.f };

	pVertices[1].vPosition = _float3(0.5f, 0.5f, 0.5f);
	pVertices[1].vTexcoord = _float2{ 1.f, 0.f };

	pVertices[2].vPosition = _float3{0.5f, 0.5f, -0.5f};
	pVertices[2].vTexcoord = _float2{ 1.f, 1.f };

	pVertices[3].vPosition = _float3{-0.5f, 0.5f, -0.5f};
	pVertices[3].vTexcoord = _float2{ 0.f, 1.f };

	//아래
	pVertices[4].vPosition = _float3(-0.5f, -0.5f, 0.5f);
	pVertices[4].vTexcoord = _float2{ 0.f, 0.f };

	pVertices[5].vPosition = _float3(0.5f, -0.5f, 0.5f);
	pVertices[5].vTexcoord = _float2{ 1.f, 0.f };

	pVertices[6].vPosition = _float3{0.5f, -0.5f, -0.5f};
	pVertices[6].vTexcoord = _float2{ 1.f, 1.f };

	pVertices[7].vPosition = _float3{ -0.5f, -0.5f,- 0.5f};
	pVertices[7].vTexcoord = _float2{ 0.f, 1.f };

	//왼
	pVertices[8].vPosition = _float3(-0.5f, 0.5f, 0.5f);
	pVertices[8].vTexcoord = _float2{ 0.f, 0.f };

	pVertices[9].vPosition = _float3(-0.5f, 0.5f, -0.5f);
	pVertices[9].vTexcoord = _float2{ 1.f, 0.f };

	pVertices[10].vPosition = _float3{-0.5f,-0.5f, -0.5f};
	pVertices[10].vTexcoord = _float2{ 1.f, 1.f };

	pVertices[11].vPosition = _float3{ -0.5f, -0.5f, 0.5f};
	pVertices[11].vTexcoord = _float2{ 0.f, 1.f };

	//오
	pVertices[12].vPosition = _float3(0.5f, 0.5f, -0.5f);
	pVertices[12].vTexcoord = _float2{ 0.f, 0.f };

	pVertices[13].vPosition = _float3(0.5f, 0.5f, 0.5f);
	pVertices[13].vTexcoord = _float2{ 1.f, 0.f };

	pVertices[14].vPosition = _float3{0.5f, -0.5f, 0.5f};
	pVertices[14].vTexcoord = _float2{ 1.f, 1.f };

	pVertices[15].vPosition = _float3{0.5f, -0.5f, -0.5f};
	pVertices[15].vTexcoord = _float2{ 0.f, 1.f };

	//앞
	pVertices[16].vPosition = _float3(-0.5f, 0.5f, -0.5f);
	pVertices[16].vTexcoord = _float2{ 0.f, 0.f };

	pVertices[17].vPosition = _float3(0.5f, 0.5f, -0.5f);
	pVertices[17].vTexcoord = _float2{ 1.f, 0.f };

	pVertices[18].vPosition = _float3{0.5f, -0.5f, -0.5f};
	pVertices[18].vTexcoord = _float2{ 1.f, 1.f };

	pVertices[19].vPosition = _float3{-0.5f, -0.5f, -0.5f};
	pVertices[19].vTexcoord = _float2{ 0.f, 1.f };

	//뒤
	pVertices[20].vPosition = _float3(-0.5f, 0.5f, 0.5f);
	pVertices[20].vTexcoord = _float2{ 0.f, 0.f };

	pVertices[21].vPosition = _float3(0.5f, 0.5f, 0.5f);
	pVertices[21].vTexcoord = _float2{ 1.f, 0.f };

	pVertices[22].vPosition = _float3{0.5f, -0.5f, 0.5f};
	pVertices[22].vTexcoord = _float2{ 1.f, 1.f };

	pVertices[23].vPosition = _float3{-0.5f, -0.5f, 0.5f};
	pVertices[23].vTexcoord = _float2{ 0.f, 1.f };


	m_pVB->Unlock();


	// 인덱스 버퍼
	if (FAILED(m_pGraphic_Device->CreateIndexBuffer(m_iIndexStride * m_iNumIndices, 0, m_eIndexFormat, D3DPOOL_MANAGED, &m_pIB, nullptr)))
		return E_FAIL;

	_ushort* pIndices = { nullptr };

	m_pIB->Lock(0, 0, reinterpret_cast<void**>(&pIndices), 0);

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
	}

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

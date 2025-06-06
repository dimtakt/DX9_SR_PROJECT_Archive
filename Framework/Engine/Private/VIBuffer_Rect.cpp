#include "VIBuffer_Rect.h"

CVIBuffer_Rect::CVIBuffer_Rect(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CVIBuffer { pGraphic_Device }
{
}

CVIBuffer_Rect::CVIBuffer_Rect(const CVIBuffer_Rect& Prototype)
    : CVIBuffer { Prototype }
{
}

HRESULT CVIBuffer_Rect::Initialize_Prototype()
{
	m_iNumVertices = 4;
	m_iVertexStride = sizeof(VTXNORTEX);
	m_iFVF = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_NORMAL;
	m_ePrimitiveType = D3DPT_TRIANGLELIST;
	m_iNumPrimitive = 2;
	m_iIndexStride = 2;
	m_iNumIndices = 6;
	m_eIndexFormat = D3DFMT_INDEX16;

#pragma region VERTEX_BUFFER
	if (FAILED(m_pGraphic_Device->CreateVertexBuffer(m_iVertexStride * m_iNumVertices, 0, m_iFVF, D3DPOOL_MANAGED, &m_pVB, nullptr)))
		return E_FAIL;

	VTXNORTEX* pVertices = { nullptr };
	m_pVertexPositions = new _float3[m_iNumVertices];

	m_pVB->Lock(0, /*m_iNumVertices * m_iVertexStride*/0, reinterpret_cast<void**>(&pVertices), 0);

	m_pVertexPositions[0] = pVertices[0].vPosition = _float3(-0.5f, 0.5f, 0.f);
	pVertices[0].vTexcoord = _float2(0.f, 0.f);
	pVertices[0].vNormal = _float3(0.f, 0.f, 0.f);
	m_pVertexPositions[1] = pVertices[1].vPosition = _float3(0.5f, 0.5f, 0.f);
	pVertices[1].vTexcoord = _float2(1.f, 0.f);
	pVertices[1].vNormal = _float3(0.f, 0.f, 0.f);
	m_pVertexPositions[2] = pVertices[2].vPosition = _float3(0.5f, -0.5f, 0.f);
	pVertices[2].vTexcoord = _float2(1.f, 1.f);
	pVertices[2].vNormal = _float3(0.f, 0.f, 0.f);
	m_pVertexPositions[3] = pVertices[3].vPosition = _float3(-0.5f, -0.5f, 0.f);
	pVertices[3].vTexcoord = _float2(0.f, 1.f);
	pVertices[3].vNormal = _float3(0.f, 0.f, 0.f);

#pragma endregion 


#pragma region INDEX_BUFFER

	if (FAILED(m_pGraphic_Device->CreateIndexBuffer(m_iIndexStride * m_iNumIndices, 0, m_eIndexFormat, D3DPOOL_MANAGED, &m_pIB, nullptr)))
		return E_FAIL;

	_ushort* pIndices = { nullptr };
	m_pIndices = new _ushort[m_iNumIndices];
	ZeroMemory(m_pIndices, sizeof(_ushort) * m_iNumIndices);

	m_pIB->Lock(0, 0, reinterpret_cast<void**>(&pIndices), 0);

	pIndices[0] = 0;
	pIndices[1] = 1;
	pIndices[2] = 2;

	pIndices[3] = 0;
	pIndices[4] = 2;
	pIndices[5] = 3;

	_float3 vSourDir, vDestDir, vNormal;

	// »ï°¢Çü 1 (0,1,2)
	vSourDir = pVertices[2].vPosition - pVertices[1].vPosition;
	vDestDir = pVertices[1].vPosition - pVertices[0].vPosition;
	D3DXVec3Cross(&vNormal, &vSourDir, &vDestDir);
	D3DXVec3Normalize(&vNormal, &vNormal);
	pVertices[0].vNormal += vNormal;
	pVertices[1].vNormal += vNormal;
	pVertices[2].vNormal += vNormal;

	// »ï°¢Çü 2 (0,2,3)
	vSourDir = pVertices[3].vPosition - pVertices[2].vPosition;
	vDestDir = pVertices[2].vPosition - pVertices[0].vPosition;
	D3DXVec3Cross(&vNormal, &vSourDir, &vDestDir);
	D3DXVec3Normalize(&vNormal, &vNormal);
	pVertices[0].vNormal += vNormal;
	pVertices[2].vNormal += vNormal;
	pVertices[3].vNormal += vNormal;

	for (size_t i = 0; i < m_iNumVertices; i++)
		D3DXVec3Normalize(&pVertices[i].vNormal, &pVertices[i].vNormal);

	m_pVB->Unlock();
	m_pIB->Unlock();

	memcpy(m_pIndices, pIndices, sizeof(_ushort) * m_iNumIndices);

#pragma endregion 

	return S_OK;
}

HRESULT CVIBuffer_Rect::Initialize_Prototype(D3DXCOLOR vColor)
{
	m_iNumVertices = 4;
	m_iVertexStride = sizeof(VTXPOSCOL);
	m_iFVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;
	m_ePrimitiveType = D3DPT_TRIANGLELIST;
	m_iNumPrimitive = 2;
	m_iIndexStride = 2;
	m_iNumIndices = 6;
	m_eIndexFormat = D3DFMT_INDEX16;


	m_pVertexPositions = new _float3[m_iNumVertices];

	if (FAILED(m_pGraphic_Device->CreateVertexBuffer(m_iVertexStride * m_iNumVertices, 0, m_iFVF, D3DPOOL_MANAGED, &m_pVB, nullptr)))
		return E_FAIL;

	VTXPOSCOL* pVertices = { nullptr };

	m_pVB->Lock(0, 0, reinterpret_cast<void**>(&pVertices), 0);

	m_pVertexPositions[0] = pVertices[0].vPosition = _float3(-0.5f, 0.5f, 0.f);
	pVertices[0].dwColor = vColor;

	m_pVertexPositions[1] = pVertices[1].vPosition = _float3(0.5f, 0.5f, 0.f);
	pVertices[1].dwColor = vColor;

	m_pVertexPositions[2] = pVertices[2].vPosition = _float3(0.5f, -0.5f, 0.f);
	pVertices[2].dwColor = vColor;

	m_pVertexPositions[3] = pVertices[3].vPosition = _float3(-0.5f, -0.5f, 0.f);
	pVertices[3].dwColor = vColor;

	m_pVB->Unlock();

	m_pIndices = new _uint[m_iNumIndices];

	if (FAILED(m_pGraphic_Device->CreateIndexBuffer(m_iIndexStride * m_iNumIndices, 0, m_eIndexFormat, D3DPOOL_MANAGED, &m_pIB, nullptr)))
		return E_FAIL;

	_ushort* pIndices = { nullptr };

	m_pIB->Lock(0, 0, reinterpret_cast<void**>(&pIndices), 0);

	pIndices[0] = 0;
	pIndices[1] = 1;
	pIndices[2] = 2;

	pIndices[3] = 0;
	pIndices[4] = 2;
	pIndices[5] = 3;

	m_pIB->Unlock();

	memcpy(m_pIndices, pIndices, m_iIndexStride * m_iNumIndices);

	return S_OK;
}

HRESULT CVIBuffer_Rect::Initialize(void* pArg)
{
    return S_OK;
}

void CVIBuffer_Rect::ChangeUV_FlipX(_bool isFlipped)
{
	VTXNORTEX* pVertices = nullptr;
	if (FAILED(m_pVB->Lock(0, 0, reinterpret_cast<void**>(&pVertices), 0)))
		return;

	if (isFlipped == false)
	{
		pVertices[0].vTexcoord = _float2(0.f, 0.f);
		pVertices[1].vTexcoord = _float2(1.f, 0.f);
		pVertices[2].vTexcoord = _float2(1.f, 1.f);
		pVertices[3].vTexcoord = _float2(0.f, 1.f);
	}
	else
	{
		pVertices[0].vTexcoord = _float2(1.f, 0.f);
		pVertices[1].vTexcoord = _float2(0.f, 0.f);
		pVertices[2].vTexcoord = _float2(0.f, 1.f);
		pVertices[3].vTexcoord = _float2(1.f, 1.f);
	}

	m_pVB->Unlock();
}

void CVIBuffer_Rect::ResetUV_FlipX()
{
	VTXNORTEX* pVertices = nullptr;
	if (FAILED(m_pVB->Lock(0, 0, reinterpret_cast<void**>(&pVertices), 0)))
		return;

	pVertices[0].vTexcoord = _float2(0.f, 0.f);
	pVertices[1].vTexcoord = _float2(1.f, 0.f);
	pVertices[2].vTexcoord = _float2(1.f, 1.f);
	pVertices[3].vTexcoord = _float2(0.f, 1.f);

	m_pVB->Unlock();
}

CComponent* CVIBuffer_Rect::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CVIBuffer_Rect* pInstance = new CVIBuffer_Rect(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CVIBuffer_Rect"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CVIBuffer_Rect::Create(LPDIRECT3DDEVICE9 pGraphic_Device, D3DXCOLOR vColor)
{
	CVIBuffer_Rect* pInstance = new CVIBuffer_Rect(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(vColor)))
	{
		MSG_BOX(TEXT("Failed to Created : CVIBuffer_Rect_Color"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CVIBuffer_Rect::Clone(void* pArg)
{
	CVIBuffer_Rect* pInstance = new CVIBuffer_Rect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CVIBuffer_Rect"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CVIBuffer_Rect::Free()
{
	__super::Free();
	if (m_isCloned == FALSE) {
		Safe_Delete(m_pIndices);
		Safe_Delete_Array(m_pVertexPositions);
	}
}

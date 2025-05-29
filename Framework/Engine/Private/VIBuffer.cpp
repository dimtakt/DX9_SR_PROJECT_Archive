#include "VIBuffer.h"
#include "GameInstance.h"

CVIBuffer::CVIBuffer(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CComponent { pGraphic_Device }
{
	
}

CVIBuffer::CVIBuffer(const CVIBuffer& Prototype)
	: CComponent { Prototype }
	, m_pVB { Prototype.m_pVB }
	, m_pIB{ Prototype.m_pIB }
	, m_iNumVertices { Prototype.m_iNumVertices }
	, m_iVertexStride { Prototype.m_iVertexStride }
	, m_iFVF { Prototype.m_iFVF }
	, m_ePrimitiveType { Prototype.m_ePrimitiveType }
	, m_iNumPrimitive { Prototype.m_iNumPrimitive }
	, m_iIndexStride{ Prototype.m_iIndexStride }
	, m_iNumIndices{ Prototype.m_iNumIndices }
	, m_eIndexFormat{ Prototype.m_eIndexFormat }
	, m_pVertexPositions { Prototype.m_pVertexPositions }
	, m_pIndices{ Prototype.m_pIndices }
{
	Safe_AddRef(m_pIB);
	Safe_AddRef(m_pVB);
}

HRESULT CVIBuffer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CVIBuffer::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CVIBuffer::Bind_Buffers()
{
	m_pGraphic_Device->SetStreamSource(0, m_pVB, 0, m_iVertexStride);

	m_pGraphic_Device->SetIndices(m_pIB);

	m_pGraphic_Device->SetFVF(m_iFVF);

	return S_OK;
}

HRESULT CVIBuffer::Render()
{
	/*m_pGraphic_Device->DrawPrimitive(m_ePrimitiveType, 0, m_iNumPrimitive);*/

	m_pGraphic_Device->DrawIndexedPrimitive(m_ePrimitiveType, 0, 0, m_iNumVertices, 0, m_iNumPrimitive);

	return S_OK;
}

_float3 CVIBuffer::Compute_PickedPosition(const _float4x4* pWorldMatrixInverse)
{
	_uint   iIndices[3] = {};
	_float3 vPickedPos = {};

	m_pGameInstance->Transform_Picking_ToLocalSpace(*pWorldMatrixInverse);

	for (size_t i = 0; i < m_iNumPrimitive; i++)
	{
		_byte* pIndices = static_cast<_byte*>(m_pIndices) + m_iIndexStride * i * 3;

		memcpy(&iIndices[0], pIndices, m_iIndexStride);
		memcpy(&iIndices[1], pIndices + m_iIndexStride, m_iIndexStride);
		memcpy(&iIndices[2], pIndices + m_iIndexStride * 2, m_iIndexStride);

		if (true == m_pGameInstance->Picking_InLocal(vPickedPos, m_pVertexPositions[iIndices[0]], m_pVertexPositions[iIndices[1]], m_pVertexPositions[iIndices[2]]))
			break;
	}

	return vPickedPos;
}

void CVIBuffer::Free()
{
	__super::Free();

	//if (false == m_isCloned)
	//{
	//	Safe_Delete_Array(m_pVertexPositions);
	//	Safe_Delete_Array(m_pIndices);
	//}

	Safe_Release(m_pIB);
	Safe_Release(m_pVB);
}

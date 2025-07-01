#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_TerrainBox final : public CVIBuffer
{
private:
	CVIBuffer_TerrainBox(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer_TerrainBox(const CVIBuffer_TerrainBox& Prototype);
	virtual ~CVIBuffer_TerrainBox() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	void Render_Subset(int iFaceIndex);

public:
	virtual _float Compute_Height(const _float3& vLocalPos) override;
	virtual _float Compute_Right(const _float& vLocalPosX, const _float& fOffsetX) override;
	virtual _float Compute_Look(const _float& vLocalPosZ, const _float& fOffsetZ) override;

public:
	static CComponent* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END
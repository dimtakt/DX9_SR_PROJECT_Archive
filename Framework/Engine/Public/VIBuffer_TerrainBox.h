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
	static CComponent* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END
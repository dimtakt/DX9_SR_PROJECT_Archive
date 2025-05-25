#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Rect final : public CVIBuffer
{
private:
	CVIBuffer_Rect(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer_Rect(const CVIBuffer_Rect& Prototype);
	virtual ~CVIBuffer_Rect() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize_Prototype(D3DXCOLOR vColor);
	virtual HRESULT Initialize(void* pArg) override;

public:
	static CComponent* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	static CComponent* Create(LPDIRECT3DDEVICE9 pGraphic_Device, D3DXCOLOR vColor);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END
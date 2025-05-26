#pragma once

#include "Base.h"

BEGIN(Engine)

class CPicking final : public CBase
{
private:
	CPicking(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CPicking() = default;

public:
	HRESULT Initialize(HWND hWnd, _uint iWinSizeX, _uint iWinSizeY);

	void Update();

	_bool Picking_InWorld(_float3& vPickedPos, const _float3& vPointA, const _float3& vPointB, const _float3& vPointC);
	_bool Picking_InLocal(_float3& vPickedPos, const _float3& vPointA, const _float3& vPointB, const _float3& vPointC);

	_bool Get_IntersectAtY(_float targetY, _float3& vIntersectPos);

	void Transform_ToLocalSpace(const _float4x4& WorldMatrixInverse);

private:
	LPDIRECT3DDEVICE9 m_pGraphic_Device = { nullptr };

private:
	HWND		m_hWnd = {};
	_uint		m_iWinSizeX{}, m_iWinSizeY{};

	_float3		m_vMouseRay = {};
	_float3		m_vMousePos = {};

	_float3		m_vLocalMouseRay = {};
	_float3		m_vLocalMousePos = {};

public:
	static CPicking* Create(LPDIRECT3DDEVICE9 pGraphic_Device, HWND hWnd, _uint iWinSizeX, _uint iWinSizeY);
	virtual void Free() override;
};

END
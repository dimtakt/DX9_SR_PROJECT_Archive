#pragma once

#include "Effect.h"
#include "PlayerEffect.h"
#include "Object_Manager.h"

BEGIN(Client)

class CEffect_Factory final : public CBase
{
	DECLARE_SINGLETON(CEffect_Factory)
public:
	CEffect_Factory();
	~CEffect_Factory() = default;

public:
	HRESULT Initialize();

public:
	void Create_Effect(const _wstring& strEffectTag, _float3 vPos, D3DXQUATERNION qRot, _float3 vScale, _bool isFlippedX = false);
	void Create_Effect(const _wstring& strEffectTag, _float4x4 matEffectWorld, _bool isFlippedX = false);
	void Create_Effect(const _wstring& strEffectTag, _float4x4 matEffectWorld, _float4x4 matOffsetWorld, _bool isFlippedX = false);
	void Create_Effect_RotationByParent(const _wstring& strEffectTag, _float4x4 matEffectWorld, const _float3 axis, _bool isFlippedX = false, CTransform* parent = nullptr, _float fRadian = 0);


private:
	CGameInstance* m_pGameInstance = { nullptr };

public:
	static CEffect_Factory* Create();
	virtual void Free() override;

};

END;
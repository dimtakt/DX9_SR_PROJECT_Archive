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
	HRESULT Initialize();
	~CEffect_Factory() = default;

public:
	void Create_Effect(const _wstring& strEffectTag, _float3 vPos, D3DXQUATERNION qRot, _float3 vScale);

private:
	CGameInstance* m_pGameInstance = { nullptr };

public:
	static CEffect_Factory* Create();
	virtual void Free() override;

};

END;
#pragma once
#include "Base.h"

BEGIN(Engine)

class CParticle_Manager final : public CBase
{
private:
	CParticle_Manager();
	virtual ~CParticle_Manager() = default;

public:
	HRESULT Create_Particle(_uint iParticleType, _uint iLayerLevelIndex, const _wstring& strLayerTag, _float3 vScale, _bool bUseOrtho);
	HRESULT Play(_uint iParticleType, _float3 vPos);
		

	void Create_OffSet();

	void Priority_Update(_float fTimeDelta);
	void Update(_float fTimeDelta, _uint iParticleType);
	void Late_Update(_float fTimeDelta, _uint iParticleType);
	void On_Active();

private:
	class CGameInstance* m_pGameInstance = { nullptr };

	map<_uint, vector<class CParticle*>> m_mParticle = {};

	vector<_float3> m_vOffSet = {};

public:
	static CParticle_Manager* Create();
	virtual void Free() override;
};

END


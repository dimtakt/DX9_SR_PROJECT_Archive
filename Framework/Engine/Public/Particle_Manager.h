#pragma once
#include "Base.h"

BEGIN(Engine)

class CParticle_Manager final : public CBase
{
private:
	CParticle_Manager();
	virtual ~CParticle_Manager() = default;

public:
	HRESULT Create_Particle_Fast(_uint iParticleType, _uint iLayerLevelIndex, const _wstring& strLayerTag, _int iOffSetType, _float3 vScale, _bool bUseOrtho);
	HRESULT Create_Particle_Low(_uint iParticleType, _uint iLayerLevelIndex, const _wstring& strLayerTag, _int iOffSetType, _bool bUseOrtho);

	HRESULT Play(_uint iParticleType, _float3 vPos);
		

	void Create_OffSet_20();
	void Create_OffSet_30();
	void Clear();

	void Priority_Update(_float fTimeDelta);
	void Update(_float fTimeDelta, _uint iParticleType);
	void Late_Update(_float fTimeDelta, _uint iParticleType);
	// void On_Active(); 매니저가 관리하는게 좋아보이긴 함. 수정 생각


private:
	class CGameInstance* m_pGameInstance = { nullptr };

	map<_uint, vector<class CParticle*>> m_mParticle = {};

	vector<_float3> m_vOffSet = {};

public:
	static CParticle_Manager* Create();
	virtual void Free() override;
};

END


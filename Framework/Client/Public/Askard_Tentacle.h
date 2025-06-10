#pragma once
#include "Monster.h"

BEGIN(Client)

class CAskard_Tentacle : public CMonster
{
private:
	CAskard_Tentacle(LPDIRECT3DDEVICE9 pGraphic_Device);
	CAskard_Tentacle(const CAskard_Tentacle& Prototype);
	virtual ~CAskard_Tentacle() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Components(void* pArg);

public:
	virtual void OnCollision(CGameObject* pGameObject) override;

private:
	// ÄÄÆ÷³ÍÆ®µé


	static CAskard_Tentacle* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};


END
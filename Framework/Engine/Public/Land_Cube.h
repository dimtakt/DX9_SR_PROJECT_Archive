#pragma once
#include "LandObject.h"

BEGIN(Engine)

class ENGINE_DLL CLand_Cube abstract : public CLandObject
{
protected:
	CLand_Cube(LPDIRECT3DDEVICE9 pGraphic_Device);
	CLand_Cube(const CLand_Cube& Prototype);
	virtual ~CLand_Cube() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
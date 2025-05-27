#pragma once
#include "Client_Defines.h"
#include "Land_Cube.h"

BEGIN(Client)

class CLand final : public CLand_Cube
{
private:
	CLand(LPDIRECT3DDEVICE9 pGraphic_Device);
	CLand(const CLand& Prototype);
	virtual ~CLand() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Components(void* pArg);
	HRESULT Ready_Material();

public:
	static CLand* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
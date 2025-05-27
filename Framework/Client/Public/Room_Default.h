#pragma once
#include "Client_Defines.h"
#include "Room.h"

BEGIN(Client)

class CRoom_Default final : public CRoom
{
private:
	CRoom_Default(LPDIRECT3DDEVICE9 pGraphic_Device);
	CRoom_Default(const CRoom_Default& Prototype);
	virtual ~CRoom_Default() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Components(void* pArg);
	HRESULT Ready_Objects(void* pArg);

public:
	static CRoom_Default* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
#pragma once
#include "ItemObject.h"
#include "Client_Defines.h"
#include "Client_Defines_Item.h"
BEGIN(Client)
class CItem_Base final : public CItemObject
{
private:
	CItem_Base(LPDIRECT3DDEVICE9 pGraphic_Device);
	CItem_Base(const CItem_Base& Prototype);
	virtual					~CItem_Base() = default;

public:
	virtual HRESULT			Initialize_Prototype();
	virtual HRESULT			Initialize(void* pArg);
	virtual void			Priority_Update(_float fTimeDelta);
	virtual void			Update(_float fTimeDelta);
	virtual void			Late_Update(_float fTimeDelta, _float3 fPos);
	virtual HRESULT			Render();

public:
	void					IsSelete();
	void					IsRotation_Slate();

private:
	HRESULT					Default_Render();
	HRESULT					Selete_Render();

	HRESULT					Ready_Componet();

private:
	_bool					m_bisSelete{};

public:
	static CItem_Base*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg) override;
	virtual void			Free() override;
};
END

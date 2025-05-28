#pragma once
#include "ItemObject.h"
#include "Client_Defines.h"

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
	virtual void			Late_Update(_float fTimeDelta);
	virtual HRESULT			Render();

private:
	HRESULT					Ready_Componet(_uint iTextureIndex);

	void					SetUp_RenderState();
	void					Reset_RenderState();

public:
	static CItem_Base*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg) override;
	virtual void			Free() override;
};
END

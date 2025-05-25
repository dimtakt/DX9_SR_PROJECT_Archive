#pragma once
#include "UIObject.h"
#include "Client_Defines.h"

BEGIN(Client)
class CHud_Dash final : public CUIObject
{
private:
									CHud_Dash(LPDIRECT3DDEVICE9 pGraphic_Device);
									CHud_Dash(const CHud_Dash& Prototype);
	virtual							~CHud_Dash() = default;

public:
	virtual HRESULT					Initialize_Prototype(LEVEL eLevel);
	virtual HRESULT					Initialize(void* pArg) override;
	virtual void					Priority_Update(_float fTimeDelta) override;
	virtual void					Update(_float fTimeDelta) override;
	virtual void					Late_Update(_float fTimeDelta) override;
	virtual HRESULT					Render() override;

private:
	LEVEL							m_eLevel = {};
	_int							m_iDashMaxValue = {};
private:
	HRESULT							Ready_Components();

	HRESULT							Ready_ChildPrototype(LEVEL eLevel);
	HRESULT							Ready_Children();
public:
	static CHud_Dash*				Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel);
	virtual CGameObject*			Clone(void* pArg) override;
	virtual void					Free() override;
};
END

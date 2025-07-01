#pragma once
#include "UIObject.h"
#include "Client_Defines.h"

class CHud_Buff final : public CUIObject
{
private:
									CHud_Buff(LPDIRECT3DDEVICE9 pGraphic_Device);
									CHud_Buff(const CHud_Buff& Prototype);
	virtual							~CHud_Buff() = default;

public:
	virtual HRESULT					Initialize_Prototype(LEVEL eLevel);
	virtual HRESULT					Initialize(void* pArg) override;
	virtual void					Priority_Update(_float fTimeDelta) override;
	virtual void					Update(_float fTimeDelta) override;
	virtual void					Late_Update(_float fTimeDelta) override;
	virtual HRESULT					Render() override;

private:
	LEVEL							m_eLevel = {};
	_uint							m_iIndex = {};
private:
	HRESULT							Ready_Components();

	HRESULT							Ready_ChildPrototype(LEVEL eLevel);
	HRESULT							Ready_Children();

	HRESULT							Reday_SyncingObject();
public:
	static CHud_Buff*				Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel);
	virtual CGameObject*			Clone(void* pArg) override;
	virtual void					Free() override;
};

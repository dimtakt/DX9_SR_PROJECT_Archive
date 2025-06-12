#pragma once
#include "UIObject.h"
#include "Client_Defines.h"

BEGIN(Client)
class CEvent_AZPattern final : public CUIObject
{
private:
	CEvent_AZPattern(LPDIRECT3DDEVICE9 pGraphic_Device);
	CEvent_AZPattern(const CEvent_AZPattern& Prototype);
	virtual							~CEvent_AZPattern() = default;

public:
	virtual HRESULT					Initialize_Prototype(LEVEL eLevel);
	virtual HRESULT					Initialize(void* pArg) override;
	virtual void					Priority_Update(_float fTimeDelta) override;
	virtual void					Update(_float fTimeDelta) override;
	virtual void					Late_Update(_float fTimeDelta) override;
	virtual HRESULT					Render() override;

private:
	LEVEL							m_eLevel = {};

private:
	HRESULT							Ready_Components();

	HRESULT							Ready_ChildPrototype(LEVEL eLevel);
	HRESULT							Ready_Children();

public:
	static CEvent_AZPattern* Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void					Free() override;
};
END

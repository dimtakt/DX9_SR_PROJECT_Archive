#pragma once
#include "UIObject.h"
#include "Client_Defines.h"

BEGIN(Client)
class CEvent_ClashPattern final : public CUIObject
{
private:
									CEvent_ClashPattern(LPDIRECT3DDEVICE9 pGraphic_Device);
									CEvent_ClashPattern(const CEvent_ClashPattern& Prototype);
	virtual							~CEvent_ClashPattern() = default;

public:
	void							Start_Event();
	void							End_Event();
public:
	virtual HRESULT					Initialize_Prototype(LEVEL eLevel);
	virtual HRESULT					Initialize(void* pArg) override;
	virtual void					Priority_Update(_float fTimeDelta) override;
	virtual void					Update(_float fTimeDelta) override;
	virtual void					Late_Update(_float fTimeDelta) override;
	virtual HRESULT					Render() override;

private:
	LEVEL							m_eLevel = {};

	_bool							m_bIsOpen = {};

private:
	HRESULT							Ready_Components();

	HRESULT							Ready_ChildPrototype(LEVEL eLevel);
	HRESULT							Ready_Children();

public:
	static CEvent_ClashPattern*		Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel);
	virtual CGameObject*			Clone(void* pArg) override;
	virtual void					Free() override;
};
END

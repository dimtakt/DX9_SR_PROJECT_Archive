#pragma once
#include "Client_Defines.h"
#include "Button.h"

BEGIN(Client)
class CTalent_Slot_Button final : public CButton
{
private:
	CTalent_Slot_Button(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTalent_Slot_Button(const CTalent_Slot_Button& Prototype);
	virtual							~CTalent_Slot_Button() = default;

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
	_uint							m_iTexIdex = {};
	_int							m_iAddValue = {};

private:
	HRESULT							Ready_Components();

	HRESULT							Ready_ChildPrototype(LEVEL eLevel);
	HRESULT							Ready_Children();

	void							Ready_ButtonSet();

	_bool							Is_Button_Pick();
	void							On_Button();

public:
	static CTalent_Slot_Button* Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void					Free() override;
};
END
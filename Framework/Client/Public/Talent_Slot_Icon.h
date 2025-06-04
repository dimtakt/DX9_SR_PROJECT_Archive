#pragma once
#include "Client_Defines.h"
#include "Button.h"

BEGIN(Client)
class CTalent_Slot_Icon final : public CButton
{
private:
	CTalent_Slot_Icon(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTalent_Slot_Icon(const CTalent_Slot_Icon& Prototype);
	virtual							~CTalent_Slot_Icon() = default;

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

	_bool							m_bGiftCheck = { false };
	_bool							m_bIsGetGift = { false };
	_int							m_iSpecial_Gift = {};
private:
	HRESULT							Ready_Components();
	void							SetUp_RenderState();
	void							Reset_RenderState();

	HRESULT							Ready_ChildPrototype(LEVEL eLevel);
	HRESULT							Ready_Children();

	void							Special_Gift_Update();
public:
	static CTalent_Slot_Icon* Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void					Free() override;
};
END